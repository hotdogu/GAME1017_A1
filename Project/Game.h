#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>

class Game
{
public:
	static const int kWidth = 1024;
	static const int kHeight = 768;


	static Game* GetInstance();
	static void DeleteInstance();
	int Init(const char* title, int xPos, int yPos);
	bool IsRunning();
	void HandleEvents();
	bool KeyDown(SDL_Scancode key);
	void Update(float deltaTime);
	void Render();
	void Clean();
	void QuitGame() { m_running = false; }
	

	SDL_Window* GetWindow() { return m_pWindow; }
	SDL_Renderer* GetRenderer() { return m_pRenderer; }
private:
	Game();
	~Game();
	bool m_running; 
	SDL_Window* m_pWindow; 
	SDL_Renderer* m_pRenderer; 

	const Uint8* m_keyStates; 


	

	inline static Game* s_instance = nullptr;
};
