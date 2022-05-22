#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cstdint>
#include <cstring>
#include <cctype>
using namespace std;

int random(const int &min,const int &max);

void initialize(const int &playerTotalMoney);

void playerBet(string &playerBetPlace, uint64_t &betMoney);

bool checkTotalOfThreeDice(const int &totalOfThreeDice, const string &playerBetPlace);

void update(const bool &win, const uint64_t &betMoney, uint64_t &playerTotalMoney);

int rollDices(int *dices);

void render(int *dices, const int &totalOfThreeDice);

bool askPlayerToContinue();

int main()
{
    srand(time(NULL));
    // Data
    uint64_t playerTotalMoney = 100;
    int *dices = new int[3];
    int totalOfThreeDice = 0;
    uint64_t betMoney;
    string playerBetPlace;
    bool win;

    while (true)
    {
        // initialize
        initialize(playerTotalMoney);

        // Bet time and save the result
        playerBet(playerBetPlace, betMoney);

        //Roll three dices:
        totalOfThreeDice = rollDices(dices);
        render(dices, totalOfThreeDice);


        // Check the result and update:
        win = checkTotalOfThreeDice(totalOfThreeDice, playerBetPlace);
        update(win, betMoney, playerTotalMoney);
        if (!askPlayerToContinue()) break;
    }
}

int random(const int &min,const int &max)
{
    return min + rand() % (max - min + 1);
}

void initialize(const int &playerTotalMoney)
{
    cout << "Your asset: " << playerTotalMoney << endl;
    cout << "Tai" << "\t\t" << "Xiu" << endl;
}

void playerBet(string &playerBetPlace, uint64_t &betMoney)
{
    cout << "Where do you want to bet? (Tai/Xiu): ";
    cin >> playerBetPlace;
    cout << "How much do you want to bet: ";
    cin >> betMoney;
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

void update(const bool &win, const uint64_t &betMoney, uint64_t &playerTotalMoney)
{
    if (win)
    {
        cout << "You win! Money has been sent to your asset!" << endl;
        cout << betMoney << "++" << endl;
        playerTotalMoney += betMoney;
        cout << "Your asset now: " << playerTotalMoney << endl;
    }
    else
    {
        cout << "You lose! Money has been deducted from your asset!" << endl;
        cout << betMoney << "--" << endl;
        playerTotalMoney -= betMoney;
        cout << "Your asset now: " << playerTotalMoney << endl;
    }
}

void render(int *dices, const int &totalOfThreeDice)
{
    cout << "Dices have been rolled" << endl;
    for (int i = 0; i < 3; i++)
    {
        cout << "Dice " << i+1 << ": " << dices[i] << endl;
    }
    cout << "Total of three dices: " << totalOfThreeDice << endl;
}

bool askPlayerToContinue()
{
    char ch;
    cout << "Do you to play again? (T/F) ";
    cin >> ch;
    if (ch == 'F') return false;
    else return true;
}