// Machine generated IDispatch wrapper class(es) created with Add Class from Typelib Wizard

// CSignalGeneration wrapper class

class CSignalGeneration : public COleDispatchDriver
{
public:
	CSignalGeneration(){} // Calls COleDispatchDriver default constructor
	CSignalGeneration(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	CSignalGeneration(const CSignalGeneration& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

	// Attributes
public:

	// Operations
public:


	// _SignalGeneration methods
public:
	long ShowSignalGenDialog()
	{
		long result;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetSignalType()
	{
		CString result;
		InvokeHelper(0x2, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	double GetAmplitude()
	{
		double result;
		InvokeHelper(0x3, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetFrequency()
	{
		double result;
		InvokeHelper(0x4, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	double GetSamplingTimePeriod()
	{
		double result;
		InvokeHelper(0x5, DISPATCH_METHOD, VT_R8, (void*)&result, NULL);
		return result;
	}
	BOOL SetSignalType(LPCTSTR strSignalType)
	{
		BOOL result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x6, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, strSignalType);
		return result;
	}
	BOOL SetAmplitude(double dblAmplitude)
	{
		BOOL result;
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x7, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dblAmplitude);
		return result;
	}
	BOOL SetFrequency(double dblFrequency)
	{
		BOOL result;
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x8, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dblFrequency);
		return result;
	}
	BOOL SetSamplingTimePeriod(double dblSamplingTime)
	{
		BOOL result;
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x9, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, dblSamplingTime);
		return result;
	}
	BOOL RedrawChart()
	{
		BOOL result;
		InvokeHelper(0xa, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL CreateChartForm()
	{
		BOOL result;
		InvokeHelper(0xb, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	BOOL bIsOKPressed()
	{
		BOOL result;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	double dblGetAmplitudeAtTimeTriangle(double dblXSamplingPoint, double dblAmplitude, double dblTimePeriod)
	{
		double result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_R8, (void*)&result, parms, dblXSamplingPoint, dblAmplitude, dblTimePeriod);
		return result;
	}
	double dblGetAmplitudeAtTimeSine(double dblXSamplingPoint, double dblAmplitude, double dblFrequency)
	{
		double result;
		static BYTE parms[] = VTS_R8 VTS_R8 VTS_R8 ;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_R8, (void*)&result, parms, dblXSamplingPoint, dblAmplitude, dblFrequency);
		return result;
	}

	// _SignalGeneration properties
public:

};
