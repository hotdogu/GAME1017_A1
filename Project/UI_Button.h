#pragma once
#include <string>
#include <SDL.h>

class UI_Button
{
public:
	UI_Button() {}
	UI_Button(SDL_Rect source, SDL_FRect destination, SDL_Texture* texture);
	virtual void Update(float deltaTime);
	virtual void Render();

protected:
	SDL_Rect m_sourceTransform;
	SDL_FRect m_destinationTransform;
	SDL_Texture* pTexture;

	enum class ButtonState
	{
		STATE_UP,
		STATE_OVER,
		STATE_DOWN
	};


	virtual void Execute() = 0;
};
