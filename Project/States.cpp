#include "States.h"
#include "Game.h"
#include "StateManager.h"
#include <iostream>
#include "Utilities.h"

void State::Update(float deltaTime)
{
	if (Game::GetInstance()->KeyDown(SDL_SCANCODE_ESCAPE))
	{
		std::cout << "State Quit game" << std::endl;
		Game::GetInstance()->QuitGame();
	}
}

///////////////////
////begin title state
/////////////////////
void TitleState::Enter()
{
	std::cout << "Entering Title State" << std::endl;

	m_pBGText = nullptr;
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();
	m_pBGText = IMG_LoadTexture(pRenderer, "Assets/Images/space.jpg");
	m_pPlayText = IMG_LoadTexture(pRenderer, "Assets/Images/playButton.png");


	SDL_FRect bDest;
	bDest.h = 150;
	bDest.w = 350;
	bDest.x = 10;
	bDest.y = 400;

	playButton = new PlayButton(bDest, m_pPlayText);
}




void TitleState::Update(float deltaTime)
{
	State::Update(deltaTime);

	playButton->Update(deltaTime);


	if (Game::GetInstance()->KeyDown(SDL_SCANCODE_N))
	{
		std::cout << "Titlestate changing to game state" << std::endl;
		StateManager::ChangeState(new GameState());
	}
}
void TitleState::Render()
{
	
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();

	SDL_RenderCopyF(pRenderer, m_pBGText, NULL, NULL);
	playButton->Render();
}
void TitleState::Exit()
{
	std::cout << "Exiting Title State" << std::endl;
}
////////////////
//end title state
///////////////


//////////////
///begin game state
////////////////
void GameState::Enter()
{
	std::cout << "Entering Game State" << std::endl;

	//Setting Variables NEW
	m_shouldClearEnemies = false;
	m_shouldClearPlayerBullets = false;
	m_shouldClearEnemyBullets = false;
	m_canShoot = true;
	m_pBGText = nullptr;
	m_pSpriteTexture = nullptr;
	m_pPlayer = nullptr;
	m_enemySpawnTime = 0;
	m_enemySpawnMaxTime = 1.5f;
	m_pivot = { 0,0 };
	m_pMusic = nullptr;


	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();

	m_pBGText = IMG_LoadTexture(pRenderer, "Assets/Images/background.png");
	m_pSpriteTexture = IMG_LoadTexture(pRenderer, "Assets/Images/sprites.png");

	m_bgArray[0] = { {0, 0, kWidth, kHeight}, {0, 0, kWidth, kHeight} };
	m_bgArray[1] = { {0, 0, kWidth, kHeight}, {kWidth, 0, kWidth, kHeight} };

	m_pMusic = Mix_LoadMUS("Assets/Audio/game.mp3"); // Load the music track.
	if (m_pMusic == nullptr)
	{
		std::cout << "Failed to load music: " << Mix_GetError() << std::endl;
	}

	m_sounds.reserve(3);
	Mix_Chunk* sound = Mix_LoadWAV("Assets/Audio/enemy.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	sound = Mix_LoadWAV("Assets/Audio/laser.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	sound = Mix_LoadWAV("Assets/Audio/explode.wav");
	if (sound == nullptr)
	{
		std::cout << "Failed to load sound: " << Mix_GetError() << std::endl;
	}
	m_sounds.push_back(sound);

	m_pPlayer = new Player({ kPlayerSourceXPosition, kPlayerSourceYPosition, kPlayerWidth, kPlayerHeight }, // Image Source
		{ kWidth / 4 ,kHeight / 2 - kPlayerHeight / 2,kPlayerWidth,kPlayerHeight }); // Window Destination

	Mix_PlayMusic(m_pMusic, -1); // Play. -1 = looping.
	Mix_VolumeMusic(16); // 0-MIX_MAX_VOLUME (128).
}
void GameState::Update(float deltaTime)
{
	SDL_Event event;
	SDL_PollEvent(&event);
	//Change this code it is terrible
	if (event.key.keysym.sym == SDLK_SPACE)
	{
		m_canShoot = true;//Change This NEW		
	}



	State::Update(deltaTime);
	
	// Scroll the backgrounds. 
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		m_bgArray[i].GetDestinationTransform()->x -= kBackgroundScrollSpeed * deltaTime;
	}

	// Check if they need to snap back.
	if (m_bgArray[1].GetDestinationTransform()->x <= 0)
	{
		m_bgArray[0].GetDestinationTransform()->x = 0;
		m_bgArray[1].GetDestinationTransform()->x = kWidth;
	}

	// Player animation/movement.
	m_pPlayer->Animate(deltaTime); // Oh! We're telling the player to animate itself. This is good! Hint hint.

	if (Game::GetInstance()->KeyDown(SDL_SCANCODE_A) && m_pPlayer->GetDestinationTransform()->x > m_pPlayer->GetDestinationTransform()->h)
	{
		m_pPlayer->GetDestinationTransform()->x -= kPlayerSpeed * deltaTime;
	}
	else if (Game::GetInstance()->KeyDown(SDL_SCANCODE_D) && m_pPlayer->GetDestinationTransform()->x < kWidth / 2)
	{
		m_pPlayer->GetDestinationTransform()->x += kPlayerSpeed * deltaTime;
	}
	if (Game::GetInstance()->KeyDown(SDL_SCANCODE_W) && m_pPlayer->GetDestinationTransform()->y > 0)
	{
		m_pPlayer->GetDestinationTransform()->y -= kPlayerSpeed * deltaTime;
	}
	else if (Game::GetInstance()->KeyDown(SDL_SCANCODE_S) && m_pPlayer->GetDestinationTransform()->y < kHeight - m_pPlayer->GetDestinationTransform()->w)
	{
		m_pPlayer->GetDestinationTransform()->y += kPlayerSpeed * deltaTime;
	}

	if (Game::GetInstance()->KeyDown(SDL_SCANCODE_SPACE) && m_canShoot)
	{
		m_canShoot = false;
		m_playerBullets.push_back(
			new Bullet({ kPlayerBulletSourceXPosition, kPlayerBulletSourceYPosition, kPlayerBulletWidth, kPlayerBulletHeight },
				{ m_pPlayer->GetDestinationTransform()->x + kPlayerWidth - kPlayerBulletWidth, m_pPlayer->GetDestinationTransform()->y + kPlayerHeight / 2 - kPlayerBulletWidth, kPlayerBulletWidth, kPlayerBulletHeight },
				kPlayerBulletSpeed));
		Mix_PlayChannel(-1, m_sounds[1], 0);
	}

	// Enemy animation/movement.
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		m_enemies[i]->Update(deltaTime);
		if (m_enemies[i]->GetDestinationTransform()->x < -m_enemies[i]->GetDestinationTransform()->h)
		{
			delete m_enemies[i];
			m_enemies[i] = nullptr;
			m_shouldClearEnemies = true;
		}
	}
	if (m_shouldClearEnemies)
	{
		CleanVector<Enemy*>(m_enemies, m_shouldClearEnemies);
	}

	// Update enemy spawns.
	m_enemySpawnTime += deltaTime;
	if (m_enemySpawnTime > m_enemySpawnMaxTime)
	{
		// Randomizing enemy bullet spawn rate
		float bulletSpawnRate = 0.5f + (rand() % 3) / 2.0f;
		// Random starting y location
		float yEnemyLocation = (float)(kEnemyHeight + rand() % (kHeight - kEnemyHeight));
		m_enemies.push_back(
			new Enemy({ kEnemySourceXPosition, kEnemySourceYPosition, kEnemyWidth, kEnemyHeight },
				{ kWidth, yEnemyLocation, kEnemyWidth, kEnemyHeight },
				&m_enemyBullets,
				m_sounds[0],
				bulletSpawnRate));

		m_enemySpawnTime = 0;
	}

	// Update the bullets. Player's first.
	for (size_t i = 0; i < m_playerBullets.size(); i++)
	{
		m_playerBullets[i]->Update(deltaTime);
		if (m_playerBullets[i]->GetDestinationTransform()->x > kWidth)
		{
			delete m_playerBullets[i];
			m_playerBullets[i] = nullptr;
			m_shouldClearPlayerBullets = true;
		}
	}

	if (m_shouldClearPlayerBullets)
	{
		CleanVector<Bullet*>(m_playerBullets, m_shouldClearPlayerBullets);
	}

	// Now enemy bullets.
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		m_enemyBullets[i]->Update(deltaTime);
		if (m_enemyBullets[i]->GetDestinationTransform()->x < -m_enemyBullets[i]->GetDestinationTransform()->w)
		{
			delete m_enemyBullets[i];
			m_enemyBullets[i] = nullptr;
			m_shouldClearEnemyBullets = true;
		}
	}

	if (m_shouldClearEnemyBullets)
	{
		CleanVector<Bullet*>(m_enemyBullets, m_shouldClearEnemyBullets);
	}

	CheckCollision();

	//Pausing
	if (Game::GetInstance()->KeyDown(SDL_SCANCODE_P))
	{
		StateManager::PushState(new PauseState);
	}
}
void GameState::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();

	// Render stuff. Background first.
	for (int i = 0; i < kNumberOfBackgrounds; i++)
	{
		SDL_RenderCopyF(pRenderer, m_pBGText, m_bgArray[i].GetSourceTransform(), m_bgArray[i].GetDestinationTransform());
	}

	// Player.
	SDL_RenderCopyExF(pRenderer,
		m_pSpriteTexture,
		m_pPlayer->GetSourceTransform(),
		m_pPlayer->GetDestinationTransform(),
		m_pPlayer->GetAngle(),
		&m_pivot,
		SDL_FLIP_NONE);

	// Player bullets.	
	for (size_t i = 0; i < m_playerBullets.size(); i++)
	{
		SDL_RenderCopyExF(pRenderer, m_pSpriteTexture, m_playerBullets[i]->GetSourceTransform(), m_playerBullets[i]->GetDestinationTransform(), 90, &m_pivot, SDL_FLIP_NONE);
	}

	// Enemies.
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		SDL_RenderCopyExF(pRenderer, m_pSpriteTexture, m_enemies[i]->GetSourceTransform(), m_enemies[i]->GetDestinationTransform(), -90, &m_pivot, SDL_FLIP_NONE);
	}

	// Enemy bullets.
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		SDL_RenderCopyF(pRenderer, m_pSpriteTexture, m_enemyBullets[i]->GetSourceTransform(), m_enemyBullets[i]->GetDestinationTransform());
	}
}

void GameState::Exit()
{
	std::cout << "Exiting Game State" << std::endl;

	std::cout << "Clean ..." << std::endl;

	// Delete Player
	delete m_pPlayer;
	m_pPlayer = nullptr;

	// Destroy Enemies
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		delete m_enemies[i];
		m_enemies[i] = nullptr;
	}
	m_enemies.clear();

	// Destroy Player Bullets
	for (size_t i = 0; i < m_playerBullets.size(); i++)
	{
		delete m_playerBullets[i];
		m_playerBullets[i] = nullptr;
	}
	m_playerBullets.clear();

	// Destroy Enemy Bullets
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		delete m_enemyBullets[i];
		m_enemyBullets[i] = nullptr;
	}
	m_enemyBullets.clear();

	// Clean sounds up
	for (size_t i = 0; i < m_sounds.size(); i++)
	{
		Mix_FreeChunk(m_sounds[i]);
	}
	m_sounds.clear();

	Mix_FreeMusic(m_pMusic);
}

void GameState::CheckCollision()
{
	// Player vs. Enemy.
	SDL_Rect playerCollisionRect = { (int)m_pPlayer->GetDestinationTransform()->x - kPlayerHeight, (int)m_pPlayer->GetDestinationTransform()->y, kPlayerHeight, kPlayerWidth };
	for (size_t i = 0; i < m_enemies.size(); i++)
	{
		SDL_Rect enemyCollisionRect = { (int)m_enemies[i]->GetDestinationTransform()->x, (int)m_enemies[i]->GetDestinationTransform()->y - kEnemyWidth, kEnemyHeight, kEnemyWidth };
		if (SDL_HasIntersection(&playerCollisionRect, &enemyCollisionRect))
		{
			// Game over!
			std::cout << "Player goes boom!" << std::endl;
			Mix_PlayChannel(-1, m_sounds[2], 0);
			delete m_enemies[i];
			m_enemies[i] = nullptr;
			m_shouldClearEnemies = true;
			StateManager::ChangeState(new LoseState());
			return;
		}
	}

	if (m_shouldClearEnemies)
	{
		CleanVector<Enemy*>(m_enemies, m_shouldClearEnemies);
	}

	// Player bullets vs. Enemies.
	for (size_t i = 0; i < m_playerBullets.size(); i++)
	{
		SDL_Rect bulletCollisionRect = { (int)m_playerBullets[i]->GetDestinationTransform()->x - kPlayerBulletHeight, (int)m_playerBullets[i]->GetDestinationTransform()->y, kPlayerBulletHeight, kPlayerBulletWidth };
		for (size_t j = 0; j < m_enemies.size(); j++)
		{
			if (m_enemies[j] != nullptr)
			{
				SDL_Rect enemyCollisionRect = { (int)m_enemies[j]->GetDestinationTransform()->x, (int)m_enemies[j]->GetDestinationTransform()->y - kEnemyWidth, kEnemyHeight, kEnemyWidth };
				if (SDL_HasIntersection(&bulletCollisionRect, &enemyCollisionRect))
				{
					Mix_PlayChannel(-1, m_sounds[2], 0);
					delete m_enemies[j];
					m_enemies[j] = nullptr;
					delete m_playerBullets[i];
					m_playerBullets[i] = nullptr;
					m_shouldClearEnemies = true;
					m_shouldClearPlayerBullets = true;
					StateManager::ChangeState(new WinState());
					return;
					//break;
				}
			}
		}
	}

	if (m_shouldClearEnemies)
	{
		CleanVector<Enemy*>(m_enemies, m_shouldClearEnemies);
	}

	if (m_shouldClearPlayerBullets)
	{
		CleanVector<Bullet*>(m_playerBullets, m_shouldClearPlayerBullets);
	}

	// Enemy bullets vs. player.
	for (size_t i = 0; i < m_enemyBullets.size(); i++)
	{
		SDL_Rect enemyBulletCollisionRect = { (int)m_enemyBullets[i]->GetDestinationTransform()->x, (int)m_enemyBullets[i]->GetDestinationTransform()->y, (int)m_enemyBullets[i]->GetDestinationTransform()->w, (int)m_enemyBullets[i]->GetDestinationTransform()->h };
		if (SDL_HasIntersection(&playerCollisionRect, &enemyBulletCollisionRect))
		{
			// Game over!
			std::cout << "Player goes boom!" << std::endl;
			Mix_PlayChannel(-1, m_sounds[2], 0);
			delete m_enemyBullets[i];
			m_enemyBullets[i] = nullptr;
			m_shouldClearEnemyBullets = true;
			StateManager::ChangeState(new LoseState());
			return;
		}
	}

	if (m_shouldClearEnemyBullets)
	{
		CleanVector<Bullet*>(m_enemyBullets, m_shouldClearEnemyBullets);
	}
}

void GameState::Pause()
{
	std::cout << "pausing" << std::endl;
}

void GameState::Resume()
{
	std::cout << "Resuming" << std::endl;
}
////////////
//end game state
//////////////


///////////////////
////begin Pause state
/////////////////////
void PauseState::Enter()
{
	std::cout << "Entering Pause State" << std::endl;
	
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();
	m_pBGText = IMG_LoadTexture(pRenderer, "Assets/Images/Pause.jpg");
	m_pPText = IMG_LoadTexture(pRenderer, "Assets/Images/PButton.png");


	SDL_FRect bDest;
	bDest.h = 200;
	bDest.w = 400;
	bDest.x = 312;
	bDest.y = 284;

	pButton = new PButton(bDest, m_pPText);
}
void PauseState::Update(float deltaTime)
{
	State::Update(deltaTime);
	pButton->Update(deltaTime);

	if (Game::GetInstance()->KeyDown(SDL_SCANCODE_R))
	{
		std::cout << "unpausing" << std::endl;
		StateManager::PopState();
	}
}
void PauseState::Render()
{
		StateManager::GetStates().front()->Render();
		SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();

		SDL_SetRenderDrawBlendMode(Game::GetInstance()->GetRenderer(), SDL_BLENDMODE_BLEND);
		SDL_SetRenderDrawColor(Game::GetInstance()->GetRenderer(), 128, 128, 128, 128);
		SDL_Rect bg{ 256,128, 512, 512 };
		SDL_RenderFillRect(Game::GetInstance()->GetRenderer(), &bg);
		//SDL_RenderCopyF(pRenderer, m_pBGText, NULL, NULL);
		pButton->Render();


		
}
void PauseState::Exit()
{
	std::cout << "Exiting Pause State" << std::endl;
}
////////////////
//end pause state
///////////////

///////////////////
////begin Lose state
/////////////////////
void LoseState::Enter()
{
	std::cout << "Entering lose State" << std::endl;

	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();
	m_pBGText = IMG_LoadTexture(pRenderer, "Assets/Images/Lose.jpg");
	m_pMMText = IMG_LoadTexture(pRenderer, "Assets/Images/MMButton.png");


	SDL_FRect bDest;
	bDest.h = 200;
	bDest.w = 200;
	bDest.x = 50;
	bDest.y = 50;

	mmButton = new MMButton(bDest, m_pMMText);
}
void LoseState::Update(float deltaTime)
{
	State::Update(deltaTime);
	mmButton->Update(deltaTime);

	
}
void LoseState::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();

	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(pRenderer, 128, 128, 128, 50);
	SDL_RenderClear(pRenderer);
	SDL_RenderCopyF(pRenderer, m_pBGText, NULL, NULL);
	mmButton->Render();



}
void LoseState::Exit()
{
	std::cout << "Exiting lose State" << std::endl;
}
////////////////
//end lose state
///////////////

///////////////////
////begin win state
/////////////////////
void WinState::Enter()
{
	std::cout << "Entering win State" << std::endl;

	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();
	m_pBGText = IMG_LoadTexture(pRenderer, "Assets/Images/win.jpg");
	m_pMMText = IMG_LoadTexture(pRenderer, "Assets/Images/MMButton.png");


	SDL_FRect bDest;
	bDest.h = 200;
	bDest.w = 200;
	bDest.x = 50;
	bDest.y = 50;

	mmButton = new MMButton(bDest, m_pMMText);
}
void WinState::Update(float deltaTime)
{
	State::Update(deltaTime);
	mmButton->Update(deltaTime);


}
void WinState::Render()
{
	SDL_Renderer* pRenderer = Game::GetInstance()->GetRenderer();


	SDL_SetRenderDrawBlendMode(pRenderer, SDL_BLENDMODE_BLEND);
	SDL_SetRenderDrawColor(pRenderer, 128, 128, 128, 50);
	SDL_RenderClear(pRenderer);
	SDL_RenderCopyF(pRenderer, m_pBGText, NULL, NULL);
	mmButton->Render();



}
void WinState::Exit()
{
	std::cout << "Exiting win State" << std::endl;
}
////////////////
//end win state
///////////////
