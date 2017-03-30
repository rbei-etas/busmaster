#include "stdafx.h"

#include "Utility/Utility.h"
#include "CANSignalExtractor.h"
#include "CANDefines.h"
#define DEF_MINIMUM_TOKENS		6
#define	DEF_SIGNAL_VARIABLE		"SIGNAL"
#define	DEF_CHANNEL_VARIABLE	"CHANNEL"


bool operator==(const VariableChangeListnerInfo& lhs, const VariableChangeListnerInfo& rhs)
{
	return lhs.mListner == rhs.mListner && lhs.mRegVariablePath == rhs.mRegVariablePath;
}

int ClientManager::AddListner(unsigned int canId, unsigned int channel, std::string variableName, VariableChangeListnerInfo listner, IFrame* frame)
{
	bool clientFound = false;
	for (auto &client : mClientList)
	{
		if (client.mCanId == canId && client.mChannel == channel)
		{
			client.AddLister(variableName, listner);
			clientFound = true;
		}
	}
	if (clientFound == false)
	{
		FrameClient newClient;
		newClient.mCanId = canId;
		newClient.mChannel = channel;
		newClient.mFrame = frame;
		newClient.AddLister(variableName, listner);
		mClientList.push_back(newClient);
        clientFound = true;
	}
    return clientFound;
}
int ClientManager::RemoveListner(unsigned int canId, unsigned int channel, std::string variableName, VariableChangeListnerInfo listner)
{
	//Client* reqClient = nullptr;
	for (auto &client : mClientList)
	{
		if (client.mCanId == canId && client.mChannel == channel)
		{
            client.RemoveLister(variableName, listner);
			return S_OK;
		}
	}
	return S_FALSE;
}

void ClientManager::Clear()
{
    mClientList.clear();
}

//bool ClientManager::RemoveListner(std::string variableName, VariableChangeListnerInfo listner)
//{
//    for (auto &client : mClientList)
//    {
//        if (client.RemoveListner(variableName, listner) == true)
//        {
//            return true;
//        }
//    }
//    return false;
//}

int ClientManager::NotifyVariableChange(const STCAN_MSG& canMsg)
{
	for (auto& client : mClientList)
	{
		if (client.mCanId == canMsg.m_unMsgID && client.mChannel == canMsg.m_ucChannel-1)
		{
			client.NotifyVariableChange(canMsg);
			return S_OK;
		}
	}
	return S_FALSE;
}

int FrameClient::AddLister(std::string variableName, VariableChangeListnerInfo listner)
{
	auto existingListner = mSignalNameListnerList.find(variableName);
	if (existingListner == mSignalNameListnerList.end())
	{
		std::list<VariableChangeListnerInfo > listnerList;
		listnerList.push_back(listner);
		mSignalNameListnerList[variableName] = listnerList;
	}
	else
	{
		existingListner->second.push_back(listner);
	}
	return S_OK;
}
int FrameClient::RemoveLister(std::string variableName, VariableChangeListnerInfo listner)
{
	auto existingListner = mSignalNameListnerList.find(variableName);
	if (existingListner != mSignalNameListnerList.end())
	{
		existingListner->second.remove(listner);
		return S_OK;
	}
	return S_FALSE;
}

int FrameClient::NotifyVariableChange(const STCAN_MSG& canMsg)
{
	VariableData data;
	
	if (nullptr != mFrame)
	{
		std::vector<SignalValue> signalList;
		mFrame->InterpretSignals(canMsg.m_ucData, canMsg.m_ucDataLen, signalList);
		for (auto& interPretedSignal : signalList)
		{
			auto &clientList = mSignalNameListnerList.find(interPretedSignal.mName);
			if (clientList != mSignalNameListnerList.end())
			{
				
				data.mValue.mRawValueType = Ulong;
				data.mValue.ULongValue = interPretedSignal.mUnValue;// _strtoui64(interPretedSignal.m_omRawValue.c_str(), nullptr, 10);
				data.mPhysicalValue = interPretedSignal.mPhyicalValue;// strtod(interPretedSignal.m_omEnggValue.c_str(), nullptr);
				
				for (auto &client : clientList->second)
				{
					data.mVariablePath = client.mRegVariablePath;
					if (nullptr != client.mListner)
					client.mListner->OnVariableChange(&data);
					
				} 
			}
		}
		return S_OK;
	}
	return S_FALSE;
}


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
				vGetDataBytesFromSignal(value->mValue.ULongValue, signalInfo.second.m_nStartBit, signalInfo.second.m_ouSignalEndianess, length, data, 64);
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
        mClientManager.Clear();
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
