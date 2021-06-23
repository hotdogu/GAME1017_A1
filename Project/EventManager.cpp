#include "EventManager.h"
#include "Game.h"
#include <cstring>
#include <iostream>

void EventManager::Init()
{
	s_currentKeyState = SDL_GetKeyboardState(&s_numKeys);
	s_lastKeyState = new Uint8[s_numKeys];
	std::memcpy(s_lastKeyState, s_currentKeyState, s_numKeys);
	s_currentMouseState = SDL_GetMouseState(&s_mousePos.x, &s_mousePos.y);
	s_lastMouseState = s_currentMouseState;
	std::cout << "EventManager init done!" << std::endl;
}

void EventManager::HandleEvents()
{
	SDL_Event event;

	std::memcpy(s_lastKeyState, s_currentKeyState, s_numKeys);
	s_lastMouseState = s_currentMouseState;
	s_lastKeyDown = -1;
	s_lastKeyUp = -1;
	s_currentKeyState = SDL_GetKeyboardState(&s_numKeys);
	s_currentMouseState = SDL_GetMouseState(&s_mousePos.x, &s_mousePos.y);
}

bool EventManager::KeyHeld(const SDL_Scancode key)
{
	if (s_currentKeyState)
	{
		return s_currentKeyState[key] == 1;
	}
	return false;

}

bool EventManager::KeyPressed(const SDL_Scancode key)
{
	return (s_currentKeyState[key] > s_lastKeyState[key]);
}

bool EventManager::KeyReleased(const SDL_Scancode key)
{
	return (s_currentKeyState[key] < s_lastKeyState[key]);
}

int EventManager::LastKeyDown()
{
	return s_lastKeyDown;
}

int EventManager::LastKeyUp()
{
	return s_lastKeyUp;
}

bool EventManager::MouseHeld(const int button)
{
	if (button >= 1 && button <= 3)
		return (s_currentMouseState & SDL_BUTTON(button));
	else
		return false;
}

bool EventManager::MousePressed(const int button)
{
	return ((s_currentMouseState & SDL_BUTTON(button)) > (s_lastMouseState & SDL_BUTTON(button)));
}

bool EventManager::MouseReleased(const int button)
{
	return ((s_currentMouseState & SDL_BUTTON(button)) < (s_lastMouseState & SDL_BUTTON(button)));
}

SDL_Point& EventManager::GetMousePos()
{
	return s_mousePos;
}

void EventManager::SetCursor(const SDL_SystemCursor& cursor)
{
	SDL_FreeCursor(s_cursor);
	s_cursor = SDL_CreateSystemCursor(cursor);
	SDL_SetCursor(s_cursor);
}

void EventManager::Quit()
{
	delete s_lastKeyState;
	SDL_FreeCursor(s_cursor);
}

const Uint8* EventManager::s_currentKeyState = nullptr;
Uint8* EventManager::s_lastKeyState;
int EventManager::s_numKeys;

int EventManager::s_lastKeyDown;
int EventManager::s_lastKeyUp;
SDL_Point EventManager::s_mousePos = { 0,0 };
Uint32 EventManager::s_currentMouseState;
Uint32 EventManager::s_lastMouseState;

SDL_Cursor* EventManager::s_cursor;