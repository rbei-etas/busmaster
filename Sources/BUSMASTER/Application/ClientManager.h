#pragma once
#include <vector>
#include <string>
#include "IBMNetWorkGetService.h"
#include "CANDriverDefines.h"
#include "LINDriverDefines.h"
#include "BaseDIL_CAN.h"
#include "BaseDIL_LIN.h"
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

bool operator==(const VariableChangeListnerInfo& lhs, const VariableChangeListnerInfo& rhs);

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
	int NotifyVariableChange(const STLIN_MSG& canMsg);
	void HandleImportInstruments();

private:
	std::map<std::string, SignalValue> m_mapPreviousSignalData;
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
	int NotifyVariableChange(const STLIN_MSG& canMsg);
	void HandleImportInstruments();
};