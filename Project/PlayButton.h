#pragma once
#include "UI_Button.h"
class PlayButton : public UI_Button
{
public:
	PlayButton(SDL_FRect destination, SDL_Texture* texture);
	virtual void Render() override;
private:
	virtual void Execute() override;
};

class MMButton : public UI_Button
{
public:
	MMButton(SDL_FRect destination, SDL_Texture* texture);
	virtual void Render() override;
private:
	virtual void Execute() override;
};

class PButton : public UI_Button
{
public:
	PButton(SDL_FRect destination, SDL_Texture* texture);
	virtual void Render() override;
private:
	virtual void Execute() override;
};
