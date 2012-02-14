// SignalDefinerDlg.cpp : implementation file
//

#include "SignalDefiner_stdafx.h"
#include "SignalDefiner.h"
#include "SignalDefinerDlg.h"
#include "SignalDefiner_Extern.h"

#define M_PI       3.14159265358979323846
const float SINE_COEFF = (8 / (M_PI * M_PI));

// CSignalDefinerDlg dialog

IMPLEMENT_DYNAMIC(CSignalDefinerDlg, CDialog)

CSignalDefinerDlg::CSignalDefinerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSignalDefinerDlg::IDD, pParent)
	, m_fAmplitude(10)
	, m_fFrequency(1)
	, m_dblSamplingTimePeriod(125)
	, m_nSelCycle(3)
	, m_nSignalType(1)
{
	AfxEnableControlContainer();
	m_poDMGraphCtrl = NULL;
	m_strSignalName = _T("");
}

CSignalDefinerDlg::~CSignalDefinerDlg()
{
}

void CSignalDefinerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT_SIGNAL_AMPLITUDE, m_fAmplitude);
	DDX_Text(pDX, IDC_EDIT_SIGNAL_FREQUENCY, m_fFrequency);
	DDX_Text(pDX, IDC_EDIT_SIGNAL_SAMPLING_TIME, m_dblSamplingTimePeriod);
	DDX_CBIndex(pDX, IDC_COMBO_CYCLES, m_nSelCycle);
	DDX_Control(pDX, IDC_COMBO_SIGNAL_TYPE, m_ctrSignalType);
	DDX_Control(pDX, IDC_COMBO_CYCLES, m_ctrNoOfCycles);			
	DDX_CBIndex(pDX, IDC_COMBO_SIGNAL_TYPE, m_nSignalType);
}


BEGIN_MESSAGE_MAP(CSignalDefinerDlg, CDialog)
	ON_CBN_SELCHANGE(IDC_COMBO_SIGNAL_TYPE, OnCbnSelchangeComboSignalType)
	ON_CBN_SELCHANGE(IDC_COMBO_CYCLES, OnCbnSelchangeComboCycles)
	ON_EN_CHANGE(IDC_EDIT_SIGNAL_AMPLITUDE, OnEnChangeEditSignalAmplitude)
	ON_EN_CHANGE(IDC_EDIT_SIGNAL_FREQUENCY, OnEnChangeEditSignalFrequency)
	ON_EN_CHANGE(IDC_EDIT_SIGNAL_SAMPLING_TIME, OnEnChangeEditSignalSamplingTime)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


BOOL CSignalDefinerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();


	UpdateData(FALSE);	
	m_ctrNoOfCycles.SetCurSel(m_nSelCycle);

	//Get the CWnd reference to the DMGraph ActiveX control
	m_poDMGraphCtrl = GetDlgItem(IDC_DMGRAPHCTRL);

	vGenerateWave();
	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

// CSignalDefinerDlg message handlers

void CSignalDefinerDlg::OnCbnSelchangeComboSignalType()
{
	vGenerateWave();
}

void CSignalDefinerDlg::OnCbnSelchangeComboCycles()
{
	vGenerateWave();
}

void CSignalDefinerDlg::OnEnChangeEditSignalAmplitude()
{
	vGenerateWave();
}

void CSignalDefinerDlg::OnEnChangeEditSignalFrequency()
{
	vGenerateWave();
}

void CSignalDefinerDlg::OnEnChangeEditSignalSamplingTime()
{		
	vGenerateWave();
}
void CSignalDefinerDlg::OnBnClickedOk()
{
	OnOK();
}

/**
* \brief         Helper to convert degrees to radians
* \param[out]    -
* \param[in]     Degrees as double
* \return        Radians as double
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/

double DegreesToRadians(double dblDegrees)
{
	return 2 * M_PI * dblDegrees / 360.0;
}

/**
* \brief         Helper function to get the Triangle values calculated
* \param[out]    -
* \param[in]     dblXSamplingPoint, fAmplitude, dblTimePeriod 
* \return        double is the Y value calculated
* \authors       Arunkumar Karri
* \date          07.02.2012 Created
*/
double CalculateYatXForTriangleWave(double dblXSamplingPoint,float fAmplitude,double dblTimePeriod)
{
        double dblYResult;

        if ( dblXSamplingPoint <= dblTimePeriod / 4 )
		{
            dblYResult = ((4 * fAmplitude) / dblTimePeriod) * dblXSamplingPoint;
		}
		else if ( dblTimePeriod / 4 < dblXSamplingPoint && dblXSamplingPoint <= dblTimePeriod / 2 )
		{
            dblYResult = fAmplitude - ((((4 * fAmplitude) / dblTimePeriod) * dblXSamplingPoint) - fAmplitude);
		}
        else if ( dblTimePeriod / 2 < dblXSamplingPoint && dblXSamplingPoint <= 0.75 * dblTimePeriod )
		{
            dblYResult = -(((4 * fAmplitude) / dblTimePeriod) * (dblXSamplingPoint - (dblTimePeriod / 2)));
		}
        else if ( 0.75 * dblTimePeriod < dblXSamplingPoint && dblXSamplingPoint <= dblTimePeriod )
		{
            dblYResult = -(fAmplitude - ((((4 * fAmplitude) / dblTimePeriod) * (dblXSamplingPoint - (dblTimePeriod / 2))) - fAmplitude));
		}        
		return dblYResult;
}

/**
* \brief         Generates points based on selected signal type
* \param[out]    -
* \param[in]     Signal Type
* \return        void
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
void CSignalDefinerDlg::vGenerateWave()
{	
	UpdateData();

	SIGNAL_TYPE enSignalType = (SIGNAL_TYPE)(m_ctrSignalType.GetCurSel());
	int nPointCount;
	double dblFrqStep, dblTimePeriod;

	/*Setting the Frequency Resolution to nResolution*/
	dblFrqStep = m_dblSamplingTimePeriod;

	/* Calculate Time period per each cycle */
	dblTimePeriod = 1 / m_fFrequency;

	//Calculate number of points to be plotted
	nPointCount = ((dblTimePeriod * 1000) + (dblFrqStep / 10)) * (m_nSelCycle+1);

	/*For variant packing purpose*/
	CComVariant varrX, varrY;
	varrX.parray = SafeArrayCreateVector(VT_R8, 0, nPointCount);
	if(varrX.parray == NULL)
		return;
	varrX.vt = VT_ARRAY|VT_R8;

	varrY.parray = SafeArrayCreateVector(VT_R8, 0, nPointCount);
	if(varrY.parray == NULL)
		return;
	varrY.vt = VT_ARRAY|VT_R8;

	LONG lngCount = 0;

	/*Currently using the Peak to Peak Amplitude as 0 to 2*Amplitude 
	instead of -Amplitude to +Amplitude*/
	for(double dblCounter=0; dblCounter<nPointCount; dblCounter+=dblFrqStep)	
	{		
		double dblX, dblY; 		
		dblCounter /= 1000;
		switch(enSignalType)
		{
		case SINE_WAVE:
			dblX = dblCounter*1000;				
			
			dblY = m_fAmplitude + 
				   m_fAmplitude * sin( DegreesToRadians(2 * 180 * m_fFrequency * dblCounter) );			

			break;
		case COS_WAVE:
			dblX = dblCounter*1000;				
			
			dblY = m_fAmplitude + 
				   m_fAmplitude * cos( DegreesToRadians(2 * 180 * m_fFrequency * dblCounter) );
			break;
		case TRIANGULAR_WAVE:
			double dblSamplingPoint;
            dblSamplingPoint = dblCounter;
            while ( dblSamplingPoint > dblTimePeriod )
			{
				dblSamplingPoint -= dblTimePeriod;
			}
			dblX = dblCounter*1000;				
			dblY = m_fAmplitude +
				   CalculateYatXForTriangleWave(dblSamplingPoint, m_fAmplitude, dblTimePeriod);

			break;
		}
		HRESULT hr;
		hr = SafeArrayPutElement(varrX.parray, &lngCount, &dblX);
		hr = SafeArrayPutElement(varrY.parray, &lngCount, &dblY);			
		lngCount++;

		dblCounter *= 1000;
	}

	SetGraphData(&varrX, &varrY);
}

/**
* \brief         Plots the element points available onto the Graph
* \param[out]    -
* \param[in]     pvarrX, pvarrY; Takes the element points in the form of VARIANT array
* \return        void
* \authors       Arunkumar Karri
* \date          03.02.2012 Created
*/
void CSignalDefinerDlg::SetGraphData(VARIANT* pvarrX, VARIANT* pvarrY)
{
	CComBSTR bsName("Signal");

	IDMGraphCtrl* pDMGraphCtrl = NULL;
	CComPtr<IDMGraphCollection> spElements;
	CComPtr<IDMGraphElement> spGraphElement;

	if( m_poDMGraphCtrl->m_hWnd != NULL )
    {		
		LPUNKNOWN pUnk = m_poDMGraphCtrl->GetControlUnknown();

		pUnk->QueryInterface(IID_IDMGraphCtrl, (void **) &pDMGraphCtrl);
		if (  pDMGraphCtrl ==NULL )
		{
			return;
		}
	}

	HRESULT hr = pDMGraphCtrl->get_Elements(&spElements);

	//Set the signal name as caption	
	pDMGraphCtrl->put_Caption(m_strSignalName);

	long i, nElementCount = 0;
	BOOL bReplace = FALSE;
	hr = spElements->get_Count(&nElementCount);
	for(i=0; i<nElementCount; i++)
	{
		CComBSTR bsElemName;
		CComPtr<IDispatch> spDispatch;

		hr = spElements->get_Item(i, &spDispatch);
		hr = spDispatch.QueryInterface(&spGraphElement);

		spGraphElement->get_Name(&bsElemName);
		if(_wcsicmp(bsElemName, bsName) == 0)
		{						
			spGraphElement->Plot(*pvarrX, *pvarrY);
			pDMGraphCtrl->AutoRange();
			return;
		}
		else
			spGraphElement = NULL;
	}
	if(bReplace == FALSE || spGraphElement == NULL)
	{
		CComPtr<IDispatch> spDispatch;
		hr = spElements->Add(&spDispatch);
		spGraphElement = NULL;
		hr = spDispatch.QueryInterface(&spGraphElement);
	}

	hr = spGraphElement->put_Name(bsName);

	hr = spGraphElement->put_PointSymbol( Dots );
	hr = spGraphElement->put_PointSize(2);
	hr = spGraphElement->Plot(*pvarrX, *pvarrY);


	hr = pDMGraphCtrl->AutoRange();
}
