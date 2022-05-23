#ifndef LTEXTURE_H
#define LTEXTURE_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include "Painter.h"

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Texture wrapper class
struct LTexture
{
	//The actual hardware texture
	SDL_Texture* mTexture;

	// Font of the Texture (Optional)
	TTF_Font* mFont;

	//Image dimensions
	int mWidth;
	int mHeight;

	public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile( std::string path );

	//Creates image from font string
	bool loadFromRenderedText( std::string textureText, SDL_Color textColor );

	//Deallocates texture
	void free();

	void freeAll();

	//Renders texture at given point
	void render( int x, int y, SDL_Rect* clip = NULL );
};

#endif