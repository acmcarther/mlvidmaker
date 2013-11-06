#pragma once
#include "BaseDataParams.h"
#include "IDataSet.h"
#include <rapidjson\document.h>

typedef rapidjson::GenericValue<rapidjson::UTF8<char>, rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator>> jsonData;

class IDataSetFactory
{
	protected:
	public:
		virtual IDataSet* createObject(jsonData& objectParams) const =0;
};