#pragma once
#include "ImageDataSet.h"
#include "IDataSetFactory.h"

class ImageDataSetFactory :
	public IDataSetFactory
{
	public:
		ImageDataSetFactory(void);
		~ImageDataSetFactory(void);
		ImageDataSet* createObject(jsonData& objectParams) const;

};

