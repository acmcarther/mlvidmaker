#pragma once
#include <SDL.h>
#include <gl\glew.h>
#include <SDL_opengl.h>
#include <gl\glu.h>
#include <set>
#include "IDataSet.h"
class SDLManager
{
	protected:	
		// Protected Values
		static const int SCREEN_WIDTH = 640;
		static const int SCREEN_HEIGHT = 480;
		SDL_Window* gWindow;
		SDL_GLContext gContext;
		bool gRenderQuad;
		GLuint gProgramID;
		GLint gVertexPos2DLocation;
		GLuint gVBO;
		GLuint gIBO;
		bool quit;
		bool windowOpen;
		std::set<IDataSet*> dataSets;
		int frameNumber;

		// Protected Functions
		bool initGL();
		void handleKeys( unsigned char key, int x, int y );
		void close();
		void printProgramLog( GLuint program );
		void printShaderLog( GLuint shader );

	public:
		// Public Functions
		SDLManager(void);
		~SDLManager(void);
		bool initialize();
		bool isOpen(void);
		void update(void);
		void handleEventQueue(void);
		void render(void);
		void manageFlags();

		void registerObserver(IDataSet* newDataSet);
		void removeObserver(IDataSet* removedDataSet);
};

