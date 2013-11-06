/*This source code copyrighted by Lazy Foo' Productions (2004-2013)
and may not be redistributed without written permission.*/

//Using SDL, SDL OpenGL, GLEW, standard IO, file streams, and strings
#include <stdio.h>
#include <string>
#include <fstream>
#include <iostream>
#include "SDLManager.h"
#include "JsonLoader.h"
#include <rapidjson/reader.h>
#include "ImageDataSetFactory.h"

int main( int argc, char* args[] )
{
	SDLManager window;
	JsonLoader loader(&window);
	bool success;

	// Verify correct number of command line parameters
	if ( argc < 2 )
	{
		std::cout << "Usage: program.exe jsonfilename.json" << std::endl;
		return 0;
	}

	// Give the loader the imageset factory
	loader.registerFactory("imageset",new ImageDataSetFactory());

	// Read the json file
	success = loader.parseFileToSDL(args[1]);

	// If we didn't read correctly, exit and notify
	if( !success )
	{
		std::cout << "ERROR: Misformatted JSON file, exiting" << std::endl;
		return 0;
	}

	// Initialize the OpenGL and SDL contents
	window.initialize();

	// Run main render loop
	while( window.isOpen() )
	{
		// Update any window stuff that needs to be updated	
		window.update();

		// Handle user events
		window.handleEventQueue();

		// Render the next frame
		window.render();

		// Manage any flags
		window.manageFlags();
	}

	// End of program, return success
	return 0;
}