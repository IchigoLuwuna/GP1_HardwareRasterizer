// External includes
#include <SDL.h>
#include <SDL_surface.h>
#include <SDL_image.h>
#include <SDL_syswm.h>
#include <SDL_video.h>
#undef main

// Standard includes
#include <iostream>

// Project includes
#include "Timer.h"
#include "Renderer.h"
#if defined( _DEBUG )
#	include "LeakDetector.h"
#endif

using namespace dae;

void ShutDown( SDL_Window* pWindow )
{
	SDL_DestroyWindow( pWindow );
	SDL_Quit();
}

int main( int argc, char* args[] )
{
	// Unreferenced parameters
	(void)argc;
	(void)args;

// Leak detection
#if defined( _DEBUG )
	LeakDetector detector{};
#endif

	// Create window + surfaces
	SDL_Init( SDL_INIT_VIDEO );

	const uint32_t width = 640;
	const uint32_t height = 480;

// Add the vulkan flag to support DXVK on Linux
#ifdef __LINUX__
	const SDL_WindowFlags windowFlags{ SDL_WINDOW_VULKAN };
#else
	const SDL_WindowFlags windowFlags{};
#endif

	SDL_Window* pWindow = SDL_CreateWindow( "DirectX - ***Insert Name/Class***",
											SDL_WINDOWPOS_UNDEFINED,
											SDL_WINDOWPOS_UNDEFINED,
											width,
											height,
											windowFlags );

	if ( !pWindow )
		return 1;

	// Initialize "framework"
	Timer timer{};
	Renderer renderer{ pWindow };

	// Start loop
	timer.Start();
	float printTimer = 0.f;
	bool isLooping = true;
	while ( isLooping )
	{
		//--------- Get input events ---------
		SDL_Event e;
		while ( SDL_PollEvent( &e ) )
		{
			switch ( e.type )
			{
			case SDL_QUIT:
				isLooping = false;
				break;
			case SDL_KEYUP:
				// Test for a key
				// if (e.key.keysym.scancode == SDL_SCANCODE_X)
				break;
			default:;
			}
		}

		//--------- Update ---------
		renderer.Update( timer );

		//--------- Render ---------
		renderer.Render();

		//--------- Timer ---------
		timer.Update();
		printTimer += timer.GetElapsed();
		if ( printTimer >= 1.f )
		{
			printTimer = 0.f;
			std::cout << "dFPS: " << timer.GetdFPS() << std::endl;
		}
	}
	timer.Stop();

	ShutDown( pWindow );
	return 0;
}
