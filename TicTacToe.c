#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "TicTacToe.h"
#include <limits.h>
#include <time.h>
#include <float.h>

#define max(x,y) (x.cost>y.cost)?x:y
#define min(x,y) (x.cost<y.cost)?x:y

char myPiece = 'X';
char theirPiece = 'O';

/* Function: main
 * Description: Main function that initalizes the game, and loops through getting
 * the users input and updating the board. It also checks to see if the game has
 * ended after any given move.
 */
int main(int argc, char** argv){
    /* Program Setup
     *
     * Creates and initializes the game board, Game Over variable, and the
     * players piece
     */
    struct gameState mainGame;
    initializeGameBoard(&mainGame);

    struct nineBoard nineB;
    initializeNineBoard(&nineB);
    int playerMove;
    int boardSize;
    if (argc > 1) {
        if (atoi(argv[1]) == 9) {
            boardSize = 9;
        }
        else {
            boardSize = 3;
        }
    }
    else {
        return -10;
    }
    while (boardSize != 3 && boardSize != 9) {
        fprintf(stderr, "That was not a valid option. ");
        scanf("%d", &boardSize);
    }

    /* Initial Player Input
     *
     * Asks the user for what piece they would like to play as
     */
    while (1) {
        initializeNineBoard(&nineB);
        initializeGameBoard(&mainGame);
        fprintf(stderr, "Would you like to play as X or O?\n");
        scanf(" %c", &theirPiece);

        //if they entered a lowercase number, make it uppercase
        if (theirPiece > 96) {
            theirPiece -= 32;
        }
        if (theirPiece == 'O') {
            myPiece = 'X';
        }
        else {
            myPiece = 'O';
        }
        if (boardSize == 3) {
            /* First Move
             *
             * If the player selects X, let them make the first move before we begin
             */
            if (theirPiece == 'X') {
                myPiece = 'O';
                fprintf(stderr, "Please make your move: ");
                scanf("%d", &playerMove);
                if (validMove(playerMove, mainGame)) {
                    mainGame.gameboard[(playerMove - 1)/3][(playerMove - 1)%3] = theirPiece;
                }
            }

            mainGame.turn = myPiece;
            while (terminalState(mainGame) == 3) {
                clock_t s, e;
                s = clock();
                move myMove = minimax(mainGame, 20);
                e = clock();
                fprintf(stderr, "Spot [%d] \t utility [%d] \t time [%f]\n", myMove.spot, myMove.cost,((double)(e - s))/(CLOCKS_PER_SEC));
                mainGame.gameboard[(myMove.spot - 1)/3][(myMove.spot - 1)%3] = myPiece;
                mainGame.turn = theirPiece;
                printBoard(mainGame);
                if (terminalState(mainGame) != 3) {
                    break;
                }
                fprintf(stderr, "Please make your move: ");
                scanf("%d", &playerMove);
                if (validMove(playerMove, mainGame)) {
                    mainGame.gameboard[(playerMove - 1)/3][(playerMove - 1)%3] = theirPiece;
                }
                else {
                    while (!validMove(playerMove, mainGame)) {
                        printBoard(mainGame);
                        fprintf(stderr, "Invalid Move! Please make your move: ");
                        scanf("%d", &playerMove);
                        if (validMove(playerMove, mainGame)) {
                            mainGame.gameboard[(playerMove - 1)/3][(playerMove - 1)%3] = theirPiece;
                            break;
                        }
                    }
                }
                mainGame.turn = myPiece;
                printBoard(mainGame);
            }
            if (terminalState(mainGame) == -1) {
                fprintf(stderr, "You Lost\n");
            }
            else if (terminalState(mainGame) == 0) {
                fprintf(stderr, "It's a Tie\n");
            }
            else if (terminalState(mainGame) == 1) {
                fprintf(stderr, "You Won!\n");
            }
            printBoard(mainGame);
        }
        else {
            /* First Move
             *
             * If the player selects X, let them make the first move before we begin
             */
            int playerSpot;
            if (theirPiece == 'X') {
                myPiece = 'O';
                fprintf(stderr, "Please make your move (board spot): ");
                scanf("%d", &playerSpot);
                scanf("%d", &playerMove);
                nineB.lastSpot = playerSpot;
                if (validMoveNine(playerSpot, playerMove, nineB)) {
                    printf("Valid\n");
                    nineB.gameboard[playerSpot - 1][(playerMove - 1)/3][(playerMove - 1)%3] = theirPiece;
                    nineB.lastSpot = playerMove;
                    nineB.lastBoard = playerSpot;
                }
            }
            else {
                nineB.lastSpot = 5;
            }

            nineB.turn = myPiece;
            int depth = 12;
            while (terminalNineBoard(nineB) == 3) {
                clock_t s, e;
                s = clock();
                nineMove myMove = minimaxNine(nineB, depth, DBL_MIN, DBL_MAX);
                e = clock();
                if ((double)(e - s)/CLOCKS_PER_SEC < 15.0 && (depth < 20))
                    depth++;
                fprintf(stderr, "Spot [%d %d] \t utility [%f] \t time [%f s]\n", myMove.board, myMove.spot, myMove.cost, ((double)(e - s))/(CLOCKS_PER_SEC));
                nineB.gameboard[myMove.board - 1][(myMove.spot - 1)/3][(myMove.spot - 1)%3] = myPiece;
                nineB.lastSpot = myMove.spot;
                nineB.lastBoard = myMove.board;
                mainGame.turn = theirPiece;
                printNineBoard(nineB);
                if (terminalNineBoard(nineB) != 3) {
                    break;
                }
                fprintf(stderr, "Please make your move: ");
                scanf("%d", &playerSpot);
                scanf("%d", &playerMove);

                if (validMoveNine(playerSpot, playerMove, nineB)) {
                    nineB.gameboard[playerSpot - 1][(playerMove - 1)/3][(playerMove - 1)%3] = theirPiece;
                }
                else {
                    while (!validMoveNine(playerSpot, playerMove, nineB)) {
                        printNineBoard(nineB);
                        fprintf(stderr, "Invalid Move! Please make your move: ");
                        scanf("%d", &playerSpot);
                        scanf("%d", &playerMove);
                        if (validMoveNine(playerSpot, playerMove, nineB)) {
                            nineB.gameboard[playerSpot - 1][(playerMove - 1)/3][(playerMove - 1)%3] = theirPiece;
                            break;
                        }
                    }
                }
                nineB.turn = myPiece;
                nineB.lastSpot = playerMove;
                nineB.lastBoard = playerSpot;
                printNineBoard(nineB);
            }
            /* Game Over Notification */
            if (terminalNineBoard(nineB) == -1) {
                fprintf(stderr, "AI Lost\n");
            }
            else if (terminalNineBoard(nineB) == 0) {
                fprintf(stderr, "It's a Tie\n");
            }
            else if (terminalNineBoard(nineB) == 1) {
                fprintf(stderr, "AI Won!\n");
            }
        }
    }
    return 0;
}

/* Function: initializeGameBoard
 * Arguments: gs - pointer to the gameState
 * Description: Loops through the board and sets all the spaces to '_'
 */
int initializeGameBoard(struct gameState *gs) {
    for(int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            (*gs).gameboard[i][j] = '_';
        }
    }
    return 1;
}

/* Function: initializeNineBoard
 * Arguments: nb - pointer to the game state
 * Desctiption: Loops through the board and sets all the spaces to '_'
 */
int initializeNineBoard(struct nineBoard *nb) {
    nb->turn = 'X';
    nb->lastSpot = -1;
    nb->lastBoard = -1;
    for (int b = 0; b < 9; b++) {
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                (*nb).gameboard[b][i][j] = '_';
            }
        }
    }
    return 1;
}

/* Funcion: printNineBoard
 * Arguments: nb - game state
 * Description: Prints out the board for the player to see. prints to stderr
 */
int printNineBoard(struct nineBoard nb) {
    for (int boardOffset = 0; boardOffset <= 6; boardOffset+=3) {
        for (int row = 0; row < 3; row++) {
            for (int board = 0; board < 3; board++) {
                for (int col = 0; col < 3; col++) {
                    fprintf(stderr, "%c ", nb.gameboard[board+boardOffset][row][col]);
                }
                fprintf(stderr, " ");
            }
            fprintf(stderr, "\n");
        }
        fprintf(stderr, "\n");
    }
}

/* Function: validMoveNine
 * Arguments: board - the board where the move is being played
 * move - the spot on the board
 * nb - the game state
 * Description: Checks to see if the given move is valid (ie follows game rules
 * and checks that the space is empty)
 */
int validMoveNine (int board, int move, struct nineBoard nb) {
    if (board < 0) {
        return 1;
    }
    else if (board != nb.lastSpot) {
        return 0;
    }
    return (nb.gameboard[board - 1][(move - 1)/3][(move - 1)%3] == '_');
}

/* Function: terminalNineBoard
 * Arguments: nb - the game state
 * Description: Checks to see if the board is in a terminal state, ie there are
 * three of the same pieces in a row.
 */
int terminalNineBoard (struct nineBoard nb) {
    int fullBoard = 0;
    for (int i = 0; i < 9; i++) {
        struct gameState gs;
        copyBoard(&gs, &nb, i);
        if (terminalState(gs) == 1 || terminalState(gs) == -1) {
            return terminalState(gs);
        }
        if (terminalState(gs) == 0) {
            fullBoard++;
        }
    }
    return (fullBoard == 9)?0:3;
}

/* Function: copyBoard
 * Arguments: gs - 3x3 game state pointer, nb - pointer to the 9 Board, board - the board to copy
 * Description: copies one of the boards to a 3x3 game state struct so that it can
 * be used with the old 3x3 functions
 */
void copyBoard (struct gameState *gs, struct nineBoard *nb, int board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            gs->gameboard[i][j] = nb->gameboard[board][i][j];
        }
    }
}

/* Function: printBoard
 * Arguments: gs - game state
 * Description: prints out the 3x3 board
 */
void printBoard(struct gameState gs) {
    for (int i = 0; i < 3; i++) {
        for(int j = 0; j < 3; j++) {
            fprintf(stderr, " %c ", gs.gameboard[i][j]);
        }
        fprintf(stderr, "\n");
    }
}

/* Function: minimax
 * Arguments: gs: game state including board and current player, maxDepth: how
 * far down the tree to dive
 * Description: The minimax alorithm as discussed in class that computes the best
 * move to make assuming the opponent plays perfectly. This is loosely based on
 * the algorithm provided in the textbook.
 */
move minimax(struct gameState gs, int maxDepth) {
    struct gameState testState = gs;
    if (maxDepth == 0 || terminalState(gs) < 3) {
        if (maxDepth == 0) {
            //fprintf(stderr, "Depth exceeded\n");
        }
        move end = {0, utilityHeuristic(gs)};
        return end;
    }

    else if (testState.turn == myPiece){
        move bestplay = {0, INT_MIN};
        for (int i = 1; i <= 9; i++) {
            if (!validMove(i, testState)) {
                continue;
            }
            else {
                testState.gameboard[(i - 1)/3][(i - 1)%3] = myPiece;
                testState.turn = theirPiece;
                move path = minimax(testState, maxDepth - 1);
                if (path.cost > bestplay.cost) {
                    bestplay.cost = path.cost;
                    bestplay.spot = i;
                }
            }
            testState = gs;
        }
        return bestplay;
    }
    else {
        move bestplay = {0, INT_MAX};
        for (int j = 1; j <= 9; j++) {
            if (!validMove(j, testState)) {
                continue;
            }
            else {
                testState.gameboard[(j - 1)/3][(j - 1)%3] = theirPiece;
                testState.turn = myPiece;
                move path = minimax(testState, maxDepth - 1);
                if (path.cost < bestplay.cost) {
                    bestplay.cost = path.cost;
                    bestplay.spot = j;
                }
            }
            testState = gs;
        }
        return bestplay;
    }
}

/* Function: minimax
 * Arguments: nb: game state including board and current player, depth: how
 * far down the tree to dive, alpha & beta used for alpha beta pruning
 * Description: The minimax alorithm as discussed in class that computes the best
 * move to make assuming the opponent plays perfectly. This is loosely based on
 * the algorithm provided in the textbook.
 */
nineMove minimaxNine (struct nineBoard nb, int depth, double alpha, double beta) {
    struct nineBoard testState = nb;
    if (depth == 0 || terminalNineBoard(nb) < 3) {
        if (depth == 0) {
            //fprintf(stderr, "Depth exceeded\n");
        }
        nineMove end = {0, 0, utilityHeuristicNine(nb)};
        return end;
    }

    else if (testState.turn == myPiece){
        nineMove bestplay = {testState.lastSpot, 0, INT_MIN};
        for (int i = 1; i <= 9; i++) {
            if (!validMoveNine(testState.lastSpot, i, testState)) {
                continue;
            }
            else {
                testState.gameboard[testState.lastSpot][(i - 1)/3][(i - 1)%3] = myPiece;
                testState.turn = theirPiece;
                testState.lastBoard = testState.lastSpot;
                testState.lastSpot = i;
                nineMove path = minimaxNine(testState, depth - 1, alpha, beta);
                if (path.cost >= bestplay.cost) {
                    bestplay.cost = path.cost;
                    bestplay.spot = i;

                }
                if (path.cost >= beta) {
                    return bestplay;
                }
                else {
                    alpha = (alpha>path.cost)?alpha:path.cost;
                }
            }
            testState = nb;
        }
        return bestplay;
    }
    else {
        nineMove bestplay = {testState.lastSpot, 0, INT_MAX};
        for (int j = 1; j <= 9; j++) {
            if (!validMoveNine(testState.lastSpot, j, testState)) {
                continue;
            }
            else {
                testState.gameboard[testState.lastSpot][(j - 1)/3][(j - 1)%3] = theirPiece;
                testState.turn = myPiece;
                testState.lastBoard = testState.lastSpot;
                testState.lastSpot = j;
                nineMove path = minimaxNine(testState, depth - 1, alpha, beta);
                if (path.cost < bestplay.cost) {
                    bestplay.cost = path.cost;
                    bestplay.spot = j;
                }
                if (path.cost <= alpha) {
                    return bestplay;
                }
                else {
                    beta = (beta<path.cost)?beta:path.cost;
                }
            }
            testState = nb;
        }
        return bestplay;
    }
}

/* Function: utilityHeuristic
 * Arguments: gs - game state
 * Description: returns the utility of the state, or the heuristic
 */
int utilityHeuristic(struct gameState gs) {
    /* the state is terminal, so return that */
    if (terminalState(gs) < 3) {
        return terminalState(gs);
    }

    /* High Quality Heuristic I got going here, never actually used */
    return 0;
}

/* Function: utilityHeuristicNine
 * Arguments: ng - game state
 * Description: if the state is terminal returns the utility, otherwise checks
 * the heursic function. It looks for 2 in a row, or then looks for key spaces.
 * If that fails it tries to balance the number of pieces on each board
 */
double utilityHeuristicNine(struct nineBoard nb) {
    /* the state is terminal, so return that */
    if (terminalNineBoard(nb) < 3) {
        return terminalNineBoard(nb);
    }
    else if (nb.lastSpot == 5) {
        return -0.5;
    }
    else if (nb.lastSpot == 1) {
        return -1;
    }
    else if ((nb.lastSpot == 3 || nb.lastSpot == 7 || nb.lastSpot == 9)) {
        return 0.5;
    }

    for (int i = 0; i < 9; i++) {
        for (int j = 0; j < 3; j++) {
            if ((nb.gameboard[i][j][0] == nb.gameboard[i][j][1]) ||
            (nb.gameboard[i][j][0] == nb.gameboard[i][j][2])) {
                if (nb.gameboard[i][j][0] == myPiece) {
                    return 0.8;
                }
                else if (nb.gameboard[i][j][0] == theirPiece) {
                    return -0.8;
                }
            }

            if ((nb.gameboard[i][j][1] == nb.gameboard[i][j][2])) {
                if (nb.gameboard[i][j][0] == myPiece) {
                    return 0.8;
                }
                else if (nb.gameboard[i][j][0] == theirPiece) {
                    return -0.8;
                }
            }

            if ((nb.gameboard[i][0][j] == nb.gameboard[i][1][j]) ||
            (nb.gameboard[i][0][j] == nb.gameboard[i][2][j])) {
                if (nb.gameboard[i][0][j] == myPiece) {
                    return 0.8;
                }
                else if (nb.gameboard[i][0][j] == theirPiece) {
                    return -0.8;
                }
            }

            if ((nb.gameboard[i][1][j] == nb.gameboard[i][2][j])) {
                if (nb.gameboard[i][1][j] == myPiece) {
                    return 0.8;
                }
                else if (nb.gameboard[i][0][j] == theirPiece) {
                    return -0.8;
                }
            }
        }
    }

    int numPieces[9];

    for (int i = 0; i < 9; i++) {
        numPieces[i] = 0;
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                if (nb.gameboard[i][j][k] != '_') {
                    numPieces[i]++;
                }
            }
        }
    }

    for (int i = 0; i < 8; i++) {
        if (numPieces[i] != numPieces[i+1]) {
            return -0.2;
        }
    }
    /* High Quality Heuristic I got going here, huh */
    return 0;
}

/* Function: validMove
 * Arguments: move: position to take, gs: game state including board
 * Description: returns 1 if the spot is empty, and 0 if there is a piece already
 * played there
 */
int validMove(int move, struct gameState gs) {
    if (move < 1 || move > 9) {
        return 0;
    }
    return (gs.gameboard[(move - 1)/3][(move - 1)%3] == '_');
}

/* Function: terminalState
 * Arguments: gs: game state including the board
 * Description: returns 1 if the state is terminal and in the benefit of me, 0
 * if the state is terminal and no one wins, -1 if the state is terminal and the
 * opponent wins. If it is not a terminal state the function returns 3
 */
int terminalState(struct gameState gs) {
    for (int i = 0; i < 3; i++) {
        if (gs.gameboard[0][i] == gs.gameboard[1][i] &&
        gs.gameboard[0][i] == gs.gameboard[2][i] &&
        gs.gameboard[0][i] != '_') {
            if (gs.gameboard[0][i] == myPiece) {
                return 1;
            }
            else {
                return -1;
            }
        }
        else if (gs.gameboard[i][0] == gs.gameboard[i][1] &&
        gs.gameboard[i][0] == gs.gameboard[i][2] &&
        gs.gameboard[i][0] != '_') {
            if (gs.gameboard[i][0] == myPiece) {
                return 1;
            }
            else {
                return -1;
            }
        }
    }
    if (gs.gameboard[0][0] == gs.gameboard[1][1] &&
    gs.gameboard[0][0] == gs.gameboard[2][2] &&
    gs.gameboard[0][0] != '_') {
        if (gs.gameboard[0][0] == myPiece) {
            return 1;
        }
        else {
            return -1;
        }
    }
    else if (gs.gameboard[0][2] == gs.gameboard[1][1] &&
    gs.gameboard[0][2] == gs.gameboard[2][0] &&
    gs.gameboard[0][2] != '_') {
        if (gs.gameboard[0][2] == myPiece) {
            return 1;
        }
        else {
            return -1;
        }
    }
    /* Test for full board */
    int full = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (gs.gameboard[i][j] == '_') {
                full = 0;
                break;
            }
        }
    }
    if (full) {
        return 0;
    }
    else {
        return 3;
    }
}

void runTests() {
    struct gameState A;
    struct gameState B;
    struct gameState C;
    struct gameState D;
    struct gameState E;
    struct gameState F;

    initializeGameBoard(&A);
    initializeGameBoard(&B);
    initializeGameBoard(&C);
    initializeGameBoard(&D);
    initializeGameBoard(&E);
    initializeGameBoard(&F);

    A.gameboard[0][0] = 'X';
    A.gameboard[0][1] = 'X';
    A.gameboard[0][2] = 'O';
    A.gameboard[1][0] = 'O';
    A.gameboard[1][1] = 'O';
    A.gameboard[2][0] = 'X';
    A.gameboard[2][2] = 'X';
    A.turn = 'X';
    myPiece = 'X';
    theirPiece = 'O';
    move t = minimax(A, 20);
    printBoard(A);

    A.gameboard[2][1] = 'X';
    fprintf(stderr, "Utility [%d]\n", utilityHeuristic(A));
    fprintf(stderr, "Spot [%d] \t Cost [%d]\n", t.spot, t.cost);

    B.gameboard[0][0] = 'X';
    B.gameboard[0][2] = 'X';
    B.gameboard[1][1] = 'O';
    B.gameboard[2][0] = 'O';
    B.gameboard[2][1] = 'X';
    B.turn = 'O';
    myPiece = 'O';
    theirPiece = 'X';
    t = minimax(B, 20);
    printBoard(B);

    fprintf(stderr, "Spot [%d] \t Cost [%d]\n", t.spot, t.cost);
}
