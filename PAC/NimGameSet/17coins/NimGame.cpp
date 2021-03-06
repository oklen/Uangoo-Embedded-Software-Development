#include "NimGame.h"
#include<iostream>
#include<iomanip>
#include"vector.h"
//#include"console.h"

using namespace std;

const int N_COINS = 17;
const int MAX_MOVE = 4;
const int NO_GOOD_MOVE = -1;

const int WINNING_POSITION=1000;
const int LOSING_POSITION= -WINNING_POSITION;
const int MAX_DEPTH=10;

NimGame::NimGame()
{
}


NimGame::~NimGame()
{
}

void NimGame::printInstructions()
{
    cout << "Welcome to NimGame!\n";
    cout << "If you don't know how to play this game, ask the people" << endl;
    cout << "who give you this game for help." << endl;
}

void NimGame::play()
{
    initGame();
    while (!gameIsOver())
    {
        displayGame();
        if (getCurrentPlayer() == HUMAN) {
            makeMove(getUserMove());
        }
        else {
            Move move = getComputerMove();
            displayMove(move);
            makeMove(move);
        }
        switchTurn();
    }
    announceResult();
}

void NimGame::initGame()
{
    coinLeft = N_COINS;
    humanTotalCoins=0;
    computerTotalCoins=0;
    cout << "If you want to move frist?(y/n)" << endl;
    cout << "I choose:";
    char inWord;

    while (true)
    {
        cin >> inWord;
        if (inWord == 'y') {
            whosTurn = HUMAN;
            break;
        }
        else if (inWord == 'n') {
            whosTurn = COMPUTER;
            break;
        }
        else {
            cout << "What are you doing?! enter 'y' or 'n' ,try again!" << endl;
            cout << "I choose:";
        }

    }

}

bool NimGame::gameIsOver()
{
    return coinLeft == 0;
}

void NimGame::displayGame()
{
    cout<<"You have "<<humanTotalCoins<<" coins in hand."<<endl;
    cout<<"I have "<<computerTotalCoins<<" coins in hand."<<endl;
    if (coinLeft == 1)
        cout << "There is one coin left." << endl;
    else cout << "There is " << coinLeft << " coins left." << endl<<endl;
}

side NimGame::getCurrentPlayer()
{
    if (whosTurn == HUMAN)
        return HUMAN;
    return COMPUTER;
}

Move NimGame::getUserMove()
{
    int coinWanted;
    Move move;
    while (true)
    {
        cout << "How many coins you want to take?(Enter 1~4)"<<endl;
        cout << "Enter here:";
        cin >> coinWanted;
        if (coinWanted <= coinLeft&& coinWanted <= MAX_MOVE&&coinWanted >= 1)
            break;
        cout << "Invaild number please try again!" << endl;
    }
    move.nTaken = coinWanted;
    return move;
}

void NimGame::displayMove(Move move)
{
    cout << "I will take " << move.nTaken << endl;
}



void NimGame::makeMove(Move move)
{
    coinLeft -= move.nTaken;
    if(whosTurn==HUMAN){
      humanTotalCoins += move.nTaken;
    }
    else
        computerTotalCoins += move.nTaken;

}

void NimGame::retractMove(Move move)
{

    coinLeft += move.nTaken;
    if(whosTurn==HUMAN) humanTotalCoins -= move.nTaken;
    else computerTotalCoins -= move.nTaken;

}

void NimGame::switchTurn()
{
    if (whosTurn == HUMAN)
        whosTurn = COMPUTER;
    else whosTurn = HUMAN;
}

void NimGame::announceResult()
{
    if (humanTotalCoins%2 == 0)
        cout << "You.. You dare to win this,what a genius are you!" << endl;
    else
        cout << "Poor loser! You can never win this!" << endl;
}

void NimGame::generateMoveList(Vector<Move> & move)
{
    for(int i=1;i<=MAX_MOVE;i++){
        if(i <= coinLeft){
            Move tmpMove;
            tmpMove.nTaken=i;
            move.add(tmpMove);
        }
    }
}

Move NimGame::getComputerMove()
{
    return findBestMove();
}

Move NimGame::findBestMove()
{
    int rating=0;
    return findBestMove(0, rating);
}

Move NimGame::findBestMove(int depth, int & rating)
{
    Vector<Move> moveList;
    Move bestMove;
    int minRating=WINNING_POSITION+1;
    generateMoveList(moveList);
    if(moveList.isEmpty()) error("No Vaild move found, program has a bug!");
    for(Move move : moveList){
        makeMove(move);
        switchTurn();
        int moveRating=evaluatePosition(depth+1);
        if(moveRating< minRating){
            bestMove=move;
            minRating=moveRating;
        }
        switchTurn();
        retractMove(move);
    }
    rating=-minRating;

    return bestMove;

}

int NimGame::evaluatePosition(int depth)
{
    if(gameIsOver() || depth >= MAX_DEPTH)
        return evaluateStaticPosition();
    int rating=0;
    findBestMove(depth,rating);  // Ignore this step that rating value is return by using &.
    return rating;
}

int NimGame::evaluateStaticPosition()
{
    if(whosTurn==HUMAN){
        if(humanTotalCoins%2==0&&coinLeft==1)
            return LOSING_POSITION;
        if(gameIsOver()&&humanTotalCoins%2!=0)
            return LOSING_POSITION;
        if(gameIsOver()&&humanTotalCoins%2==0)
            return WINNING_POSITION;
    }
    else{
        if(computerTotalCoins%2==0&&coinLeft==1)
            return LOSING_POSITION;
        if(gameIsOver()&&computerTotalCoins%2!=0)
            return LOSING_POSITION;
        if(gameIsOver()&&computerTotalCoins%2==0)
            return WINNING_POSITION;
    }

    return coinLeft-WINNING_POSITION;
}
