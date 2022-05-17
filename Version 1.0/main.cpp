
#include <SDL2/SDL.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstdint>
#include "LTexture.cpp"
#include "Button.cpp"
#include "Painter.h"
using namespace std;

enum gameProcess
{
	startPhase,
	betTime,
	rollingTime,
	waitingPhase,
	resultTime,
	finishTime,
	endPhase
};

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 600;

//Button constants
const int SicBo_BUTTON_WIDTH = 274;
const int SicBo_BUTTON_HEIGHT = 342;
const int TOTAL_BUTTONS = 2;

// Waiting time between periods:
const int waitingTime = 600;

// Tai and Xiu Button Position:
SDL_Point pointTai = {364, 0};
SDL_Point pointXiu = {0, 0};

// Three dices position:
SDL_Point firstDicePos = {770, 262};
SDL_Point secondDicePos = {918, 262};
SDL_Point thirdDicePos = {1073, 262};

// Banner position:
SDL_Point bannerPosition = {0, 160};

// Text positions:
SDL_Point clickToContinuePosition = {688, 542};
SDL_Point assetTextPos = {648, 6};
SDL_Point totalTextPos = {893, 390};

// Portion of screen:
SDL_Rect assetViewport = {assetTextPos.x, assetTextPos.y, 616, 50};

// 3 main function:
bool init();
bool loadMedia();
void close();

// Render function
void renderGameBoard();
void renderDices(int *dices);
void renderBanner(bool &isWin);
void renderClickContinue(bool wantDelete = false);
void renderAsset(const int64_t &playerTotalMoney);
void renderTotal(const int &TotalOfThreeDice);

// Audio function
void sicboAudio(const int &totalOfThreeDice);

// Dice data function:
int random(const int &min,const int &max);
int rollDices(int *dices);
bool checkTotalOfThreeDice(const int &totalOfThreeDice, const string &playerBetPlace);

// calculate function:
void updatePlayerAsset(const bool &isWin, int64_t &playerTotalMoney);

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//Scene sprites
LTexture gSpriteSicboBoard, gSpriteTai, gSpriteXiu;
// Dices sprites;
LTexture gSpriteDices[6];
// Win, lose banner:
LTexture gSpriteWin;
LTexture gSpriteLose;
// Text sprite:
LTexture gSpriteClickContinue;
// Font Texture:
LTexture gTextAsset;
LTexture gTextTotal;
// Deleter
LTexture gDeleter;

// Tai and Xiu Button:
LButton gButtonTai;
LButton gButtonXiu;
LButton gButtonAnywhere;

int main( int argc, char* args[] )
{
	// Variables:
	int *dices = new int[3];
    int totalOfThreeDice = 0;
	string playerBetPlace;
	bool isWin;
	int64_t playerTotalMoney = 10;

	srand((int)time(NULL));

	//Start up SDL and create window
	init();
	loadMedia();	
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	// Default gameProcess;
	gameProcess currentPeriod = startPhase;

	//While application is running
	while( !quit )
	{
		if (currentPeriod == startPhase)
		{
			renderGameBoard();
			renderAsset(playerTotalMoney);
			SDL_RenderPresent(gRenderer);
			currentPeriod = betTime;
		}
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
					playerBetPlace = "Tai";
					playerTotalMoney--;
					currentPeriod = rollingTime;
				}
				if (gButtonXiu.isInside(&e))
				{
					playerBetPlace = "Xiu";
					playerTotalMoney--;
					currentPeriod = rollingTime;
				}
				renderAsset(playerTotalMoney);
			}
			if (currentPeriod == waitingPhase)
			{
				renderClickContinue();
				if (gButtonAnywhere.isInside(&e))
				{
					currentPeriod = resultTime;
				}
			}
			if (currentPeriod == endPhase)
			{
				renderClickContinue();
				if (gButtonAnywhere.isInside(&e))
				{
					currentPeriod = startPhase;
				}
			} 
		}
		if (currentPeriod == rollingTime)
		{
			for (int i = 0; i < 20; i++)
			{
				rollDices(dices);
				renderDices(dices);
				SDL_RenderPresent(gRenderer);
				SDL_Delay(40);
			}
			totalOfThreeDice = rollDices(dices);
			renderDices(dices);
			renderTotal(totalOfThreeDice);
			SDL_RenderPresent(gRenderer);
			currentPeriod = waitingPhase; // Waiting phase is in event loop
		}

		if (currentPeriod == resultTime)
		{
			if (checkTotalOfThreeDice(totalOfThreeDice, playerBetPlace))
			{
				isWin = true;
			}
			else isWin = false;
			updatePlayerAsset(isWin, playerTotalMoney);
			
			currentPeriod = finishTime;
		}

		if (currentPeriod == finishTime)
		{
			renderAsset(playerTotalMoney);
			renderBanner(isWin);
			SDL_RenderPresent( gRenderer );
			currentPeriod = endPhase;
		}
		SDL_RenderPresent(gRenderer);
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
            success = false;
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

				//Initialize SDL_ttf
				if( TTF_Init() == -1 )
				{
					printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
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
	if( !gSpriteSicboBoard.loadFromFile( "Image/Sicbo.png" ) ||!gSpriteTai.loadFromFile( "Image/Tai.png" ) || !gSpriteXiu.loadFromFile( "Image/Xiu.png" ))
	{
		printf( "Failed to load resources!\n" );
		success = false;
	}
	// Load dices images
	gSpriteDices[0].loadFromFile("Image/Dices/dice1.png");
	gSpriteDices[1].loadFromFile("Image/Dices/dice2.png");
	gSpriteDices[2].loadFromFile("Image/Dices/dice3.png");
	gSpriteDices[3].loadFromFile("Image/Dices/dice4.png");
	gSpriteDices[4].loadFromFile("Image/Dices/dice5.png");
	gSpriteDices[5].loadFromFile("Image/Dices/dice6.png");
	// Load win and lose banner
	gSpriteWin.loadFromFile("Image/Banner/Win.png");
	gSpriteLose.loadFromFile("Image/Banner/Lose.png");
	// Load Deleter:
	gDeleter.loadFromFile("Image/Delete/Deleter.png");
	//Load some text
	gSpriteClickContinue.loadFromFile("Image/click.png");

	// Load Text:
	gTextAsset.mFont = TTF_OpenFont("Font/helvet.ttf", 30);
	gTextTotal.mFont = TTF_OpenFont("Font/helvet.ttf", 30);

	// Set Tai Button and Xiu Button
    gButtonTai.setPosition(pointTai.x, pointTai.y, SicBo_BUTTON_WIDTH, SicBo_BUTTON_HEIGHT);
    gButtonXiu.setPosition(pointXiu.x, pointXiu.y, SicBo_BUTTON_WIDTH, SicBo_BUTTON_HEIGHT);
	// Set anywhere button:
	gButtonAnywhere.setPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	return success;
}

void close()
{
	// Close font before loading
	//Free loaded images
	gSpriteSicboBoard.freeAll();
	gSpriteWin.freeAll();
	gSpriteLose.freeAll();
	for (int i = 0; i < 6; i++)
	{
		gSpriteDices[i].freeAll();
	}
	gSpriteClickContinue.freeAll();
	gTextAsset.freeAll();
	gTextTotal.freeAll();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );

	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	TTF_Quit();
	IMG_Quit();
	SDL_Quit();
}

void renderGameBoard()
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

void renderClickContinue(bool wantDelete)
{
	if (!wantDelete)
	{
		gSpriteClickContinue.render(clickToContinuePosition.x, clickToContinuePosition.y, NULL);
		SDL_RenderPresent(gRenderer);
	}
}

void sicboAudio(const int &totalOfThreeDice)
{
	if (totalOfThreeDice <= 10) system("xiu.mp3");
	else system("tai.mp3");
}

void renderAsset(const int64_t &playerTotalMoney)
{
	// Create message and number
	string message = "Your asset: ";
	char playerMoney[64];
	sprintf(playerMoney, "%lld", playerTotalMoney);
	string money = string(playerMoney);
	money = message + playerMoney;

	// Delete one before
	gDeleter.render(assetTextPos.x, assetTextPos.y, &assetViewport);

	// Loading and Rendering:
	gTextAsset.loadFromRenderedText(money.c_str(), BLACK_COLOR);
	gTextAsset.render(assetTextPos.x, assetTextPos.y, NULL);
	SDL_RenderPresent(gRenderer);
}

void renderTotal(const int &TotalOfThreeDice)
{
	char total[3];
	string message = "Total: ";
	sprintf(total, "%d", TotalOfThreeDice);
	string sTotal = string(total);
	sTotal = message + sTotal;
	gTextTotal.loadFromRenderedText(sTotal, BLACK_COLOR);
	gTextTotal.render(totalTextPos.x, totalTextPos.y, NULL);
	SDL_RenderPresent(gRenderer);
}

void updatePlayerAsset(const bool &isWin, int64_t &playerTotalMoney)
{
	if (isWin) playerTotalMoney += 2;
}

