#pragma once
#include <vector>
#include <string>
#include "IBMNetWorkGetService.h"
#include "CANDriverDefines.h"
#include "BaseDIL_CAN.h"
#include "IVariableLayer.h"
#include "BusmasterEvents.h"
#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>
using namespace VariableManager;

class VariableChangeListnerInfo
{
public:
	IVariableChangeListner* mListner = nullptr;
	std::string mRegVariablePath = "";
};


class FrameClient
{
public:
	unsigned int mCanId;
	unsigned int mChannel;
	IFrame* mFrame;
    //Signal - Client
	Concurrency::concurrent_unordered_map<std::string, std::list<VariableChangeListnerInfo>> mSignalNameListnerList;

	int AddLister(std::string variableName, VariableChangeListnerInfo listner);
	int RemoveLister(std::string variableName, VariableChangeListnerInfo listner);
	int NotifyVariableChange(const STCAN_MSG& canMsg);
};

class ClientManager
{
	//Frame - SignalClients
	Concurrency::concurrent_vector<FrameClient> mClientList;
public:
	int AddListner(unsigned int canId, unsigned int channel, std::string variableName, VariableChangeListnerInfo listner, IFrame* frame);
    int RemoveListner(unsigned int canId, unsigned int channel, std::string variableName, VariableChangeListnerInfo listner);
    //bool RemoveListner(std::string variableName, VariableChangeListnerInfo listner);
    void Clear();
	int NotifyVariableChange(const STCAN_MSG& canMsg);
};



class CANSignalExtractor
{
	IBMNetWorkGetService* mBmDbService = nullptr;
	ClientManager mClientManager;
	std::list<VariableChangeListnerInfo> mRegisterVariables;

	std::map<int, unsigned char*> mPreviousData[defCHANNEL_CAN_MAX];

	CBaseDIL_CAN* mCanService = nullptr;
	int mClientId = 0;
public:
	CANSignalExtractor();
	~CANSignalExtractor();

	void setBmDatabaseInterface(IBMNetWorkGetService* bmDbService);
	int registerVariable(std::string&, VariableManager::IVariableChangeListner*);
	int unRegisterVariable(std::string& variablePath, VariableManager::IVariableChangeListner*);
	int setVariableValue(VariableData* value, bool isPhysicalValue);
	
	int onMessageRecieved(STCANDATA& canData);
	void setCANService(CBaseDIL_CAN* mCanService, int mClientid);
    bool onDatabaseChanged(ETYPE_BUS bus, const std::string& filePath, const std::string& channel, bool bAdded);

	void handleBusEvent(Event_Bus_Staus busEvent);
private:
	bool isValidVaraiables(std::vector<std::string>&);
	bool getMessageInfo(std::vector<std::string>&, unsigned int& msgId, unsigned int& channelIndex, IFrame** frame);
	std::pair<ISignal*, SignalInstanse> GetSignal(IFrame*, std::string strSigName);
	void sendMessage(IFrame* frame, unsigned int channel, unsigned char *data, unsigned int dlc);
	void handleBusDisconnect();
};

