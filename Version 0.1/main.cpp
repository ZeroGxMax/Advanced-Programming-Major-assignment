
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "LTexture.cpp"
#include "Button.cpp"
using namespace std;

enum gameProcess
{
	betTime,
	rollingTime,
	resultTime,
	finishTime
};

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 600;

// Tai and Xiu Button Position:
SDL_Point pointTai = {364, 0};
SDL_Point pointXiu = {0, 0};

// Three dices position:
SDL_Point firstDicePos = {770, 262};
SDL_Point secondDicePos = {918, 262};
SDL_Point thirdDicePos = {1073, 262};

// Banner position:
SDL_Point bannerPosition = {0, 160};

// 3 main function:
bool init();
bool loadMedia();
void close();

// Render function
void renderGamePlay();
void renderDices(int *dices);
void renderBanner(bool &isWin);

// Dice data function:
int random(const int &min,const int &max);
int rollDices(int *dices);
bool checkTotalOfThreeDice(const int &totalOfThreeDice, const string &playerBetPlace);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//Scene sprites
LTexture gSpriteSicboBoard, gSpriteTai, gSpriteXiu;
// Dices sprites;
LTexture gSpriteDices[6];
// Win, lose banner:
LTexture gSpriteWin;
LTexture gSpriteLose;

// Tai and Xiu Button:
LButton gButtonTai;
LButton gButtonXiu;


int main( int argc, char* args[] )
{
	// Variables:
	int *dices = new int[3];
    int totalOfThreeDice = 0;
	string playerBetPlace;
	bool isWin;

	srand((int)time(NULL));

	//Start up SDL and create window
	if( !init() )
	{
		printf( "Failed to initialize!\n" );
	}
	else
	{
		//Load media
		if( !loadMedia() )
		{
			printf( "Failed to load media!\n" );
		}
		else
		{	
			//Main loop flag
			bool quit = false;

			//Event handler
			SDL_Event e;

			// Default gameProcess;
			gameProcess currentPeriod = betTime;

            renderGamePlay();

			//While application is running
			while( !quit )
			{
				//Handle events on queue
				while( SDL_PollEvent( &e ) != 0 )
				{
					//User requests quit
					if( e.type == SDL_QUIT )
					{
						quit = true;
					}
					if (currentPeriod == betTime)
					{
						// Handle mouse event:
                    	if (gButtonTai.isInside(&e))
						{
							currentPeriod = rollingTime;
							playerBetPlace = "Tai";
						}
						if (gButtonXiu.isInside(&e))
						{
							currentPeriod = rollingTime;
							playerBetPlace = "Xiu";
						}
					}
				}

				if (currentPeriod == rollingTime)
				{
					SDL_Delay(600);
					totalOfThreeDice = rollDices(dices);
					renderDices(dices);
					SDL_RenderPresent(gRenderer);
					currentPeriod = resultTime;
				}

				if (currentPeriod == resultTime)
				{
					SDL_Delay(600);
					if (checkTotalOfThreeDice(totalOfThreeDice, playerBetPlace))
					{
						isWin = true;
					}
					else
					{
						isWin = false;
					}
					currentPeriod = finishTime;
				}

				if (currentPeriod == finishTime)
				{
					SDL_Delay(1000);
					renderBanner(isWin);
					SDL_RenderPresent( gRenderer );
				}

				//Update screen
				SDL_RenderPresent( gRenderer );
			}
		}
	}

	//Free resources and close SDL
	close();

	return 0;
}

bool init()
{
    bool success = true;
    if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("Unable to initialize! Error: %s\n", SDL_GetError());
        success = false;
    }
    else
    {
        if (!SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "1"))
        {
            printf("Unable to set hint: Linear");
        }
        gWindow = SDL_CreateWindow("SicBo", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
        if (gWindow == NULL)
        {
            printf("Unable to create Window! Error: %s\n", SDL_GetError());
            success = NULL;
        }
        else
        {
            gRenderer = SDL_CreateRenderer(gWindow, -1, SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);
            if (gRenderer == NULL)
            {
                printf("Unable to create renderer! Error: %s", SDL_GetError());
                success = false;
            }
            else
            {
                SDL_SetRenderDrawColor(gRenderer, 0, 0, 0, 255);
                
                int imgFlags = IMG_INIT_PNG;
                if (!(IMG_Init(imgFlags) && IMG_INIT_PNG))
                {
                    printf("Unable to initialize PNG! Error: %s", IMG_GetError());
                    success = false;
                }
            }
        }
    }
    return success;
}

bool loadMedia()
{
	//Loading success flag
	bool success = true;

	//Load sprite sheet texture
	if( !gSpriteSicboBoard.loadFromFile( "Image/SicBo.png" ) ||!gSpriteTai.loadFromFile( "Image/Tai.png" ) || !gSpriteXiu.loadFromFile( "Image/Xiu.png" ))
	{
		printf( "Failed to load resources!\n" );
		success = false;
	}
	else
	{
        // Set Tai Button and Xiu Button
        gButtonTai.setPosition(pointTai.x, pointTai.y);
        gButtonXiu.setPosition(pointXiu.x, pointXiu.y);
	}
	gSpriteDices[0].loadFromFile("Image/Dices/dice1.png");
	gSpriteDices[1].loadFromFile("Image/Dices/dice2.png");
	gSpriteDices[2].loadFromFile("Image/Dices/dice3.png");
	gSpriteDices[3].loadFromFile("Image/Dices/dice4.png");
	gSpriteDices[4].loadFromFile("Image/Dices/dice5.png");
	gSpriteDices[5].loadFromFile("Image/Dices/dice6.png");
	gSpriteWin.loadFromFile("Image/Banner/Win.png");
	gSpriteLose.loadFromFile("Image/Banner/Lose.png");

	return success;
}

void close()
{
	//Free loaded images
	gSpriteSicboBoard.free();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );

	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	IMG_Quit();
	SDL_Quit();
}

void renderGamePlay()
{
    //Clear screen
	SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
	SDL_RenderClear( gRenderer );

    gSpriteSicboBoard.render(0, 0, NULL);
}

int random(const int &min,const int &max)
{
    return min + rand() % (max - min + 1);
}

int rollDices(int *dices)
{
    int sum = 0;
    for (int i = 0; i < 3; i++)
    {
        dices[i] = random(1, 6);
        sum += dices[i];
    }

    return sum;
}

bool checkTotalOfThreeDice(const int &totalOfThreeDice, const string &playerBetPlace)
{
    if (totalOfThreeDice > 10 && playerBetPlace == "Tai") return true;
    else if (totalOfThreeDice <= 10 && playerBetPlace == "Xiu") return true;
    else return false;
}

void renderDices(int *dices)
{
	switch (dices[0])
	{
		case 1:
			gSpriteDices[0].render(firstDicePos.x, firstDicePos.y, NULL);
			break;

		case 2:
			gSpriteDices[1].render(firstDicePos.x, firstDicePos.y, NULL);
			break;
		
		case 3:
			gSpriteDices[2].render(firstDicePos.x, firstDicePos.y, NULL);
			break;
		
		case 4:
			gSpriteDices[3].render(firstDicePos.x, firstDicePos.y, NULL);
			break;
		
		case 5:
			gSpriteDices[4].render(firstDicePos.x, firstDicePos.y, NULL);
			break;

		case 6:
			gSpriteDices[5].render(firstDicePos.x, firstDicePos.y, NULL);
			break;
	}
	switch (dices[1])
	{
		case 1:
			gSpriteDices[0].render(secondDicePos.x, secondDicePos.y, NULL);
			break;

		case 2:
			gSpriteDices[1].render(secondDicePos.x, secondDicePos.y, NULL);
			break;
		
		case 3:
			gSpriteDices[2].render(secondDicePos.x, secondDicePos.y, NULL);
			break;
		
		case 4:
			gSpriteDices[3].render(secondDicePos.x, secondDicePos.y, NULL);
			break;
		
		case 5:
			gSpriteDices[4].render(secondDicePos.x, secondDicePos.y, NULL);
			break;

		case 6:
			gSpriteDices[5].render(secondDicePos.x, secondDicePos.y, NULL);
			break;
	}
	switch (dices[2])
	{
		case 1:
			gSpriteDices[0].render(thirdDicePos.x, thirdDicePos.y, NULL);
			break;

		case 2:
			gSpriteDices[1].render(thirdDicePos.x, thirdDicePos.y, NULL);
			break;
		
		case 3:
			gSpriteDices[2].render(thirdDicePos.x, thirdDicePos.y, NULL);
			break;
		
		case 4:
			gSpriteDices[3].render(thirdDicePos.x, thirdDicePos.y, NULL);
			break;
		
		case 5:
			gSpriteDices[4].render(thirdDicePos.x, thirdDicePos.y, NULL);
			break;

		case 6:
			gSpriteDices[5].render(thirdDicePos.x, thirdDicePos.y, NULL);
			break;
	}
}

void renderBanner(bool &isWin)
{
	if (isWin)
	{
		gSpriteWin.render(bannerPosition.x, bannerPosition.y, NULL);
	}
	else
	{
		gSpriteLose.render(bannerPosition.x, bannerPosition.y, NULL);
	}
}