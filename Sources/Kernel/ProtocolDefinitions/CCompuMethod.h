#pragma once

class  ValueRange
{
public:
    unsigned long long m_dwLowerLimit;      //RAW Value
    unsigned long long m_dwUpperLimit;
};

enum Validity
{
    eValid,
    eNotValid,
    eNotAvailable,
    eValidityNone
};

enum eCompuType
{
    IDENTICAL_ENUM = 0,              //No change
    LINEAR_ENUM,                     //Linear expression N0,N1,D1
    SCALE_LINEAR_ENUM,               //More than 1 Linear exp
    TEXTTABLE_ENUM,                  //Description for values
    SCALE_LINEAR_TEXTTABLE_ENUM,     //Linear with Descriptin
    TAB_NOINTP_ENUM,                 //1 to 1 mapping for some values
    FORMULA_ENUM                     //User defined formula
};

class  CompuIdenticalCode
{
public:
    ValueRange   m_sRange;
    Validity m_eValidity;
} ;

class  CompuLinearCode
{

public:

    ValueRange m_sRange;
    double m_dN0;   // AppValue =
    double m_dN1;   // (m_dN0 // offset + m_dN1 //factor * Trans. Val) / m_dD0
    double m_dD0;
};

//For textable
class  CompuTextable
{
public:
    ValueRange m_sRange;
    std::string m_aTextName;
    CompuTextable& operator=(const CompuTextable& RefObj)
    {
        m_sRange = RefObj.m_sRange;
        m_aTextName = RefObj.m_aTextName;
        return *this;
    }
};

//For TAB-NOINTP
class  CompuTabCode
{
public:

    ValueRange m_sRange;
    double m_dPhysVal;
};

/*class  FormulaOperands
{
    char m_cOperandName;
    float m_fOperandValue;
    FormulaOperands* m_pNextOpearnd;
public:
    FormulaOperands();
    FormulaOperands& operator=(const FormulaOperands& RefObj);
    ~FormulaOperands();
};*/

/*class  CompuFormula
{
    ValueRange m_sRange;
    std::string m_omFormula;
    FormulaOperands m_ouFirstOperand;
public:
    CompuFormula();

    CompuFormula& operator=(const CompuFormula& RefObj);

    ~CompuFormula();

    std::string strGetEnggValue(unsigned long  dwRawValue);

    bool bGetEnggValue(unsigned long  dwRawValue, double& dEnggValue) const;

};*/

//Only one of the above struct will be assigned to each element
class  CCompuMethod
{
public:
    //CompuType
    eCompuType  m_eCompuType;

    //Compu Methods
    std::list<CompuTabCode> m_ouTabCode;
    std::list<CompuTextable> m_ouTextCode;
    CompuLinearCode m_ouLinearCode;
    std::list<CompuIdenticalCode> m_ouIdenticalCode;
    std::list<CompuLinearCode> m_ouCompuScaleLinear;
    //CompuFormula m_ouFormula;
    CCompuMethod& operator=(const CCompuMethod& RefObj)
    {
        m_eCompuType = RefObj.m_eCompuType;
        m_ouLinearCode = RefObj.m_ouLinearCode;                 //TODO COPY
        m_ouIdenticalCode = RefObj.m_ouIdenticalCode;
        //m_ouFormula = RefObj.m_ouFormula;
        m_ouTabCode = RefObj.m_ouTabCode;
        m_ouTextCode = RefObj.m_ouTextCode;
        m_ouCompuScaleLinear = RefObj.m_ouCompuScaleLinear;
        return *this;
    }
};

