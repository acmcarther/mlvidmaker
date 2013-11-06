#pragma once
#include "idatasetfactory.h"
#include "LabelDataSet.h"
class LabelDataSetFactory :
	public IDataSetFactory
{
public:
	LabelDataSetFactory(void);
	~LabelDataSetFactory(void);
	LabelDataSet* createObject(jsonData& objectParams) const;
};

