#pragma once
#include <unordered_map>
#include "SDLManager.h"
#include "IDataSetFactory.h"
#include <string>
#include <fstream>
#include <rapidjson\filestream.h>
#include <rapidjson\document.h>
#include <rapidjson\prettywriter.h>

class JsonLoader
{
	private:
		std::unordered_map<std::string, IDataSetFactory*> factoryMap;
		SDLManager *sdlMain;
	public:
		JsonLoader(SDLManager *sdlMain);
		~JsonLoader(void);
		bool parseFileToSDL(std::string filepath);
		void registerFactory(std::string key, IDataSetFactory* factory);
};

