#include "JsonLoader.h"

#include <iostream>

JsonLoader::JsonLoader(SDLManager *sdlIn)
{
	// Save sdl here for registering observers
	sdlMain = sdlIn;
}


JsonLoader::~JsonLoader(void)
{
	// We don't release SDL here because it is from main

	// But we'd better release the factories
	for ( auto it = factoryMap.begin(); it != factoryMap.end(); ++it )
	{
		delete it->second;
	}

}

bool JsonLoader::parseFileToSDL(std::string filepath)
{
	// Declare function values
	FILE* pFile;
	bool hasObjects;
	int objectCounter;
	
	// Open the File. Appease VS2012, but keep crossplat
	#ifdef WIN32
    errno_t err;
    if( (err  = fopen_s( &pFile, filepath.c_str(), "rb" )) !=0 )
	#else
    if ((pFile = fopen(filepath.c_str(), "rb")) == NULL)
	#endif
	{
		std::cout << filepath << " is not in the executable directory. Please put the json file there." << std::endl;
		return false;
	}

	// Define the rest of the rapidjson variables
	rapidjson::FileStream inputStream(pFile);
	rapidjson::Document jsonDocument;

	// Read the json file
	jsonDocument.ParseStream<0,rapidjson::FileStream>(inputStream);

	// Validate file
	if(!jsonDocument.IsObject())
	{
		std::cout << filepath << " was not able to be processed. Please ensure that it is valid JSON." << std::endl;
		return false;
	}

	// Process all of the sets in the json document
	hasObjects = true;
	objectCounter  = 1;
	while( hasObjects )
	{
		// Build setname (incremented by 1 at the end of each read)
		std::string setName = "set" + std::to_string(objectCounter);

		// Check for a set
		if(jsonDocument.HasMember(setName.c_str()))
		{

			// Check if that member has the required set indicator
			if(jsonDocument[setName.c_str()].HasMember("datatype") && jsonDocument[setName.c_str()]["datatype"].IsString())
			{

				// Get the type name
				std::string setType = std::basic_string<char>(jsonDocument[setName.c_str()]["datatype"].GetString(),jsonDocument[setName.c_str()]["datatype"].GetStringLength());
				
				// Check that we have a factory corresponding to that type
				std::unordered_map<std::string,IDataSetFactory*>::const_iterator factoryEntry = factoryMap.find(setType);
				if(factoryEntry != factoryMap.end())
				{
					// Make a pointer to an IDataSetFactory
					IDataSet *dataSet;

					// Get the information of value out of the json document
					dataSet = factoryEntry->second->createObject(jsonDocument[setName.c_str()]);

					// Add this as a listener if it is valid
					if(dataSet != nullptr)
					{
						// Add the new object as an observer to the SDL main class
						sdlMain->registerObserver(dataSet);
					}
					else
					{
						std::cout << "Found Item " << setName << " with setType type " << setType << " but it did not have the data that the factory required." << std::endl;
					}


				}
				else
				{
					std::cout << "Found Item " << setName << " with setType type " << setType << " but this program does not have a factory to process that type of data." << std::endl;
				}

			}
			else
			{
				// Print error to screen
				std::cout << "Found Item " << setName << " but it did not have the datatype indicator." << std::endl;
			}

			// Prepare for next loop
			objectCounter++;
		}
		else
		{
			// The last object was the last valid object, set flag false
			hasObjects = false;
		}

	}

	// DEBUG: Print the JSON to console
	printf("\nModified JSON with reformatting:\n");
	rapidjson::FileStream f(stdout);
	rapidjson::PrettyWriter<rapidjson::FileStream> writer(f);
	jsonDocument.Accept(writer);	// Accept() traverses the DOM and generates Handler events.

	// Return completion (this cannot currently return false)
	return true;
}

void JsonLoader::registerFactory(std::string key, IDataSetFactory* factory)
{
	// Add the new factory to the factory map
	factoryMap.insert(std::make_pair(key,factory));
}
