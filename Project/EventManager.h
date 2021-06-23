#pragma once
#include <SDL.h>

class EventManager
{
public:
	static void Init();
	static void HandleEvents();

	static bool KeyHeld(const SDL_Scancode key);
	static bool KeyPressed(const SDL_Scancode key);
	static bool KeyReleased(const SDL_Scancode key);
	static int LastKeyDown();
	static int LastKeyUp();

	static bool MouseHeld(const int button);
	static bool MousePressed(const int button);
	static bool MouseReleased(const int button);
	static SDL_Point& GetMousePos();

	static void SetCursor(const SDL_SystemCursor& cursor);

	static void Quit();

private:

	// Keyboard state containers.
	static const Uint8* s_currentKeyState;
	static Uint8* s_lastKeyState;
	static int s_numKeys;

	// Mouse state containers.
	static int s_lastKeyDown;
	static int s_lastKeyUp;
	static SDL_Point s_mousePos;
	static Uint32 s_currentMouseState;
	static Uint32 s_lastMouseState;

	static SDL_Cursor* s_cursor;

private:
	EventManager() {}
};

