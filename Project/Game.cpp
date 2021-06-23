#include "Game.h"
#include "Utilities.h"
#include <iostream>
#include "StateManager.h"
#include "States.h"
#include "EventManager.h"

Game::Game() 
	: m_running(false)
	, m_pWindow(nullptr)
	, m_pRenderer(nullptr)
	, m_keyStates(nullptr)
{

}

Game::~Game()
{

}

Game* Game::GetInstance()
{
	if (s_instance == nullptr)
	{
		s_instance = new Game();
	}
	return s_instance;
}

void Game::DeleteInstance()
{
	delete s_instance;
	s_instance = nullptr;
}

int Game::Init(const char* title, int xPos, int yPos)
{
	std::cout << "Init Game ..." << std::endl;
	srand((unsigned)time(nullptr));

	int errorCode = SDL_Init(SDL_INIT_EVERYTHING);
	if (errorCode == 0)
	{
		std::cout << "SDL_Init() succeeded!" << std::endl;
	}
	else
	{
		std::cout << "SDL_Init() failed. Error code: " << errorCode << ": " << SDL_GetError() << std::endl;
		system("pause");
		return errorCode;
	}

	m_pWindow = SDL_CreateWindow(title, xPos, yPos, kWidth, kHeight, 0);
	if (m_pWindow != nullptr)
	{
		std::cout << "SDL_CreateWindow() succeeded!" << std::endl;
	}
	else
	{
		std::cout << "SDL_CreateWindow() failed. Error: " << SDL_GetError() << std::endl;
		SDL_Quit();
		system("pause");
		return -1;
	}

	m_pRenderer = SDL_CreateRenderer(m_pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (m_pRenderer != nullptr)
	{
		std::cout << "SDL_CreateRenderer() succeeded!" << std::endl;
	}
	else
	{
		std::cout << "SDL_CreateRenderer() failed. Error: " << SDL_GetError() << std::endl;
		SDL_DestroyWindow(m_pWindow);
		SDL_Quit();
		system("pause");
		return -1;
	}

	errorCode = Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024);
	if (errorCode == 0)
	{
		std::cout << "Mix_OpenAudio() succeeded!" << std::endl;
	}
	else
	{
		std::cout << "Mix_OpenAudio() failed. Error: " << Mix_GetError() << std::endl;
		SDL_DestroyWindow(m_pWindow);
		SDL_Quit();
		system("pause");
		return -1;
	}

	int audioFlags = MIX_INIT_MP3;
	errorCode = Mix_Init(audioFlags);
	if (errorCode != audioFlags)
	{
		std::cout << "Mix_OpenAudio() failed. Error: " << Mix_GetError() << std::endl;
		SDL_DestroyWindow(m_pWindow);
		SDL_Quit();
		system("pause");
		return -1;
	}
	std::cout << "Initialization successful!" << std::endl;

	m_keyStates = SDL_GetKeyboardState(nullptr);

	StateManager::ChangeState(new TitleState());

	EventManager::Init();

	m_running = true;
	return 0;
}

bool Game::IsRunning()
{
	return m_running;
}

void Game::HandleEvents()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_QUIT: // User pressed window's 'x' button.
			QuitGame();
			break;
		case SDL_KEYDOWN: // Try SDL_KEYUP instead.
			if (event.key.keysym.sym == SDLK_ESCAPE)
				QuitGame();
			break;
		case SDL_KEYUP:
			if (event.key.keysym.sym == SDLK_SPACE)
				//m_canShoot = true;//Change This NEW
			break;
		}
	}

	EventManager::HandleEvents();
}

bool Game::KeyDown(SDL_Scancode key)
{
	if(m_keyStates)
	{
		return m_keyStates[key] == 1;
	}
	return false;
}



void Game::Update(float deltaTime)
{
	StateManager::Update(deltaTime);
	
}

void Game::Render()
{
	SDL_SetRenderDrawColor(m_pRenderer, 0, 0, 0, 255);
	SDL_RenderClear(m_pRenderer); // Clear the screen with the draw color.

	StateManager::Render();
	SDL_RenderPresent(m_pRenderer);
}

void Game::Clean()
{
	Mix_CloseAudio();
	Mix_Quit();
	
	SDL_DestroyRenderer(m_pRenderer);
	SDL_DestroyWindow(m_pWindow);
	SDL_Quit();
}

