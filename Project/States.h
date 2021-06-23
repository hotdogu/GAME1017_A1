#pragma once
#include <SDL.h>
#include "Sprites.h"
#include "PlayButton.h"

class State
{
public:
	virtual void Enter() = 0;
	virtual void Update(float deltaTime);
	virtual void Render() = 0;
	virtual void Exit() = 0;
	virtual void Pause() { }
	virtual void Resume() { }
};

class TitleState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;

	SDL_Texture* m_pBGText; // For the bg.
	SDL_Texture* m_pPlayText; // For the play button
	PlayButton* playButton;
};

class GameState : public State
{
public:
	static const int kWidth = 1024; //Take these out
	static const int kHeight = 768;
	static const int kPlayerSpeed = 400;

private: 
	static const int kPlayerSourceXPosition = 0;
	static const int kPlayerSourceYPosition = 0;
	static const int kPlayerWidth = 94;
	static const int kPlayerHeight = 100;

	static const int kPlayerBulletSpeed = 600;
	static const int kPlayerBulletSourceXPosition = 376;
	static const int kPlayerBulletSourceYPosition = 0;
	static const int kPlayerBulletWidth = 10;
	static const int kPlayerBulletHeight = 100;

	static const int kEnemySourceXPosition = 0;
	static const int kEnemySourceYPosition = 100;
	static const int kEnemyWidth = 40;
	static const int kEnemyHeight = 46;

	static const int kBackgroundScrollSpeed = 30;
	static const int kNumberOfBackgrounds = 2;

	// These three flags check if we need to clear the respective vectors of nullptrs.
	bool m_shouldClearEnemies;
	bool m_shouldClearPlayerBullets;
	bool m_shouldClearEnemyBullets;

	bool m_canShoot; // This restricts the player from firing again unless they release the Spacebar.
	

	SDL_Texture* m_pBGText; // For the bg.
	Sprite m_bgArray[kNumberOfBackgrounds];

	SDL_Texture* m_pSpriteTexture; // For the sprites.
	Player* m_pPlayer;

	// The enemy spawn frame timer properties.
	float m_enemySpawnTime;
	float m_enemySpawnMaxTime;

	SDL_FPoint m_pivot;

	Mix_Music* m_pMusic;
	std::vector<Mix_Chunk*> m_sounds;

	std::vector<Enemy*> m_enemies;
	std::vector<Bullet*> m_playerBullets;
	std::vector<Bullet*> m_enemyBullets;

public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;
	virtual void Pause() override;
	virtual void Resume() override;
	void CheckCollision();

};

class PauseState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;

	SDL_Texture* m_pPText; // For the main menu button
	PButton* pButton;
	SDL_Texture* m_pBGText;


};

class LoseState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;

	SDL_Texture* m_pMMText; // For the main menu button
	MMButton* mmButton;
	SDL_Texture* m_pBGText;
};

class WinState : public State
{
public:
	virtual void Enter() override;
	virtual void Update(float deltaTime) override;
	virtual void Render() override;
	virtual void Exit() override;

	SDL_Texture* m_pMMText; // For the main menu button
	MMButton* mmButton;
	SDL_Texture* m_pBGText;
};

