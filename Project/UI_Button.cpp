#include "UI_Button.h"
#include "Game.h"
#include "EventManager.h"
#include <iostream>

UI_Button::UI_Button(SDL_Rect source, SDL_FRect destination, SDL_Texture* texture)
	:m_sourceTransform(source), m_destinationTransform(destination), pTexture(texture)
{

}

void UI_Button::Render()
{
	//m_sourceTransform.x = m_sourceTransform.w * (int)m_state;
	SDL_RenderCopyF(Game::GetInstance()->GetRenderer(), pTexture, &m_sourceTransform, &m_destinationTransform);
}

void UI_Button::Update(float deltaTime)
{
	SDL_Rect intDestinationRect = { (int)m_destinationTransform.x, (int)m_destinationTransform.y, (int)m_destinationTransform.w, (int)m_destinationTransform.h };
	bool mouseOverlap = SDL_PointInRect(&EventManager::GetMousePos(), &intDestinationRect);
		if (EventManager::MouseReleased(1)) // Left mouse released.
		{
			if (mouseOverlap)
			{
				Execute();
			}
		}	
}