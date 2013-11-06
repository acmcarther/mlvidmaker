#include "ImageDataSetFactory.h"

ImageDataSetFactory::ImageDataSetFactory(void)
{
}


ImageDataSetFactory::~ImageDataSetFactory(void)
{
}

ImageDataSet* ImageDataSetFactory::createObject(jsonData& objectParams) const
{
	// Define function variables

	// Verify that we have all of the required function members
	if (objectParams.HasMember("x") && objectParams["x"].IsInt() &&
		objectParams.HasMember("y") && objectParams["y"].IsInt() &&
		objectParams.HasMember("xSize") && objectParams["xSize"].IsInt() &&
		objectParams.HasMember("ySize") && objectParams["ySize"].IsInt() &&
		objectParams.HasMember("imagePath") && objectParams["imagePath"].IsString())
	{

		// Sets new image dataset. May cause problem with lost memory
		return new ImageDataSet(objectParams["x"].GetInt(),
			objectParams["y"].GetInt(),
			objectParams["xSize"].GetInt(),
			objectParams["ySize"].GetInt(),
			objectParams["imagePath"].GetString());
	}
	else
	{
		// Returning nullptr, someone please watch out for this
		return nullptr;
	}
}
