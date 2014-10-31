#include "SporadicFrameEditDlg.h"
#include "LDFDatabaseManager.h"
#include "IClusterProps.h"
#include "LDFUtility.h"
#include "Defines.h"
#include "qdialogbuttonbox.h"
#include "qmessagebox.h"

SporadicFrameEditDlg::SporadicFrameEditDlg(IFrame** pouFrame,eMode ouMode, QWidget* parent)
{
    m_pouFrame = pouFrame;
    ui.setupUi(this);
    m_pLdfCluster = LDFDatabaseManager::GetDatabaseManager()->GetLDFCluster();
    m_ouMode = ouMode;

    connect(ui.buttonOk, SIGNAL(accepted()), this, SLOT(OnButtonClickOk()));
    connect(ui.buttonOk, SIGNAL(rejected()), this, SLOT(reject()));

    vPrepareValidations();

    setWindowTitle("Create SporadicFrame");

    if(m_ouMode == eEdit)
    {
        vPrepareUIForEditMode();
    }

    QIcon ouWindowIcon;
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/16x16/Frame.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/22x22/Frame.png", QSize(16, 16));
    ouWindowIcon.addFile(":/LDFEditorIcons/Resources/icons/32x32/Frame.png", QSize(16, 16));

    setWindowIcon(ouWindowIcon);

}


SporadicFrameEditDlg::~SporadicFrameEditDlg(void)
{
}

void SporadicFrameEditDlg::vPrepareUIForEditMode()
{
    IFrame* pouFrame = *m_pouFrame;
    std::string strName;

    if ( nullptr != pouFrame )
    {
        unsigned int nUid;
        pouFrame->GetFrameId(nUid);
        pouFrame->GetName(strName);

        QString strTitle = "Edit SporadicFrame - ";
        strTitle+= + strName.c_str();
        setWindowTitle(strTitle);

        ui.editFrameName->setText(strName.c_str());
    }
}

void SporadicFrameEditDlg::vPrepareValidations()
{
    ui.editFrameName->setValidator(new QRegExpValidator(QRegExp(defIdentifier_RegExp)));
    vFillElementList();
}

void SporadicFrameEditDlg::vFillElementList()
{
    m_strFrameNames.clear();
    std::map<UID_ELEMENT, IElement*> pElementMap;
    m_pLdfCluster->GetElementList(eFrameElement, pElementMap);
    std::string strName;
    unsigned int unFrameid;
    unsigned int unCurrentFrameid = -1;
    FrameProps ouFrameProps;
    if ( eEdit == m_ouMode && (m_pouFrame != nullptr ) )
    {
        (*m_pouFrame)->GetProperties(ouFrameProps);
    }

for ( auto itr : pElementMap )
    {
        ((IFrame*)itr.second)->GetFrameId(unFrameid);
        vAddFrameToTable((IFrame*)itr.second, ouFrameProps.m_ouLINSporadicFrameProps.m_pouUnconditionalFrame);
        (IFrame*)itr.second->GetName(strName);
        m_strFrameNames[strName] = strName;
    }
    if ( eEdit == m_ouMode && (m_pouFrame != nullptr ) )
    {
        (*m_pouFrame)->GetName(strName);
        auto itr2 = m_strFrameNames.find(strName);
        m_strFrameNames.erase(itr2);
    }
}

void SporadicFrameEditDlg::OnButtonClickOk()
{
    if ( 0 == nValidateValues() )
    {
        IFrame* pFrame;
        FrameProps ouFrameProps;
        if ( eEdit == m_ouMode )
        {
            pFrame = *m_pouFrame;
        }
        else
        {
            m_pLdfCluster->CreateElement(eFrameElement, (IElement**)&pFrame);
            *m_pouFrame = pFrame;
        }
        if ( nullptr != pFrame )
        {
            pFrame->GetProperties(ouFrameProps);

            //Frame Type
            ouFrameProps.m_eFrameType = eLIN_Sporadic;

            ouFrameProps.m_ouLINSporadicFrameProps.m_pouUnconditionalFrame.clear();
            //Conditional Frames
            int nRow = ui.tableFrames->rowCount();
            IFrame* pouTempFrame;
            for ( int i = 0 ; i < nRow; i++ )
            {
                QTableWidgetItem* pItem = ui.tableFrames->item(i, 0);
                UID_ELEMENT uid = pItem->data(Qt::UserRole).value<UID_ELEMENT>();
                if ( Qt::Checked == pItem->checkState() )
                {
                    m_pLdfCluster->GetElement(eFrameElement, uid, (IElement**)&pouTempFrame);
                    if ( nullptr != pouTempFrame )
                    {
                        ouFrameProps.m_ouLINSporadicFrameProps.m_pouUnconditionalFrame[uid] = uid;
                    }
                }
            }

            pFrame->SetProperties(ouFrameProps);

            //Frame Name
            pFrame->SetName(ui.editFrameName->text().toStdString());
        }
        LDFDatabaseManager::GetDatabaseManager()->setDocumentModified(true);
        accept();
    }
}

int SporadicFrameEditDlg::nValidateValues()
{
    if ( 0 == ui.editFrameName->text().length() )
    {
        QMessageBox::critical(this, "Error", "Name Should Not Be Empty",  QMessageBox::Ok);
        ui.editFrameName->setFocus();
        return -1;
    }

    if ( m_strFrameNames.find(ui.editFrameName->text().toStdString()) != m_strFrameNames.end() )
    {
        QMessageBox::critical(this, "Error", "Name Already Exists",  QMessageBox::Ok);
        ui.editFrameName->setFocus();
        return -1;
    }
    return 0;
}


void SporadicFrameEditDlg::vAddFrameToTable(IFrame* pFrame, std::map<UID_ELEMENT, UID_ELEMENT> ouSelectedElements)
{
    if ( nullptr == pFrame )
    {
        return;
    }

    eFrameType ouFrameType;
    EcuProperties ouEcuProps;
    std::list<IEcu*> ouEcuList;
    pFrame->GetFrameType(ouFrameType);
    pFrame->GetEcus(eTx, ouEcuList);
    auto itrTxEcu  = ouEcuList.begin();

    if ( itrTxEcu != ouEcuList.end() )
    {
        ((IEcu*)*itrTxEcu)->GetProperties(ouEcuProps);
    }

    if ( eLIN_Unconditional == ouFrameType)
    {
        std::string strName;
        std::string strTxEcu;
        std::string strRxEcu;
        unsigned int unId;
        FrameProps ouFrameprops;
        pFrame->GetName(strName);
        pFrame->GetFrameId(unId);
        pFrame->GetProperties(ouFrameprops);
        ((IEcu*)*itrTxEcu)->GetName(strTxEcu);

        QList<QVariant> ouCloumns;
        ouCloumns.push_back(QVariant(QString::fromStdString(strName)));
        ouCloumns.push_back(QVariant(ouFrameprops.m_ouLINUnConditionFrameProps.m_unId));
        ouCloumns.push_back(QVariant( GetString(ouFrameprops.m_ouLINUnConditionFrameProps.m_nLength, 10)));
        ouCloumns.push_back(QVariant(QString::fromStdString(strTxEcu)));
        ouEcuList.clear();
        pFrame->GetEcus(eRx, ouEcuList);
        nGetSubscribers(ouEcuList, strRxEcu);
        ouCloumns.push_back(QVariant(QString::fromStdString(strRxEcu)));
        ui.tableFrames->setRowCount(ui.tableFrames->rowCount()+1);
        ui.tableFrames->InsertRow(ui.tableFrames->rowCount()-1, ouCloumns);
        if ( ouSelectedElements.find(pFrame->GetUniqueId()) == ouSelectedElements.end() )
        {
            ui.tableFrames->item(ui.tableFrames->rowCount()-1, 0)->setCheckState(Qt::Unchecked);
        }
        else
        {
            ui.tableFrames->item(ui.tableFrames->rowCount()-1, 0)->setCheckState(Qt::Checked);
        }
        ui.tableFrames->item(ui.tableFrames->rowCount()-1, 0)->setData(Qt::UserRole, QVariant(pFrame->GetUniqueId()));
    }
}