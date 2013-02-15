#include <windows.h>
#include "CANIncludes.h"

INT output_CAPL(void* ouMsg)
{
    return SendMsg(*((STCAN_MSG*)ouMsg));
}
INT output_CAPL()
{
    char pchText[] = "Error Message";
    Trace(pchText);
    return 0;
}

UINT GetCaplKeyValue(UCHAR ucKeyVal)
{
    UINT unCaplValue;
    switch(ucKeyVal)
    {
        case BMKEY_BACK:
            unCaplValue =  0X8;
            break;
        case BMKEY_TAB:
            unCaplValue =  0x9;
            break;
        case BMKEY_RETURN:
            unCaplValue =  0xD;
            break;
        case BMKEY_PAGEUP:
            unCaplValue =  0x4900;
            break;
        case BMKEY_PAGEDOWN:
            unCaplValue =  0x5100;
            break;
        case BMKEY_END:
            unCaplValue =  0x4F00;
            break;
        case BMKEY_HOME:
            unCaplValue =  0X4700;
            break;
        case BMKEY_LEFTARROW:
            unCaplValue =  0x4B00;
            break;
        case BMKEY_UPARROW:
            unCaplValue =  0x4800;
            break;
        case BMKEY_RIGHTARROW:
            unCaplValue =  0x4D00;
            break;
        case BMKEY_DOWNARROW:
            unCaplValue =  0x5000;
            break;
        case BMKEY_INSERT:
            unCaplValue =  0x5200;
            break;
        case BMKEY_DELETE:
            unCaplValue =  0x5300;
            break;
        case BMKEY_F1:
            unCaplValue =  0x5400;
            break;
        case BMKEY_F2:
            unCaplValue =  0x5500;
            break;
        case BMKEY_F3:
            unCaplValue =  0x5600;
            break;
        case BMKEY_F4:
            unCaplValue =  0x5700;
            break;
        case BMKEY_F5:
            unCaplValue =  0x5800;
            break;
        case BMKEY_F6:
            unCaplValue =  0x5900;
            break;
            /*case BMKEY_F7:
                unCaplValue =  0x5100;
                break;
            case BMKEY_F8:
                unCaplValue =  0x5100;
                break;
            case BMKEY_F9:
                unCaplValue =  0x5100;
                break;*/
        case BMKEY_F10:
            unCaplValue =  0x5D00;
            break;
        case BMKEY_F11:
            unCaplValue =  0x8700;
            break;
        case BMKEY_F12:
            unCaplValue =  0x8800;
            break;
        default:
            unCaplValue = ucKeyVal;
            break;
    }//switch
    return unCaplValue;
}
