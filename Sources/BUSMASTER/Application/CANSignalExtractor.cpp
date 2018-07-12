#include "stdafx.h"

#include "Utility/Utility.h"
#include "CANSignalExtractor.h"
#include "CANDefines.h"
#include "ClientManager.h"
#include <sstream>
#define DEF_MINIMUM_TOKENS		6
#define	DEF_SIGNAL_VARIABLE		"SIGNAL"
#define	DEF_CHANNEL_VARIABLE	"CHANNEL"

CANSignalExtractor::CANSignalExtractor()
{
}

CANSignalExtractor::~CANSignalExtractor()
{
}

void CANSignalExtractor::setBmDatabaseInterface(IBMNetWorkGetService* bmDbService)
{
	mBmDbService = bmDbService;
}

bool CANSignalExtractor::bIsVariableExists(std::string variablePath)
{
	std::vector<std::string> variableList;
	tokenize(variablePath, DEF_VARIABLE_PATH_SEPERATOR, variableList);
	if (isValidVaraiables(variableList) == false)
	{
		return false;
	}

	unsigned int msgId;
	unsigned int channelIndex;
	IFrame* frame;
	if (getMessageInfo(variableList, msgId, channelIndex, &frame) == true)
	{
		return true;
	}
	return false;
}

int CANSignalExtractor::isSignalValueValid(std::string &variablePath, double& physical)
{
	std::vector<std::string> variableList;
	tokenize(variablePath, DEF_VARIABLE_PATH_SEPERATOR, variableList);


	if (isValidVaraiables(variableList) == false)
	{
		return false;
	}

	unsigned int msgId;
	unsigned int channelIndex;
	unsigned __int64 unI64RawValue;
	IFrame* frame;
	if (getMessageInfo(variableList, msgId, channelIndex, &frame) == true)
	{
		auto signalInfo = GetSignal(frame, variableList[5]);
		if (nullptr != signalInfo.first)
		{
			signalInfo.first->GetRawValueFromEng(physical, unI64RawValue);			
			signalInfo.first->GetEnggValueFromRaw(unI64RawValue, physical);		

			std::stringstream strEngVal;
			strEngVal << physical;
			physical = std::stod(strEngVal.str());
		}
	}

	return 0;
}
int CANSignalExtractor::getVariableType(std::string &variablePath, unsigned int& unType)
{
	std::vector<std::string> variableList;
	tokenize(variablePath, DEF_VARIABLE_PATH_SEPERATOR, variableList);


	if (isValidVaraiables(variableList) == false)
	{
		return false;
	}

	unsigned int msgId;
	unsigned int channelIndex;
	IFrame* frame;
	if (getMessageInfo(variableList, msgId, channelIndex, &frame) == true)
	{
		auto signalInfo = GetSignal(frame, variableList[5]);
		if (nullptr != signalInfo.first)
		{
			eSignalDataType eDataType;
			signalInfo.first->GetDataType(eDataType);
			unType = (unsigned int)eDataType;
		}
	}
	return false;
}

int CANSignalExtractor::registerVariable(std::string& variablePath, VariableManager::IVariableChangeListner* listner)
{
	std::vector<std::string> variableList;
	tokenize(variablePath, DEF_VARIABLE_PATH_SEPERATOR, variableList);
	

	if (isValidVaraiables(variableList) == false)
	{
		return false;
	}

	VariableChangeListnerInfo listnerInfo;
	listnerInfo.mListner = listner;
    listnerInfo.mRegVariablePath = variablePath;

	mRegisterVariables.push_back(listnerInfo);


	unsigned int msgId;
	unsigned int channelIndex;
	IFrame* frame;
	if (getMessageInfo(variableList, msgId, channelIndex, &frame) == true)
	{
		VariableChangeListnerInfo listnerInfo;
		listnerInfo.mListner = listner;
		listnerInfo.mRegVariablePath = variablePath;
		return mClientManager.AddListner(msgId, channelIndex, variableList[5], listnerInfo, frame);
	}
	return false;
}
int CANSignalExtractor::unRegisterVariable(std::string& variablePath, VariableManager::IVariableChangeListner* listner)
{
	std::vector<std::string> variableList;
	tokenize(variablePath, DEF_VARIABLE_PATH_SEPERATOR, variableList);

	VariableChangeListnerInfo listnerInfo;
	listnerInfo.mListner = listner;
	listnerInfo.mRegVariablePath = variablePath;

	mRegisterVariables.remove(listnerInfo);


	if (isValidVaraiables(variableList) == false)
	{
		return S_FALSE;
	}
	unsigned int msgId;
	unsigned int channelIndex;
	IFrame* frame;
	if (getMessageInfo(variableList, msgId, channelIndex, &frame) == true)
	{
		
		return mClientManager.RemoveListner(msgId, channelIndex, variableList[5], listnerInfo);
	}
	return S_FALSE;
}

int CANSignalExtractor::setVariableValue(VariableData* value, bool isPhysicalValue)
{
	std::vector<std::string> variableList;
	tokenize(value->mVariablePath, DEF_VARIABLE_PATH_SEPERATOR, variableList);


	if (isValidVaraiables(variableList) == false)
	{
		return S_FALSE;
	}

	unsigned int msgId;
	unsigned int channelIndex;
	unsigned int length;
	unsigned char *data;
	//memset(data, 0, sizeof(data));
	IFrame* frame;
	if (getMessageInfo(variableList, msgId, channelIndex, &frame) == true)
	{
		auto existingData = mPreviousData[channelIndex].find(msgId);
		if (existingData != mPreviousData[channelIndex].end())
		{
			data = existingData->second;
		}
		else
		{
			data = new unsigned char[64];
			memset(data, 0, 64);
			mPreviousData[channelIndex][msgId] = data;
		}
		auto signalInfo = GetSignal(frame, variableList[5]);
		if (nullptr != signalInfo.first)
		{
			signalInfo.first->GetLength(length);
			if (false == isPhysicalValue)
			{
				if (value->mValue.mRawValueType == VariableRawValueType::Long)
				{
					vGetDataBytesFromSignal(value->mValue.LongValue, signalInfo.second.m_nStartBit, signalInfo.second.m_ouSignalEndianess, length, data, 64);
				}
				else
				{
					vGetDataBytesFromSignal(value->mValue.ULongValue, signalInfo.second.m_nStartBit, signalInfo.second.m_ouSignalEndianess, length, data, 64);
				}
			}
			else
			{
				unsigned long long rawValue;
				signalInfo.first->GetRawValueFromEng(value->mPhysicalValue, rawValue);
				vGetDataBytesFromSignal(rawValue, signalInfo.second.m_nStartBit, signalInfo.second.m_ouSignalEndianess, length, data, 64);
			}
			if (nullptr != mCanService)
			{
				sendMessage(frame, channelIndex, data, 64);
			}
		}
	}

}
int CANSignalExtractor::onMessageRecieved(STCANDATA& canData)
{
	if (canData.m_ucDataType != ERR_FLAG)
	{
		mClientManager.NotifyVariableChange(canData.m_uDataInfo.m_sCANMsg);
	}
	return S_OK;
}
void CANSignalExtractor::setCANService(CBaseDIL_CAN* canService, int clientid)
{
	mCanService = canService;
	mClientId = clientid;
}
bool CANSignalExtractor::onDatabaseChanged(ETYPE_BUS bus, const std::string& filePath, const std::string& channel, bool bAdded)
{
    if (bus == ETYPE_BUS::CAN && bAdded == false)
    {
        //mClientManager.Clear();
    }
    return true;
}

void CANSignalExtractor::handleBusEvent(Event_Bus_Staus busEvent)
{
	if (busEvent.mBus != ETYPE_BUS::CAN)
	{
		return;
	}
	switch (busEvent.mEventType)
	{
	case ON_CONNECT:
		break;
	case ON_PRE_CONNECT:
		break;
	case ON_DISCONNECT:
		handleBusDisconnect();
		break;
	default:
		break;
	}
}
void CANSignalExtractor::handleBusDisconnect()
{
	for (int i = 0; i < defCHANNEL_CAN_MAX; i++)
	{
		for (auto &itr : mPreviousData[i])
		{
			if (nullptr != itr.second)
			{
				delete itr.second;
			}
		}
		mPreviousData[i].clear();
	}
}
bool CANSignalExtractor::isValidVaraiables(std::vector<std::string>& variableList)
{
	if (variableList.size() < DEF_MINIMUM_TOKENS)
	{
		return false;
	}
	if (!(variableList[0] == DEF_SIGNAL_VARIABLE && variableList[1] == "CAN"))
	{
		return false;
	}
	return true;
}

bool CANSignalExtractor::getMessageInfo(std::vector<std::string>& variableList, unsigned int& msgId, unsigned int& channelIndex, IFrame** frame)
{
	//GetChannel
	std::string channel = variableList[2].replace(variableList[2].find(DEF_CHANNEL_VARIABLE), std::string(DEF_CHANNEL_VARIABLE).size(), "");
	channelIndex = strtoul(channel.c_str(), nullptr, 10);

	ICluster* reqCluster = nullptr;
	std::list<ICluster*> dbList;
	std::string dbName;
	mBmDbService->GetDBServiceList(ETYPE_BUS::CAN, channelIndex, dbList);
	for (auto db : dbList)
	{
		db->GetName(dbName);
		if (variableList[3] == dbName)
		{
			reqCluster = db;
			break;
		}
	}
	if (nullptr != reqCluster)
	{
		IFrame* pMsgElement;
		msgId = (unsigned int)-1;
		reqCluster->GetElementByName(eClusterElementType::eFrameElement, variableList[4], (IElement**)&pMsgElement);
		if (nullptr != pMsgElement)
		{
			pMsgElement->GetFrameId(msgId);
			*frame = pMsgElement;
			return true;
		}
	}
	return false;
}
void CANSignalExtractor::sendMessage(IFrame* frame, unsigned int channel, unsigned char *data, unsigned int dlc)
{
	if (mCanService != nullptr&& nullptr != frame)
	{
		CANFrameProps props;
		frame->GetProperties(props);

		STCAN_MSG msg;
		msg.m_bCANFD = false;	//TODO::
		msg.m_ucChannel = 1;
		msg.m_ucDataLen = props.m_unMsgSize;
		memcpy(msg.m_ucData, data, props.m_unMsgSize);	//TODO::
		msg.m_ucRTR = 0;
		msg.m_ucEXTENDED = (props.m_canMsgType == eCANMsgType::eCan_Extended);
		frame->GetFrameId(msg.m_unMsgID);
		
		mCanService->DILC_SendMsg(mClientId, msg);
	}
}

void CANSignalExtractor::HandleImportInstruments()
{
	mClientManager.HandleImportInstruments();
}

std::pair<ISignal*, SignalInstanse> CANSignalExtractor::GetSignal(IFrame* frame, std::string refSigName)
{
	std::pair<ISignal*, SignalInstanse> signalDetails;
	signalDetails.first = nullptr;
	if (nullptr != frame)
	{
		std::string name;
		std::map<ISignal*, SignalInstanse> signalList;
		frame->GetSignalList(signalList);
		for (auto signal : signalList)
		{
			signal.first->GetName(name);
			if (name == refSigName)
			{
				signalDetails.first = signal.first;
				signalDetails.second = signal.second;
			}
		}
	}
	return signalDetails;
}
