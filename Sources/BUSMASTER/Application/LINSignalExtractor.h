#pragma once
#include <vector>
#include <string>
#include "IBMNetWorkGetService.h"
#include "LINDriverDefines.h"
#include "BaseDIL_LIN.h"
#include "IVariableLayer.h"
#include "ClientManager.h"
#include "BusmasterEvents.h"
#include <concurrent_vector.h>
#include <concurrent_unordered_map.h>
using namespace VariableManager;

class LINSignalExtractor
{
	IBMNetWorkGetService* mBmDbService = nullptr;
	ClientManager mClientManager;
	std::list<VariableChangeListnerInfo> mRegisterVariables;

	std::map<int, unsigned char*> mPreviousData[defNO_OF_LIN_CHANNELS];

	CBaseDIL_LIN* mLINService = nullptr;
	int mClientId = 0;
public:
	LINSignalExtractor();
	~LINSignalExtractor();

	void setBmDatabaseInterface(IBMNetWorkGetService* bmDbService);
	int LINSignalExtractor::getVariableType(std::string &variablePath, unsigned int& unType);
	int LINSignalExtractor::isSignalValueValid(std::string &variablePath, double& physical);
	int registerVariable(std::string&, VariableManager::IVariableChangeListner*);
	int unRegisterVariable(std::string& variablePath, VariableManager::IVariableChangeListner*);
	int setVariableValue(VariableData* value, bool isPhysicalValue);
	
	int onMessageRecieved(STLINDATA& canData);
	void setLINService(CBaseDIL_LIN* mCanService, int mClientid);
    bool onDatabaseChanged(ETYPE_BUS bus, const std::string& filePath, const std::string& channel, bool bAdded);

	void handleBusEvent(Event_Bus_Staus busEvent);
	void HandleImportInstruments();

	bool bIsVariableExists(std::string variablePath);
private:
	bool isValidVaraiables(std::vector<std::string>&);
	bool getMessageInfo(std::vector<std::string>&, unsigned int& msgId, unsigned int& channelIndex, IFrame** frame);
	std::pair<ISignal*, SignalInstanse> GetSignal(IFrame*, std::string strSigName);
	void sendMessage(IFrame* frame, unsigned int channel, unsigned char *data, unsigned int dlc);
	void handleBusDisconnect();
};

