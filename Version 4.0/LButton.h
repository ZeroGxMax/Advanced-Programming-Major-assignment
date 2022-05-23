
#ifndef LBUTTON_H
#define LBUTTON_H

#include "LTexture.h"

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

		LTexture mTexture;

		// Area of button
		int buttonWidth;
		int buttonHeight;

	public:
		//Initializes internal variables
		LButton();

		//Sets top left position
		void setPosition( int x, int y , int w, int h);

		void setButtonArea(SDL_Rect *area);

		//Handles mouse event
		bool isInside( SDL_Event* e );

		void selected();

		void freeAll();

		void free();
};
#endif