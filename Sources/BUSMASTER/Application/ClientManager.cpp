#include "stdafx.h"

#include "Utility/Utility.h"
#include "CANSignalExtractor.h"
#include "CANDefines.h"
#include "ClientManager.h"
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
			client.mFrame = frame;
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

void ClientManager::HandleImportInstruments()
{
	for (auto &client : mClientList)
	{
		client.HandleImportInstruments();
		break;
	}
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

int ClientManager::NotifyVariableChange(const STLIN_MSG& canMsg)
{
	for (auto& client : mClientList)
	{
		if (client.mCanId == canMsg.m_ucMsgID && client.mChannel == canMsg.m_ucChannel - 1
			&& canMsg.m_ucMsgTyp == 1)
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
	m_mapPreviousSignalData.clear(); // Clear the map so that the other instruments gets the value updated
	return S_OK;
}
int FrameClient::RemoveLister(std::string variableName, VariableChangeListnerInfo listner)
{
	auto existingListner = mSignalNameListnerList.find(variableName);
	if (existingListner != mSignalNameListnerList.end())
	{
		existingListner->second.remove(listner);
		m_mapPreviousSignalData.clear(); // Clear the map so that the other instruments gets the value updated
		return S_OK;
	}
	return S_FALSE;
}

void FrameClient::HandleImportInstruments()
{
	m_mapPreviousSignalData.clear();
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
				if (interPretedSignal.mIsSigned == true)
				{
					data.mValue.mRawValueType = Long;
					data.mValue.LongValue = interPretedSignal.mValue;
				}
				else
				{
					data.mValue.mRawValueType = Ulong;
					data.mValue.ULongValue = interPretedSignal.mUnValue;
				}
				data.mPhysicalValue = interPretedSignal.mPhyicalValue;
				
				//bool bValueChanged = true;
				for (auto &client : clientList->second)
				{
					data.mVariablePath = client.mRegVariablePath;
					if (nullptr != client.mListner)
					{						
						bool bValueChanged = true;
						std::map<std::string, SignalValue>::iterator itr = m_mapPreviousSignalData.find(data.mVariablePath);
						if (itr != m_mapPreviousSignalData.end())
						{
							if (itr->second.mIsSigned == true)
							{
								if (itr->second.mValue == interPretedSignal.mValue)
								{
									bValueChanged = false;
								}
							}
							else
							{
								if (itr->second.mUnValue == interPretedSignal.mUnValue)
								{
									bValueChanged = false;
								}
							}
						}

						if (true == bValueChanged)
						{
							client.mListner->OnVariableChange(&data);
							m_mapPreviousSignalData[data.mVariablePath] = interPretedSignal;
						}
					}					
				} 
			}
		}
		return S_OK;
	}
	return S_FALSE;
}

int FrameClient::NotifyVariableChange(const STLIN_MSG& canMsg)
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
				if (interPretedSignal.mIsSigned == true)
				{
					data.mValue.mRawValueType = Long;
					data.mValue.LongValue = interPretedSignal.mValue;
				}
				else
				{
					data.mValue.mRawValueType = Ulong;
					data.mValue.ULongValue = interPretedSignal.mUnValue;
				}
				data.mPhysicalValue = interPretedSignal.mPhyicalValue;

				//data.mValue.mRawValueType = Ulong;
				//data.mValue.ULongValue = interPretedSignal.mUnValue;// _strtoui64(interPretedSignal.m_omRawValue.c_str(), nullptr, 10);
				//data.mPhysicalValue = interPretedSignal.mPhyicalValue;// strtod(interPretedSignal.m_omEnggValue.c_str(), nullptr);

				for (auto &client : clientList->second)
				{
					data.mVariablePath = client.mRegVariablePath;
					if (nullptr != client.mListner)
					{
						bool bValueChanged = true;
						std::map<std::string, SignalValue>::iterator itr = m_mapPreviousSignalData.find(data.mVariablePath);
						if (itr != m_mapPreviousSignalData.end())
						{
							if (itr->second.mIsSigned == true)
							{
								if (itr->second.mValue == interPretedSignal.mValue)
								{
									bValueChanged = false;
								}
							}
							else
							{
								if (itr->second.mUnValue == interPretedSignal.mUnValue)
								{
									bValueChanged = false;
								}
							}
						}

						if (true == bValueChanged)
						{
							client.mListner->OnVariableChange(&data);
							m_mapPreviousSignalData[data.mVariablePath] = interPretedSignal;
						}
					}
				}
			}
		}
		return S_OK;
	}
	return S_FALSE;
}
