/*
* Copyright Robert Bosch Engineering and Business Solutions Private Limited. All Rights Reserved.
*/
#pragma once

#include "FlexRayEnums.h"
#include "CommonDefines.h"
#include "CCompuMethod.h"

//FlexRay
class  FlxRayCompuMethods
{
public:
    CCompuMethod m_ouCompuMethod;
};



class FlexRayFrameProps
{
public:
    unsigned int m_unSlotId;
    unsigned int m_unBaseCycle;
    eFlexRaySlotType m_oueSlotType ;
    unsigned int m_unRepetition;
    unsigned int m_unLength;
    eFlexRayChannel m_oueChannel;
    eFlexRayFrameType m_oueFlexRayFrameType;
};

class WakeUp
{
public:
    short m_shWAKE_UP_SYMBOL_RX_IDLE;   // 14 to 59 inclusive
    short m_shWAKE_UP_SYMBOL_RX_LOW;    // 10 to 55 inclusive
    short m_shWAKE_UP_SYMBOL_RX_WINDOW; // 76 to 301 inclusive
    short m_shWAKE_UP_SYMBOL_TX_IDLE;   // 45 to 180 inclusive
    short m_shWAKE_UP_SYMBOL_TX_LOW;    // 15 to 60 inclusive
};

class FlexRaySettings
{
public:
    std::string m_omProtocolVersion;
    std::string m_omName;
    unsigned int m_unSpeed;
    short m_shCOLD_START_ATTEMPTS;
    short m_shACTION_POINT_OFFSET;
    short m_shDYNAMIC_SLOT_IDLE_PHASE;
    short m_shMINISLOT;
    short m_shMINISLOT_ACTION_POINT_OFFSET;
    short m_shNIT;

    /**
     * float enum - .05,.025,.0125
     */
    float m_fSAMPLE_CLOCK_PERIOD;

    /**
     * 4 to 659 inclusive
     */
    short m_shSTATIC_SLOT;

    /**
     * 0 to 139 inclusive
     */
    short m_shSYMBOL_WINDOW;

    /**
     * 3 to 15 inclusive
     */
    short m_shTSS_TRANSMITTER;
    WakeUp m_ouWAKEUP;

    /**
     * 2 to 16 inclusive
     */
    short m_shLISTEN_NOISE;

    /**
     * 10 to 16000 inclusive
     */
    short m_shMACRO_PER_CYCLE;

    /**
     * Min 1
     */
    float m_fMACROTICK;

    /**
     * 0 to 11.7 inclusive
     */
    float m_fMAX_INITIALIZATION_ERROR;

    /**
     * 1 to 15
     */
    short m_shMAX_WITHOUT_CLOCK_CORRECTION_FATAL;

    /**
     * 1 to 15
     */
    short m_shMAX_WITHOUT_CLOCK_CORRECTION_PASSIVE;

    /**
     * Min 0, default 0, 0 to 12
     */
    short m_shNETWORK_MANAGEMENT_VECTOR_LENGTH;

    /**
     * 0 to 7986
     */
    short m_shNUMBER_OF_MINISLOTS;

    /**
     * 2 to 1023
     */
    short m_shNUMBER_OF_STATIC_SLOTS;

    /**
     * 9 to 15999
     */
    short m_shOFFSET_CORRECTION_START;

    /**
     * 0 to 127
     */
    short m_shPAYLOAD_LENGTH_STATIC;

    /**
     * 2 to 15
     */
    short m_shSYNC_NODE_MAX;

    /**
     * Min. 0, short 67 to 99
     */
    short m_shCAS_RX_LOW_MAX;

    /**
     * Min 0, float enum .1,.2 or .4
     */
    float m_fBIT;

    /**
     * 10 - 16000
     */
    short m_shCYCLE;

    /**
     * 0 to 5
     */
    short m_shCLUSTER_DRIFT_DAMPING;
};

class FlexRayEcuProperties
{
public:

    eFlexRayKeySlotType  m_eKeySlotUsage;
    short               m_shMaxDynPayloadLgt;
    /**
    */
    short               m_shClusterDriftDamping;
    /**
    */
    int                 m_nDecodingCorr;
    /**
    *
    */
    int                 m_nListenTimeOut;
    /**
    */
    short               m_shMaxDrift;
    /**
    */
    short               m_shExternOffsetCorr;
    /**
    */
    short               m_shExternRateCorr;
    /**
    */
    short               m_shLatestTx;
    /**
    */
    int                 m_nMicroPreCycle;
    /**
    */
    short               m_shOffsetCorrOut;
    /**
    */
    short               m_shRateCorrOut;
    /**
    */
    short               m_shSamplePerMicrotick;
    /**
    */
    short               m_shDelayCompensationA;
    /**
    */
    short               m_shDelayCompensationB;
    /**
    */
    short               m_shWakeUpPattern;
    /**
    */
    bool                m_bAllowHaltDewToClock;
    /**
    */
    short               m_shAllowPassiveToActive;
    /**
    */
    short               m_shAcceptedStartUpRange;
    /**
    */
    short               m_shMacroInitialOffsetA;
    /**
    */
    short               m_shMacroInitialOffsetB;
    /**
    */
    short               m_shMicroInitialOffsetA;
    /**
    */
    short               m_shMicroInitialOffsetB;
    /**
    */
    bool                m_bSingleSlotEnable;
    float               m_fMicrotick;
    float               m_fMicroPerMacroNom;

    int                 m_nChannels;        //Channels to which the node is connected (A, B or both).

    int                 m_nKeySlotId;       //ID of the slot used to transmit the startup frame, sync frame, or designated single slot frame.

    int                 m_nWakeUpChannel;   //Channel used by the node to send a wakeup pattern (A or B).

    FlexRayEcuProperties()
    {
        m_eKeySlotUsage = eNone;
    }
    void Clear();
};





class FlexRaySignalProps
{
public:
    int m_nLength;
    eSignalType m_ouSignalType;
    eSignalDataType m_ouDataType;
    //eEndianess m_ouEndianess;
    std::string m_omUnit;
    //eLinSignalValueType m_ouValueType;
    unsigned __int64 m_nIntialValue;
};


class FlexRayPduProps
{
public:
    unsigned int m_unByteLength;
    eFlexRayChannel m_oueChannel;
};
