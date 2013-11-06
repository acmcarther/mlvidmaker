#pragma once
#include "BaseDataParams.h"

class IDataSet
{
	protected:
		int x,y,xSize,ySize,frameRate;

	public:
		virtual void notifyRender(){}
		virtual bool notifyUpdate(int frameNumber){return false;}
};