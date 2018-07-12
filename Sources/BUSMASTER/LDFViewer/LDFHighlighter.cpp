
#include "LDFHighlighter.h"


LDFHighlighter::LDFHighlighter(QTextDocument* parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(QBrush(QColor(0,0,255)));

    QStringList keywordPatterns;


    keywordPatterns << "\\bLIN_description_file\\b"
                    << "\\bLIN_protocol_version\\b"
                    <<"\\bLIN_language_version\\b"
                    <<"\\bLIN_speed\\b"
                    <<"\\bNodes\\b"
                    <<"\\bMaster\\b"
                    <<"\\bSlaves\\b"
                    <<"\\bSignals\\b"
                    <<"\\bDiagnostic_signals\\b"
                    <<"\\bFrames\\b"
                    <<"\\bSporadic_frames\\b"
                    <<"\\bEvent_triggered_frames\\b"
                    <<"\\bDiagnostic_frames\\b"
                    <<"\\bNode_attributes\\b"
                    <<"\\bSchedule_tables\\b"
                    <<"\\bSignal_encoding_types\\b"
                    <<"\\bSignal_representation\\b"
                    <<"\\blogical_value\\b"
                    <<"\\bphysical_value\\b"
                    <<"\\bconfigurable_frames\\b"
                    <<"\\bLIN_protocol\\b"
                    <<"\\bconfigured_NAD\\b"
                    <<"\\binitial_NAD\\b"
                    <<"\\bproduct_id\\b"
                    <<"\\bP2_min\\b"
                    <<"\\bST_min\\b"
                    <<"\\bN_As_timeout\\b"
                    <<"\\bN_Cr_timeout\\b"
                    <<"\\bMasterReq\\b"
                    <<"\\bSlaveResp\\b"
                    <<"\\bAssignFrameId\\b"
                    <<"\\bUnAssignFrameId\\b"
                    <<"\\bAssignNAD\\b"
                    <<"\\bConditionalChangeNAD\\b"
                    <<"\\bDataDump\\b"
                    <<"\\bSaveConfiguration\\b"
                    <<"\\bAssignFrameIdRange\\b"
                    <<"\\bFreeFormat\\b"
                    <<"\\bReadByIdentifier\\b"
                    <<"\\bresponse_error\\b"
                    <<"\\bfault_state_signals\\b"
                    <<"\\bdelay\\b"
                    <<"\\bms\\b"
                    <<"\\bkbps\\b"
                    ;


    IdentifierStart.setForeground(Qt::red);
    rule.pattern = QRegExp("[0-9][0-9]*");
    rule.format = IdentifierStart;
    highlightingRules.append(rule);



    IdentifierStart.setForeground(Qt::black);
    rule.pattern = QRegExp("[a-zA-Z_][a-zA-Z0-9_]*");
    rule.format = IdentifierStart;
    highlightingRules.append(rule);

    IdentifierStart.setForeground(Qt::red);
    rule.pattern = QRegExp("(0x|0X)[0-9a-fA-F]*");
    rule.format = IdentifierStart;
    highlightingRules.append(rule);

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);

        singleLineCommentFormat.setForeground(Qt::darkGreen);
        rule.pattern = QRegExp("//[^\n]*");
        rule.format = singleLineCommentFormat;
        highlightingRules.append(rule);


        quotationFormat.setForeground(Qt::red);
        rule.pattern = QRegExp("\".*\"");
        rule.format = quotationFormat;
        highlightingRules.append(rule);


        quotationFormat.setForeground(Qt::red);
        rule.pattern = QRegExp("\".*\"");
        rule.format = quotationFormat;
        highlightingRules.append(rule);

        multiLineCommentFormat.setForeground(Qt::darkGreen);


        commentStartExpression = QRegExp("/\\*");
        commentEndExpression = QRegExp("\\*/");
    }
}

void LDFHighlighter::highlightBlock(const QString& text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }

    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
    {
        startIndex = commentStartExpression.indexIn(text);
    }

    while (startIndex >= 0)
    {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}

