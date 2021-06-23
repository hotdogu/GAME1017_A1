#pragma once
#include <vector>

class State;

class StateManager
{
private:
	StateManager() {}
	static std::vector<State*> s_states;

public:
	static void Update(float deltaTime);
	static void Render();

	static void ChangeState(State* pState);


	static void PushState(State* pState);
	static void PopState();

	static void Quit();

	static std::vector<State*>& GetStates() { return s_states; }
};