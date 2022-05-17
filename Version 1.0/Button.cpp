#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include "LTexture.cpp"

enum LButtonSprite
{
	BUTTON_SPRITE_MOUSE_OUT = 0,
	BUTTON_SPRITE_MOUSE_OVER_MOTION = 1,
	BUTTON_SPRITE_MOUSE_DOWN = 2,
	BUTTON_SPRITE_MOUSE_UP = 3,
	BUTTON_SPRITE_TOTAL = 4
};

struct LButton
{
	public:
		//Top left position
		SDL_Point mPosition;

		//Currently used global sprite
		LButtonSprite mCurrentSprite;

		// Area of button
		int buttonWidth;
		int buttonHeight;

	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y , int w, int h);

		//Handles mouse event
		void handleEvent( SDL_Event* e );

		bool isInside( SDL_Event* e );
};

LButton::LButton()
{
	mPosition.x = 0;
	mPosition.y = 0;

	mCurrentSprite = BUTTON_SPRITE_MOUSE_OUT;
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

