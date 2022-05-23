#include "LButton.h"

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
}

void LButton::freeAll()
{
	mTexture.freeAll();
}

void LButton::setPosition( int x, int y, int w, int h)
{
	mPosition.x = x;
	mPosition.y = y;
	buttonWidth = w;
	buttonHeight = h;
}

bool LButton::isInside( SDL_Event* e )
{
	//Check if mouse is in button
	bool inside = false;

	//If mouse event happened
	if(e->type == SDL_MOUSEBUTTONDOWN)
	{
		//Get mouse position
		int x, y;
		SDL_GetMouseState( &x, &y );

		//Mouse is left of the button
		if( x >= mPosition.x && x <= mPosition.x + buttonWidth && y >= mPosition.y &&  y <= mPosition.y + buttonHeight)
		{
			inside = true;
		}
	}
	return inside;
}

void LButton::selected()
{
	SDL_Rect selectedButton = {mPosition.x, mPosition.y, buttonWidth, buttonHeight};
	SDL_SetRenderDrawColor(gRenderer, 255, 0, 0, 255);
	SDL_RenderDrawRect(gRenderer, &selectedButton);
	selectedButton = {mPosition.x-1, mPosition.y-1, buttonWidth, buttonHeight};
	SDL_RenderDrawRect(gRenderer, &selectedButton);
	selectedButton = {mPosition.x+1, mPosition.y+1, buttonWidth, buttonHeight};
	SDL_RenderDrawRect(gRenderer, &selectedButton);
}

void LButton::setButtonArea(SDL_Rect *area)
{
	mPosition.x = area->x;
	mPosition.y = area->y;
	buttonWidth = area->w;
	buttonHeight = area->h;
}