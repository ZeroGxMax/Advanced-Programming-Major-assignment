#include <SDL2/SDL.h>

// Start and Clear Button position:
SDL_Point pointStart = {372, 589};
SDL_Point pointClear = {737, 589};
// Bet per click button position:
SDL_Point pointBet = {1117, 100};

// Three dices position:
SDL_Point firstDicePos = {613, 404};
SDL_Point secondDicePos = {561, 492};
SDL_Point thirdDicePos = {671, 490};

// Banner position:
SDL_Point bannerPosition = {0, 160};

// Text positions:
SDL_Point clickToContinuePosition = {688, 542};
SDL_Point assetTextPos = {197, 2};
SDL_Point totalTextPos = {893, 390};
SDL_Point middleOfMessage = {650, 71};
SDL_Point middleOfPlayerArea = {650, 630};

// Portion of screen:
SDL_Rect allScreen = {0, 0, 1280, 680}; // screen width and screen height.
SDL_Rect assetViewport = {assetTextPos.x, assetTextPos.y, 443, 32};
SDL_Rect messageViewport = {349, 44, 603, 53};
SDL_Rect playerAreaViewport = {350, 585, 600, 90};
SDL_Rect leftFrameViewport = {0, 0, 190, 680};
SDL_Rect rightFrameViewport = {1090, 0, 190, 680};

// Area of bet buttons:
SDL_Rect gAreaTai = {704, 100, 386, 274};
SDL_Rect gAreaXiu = {190, 100, 386, 274};
SDL_Rect gAreaChan = {1006, 375, 82, 59};
SDL_Rect gAreaLe = {1005, 433, 84, 64};
SDL_Rect gAreaTriple[6];
SDL_Rect gAreaMono[14];
SDL_Rect gAreaDice[6];

// Area of function buttons:
SDL_Rect gAreaMusic = {1098, 182, 177, 61};
SDL_Rect gAreaSFX = {1099, 266, 177, 62};
SDL_Rect gAreaMainMenu = {4, 163, 176, 59};
SDL_Rect gAreaQuitGame = {4, 246, 176, 59};

// Area of Menu Button:
SDL_Rect gMenuBatDau = {467, 405, 320, 123};
SDL_Rect gMenuThoat = {467, 543, 320, 123};

void setArea()
{
    gAreaTriple[5] = {575, 101, 130, 49}; // 666 button
    gAreaTriple[4] = {575, 150, 130, 43}; // 555 button
    gAreaTriple[3] = {575, 192, 130, 47}; // 444 button
    gAreaTriple[2] = {575, 238, 130, 47}; // 333 button
    gAreaTriple[1] = {575, 284, 130, 46}; // 222 button
    gAreaTriple[0] = {575, 328, 130, 47}; // 111 button

    gAreaMono[0] = {190, 375, 89, 60}; // 4 button
    gAreaMono[1] = {279, 375, 81, 60}; // 5 button
    gAreaMono[2] = {359, 375, 89, 60}; // 6 button
    gAreaMono[3] = {447, 375, 87, 60}; // 7 button
    gAreaMono[4] = {741, 375, 92, 60}; // 8 button
    gAreaMono[5] = {832, 375, 85, 60}; // 9 button
    gAreaMono[6] = {917, 375, 89, 60}; // 10 button
    gAreaMono[7] = {917, 435, 89, 61}; // 11 button
    gAreaMono[8] = {832, 435, 85, 61}; // 12 button
    gAreaMono[9] = {741, 435, 92, 61}; // 13 button
    gAreaMono[10] = {447, 435, 87, 61}; // 14 button
    gAreaMono[11] = {359, 435, 89, 61}; // 15 button
    gAreaMono[12] = {279, 435, 81, 61}; // 16 button
    gAreaMono[13] = {190, 435, 89, 61}; // 17 button

    gAreaDice[0] = {215, 500, 74, 71}; // Dice 1 button
    gAreaDice[1] = {329, 501, 74, 71}; // Dice 2 button
    gAreaDice[2] = {441, 501, 74, 71}; // Dice 3 button
    gAreaDice[3] = {765, 502, 74, 70}; // Dice 4 button
    gAreaDice[4] = {883, 502, 74, 70}; // Dice 5 button
    gAreaDice[5] = {991, 502, 74, 70}; // Dice 6 button
}