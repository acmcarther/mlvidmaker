#include "SDLManager.h"
#include <iostream>

// Supporting class functions
bool SDLManager::initGL()
{
	//Success flag
	bool success = true;

	//Generate program
	gProgramID = glCreateProgram();

	//Create vertex shader
	GLuint vertexShader = glCreateShader( GL_VERTEX_SHADER );

	//Get vertex source
	const GLchar* vertexShaderSource[] =
	{
		"#version 130\nin vec2 LVertexPos2D; void main() { gl_Position = vec4( LVertexPos2D.x, LVertexPos2D.y, 0, 1 ); }"
	};

	//Set vertex source
	glShaderSource( vertexShader, 1, vertexShaderSource, NULL );

	//Compile vertex source
	glCompileShader( vertexShader );

	//Check vertex shader for errors
	GLint vShaderCompiled = GL_FALSE;
	glGetShaderiv( vertexShader, GL_COMPILE_STATUS, &vShaderCompiled );
	if( vShaderCompiled != GL_TRUE )
	{
		std::cout << "Unable to compile vertex shader " << vertexShader << std::endl;
		printShaderLog( vertexShader );
        success = false;
	}
	else
	{
		//Attach vertex shader to program
		glAttachShader( gProgramID, vertexShader );


		//Create fragment shader
		GLuint fragmentShader = glCreateShader( GL_FRAGMENT_SHADER );

		//Get fragment source
		const GLchar* fragmentShaderSource[] =
		{
			"#version 130\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
		};

		//Set fragment source
		glShaderSource( fragmentShader, 1, fragmentShaderSource, NULL );

		//Compile fragment source
		glCompileShader( fragmentShader );

		//Check fragment shader for errors
		GLint fShaderCompiled = GL_FALSE;
		glGetShaderiv( fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled );
		if( fShaderCompiled != GL_TRUE )
		{
			std::cout << "Unable to compile fragment shader " << fragmentShader << std::endl;
			printShaderLog( fragmentShader );
			success = false;
		}
		else
		{
			//Attach fragment shader to program
			glAttachShader( gProgramID, fragmentShader );


			//Link program
			glLinkProgram( gProgramID );

			//Check for errors
			GLint programSuccess = GL_TRUE;
			glGetProgramiv( gProgramID, GL_LINK_STATUS, &programSuccess );
			if( programSuccess != GL_TRUE )
			{
				std::cout << "Error linking program " << gProgramID << std::endl;
				printProgramLog( gProgramID );
				success = false;
			}
			else
			{
				//Get vertex attribute location
				gVertexPos2DLocation = glGetAttribLocation( gProgramID, "LVertexPos2D" );
				if( gVertexPos2DLocation == -1 )
				{
					std::cout << "LVertexPos2d is not a valid glsl program variable!" << std::endl;
					success = false;
				}
				else
				{
					//Initialize clear color
					glClearColor( 0.f, 0.f, 0.f, 1.f );

					//VBO data
					GLfloat vertexData[] =
					{
						-0.5f, -0.5f,
						 0.5f, -0.5f,
						 0.5f,  0.5f,
						-0.5f,  0.5f
					};

					//IBO data
					GLuint indexData[] = { 0, 1, 2, 3 };

					//Create VBO
					glGenBuffers( 1, &gVBO );
					glBindBuffer( GL_ARRAY_BUFFER, gVBO );
					glBufferData( GL_ARRAY_BUFFER, 2 * 4 * sizeof(GLfloat), vertexData, GL_STATIC_DRAW );

					//Create IBO
					glGenBuffers( 1, &gIBO );
					glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, gIBO );
					glBufferData( GL_ELEMENT_ARRAY_BUFFER, 4 * sizeof(GLuint), indexData, GL_STATIC_DRAW );
				}
			}
		}
	}
	
	return success;
}

void SDLManager::handleKeys( unsigned char key, int x, int y )
{
	// Print acknowledgement
	std::cout << "Recieved key: " << key << ", at loc: (" << x << ", " << y << ")" << std::endl;

	//Toggle quad
	if( key == 'q' )
	{
		gRenderQuad = !gRenderQuad;
	}
}

void SDLManager::close()
{
	//Disable text input
	SDL_StopTextInput();

	//Deallocate program
	glDeleteProgram( gProgramID );

	//Destroy window	
	SDL_DestroyWindow( gWindow );
	gWindow = NULL;

	//Quit SDL subsystems
	SDL_Quit();
}


void SDLManager::printProgramLog( GLuint program )
{
	//Make sure name is shader
	if( glIsProgram( program ) )
	{
		//Program log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		
		//Get info string length
		glGetProgramiv( program, GL_INFO_LOG_LENGTH, &maxLength );
		
		//Allocate string
		char* infoLog = new char[ maxLength ];
		
		//Get info log
		glGetProgramInfoLog( program, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			std::cout << infoLog << std::endl;
		}
		
		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		std::cout << "Name " << program << " is not a program" << std::endl;
	}
}

void SDLManager::printShaderLog( GLuint shader )
{
	//Make sure name is shader
	if( glIsShader( shader ) )
	{
		//Shader log length
		int infoLogLength = 0;
		int maxLength = infoLogLength;
		
		//Get info string length
		glGetShaderiv( shader, GL_INFO_LOG_LENGTH, &maxLength );
		
		//Allocate string
		char* infoLog = new char[ maxLength ];
		
		//Get info log
		glGetShaderInfoLog( shader, maxLength, &infoLogLength, infoLog );
		if( infoLogLength > 0 )
		{
			//Print Log
			std::cout << infoLog << std::endl;
		}

		//Deallocate string
		delete[] infoLog;
	}
	else
	{
		std::cout << "Name " << shader << " is not a shader" << std::endl;
	}
}


// Class function implementations
SDLManager::SDLManager(void)
{
	// Initialize class varables
	gWindow = NULL;
	gContext;
	gRenderQuad = true;
	gProgramID = 0;
	gVertexPos2DLocation = -1;
	gVBO = 0;
	gIBO = 0;
	windowOpen = false;
	quit = false;
	frameNumber = 0;
}


SDLManager::~SDLManager(void)
{
}

bool SDLManager::initialize()
{
	//Initialization flag
	bool success = true;

	//Initialize SDL
	if( SDL_Init( SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE ) < 0 )
	{
		std::cout << "SDL could not initialize! SDL Error: " << SDL_GetError() << std::endl;
		success = false;
	}
	else
	{
		//Use OpenGL 3.1 core
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 3 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 1 );
		SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

		//Create window
		gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN );
		if( gWindow == NULL )
		{
			std::cout << "Window could not be created! SDL Error: " << SDL_GetError() << std::endl;
			success = false;
		}
		else
		{
			//Create context
			gContext = SDL_GL_CreateContext( gWindow );
			if( gContext == NULL )
			{
				std::cout << "OpenGL context could not be created! SDL Error: " << SDL_GetError() << std::endl;
				success = false;
			}
			else
			{
				//Initialize GLEW
				glewExperimental = GL_TRUE; 
				GLenum glewError = glewInit();
				if( glewError != GLEW_OK )
				{
					std::cout << "Error initializing GLEW! " << glewGetErrorString( glewError ) << std::endl;
				}

				//Use Vsync
				if( SDL_GL_SetSwapInterval( 1 ) < 0 )
				{
					std::cout << "Warning: Unable to set VSync! SDL Error: " << SDL_GetError() << std::endl;
				}

				//Initialize OpenGL
				if( !initGL() )
				{
					std::cout << "Unable to initialize OpenGL!" << std::endl;
					success = false;
				}
			}
		}

		//Enable text input
		SDL_StartTextInput();
	}
	windowOpen = success;
	return success;
}

bool SDLManager::isOpen(void)
{
	return windowOpen;
}

void SDLManager::update(void)
{
	// Get a set iterator
	std::set<IDataSet*>::iterator itr;
	bool dataLeft = true, dataProcessed = true;


	// Tell all of the datasets to update
    for ( itr = dataSets.begin();
          itr != dataSets.end(); itr++ )
	{
		 dataLeft = (*itr)->notifyUpdate(frameNumber);
		 dataProcessed = dataLeft || dataProcessed;
	}

	// If nothing was processed, assume that all frames are finished?
	if (!dataProcessed)
	{
		quit = true;
	}

	// Move to the next frame
	frameNumber++;
}

void SDLManager::handleEventQueue(void)
{
	//Event handler
	SDL_Event e;

	//Handle events on queue
	while( SDL_PollEvent( &e ) != 0 )
	{
		//User requests quit
		if( e.type == SDL_QUIT )
		{
			quit = true;
		}
		//Handle keypress with current mouse position
		else if( e.type == SDL_TEXTINPUT )
		{
			int x = 0, y = 0;
			SDL_GetMouseState( &x, &y );
			handleKeys( e.text.text[ 0 ], x, y );
		}
	}
}

void SDLManager::render(void)
{
	// Get a set iterator
	std::set<IDataSet*>::iterator itr;

	//Clear color buffer
	glClear( GL_COLOR_BUFFER_BIT );
	
	SDL_Surface* screen = SDL_GetWindowSurface( gWindow );

	// Tell all of the datasets to draw
    for ( itr = dataSets.begin();
          itr != dataSets.end(); itr++ )
	{
		  (*itr)->notifyRender(screen);
	}

	SDL_UpdateWindowSurface( gWindow );

	//Update screen
	//SDL_GL_SwapWindow( gWindow );
}

void SDLManager::manageFlags()
{
	if( quit )
	{
		close();
		quit = false;
		windowOpen = false;
	}
}


void SDLManager::registerObserver(IDataSet* newDataSet)
{
	// Add this new data into the internal vector
	dataSets.insert(newDataSet);
	
}

void SDLManager::removeObserver(IDataSet* removedDataSet)
{
	// Find and remove the dataset
	dataSets.erase(removedDataSet);

	// This is really bad practice, but we're deleting the dataset here
	delete removedDataSet;
}