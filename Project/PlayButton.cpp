#include "PlayButton.h"
#include "StateManager.h"
#include "States.h"
#include "Game.h"

PlayButton::PlayButton(SDL_FRect destination, SDL_Texture* texture)
{
	m_destinationTransform = destination;
	pTexture = texture;
}

void PlayButton::Execute()
{
	StateManager::ChangeState(new GameState());
}

void PlayButton::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();
	SDL_RenderCopyF(pRenderer, pTexture, NULL, &m_destinationTransform);
}

MMButton::MMButton(SDL_FRect destination, SDL_Texture* texture)
{
	m_destinationTransform = destination;
	pTexture = texture;
}

void MMButton::Execute()
{
	StateManager::ChangeState(new TitleState());
}

void MMButton::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();
	SDL_RenderCopyF(pRenderer, pTexture, NULL, &m_destinationTransform);
}

PButton::PButton(SDL_FRect destination, SDL_Texture* texture)
{
	m_destinationTransform = destination;
	pTexture = texture;
}

void PButton::Execute()
{
	StateManager::PopState();
}

void PButton::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();
	SDL_RenderCopyF(pRenderer, pTexture, NULL, &m_destinationTransform);
}