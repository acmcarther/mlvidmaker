#pragma once
#include "BaseDataParams.h"
#include <SDL_image.h>

class IDataSet
{
	protected:
		int x,y,xSize,ySize,frameRate;

	public:
		virtual void notifyRender(SDL_Surface* screenSurface){}
		virtual bool notifyUpdate(int frameNumber){return false;}
};