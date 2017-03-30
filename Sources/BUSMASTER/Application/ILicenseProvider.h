#pragma once
#include <ctime>

class CLicenseDetails
{
public:
	std::string strAddOnName;
	int nLicenseValidity;
	tm tmDateTime;
	std::string strDateTime;
	int nValidLicense = 0; // 0 - InValid License, 1 - Valid License

	CLicenseDetails()
	{
		strAddOnName = "";
		nLicenseValidity = 0;
		strDateTime = "";
		nValidLicense = 0;
	}
	~CLicenseDetails()
	{

	}
};

class ILicenseProvider
{
protected:
	
public:
	ILicenseProvider()
	{
		
	};

	virtual void getLicenseDetails(CLicenseDetails &oulicenseDetails) = 0;

	virtual ~ILicenseProvider(){};
};
