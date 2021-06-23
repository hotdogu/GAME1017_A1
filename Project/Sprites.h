#pragma once
#include <vector>
#include <SDL.h>
#include <SDL_mixer.h>

////////////////////////////////////////////////////////
// Begin Sprite Class
////////////////////////////////////////////////////////
class Sprite
{
protected:
	SDL_Rect m_sourceTransform; // Rectangle for source pixels from image.
	SDL_FRect m_destinationTransform; // Rectangle for destination window.
	int m_angle;		// Angle of sprite.
public:
	Sprite();
	Sprite(SDL_Rect source, SDL_FRect destination, int angle = 0);
	SDL_Rect* GetSourceTransform() { return &m_sourceTransform; }
	SDL_FRect* GetDestinationTransform() { return &m_destinationTransform; }
	double GetAngle() { return m_angle; }
};

////////////////////////////////////////////////////////
// Begin AnimatedSprite Class
////////////////////////////////////////////////////////
class AnimatedSprite : public Sprite
{
protected:
	int m_currentSpriteIndex;	// Which sprite to display for animation.
	int m_maxSprites;			// How many sprites total.
	float m_currentTime;		// Time counter.
	float m_frameRate;			// When to change to next sprite.

public:
	AnimatedSprite(int angle, float frameTime, int maxSprites, 
		SDL_Rect sourceTransform, SDL_FRect destinationTransform);
	void Animate(float deltaTime);
};

////////////////////////////////////////////////////////
// Begin Player Class
////////////////////////////////////////////////////////
class Player : public AnimatedSprite
{
public:
	Player(SDL_Rect sourceTransform, SDL_FRect destinationTransform);
	void Update(float deltaTime);
};

////////////////////////////////////////////////////////
// Begin Bullet Class
////////////////////////////////////////////////////////
class Bullet : public Sprite
{
private:
	float m_speed;
	bool m_isActive;
public:
	Bullet(SDL_Rect sourceTransform, SDL_FRect destinationTransform, float speed);
	void Update(float deltaTime);
};

////////////////////////////////////////////////////////
// Begin Enemy Class
////////////////////////////////////////////////////////
class Enemy : public AnimatedSprite
{
	static const int kEnemySpeed = 150;
	static const int kBulletSpeed = -300;

	static const int kBulletSourceXPosition = 160;
	static const int kBulletSourceYPosition = 100;
	static const int kBulletWidth = 14;
	static const int kBulletHeight = 14;

private:
	float m_bulletTimer;
	float m_timerMax; 
	std::vector<Bullet*>* m_pBullets;
	Mix_Chunk* m_pShootSound;
public:
	Enemy(SDL_Rect sourceTransform, SDL_FRect destinationTransform, std::vector<Bullet*>* pBullets, Mix_Chunk* pShootSound, float bulletSpawnRate);
	void Update(float deltaTime);
};

