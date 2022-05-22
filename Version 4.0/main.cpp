
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstdint>
#include "LTexture.cpp"
#include "Button.cpp"
#include "Painter.h"
#include "Point and Area.cpp"
#include "enumType.cpp"
#include "Reward_ratio.h"
using namespace std;

//Screen dimension constants
const int SCREEN_WIDTH = 1280;
const int SCREEN_HEIGHT = 680;

//Button constants
const int SicBo_BUTTON_WIDTH = 386;
const int SicBo_BUTTON_HEIGHT = 274;
const int TOTAL_BUTTONS = 2;

// Waiting time between periods:
const int waitingTime = 600;

// 3 main function:
bool init();
bool loadMedia();
void close();

// Render function
void renderGameBoard();
void renderDices(int *dices);
void renderBanner(bool &isWin);
void renderAsset(const int64_t &playerTotalMoney);
void renderTotal(const int &TotalOfThreeDice);
void renderSelectedButton(LButton &button);
void renderStartClear();
void renderMessage(string message, const int &number, bool havingNumber = false);
void renderBetButton();

// Audio function
void sicboAudio(const int &totalOfThreeDice);

// Event handle function:
void handleBetTimeEvent(SDL_Event e, gameProcess &currentPeriod, int64_t &playerTotalMoney, string &playerBetPlace, uint64_t &betMoney, int &betPerClick);
void handleWaitingPhases(SDL_Event e, gameProcess &currentPeriod);
void handleQuitEvent(SDL_Event e, bool &quit);

// Dice data function:
int random(const int &min,const int &max);
int rollDices(int *dices);
bool checkResult(const int &totalOfThreeDice, const int *dices, const string &playerBetPlace, uint64_t &betMoney);

// calculate function:
void updatePlayerAsset(const bool &isWin, int64_t &playerTotalMoney, uint64_t &betMoney);
void changeBetPerClick(int &betPerClick);

// Other button function:
void handleSupportButtonsEvent(SDL_Event e, bool &quit);
void handleMusicState();

// Game Process handle function:
void handleMainMenu(SDL_Event e, bool &quit);
void handleStartPhase(gameProcess &currentPeriod, int64_t &playerTotalMoney, string &playerBetPlace, uint64_t &betMoney);

// other function:
void setToBegin(int64_t &playerTotalMoney, string &playerBetPlace, uint64_t &betMoney);
void setBetPlace(LButton &button, string &playerBetPlace, string betPlace, int64_t &playerTotalMoney,  uint64_t &betMoney, int &betPerClick);
//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//Main menu
LTexture gMainMenu;
LButton gButtonBatDau;
LButton gButtonThoat;

//Scene sprites
LTexture gSpriteSicboBoard;
// Dices sprites;
LTexture gSpriteDices[6];
// Win, lose banner:
LTexture gSpriteWin;
LTexture gSpriteLose;

// Text:
LTexture gTextAsset;
LTexture gTextTotal;
LTexture gTextClickContinue;
LTexture gTextMessage;
// Deleter
LTexture gDeleter;

// Tai and Xiu Button:
LButton gButtonTai;
LButton gButtonXiu;
// Triples buttons
LButton gButtonTriple[6];
// Mono buttons:
LButton gButtonMono[14];
// Dice buttons:
LButton gButtonDice[6];
// Chan and Le Button:
LButton gButtonChan;
LButton gButtonLe;

LButton gButtonAnywhere;
// Button with sprites:
LButton gButtonStart;
LButton gButtonClear;
LButton gButtonBetAmount[xTotal];
LButton gButtonMusic[2];
LButton gButtonSFX[2];
LButton gButtonMainMenu;
LButton gButtonQuitGame;

// Music and chunk:
Mix_Music *gMusic;
Mix_Chunk *gDiceRolling;
Mix_Chunk *gSoundWin;
Mix_Chunk *gSoundLose;

// other variable:
betAmount currentAmount = x1;
gameProcess currentPeriod = MainMenu;
State MusicState = ON;
State SFXState = ON;

int main( int argc, char* args[] )
{
	// Variables:
	int *dices = new int[3];
    int totalOfThreeDice = 0;
	string playerBetPlace;
	bool isWin;
	int64_t playerTotalMoney = 10;
	uint64_t betMoney = 0;
	int betPerClick = 1;

	srand((int)time(NULL));
	setArea(); // init constant values

	//Start up SDL and create window
	init();
	loadMedia();	
	//Main loop flag
	bool quit = false;

	//Event handler
	SDL_Event e;

	
	if (currentPeriod != MainMenu)
	{
		gMusic = Mix_LoadMUS("Audio/Music.mp3");
		Mix_PlayMusic(gMusic, -1);
	}
	//While application is running
	while( !quit )
	{
		if (currentPeriod != MainMenu)
		{
			handleMusicState();
			changeBetPerClick(betPerClick);
		}
		handleStartPhase(currentPeriod, playerTotalMoney, playerBetPlace, betMoney);
		//Handle events on queue
		while( SDL_PollEvent( &e ) != 0 )
		{
			handleMainMenu(e, quit);
			handleSupportButtonsEvent(e, quit);
			handleQuitEvent(e, quit);
			handleBetTimeEvent(e, currentPeriod, playerTotalMoney, playerBetPlace, betMoney, betPerClick);
			handleWaitingPhases(e, currentPeriod);
		}
		if (currentPeriod == rollingTime)
		{
			if (SFXState == ON) Mix_PlayChannel(-1, gDiceRolling, 0);
			gDeleter.render(playerAreaViewport.x, playerAreaViewport.y, &playerAreaViewport);
			for (int i = 0; i < 30; i++)
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
			if (checkResult(totalOfThreeDice, dices, playerBetPlace, betMoney))
			{
				isWin = true;
			}
			else isWin = false;
			updatePlayerAsset(isWin, playerTotalMoney, betMoney);
			
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

				int audioFlags = MIX_INIT_MP3;
				int initted = Mix_Init(audioFlags);
				if (initted&audioFlags != audioFlags)
				{
					printf("Mix_Init: Failed to initialize MP3 support!\n");
					success = false;
				}
				if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
				{
					printf ("SDL_mixer could not initialize! SDL_mixer Error: %s\n", Mix_GetError());
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

	// Loading main menu:
	gMainMenu.loadFromFile("Image/Main menu/Menu.png");
	gButtonBatDau.setButtonArea(&gMenuBatDau);
	gButtonThoat.setButtonArea(&gMenuThoat);

	//Load sprite sheet texture
	gSpriteSicboBoard.loadFromFile( "Image/Background.png" );
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

	// Load Text:
	gTextAsset.mFont = TTF_OpenFont("Font/helvet.ttf", 30);
	gTextTotal.mFont = TTF_OpenFont("Font/helvet.ttf", 30);
	gTextClickContinue.mFont = TTF_OpenFont("Font/helvet.ttf", 30);
	gTextMessage.mFont = TTF_OpenFont("Font/helvet.ttf", 30);

	// Set anywhere button:
	gButtonAnywhere.setPosition(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

	// Load Start and Clear Button images:
	gButtonStart.mTexture.loadFromFile("Image/Buttons/Start.png");
	gButtonStart.setPosition(pointStart.x, pointStart.y, gButtonStart.mTexture.mWidth, gButtonStart.mTexture.mHeight);
	gButtonClear.mTexture.loadFromFile("Image/Buttons/Clear.png");
	gButtonClear.setPosition(pointClear.x, pointClear.y, gButtonClear.mTexture.mWidth, gButtonClear.mTexture.mHeight);

	// Load bet amount per click button images:
	gButtonBetAmount[x1].mTexture.loadFromFile("Image/Buttons/x1.png");
	gButtonBetAmount[x2].mTexture.loadFromFile("Image/Buttons/x2.png");
	gButtonBetAmount[x5].mTexture.loadFromFile("Image/Buttons/x5.png");
	gButtonBetAmount[x10].mTexture.loadFromFile("Image/Buttons/x10.png");
	gButtonBetAmount[x20].mTexture.loadFromFile("Image/Buttons/x20.png");
	gButtonBetAmount[x50].mTexture.loadFromFile("Image/Buttons/x50.png");
	gButtonBetAmount[x100].mTexture.loadFromFile("Image/Buttons/x100.png");

	// Load function button images and set areas for them:
	gButtonMusic[0].mTexture.loadFromFile("Image/Buttons/Music_on.png");
	gButtonMusic[1].mTexture.loadFromFile("Image/Buttons/Music_off.png");
	gButtonSFX[0].mTexture.loadFromFile("Image/Buttons/SFX_on.png");
	gButtonSFX[1].mTexture.loadFromFile("Image/Buttons/SFX_off.png");
	for (int i = 0; i < 1; i++)
	{
		gButtonMusic[i].setButtonArea(&gAreaMusic);
		gButtonSFX[i].setButtonArea(&gAreaSFX);
	}
	gButtonMainMenu.mTexture.loadFromFile("Image/Buttons/MainMenu.png");
	gButtonMainMenu.setButtonArea(&gAreaMainMenu);
	gButtonQuitGame.mTexture.loadFromFile("Image/Buttons/QuitGame.png");
	gButtonQuitGame.setButtonArea(&gAreaQuitGame);
	

	for (int i = 0; i < xTotal-1; i++)
	{
		gButtonBetAmount[i].setPosition(pointBet.x, pointBet.y, gButtonBetAmount[i].mTexture.mWidth, gButtonBetAmount[i].mTexture.mHeight);
	}
	// Set Tai Button and Xiu Button
	gButtonTai.setButtonArea(&gAreaTai);
	gButtonXiu.setButtonArea(&gAreaXiu);
	gButtonChan.setButtonArea(&gAreaChan);
	gButtonLe.setButtonArea(&gAreaLe);
	// Set Triple buttons:
	for (int i = 0; i < 6; i++)
	{
		gButtonTriple[i].setButtonArea(&gAreaTriple[i]);
	}
	// Set Mono buttons:
	for (int i = 0; i < 14; i++)
	{
		gButtonMono[i].setButtonArea(&gAreaMono[i]);
	}
	// Set Dice buttons:
	for (int i = 0; i < 6; i++)
	{
		gButtonDice[i].setButtonArea(&gAreaDice[i]);
	}
	gDiceRolling = Mix_LoadWAV("Audio/DiceRolling.mp3");
	gSoundWin = Mix_LoadWAV("Audio/Win.mp3");
	gSoundLose = Mix_LoadWAV("Audio/Lose.mp3");
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
	gTextClickContinue.freeAll();
	gTextAsset.freeAll();
	gTextTotal.freeAll();

	//Destroy window	
	SDL_DestroyRenderer( gRenderer );
	SDL_DestroyWindow( gWindow );

	gWindow = NULL;
	gRenderer = NULL;

	//Quit SDL subsystems
	Mix_Quit();
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
	gButtonStart.mTexture.render(pointStart.x, pointStart.y, NULL);
	gButtonClear.mTexture.render(pointClear.x, pointClear.y, NULL);

	if (MusicState == ON) gButtonMusic[0].mTexture.render(gAreaMusic.x, gAreaMusic.y, NULL);
	else gButtonMusic[1].mTexture.render(gAreaMusic.x, gAreaMusic.y, NULL);

	if (SFXState == ON) gButtonSFX[0].mTexture.render(gAreaSFX.x, gAreaSFX.y, NULL);
	else gButtonSFX[1].mTexture.render(gAreaSFX.x, gAreaSFX.y, NULL);

	gButtonMainMenu.mTexture.render(gAreaMainMenu.x, gAreaMainMenu.y, NULL);
	gButtonQuitGame.mTexture.render(gAreaQuitGame.x, gAreaQuitGame.y, NULL);
	renderBetButton();
	renderMessage("Bet money: ", 0, true);
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

bool checkResult(const int &totalOfThreeDice, const int *dices, const string &playerBetPlace, uint64_t &betMoney)
{
	if (playerBetPlace == "") return false;
	if (playerBetPlace == "Tai")
	{
		if (totalOfThreeDice >= 11 && totalOfThreeDice <= 17) 
		{
			betMoney *= NORMAL_RATE;
			return true;
		}
	}
	if (playerBetPlace == "Xiu")
	{
		if (totalOfThreeDice >= 4 && totalOfThreeDice <= 10)
		{
			betMoney *= NORMAL_RATE;
			return true;
		}
	}
	if (playerBetPlace == "Chan")
	{
		if (totalOfThreeDice % 2 == 0)
		{
			betMoney *= NORMAL_RATE;
			return true;
		}
	}
	if (playerBetPlace == "Le")
	{
		if (totalOfThreeDice % 2 == 1)
		{
			betMoney *= NORMAL_RATE;
			return true;
		}
	}
	for (int i = 0; i < 6; i++)
	{
		string betPlace = to_string(i+1);
		betPlace = betPlace + betPlace + betPlace;
		if (playerBetPlace == betPlace)
		{
			if (dices[0] == (i+1) && dices[1] == (i+1) && dices[2] == (i+1))
			{
				betMoney *= TRIPLE_RATE;
				return true;
			}
		}
	}
	if (playerBetPlace == "4" || playerBetPlace == "17")
	{
		if (totalOfThreeDice == stoi(playerBetPlace))
		{
			betMoney *= TOTAL_RATE_4_17;
			return true;
		}
	}
	if (playerBetPlace == "5" || playerBetPlace == "16")
	{
		if (totalOfThreeDice == stoi(playerBetPlace))
		{
			betMoney *= TOTAL_RATE_5_16;
			return true;
		}
	}
	if (playerBetPlace == "6" || playerBetPlace == "15")
	{
		if (totalOfThreeDice == stoi(playerBetPlace))
		{
			betMoney *= TOTAL_RATE_6_15;
			return true;
		}
	}
	if (playerBetPlace == "7" || playerBetPlace == "14")
	{
		if (totalOfThreeDice == stoi(playerBetPlace))
		{
			betMoney *= TOTAL_RATE_7_14;
			return true;
		}
	}
	if (playerBetPlace == "8" || playerBetPlace == "13")
	{
		if (totalOfThreeDice == stoi(playerBetPlace))
		{
			betMoney *= TOTAL_RATE_8_13;
			return true;
		}
	}
	if (playerBetPlace == "9" || playerBetPlace == "12")
	{
		if (totalOfThreeDice == stoi(playerBetPlace))
		{
			betMoney *= TOTAL_RATE_9_12;
			return true;
		}
	}
	if (playerBetPlace == "10" || playerBetPlace == "11")
	{
		if (totalOfThreeDice == stoi(playerBetPlace))
		{
			betMoney *= TOTAL_RATE_10_11;
			return true;
		}
	}
	for (int i = 0; i < 6; i++)
	{
		string betPlace = "Dice" + to_string(i+1);
		if (playerBetPlace == betPlace)
		{
			int count = 0;
			for (int j = 0; j < 3; j++)
			{
				if (dices[j] == (i+1)) count++;
			}
			switch (count)
			{
				case 0:
					break;
				case 1:
					betMoney *= DICE_RATE[0];
					return true;
					break;
				case 2:
					betMoney *= DICE_RATE[1];
					return true;
					break;
				case 3:
					betMoney *= DICE_RATE[2];
					return true;
					break;
				default:
					break;
			}
		}
	}
	return false;
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
		if (SFXState == ON) Mix_PlayChannel(-1, gSoundWin, 0);
	}
	else
	{
		gSpriteLose.render(bannerPosition.x, bannerPosition.y, NULL);
		if (SFXState == ON) Mix_PlayChannel(-1, gSoundLose, 0);
	}
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
	// Delete player area before:
	gDeleter.render(playerAreaViewport.x, playerAreaViewport.y, &playerAreaViewport);

	char total[3];
	string message = "Total: ";
	sprintf(total, "%d", TotalOfThreeDice);
	string sTotal = string(total);
	sTotal = message + sTotal;

	// Load and render:
	gTextTotal.loadFromRenderedText(sTotal, BLACK_COLOR);
	gTextTotal.render(middleOfPlayerArea.x-gTextTotal.mWidth/2, middleOfPlayerArea.y-gTextTotal.mHeight/2, NULL);
	SDL_RenderPresent(gRenderer);
}

void updatePlayerAsset(const bool &isWin, int64_t &playerTotalMoney, uint64_t &betMoney)
{
	if (isWin) playerTotalMoney += betMoney;
}

void renderSelectedButton(LButton &button)
{
	button.selected();
	SDL_RenderPresent(gRenderer);
}

void handleBetTimeEvent(SDL_Event e, gameProcess &currentPeriod, int64_t &playerTotalMoney, string &playerBetPlace, 
						uint64_t &betMoney, int &betPerClick)
{
	if (currentPeriod == betTime)
	{
		// Handle mouse event:
        if (gButtonTai.isInside(&e) && (playerBetPlace == "" || playerBetPlace == "Tai"))
		{
			setBetPlace(gButtonTai, playerBetPlace, "Tai", playerTotalMoney, betMoney, betPerClick);
		}
		if (gButtonXiu.isInside(&e) && (playerBetPlace == "" || playerBetPlace == "Xiu"))
		{
			setBetPlace(gButtonXiu, playerBetPlace, "Xiu", playerTotalMoney, betMoney, betPerClick);
		}
		if (gButtonChan.isInside(&e) && (playerBetPlace == "" || playerBetPlace == "Chan"))
		{
			setBetPlace(gButtonChan, playerBetPlace, "Chan", playerTotalMoney, betMoney, betPerClick);
		}
		if (gButtonLe.isInside(&e) && (playerBetPlace == "" || playerBetPlace == "Le"))
		{
			setBetPlace(gButtonLe, playerBetPlace, "Le", playerTotalMoney, betMoney, betPerClick);
		}
		for (int i = 0; i < 6; i++)
		{
			string betPlace = to_string(i+1);
			betPlace = betPlace + betPlace + betPlace;
			if (gButtonTriple[i].isInside(&e) && (playerBetPlace == "" || playerBetPlace == betPlace))
			{
				setBetPlace(gButtonTriple[i], playerBetPlace, betPlace, playerTotalMoney, betMoney, betPerClick);
			}
		}
		for (int i = 0; i < 14; i++)
		{
			string betPlace = to_string(i+4);
			if (gButtonMono[i].isInside(&e) && (playerBetPlace == "" || playerBetPlace == betPlace))
			{
				setBetPlace(gButtonMono[i], playerBetPlace, betPlace, playerTotalMoney, betMoney, betPerClick);
			}
		}
		for (int i = 0; i < 6; i++)
		{
			string betPlace = "Dice";
			string suffix = to_string(i+1);
			betPlace = betPlace + suffix;
			if (gButtonDice[i].isInside(&e) && (playerBetPlace == "" || playerBetPlace == betPlace))
			{
				setBetPlace(gButtonDice[i], playerBetPlace, betPlace, playerTotalMoney, betMoney, betPerClick);
			}
		}
		
		if (gButtonClear.isInside(&e))
		{
			renderSelectedButton(gButtonClear);
			SDL_Delay(200);
			setToBegin(playerTotalMoney, playerBetPlace, betMoney);
		}
		if (gButtonStart.isInside(&e))
		{
			renderSelectedButton(gButtonStart);
			SDL_Delay(200);
			currentPeriod = rollingTime;
		}
		if (gButtonBetAmount[0].isInside(&e))
		{
			if (currentAmount == x100)
			{
				currentAmount = x1;
			}
			else currentAmount = betAmount(currentAmount + 1);
			setToBegin(playerTotalMoney, playerBetPlace, betMoney);
		}
		if (playerTotalMoney < 0)
		{
			setToBegin(playerTotalMoney, playerBetPlace, betMoney);
		}
	}
}

void handleWaitingPhases(SDL_Event e, gameProcess &currentPeriod)
{
	if (currentPeriod == waitingPhase)
	{
		renderMessage("Click to continue", 0);
		if (gButtonAnywhere.isInside(&e))
		{
			currentPeriod = resultTime;
		}
	}
	if (currentPeriod == endPhase)
	{
		renderMessage("Click to continue", 0);
		if (gButtonAnywhere.isInside(&e))
		{
			currentPeriod = startPhase;
		}
	}
}

void renderMessage(string message, const int &number, bool havingNumber)
{
	// Delete messages before
	gDeleter.render(messageViewport.x, messageViewport.y, &messageViewport);
	if (!havingNumber)
	{
		gTextMessage.loadFromRenderedText(message, BLACK_COLOR);
		gTextMessage.render(middleOfMessage.x - gTextMessage.mWidth/2, middleOfMessage.y - gTextMessage.mHeight/2, NULL);
	}
	else if (havingNumber == true)
	{
		char num[10];
		sprintf(num, "%lld", number);
		string newMessage = string(num);
		newMessage = message + newMessage;

		gTextMessage.loadFromRenderedText(newMessage, BLACK_COLOR);
		gTextMessage.render(middleOfMessage.x - gTextMessage.mWidth/2, middleOfMessage.y - gTextMessage.mHeight/2, NULL);
	}
	SDL_RenderPresent(gRenderer);
}

void renderBetButton()
{
	int current = (int) currentAmount;
	gButtonBetAmount[current].mTexture.render(pointBet.x, pointBet.y, NULL);
}

void setToBegin(int64_t &playerTotalMoney, string &playerBetPlace, uint64_t &betMoney)
{
	playerBetPlace = "";
	playerTotalMoney += betMoney;
	betMoney = 0;
	gDeleter.render(0, 0, &allScreen);
	renderGameBoard();
	renderAsset(playerTotalMoney);
}

void changeBetPerClick(int &betPerClick)
{
	switch (currentAmount)
	{
		case x1:
			betPerClick = 1;
			break;
		case x2:
			betPerClick = 2;
			break;
		case x5:
			betPerClick = 5;
			break;
		case x10:
			betPerClick = 10;
			break;
		case x20:
			betPerClick = 20;
			break;
		case x50:
			betPerClick = 50;
			break;
		case x100:
			betPerClick = 100;
			break;
	}
}

void setBetPlace(LButton &button, string &playerBetPlace, string betPlace, int64_t &playerTotalMoney,  uint64_t &betMoney, int &betPerClick)
{
	playerBetPlace = betPlace;
	playerTotalMoney -= betPerClick;
	betMoney += betPerClick;
	renderSelectedButton(button);
	renderAsset(playerTotalMoney);
	renderMessage("Bet money: ", betMoney, true);
}

void handleSupportButtonsEvent(SDL_Event e, bool &quit)
{
	if (currentPeriod != MainMenu)
	{
		if (gButtonMusic[0].isInside(&e))
		{
			if (MusicState == ON) MusicState = OFF;
			else MusicState = ON;
		}
		if (gButtonSFX[0].isInside(&e))
		{
			if (SFXState == ON) SFXState = OFF;
			else SFXState = ON;
		}

		if (gButtonMainMenu.isInside(&e))
		{
			gMusic = Mix_LoadMUS("Audio/Menu Music.mp3");
			Mix_PlayMusic(gMusic, -1);
			currentPeriod = MainMenu;
		}
		if (gButtonQuitGame.isInside(&e))
		{
			quit = true;
		}
	}
}

void handleMusicState()
{
	if (currentPeriod != endPhase)
	{
		if (MusicState == ON) 
		{
			Mix_ResumeMusic();
			gButtonMusic[0].mTexture.render(gAreaMusic.x, gAreaMusic.y, NULL);
		} else {
			Mix_PauseMusic();
			gButtonMusic[1].mTexture.render(gAreaMusic.x, gAreaMusic.y, NULL);
		}
		if (SFXState == ON) 
		{
			gButtonSFX[0].mTexture.render(gAreaSFX.x, gAreaSFX.y, NULL);
			Mix_Resume(-1);
		} else {
			gButtonSFX[1].mTexture.render(gAreaSFX.x, gAreaSFX.y, NULL);
			Mix_HaltChannel(-1);
		}
	}
}

void handleQuitEvent(SDL_Event e, bool &quit)
{
	if( e.type == SDL_QUIT )
	{
		quit = true;
	}
	if (e.type = SDL_KEYDOWN)
	{
		if (e.key.keysym.sym == SDLK_ESCAPE) quit = true;
	}
}

void handleMainMenu(SDL_Event e, bool &quit)
{
	if (currentPeriod == MainMenu)
	{
		if (!Mix_PlayingMusic())
		{
			gMusic = Mix_LoadMUS("Audio/Menu Music.mp3");
			Mix_PlayMusic(gMusic, -1);
		}
		gMainMenu.render(0, 0, &allScreen);
		if (gButtonBatDau.isInside(&e))
		{
			gMusic = Mix_LoadMUS("Audio/Music.mp3");
			Mix_PlayMusic(gMusic, -1);
			currentPeriod = startPhase;
		}
		if (gButtonThoat.isInside(&e))
		{
			quit = true;
		}
		
	}
}

void handleStartPhase(gameProcess &currentPeriod, int64_t &playerTotalMoney, string &playerBetPlace, uint64_t &betMoney)
{
	if (currentPeriod == startPhase)
	{
		playerBetPlace = "";
		betMoney = 0;
		renderGameBoard();
		renderAsset(playerTotalMoney);
		SDL_RenderPresent(gRenderer);
		currentPeriod = betTime;
	}
}