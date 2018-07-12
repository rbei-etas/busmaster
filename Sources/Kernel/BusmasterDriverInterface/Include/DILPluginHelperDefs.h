#pragma once
#define LOG_ERR_MSG()
#define VALIDATE_POINTER_RETURN_VAL(Ptr, RetVal)        if (Ptr == nullptr) {return RetVal;}
#define VALIDATE_VALUE_RETURN_VAL(Val1, Val2, RetVal)   if (Val1 != Val2) {return RetVal;}
#define VALIDATE_POINTER_RETURN_VOID(Ptr)               if (Ptr == nullptr) {return;}
#define VALIDATE_POINTER_NO_RETURN_LOG(Ptr)             if (Ptr == nullptr) {LOG_ERR_MSG();}
#define VALIDATE_POINTER_RETURN_VOID_LOG(Ptr)           if (Ptr == nullptr) {LOG_ERR_MSG(); return;}
#define VALIDATE_POINTER_RETURN_VALUE_LOG(Ptr, RetVal)  if (Ptr == nullptr) {LOG_ERR_MSG(); return RetVal;}

#define defBASE_DEC                         10
#define defBASE_HEX                         16
#define defBITS_IN_BYTE                     8
#define defBAUD_RATE_VAL                    500
#define defBAUD_RATE                        0xC03A // 500 KBPS
#define defClockVal                         16
#define defSamplingVal                      1
#define defWARNING_LIMIT_INT                96

struct SACC_FILTER_INFO
{
    unsigned char m_ucACC_Filter_Type;
    unsigned char m_ucACC_Code0;
    unsigned char m_ucACC_Code1;
    unsigned char m_ucACC_Code2;
    unsigned char m_ucACC_Code3;
    unsigned char m_ucACC_Mask0;
    unsigned char m_ucACC_Mask1;
    unsigned char m_ucACC_Mask2;
    unsigned char m_ucACC_Mask3;
};

typedef SACC_FILTER_INFO* PSACC_FILTER_INFO;

class CHANNEL_DETAILS
{
public:
    CHANNEL_DETAILS()
    {
        m_omChannelName = "";
        m_omChannelIndex = 0;
        m_omFirmware = "";
    }
    unsigned long m_omVendorId;
    std::string m_omChannelName;
    unsigned int m_omChannelIndex;
    std::string m_omFirmware;
};
class SELECTED_CHANNELS
{
public:
    SELECTED_CHANNELS()
    {
        m_nChannelCount = 0;
    }
    std::string m_omHardwareChannel[32];
    CHANNEL_DETAILS m_ouChannelDetails[32];
    unsigned int m_nChannelCount;
};
static SELECTED_CHANNELS sg_HardwareList;
static SELECTED_CHANNELS sg_SelectedChannels;