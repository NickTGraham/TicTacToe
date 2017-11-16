struct gameState {
  char gameboard[3][3];
  char turn;
};

struct nineBoard {
    char gameboard[9][3][3];
    char turn;
    int lastBoard;
    int lastSpot;
};

typedef struct {
    int board;
    int spot;
    double cost;
} nineMove;

typedef struct {
    int spot;
    int cost;
} move;

int initializeGameBoard(struct gameState *gs);
void printBoard(struct gameState gs);
void findSolution(char board[3][3], int solution[10]);
int validMove(int move, struct gameState gs);
int terminalState(struct gameState gs);
move minimax (struct gameState gs, int depth);
int utilityHeuristic(struct gameState gs);
void runTests(void);

int initializeNineBoard(struct nineBoard *nb);
int printNineBoard(struct nineBoard nb);
int validMoveNine(int board, int move, struct nineBoard nb);
int terminalNineBoard(struct nineBoard nb);
nineMove minimaxNine(struct nineBoard nb, int depth, double alpha, double beta);
double utilityHeuristicNine(struct nineBoard);
void copyBoard(struct gameState *gs, struct nineBoard *nb, int board);
