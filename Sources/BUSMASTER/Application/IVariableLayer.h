#pragma once
#include <string>

#define DEF_VARIABLE_PATH_SEPERATOR		':'

namespace VariableManager
{
	enum VariableType
	{
		Invalid,
		Signal,
		Pdu,
		Message
	};

	enum VariableRawValueType
	{
		Long,
		Ulong,
		Physical,
		ByteArray,
	};

	class VariableRawValue
	{
	public:
		VariableRawValueType mRawValueType;
		__int64 LongValue;
		unsigned __int64 ULongValue;
	};

	class VariableData
	{
	public:
		std::string mVariablePath;
		VariableType mVariableType;
		VariableRawValue mValue;
		double mPhysicalValue;
	};

	class IVariableChangeListner
	{
	public:
		virtual void OnVariableChange(VariableData*) = 0;
	};


	class IVariableLayer
	{
	public:
		virtual bool isVariableExists(std::string& variablePath) = 0;
		virtual int registerVariable(std::string& variablePath, IVariableChangeListner*, bool forTx, bool onUpdateOnly) = 0;
		virtual int UnRegisterVariable(std::string& variablePath, IVariableChangeListner*) = 0;
		virtual int setVariableValue(VariableData* value, bool isPhysicalValue) = 0;
	};
};