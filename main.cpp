#include <iostream>
#include <chrono>
#include <windows.h>
#include <stdlib.h>

using namespace std;
using namespace std::chrono;

int bestMoveRow, bestMoveCol;

char board[3][3] = {
    {' ', ' ', ' '},
    {' ', ' ', ' '},
    {' ', ' ', ' '}
};

char player = 'O', opponent = 'X';

void color(int desiredColor) { 
     SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), desiredColor);
}

bool isEmpty(int matrix) {
    if (matrix < 4) {
        if (board[0][matrix - 1] == ' ')
            return true;
    } else if (matrix < 7) {
        if (board[1][matrix - 4] == ' ')
            return true;
    } else {
        if (board[2][matrix - 7] == ' ')
            return true;
    }
    return false;
}

int random() {
    int rand;

    do {
        auto milisec = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
        rand = milisec % 9 + 1;
    } while (!isEmpty(rand));

    return rand;
}

bool isMovesLeft() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ')
                return true;
        }
    }
    return false;
}

int checkWinner() {
    // check rows
    for (int i = 0; i < 3; i++) {
        if (board[i][0] == board[i][1] &&
            board[i][1] == board[i][2]) {
            if (board[i][0] == player)
                return 10;
            else if (board[i][0] == opponent)
                return -10;
        }
    }

    // check columns
    for (int i = 0; i < 3; i++) {
        if (board[0][i] == board[1][i] &&
            board[1][i] == board[2][i]) {
            if (board[0][i] == player)
                return 10;
            else if (board[0][i] == opponent)
                return -10;
        }
    }

    // check diagonals
    if (board[0][0] == board[1][1] &&
        board[1][1] == board[2][2]) {
        if (board[0][0] == player)
            return 10;
        else if (board[0][0] == opponent) {
            return -10;
        }
    }

    if (board[0][2] == board[1][1] &&
        board[1][1] == board[2][0]) {
        if (board[0][2] == player)
            return 10;
        else if (board[0][2] == opponent)
            return -10; 
    }
    return 0;
}

void printWinner(string gameType) {
    int winner = checkWinner();
    string ply, opp;

    if (gameType == "singlePlayer") {
        ply = "Player";
        opp = "Computer";
    } else {
        ply = "Player 1";
        opp = "Player 2";
    }

    if (winner == 10) {
        color(1);
        cout << ply + " win!" << endl;
        color(7);
        exit(0);
    } else if (winner == -10) {
        color(12);
        cout << opp + " win!" << endl;
        color(7);
        exit(0);
    } else if (!isMovesLeft() && winner == 0) {
        cout << "Draw!" << endl;
        exit(0);
    }
}

int minimax(int depth, bool isMax) {
    int score = checkWinner();

    if (score == 10)
        return -10 + depth;
    if (score == -10)
        return 10 - depth;
    if (!isMovesLeft())
        return 0;

    if (isMax) {
        int best = INT_MIN;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = opponent;
                    best = max(best, minimax(depth + 1, false));
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    } else {
        int best = INT_MAX;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = player;
                    best = min(best, minimax(depth + 1, true));
                    board[i][j] = ' ';
                }
            }
        }
        return best;
    }
}

void findBestMove() {
    int bestVal = INT_MIN, moveVal;
    bestMoveRow = -1;
    bestMoveCol = -1;

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board[i][j] == ' ') {
                board[i][j] = opponent;
                moveVal = minimax(0, false);
                board[i][j] = ' ';
                if (moveVal > bestVal) {
                    bestMoveRow = i;
                    bestMoveCol = j;
                    bestVal = moveVal;
                }
            }
        }
    }
}

void fillBoard(int matrix, char turn) {
    if (matrix < 4) {
        board[0][matrix - 1] = turn;
    } else if (matrix < 7) {
        board[1][matrix - 4] = turn;
    } else {
        board[2][matrix - 7] = turn;
    }
}

void printBoard() {
    cout << ' ' << board[0][0] << " |";
    cout << ' ' << board[0][1] << " |";
    cout << ' ' << board[0][2] << ' ' << endl;

    cout << "---|---|---" << endl;

    cout << ' ' << board[1][0] << " |";
    cout << ' ' << board[1][1] << " |";
    cout << ' ' << board[1][2] << ' ' << endl;

    cout << "---|---|---" << endl;

    cout << ' ' << board[2][0] << " |";
    cout << ' ' << board[2][1] << " |";
    cout << ' ' << board[2][2] << ' ' << endl;

    cout << endl;
}

void turnInfo(string gameType, int turn) {
    int winner = checkWinner();

    if (winner != 0)
        cout << "Winner Time!\n" << endl;
    else if (winner == 0 && !isMovesLeft())
        cout << "No One Win!\n" << endl;
    else {
        if (gameType == "multiplayer") {
            if (turn == 1)
                color(1);
            else
                color(12);

                cout << "Player " << turn << " Turn\n" << endl;
        } else {
            if (turn == 1) {
                color(1);
                cout << "Player Turn\n" << endl;
            } else {
                color(12);
                cout << "Computer Turn\n" << endl;
            }
        }
        color(7);
    }
}

void tutorial(string mode) {
    char num = '0';
    string ply, opp;
    system("cls");

    if (mode == "singlePlayer") {
        ply = "Player: ";
        opp = "Computer: ";
    } else {
        ply = "Player 1: ";
        opp = "Player 2: ";
    }

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            board[i][j] = ++num; 
    }

    color(1);
    cout << ply;
    color(7);
    cout << player << ", ";

    color(12);
    cout << opp;
    color(7);
    cout << opponent << "\n\n";

    printBoard();
    system("pause");

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++)
            board[i][j] = ' '; 
    }
}

void singlePlayerTurn() {
    int input;

    system("cls");

    turnInfo("singePlayer", 1);
    printBoard();
    printWinner("singlePlayer");

    do {
        cout << "> ";
        cin >> input;
    } while (!isEmpty(input));

    fillBoard(input, player);
}

void singlePlayerEasy() {
    system("cls");

    turnInfo("singlePlayer", 2);
    printBoard();
    printWinner("singlePlayer");

    Sleep(1000);

    fillBoard(random(), opponent);
}

void singlePlayerHard() {
    system("cls");

    turnInfo("singlePlayer", 2);
    printBoard();
    printWinner("singlePlayer");

    Sleep(1000);

    findBestMove();
    board[bestMoveRow][bestMoveCol] = opponent; // same as fillboard();
}

void multiplayerTurn(int turn) {
    int input;

    system("cls");
    turnInfo("multiplayer", turn);
    printBoard();
    printWinner(__func__);

    do {
        cout << "> ";
        cin >> input;
    } while (!isEmpty(input));

    if (turn == 1) {
        fillBoard(input, player);
    } else { 
        fillBoard(input, opponent);
    }
}

void singlePlayer() {
    int mode;

    system("cls");
    cout << "Select Mode:" << endl;
    cout << "1. Easy" << endl;
    cout << "2. Hard" << endl;

    while (true) {
        cout << "\n> ";
        cin >> mode;

        if (mode == 1) {
            tutorial("singlePlayer");
            while (true) {
                singlePlayerTurn();
                singlePlayerEasy();
            }
        } else if (mode == 2) {
            tutorial("singlePlayer");
            while (true) {
                singlePlayerTurn();
                singlePlayerHard();
            }
        }
        else
            cout << "Please input correctly" << endl; 
    }
}

void multiplayer() {
    tutorial("multiplayer");
    while (true) {
        multiplayerTurn(1);
        multiplayerTurn(2);
    }
}

void startGame() {
    int menuAnswer;

    system("cls");
    cout << "Tictactoe Game" << endl;
    cout << "Menu:" << endl;
    cout << "1. Single player" << endl;
    cout << "2. Multiplayer" << endl;

    while (true) {
        cout << "\n> ";
        cin >> menuAnswer;

        if (menuAnswer == 1)
            singlePlayer();
        else if (menuAnswer == 2)
            multiplayer();
        else
            cout << "Please input correctly" << endl;
    }
}

int main() {
    startGame();
    return 0;
}