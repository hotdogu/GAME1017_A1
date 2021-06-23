#include "Sprites.h"
#include "Game.h"

////////////////////////////////////////////////////////
// Begin Sprite Class
////////////////////////////////////////////////////////
Sprite::Sprite()
	: m_sourceTransform{ 0, 0, 0, 0 }
	, m_destinationTransform{ 0, 0, 0, 0 }
	, m_angle(0)
{

}

Sprite::Sprite(SDL_Rect source, SDL_FRect destination, int angle) 
	: m_sourceTransform(source)
	, m_destinationTransform(destination)
	, m_angle(angle)
{

}


////////////////////////////////////////////////////////
// Begin AnimatedSprite Class
////////////////////////////////////////////////////////
AnimatedSprite::AnimatedSprite(int angle, float frameTime, int maxSprites, 
	SDL_Rect sourceTransform, SDL_FRect destinationTransform)
	: Sprite(sourceTransform, destinationTransform, angle)
	, m_currentSpriteIndex(0)
	, m_maxSprites(maxSprites)
	, m_currentTime(0)
	, m_frameRate(frameTime)

{

}

void AnimatedSprite::Animate(float deltaTime)
{
	m_currentTime += deltaTime;
	if (m_currentTime > m_frameRate)
	{
		m_currentTime = m_frameRate - m_currentTime;
		m_currentSpriteIndex++;
		if (m_currentSpriteIndex == m_maxSprites)
		{
			m_currentSpriteIndex = 0;
		}
	}
	m_sourceTransform.x = m_sourceTransform.w * m_currentSpriteIndex;
}

////////////////////////////////////////////////////////
// Begin Player Class
////////////////////////////////////////////////////////
Player::Player(SDL_Rect sourceTransform, SDL_FRect destinationTransform)
	: AnimatedSprite(90, 0.2f, 4, sourceTransform, destinationTransform) 
{

}

void Player::Update(float deltaTime)
{
	Animate(deltaTime);
}

////////////////////////////////////////////////////////
// Begin Bullet Class
////////////////////////////////////////////////////////
Bullet::Bullet(SDL_Rect sourceTransform, SDL_FRect destinationTransform, float speed)
	: Sprite(sourceTransform, destinationTransform)
	, m_speed(speed)
	, m_isActive(true)
{

}

void Bullet::Update(float deltaTime)
{
	m_destinationTransform.x += m_speed * deltaTime;
}


////////////////////////////////////////////////////////
// Begin Enemy Class
////////////////////////////////////////////////////////
Enemy::Enemy(SDL_Rect sourceTransform, SDL_FRect destinationTransform, std::vector<Bullet*>* pBullets, Mix_Chunk* pShootSound, float bulletSpawnRate)
	: AnimatedSprite(-90, 0.1f, 4, sourceTransform, destinationTransform)
	, m_bulletTimer(0)
	, m_timerMax(bulletSpawnRate)
	, m_pBullets(pBullets)
	, m_pShootSound(pShootSound)
{

}

void Enemy::Update(float deltaTime)
{
	Animate(deltaTime);
	m_destinationTransform.x -= kEnemySpeed * deltaTime;

	m_bulletTimer += deltaTime;
	if (m_bulletTimer > m_timerMax)
	{
		m_bulletTimer = 0;
		m_pBullets->push_back(
			new Bullet( { kBulletSourceXPosition, kBulletSourceYPosition, kBulletWidth, kBulletHeight }, 
						{ m_destinationTransform.x, m_destinationTransform.y - kBulletHeight * 2, kBulletWidth, kBulletHeight },
						kBulletSpeed));
		Mix_PlayChannel(-1, m_pShootSound, 0);
	}
}