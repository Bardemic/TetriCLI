#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 8
#define COLS 6
#define BLOCK 4
#define EMPTY 0
struct Block {
    int xPosition;
    int yPosition;
    int shape[4][2];
};

struct Block blocks[] = {
    {COLS / 3, 0, {{0,0}, {0,1}, {0,2}, {0,3}}},
    {COLS / 3, 0, {{0,0}, {1,0}, {0,1}, {1,1}}},
    {COLS / 3, 0, {{0,0}, {1,0}, {1,1}, {1,2}}},
    {COLS / 3, 0, {{1,0}, {1,1}, {1,2}, {0,2}}},
    {COLS / 3, 0, {{0,1}, {1,0}, {1,1}, {1,2}}}
};


void dropBlock(int (*array)[COLS]);
void tetrisCheck(int (*array)[COLS]);
void placeBlock(int (*array)[COLS]);
int checkCollisionDrop(int (*array)[COLS]);
void printGame(int (*array)[COLS], int score);
int playGame();
int checkCollisionLR(int (*array)[COLS], int direction /*1 for right, -1 for left*/);
void shiftBlock(int (*array)[COLS], int direction /*1 for right, -1 for left*/);


int score = 0;
int level = 1;
struct Block curBlock;
struct Block nextBlock;
int main(int argc, char *argv[]){
    int xButton, circleButton, triangleButton, squareButton;
    double timePassed = 0;
    double timeToPass = 0.5;
    curBlock = blocks[rand() % 5];
    nextBlock = blocks[rand() % 5];

    if(playGame()){
        int game_array[ROWS][COLS];
        for(int i = 0; i < ROWS; i++){
            for(int j = 0; j < COLS; j++){
                game_array[i][j] = EMPTY;
            }
            printf("\n");
        }
        while(1){
            
            timePassed += 0.000000005;
            if(timePassed >= timeToPass){
                timeToPass += 1;
                int direction = 0;
                scanf("%d", &direction);
                shiftBlock(game_array, direction);
                placeBlock(game_array);
                printGame(game_array, score); //all this does is print the array, dw about logiccc
                tetrisCheck(game_array);
                dropBlock(game_array);
            }
            
        }

    }

    

    return 0;
}

void shiftBlock(int (*array)[COLS], int direction /*1 for right, -1 for left*/){
    int count = 0;
    for(int i = 0; i < 4; i++){
        if(checkCollisionLR(array, direction)){
            count++;
        }
    }
    if(count == 0){
        for(int i = 0; i < 4; i++){
            array[curBlock.shape[i][0] + curBlock.yPosition][curBlock.shape[i][1] + curBlock.xPosition] = EMPTY;
            curBlock.shape[i][1] += direction;
        }
    }
}

void dropBlock(int (*array)[COLS]){
    int count = 0;
    for(int i = 0; i < 4; i++){
        if(checkCollisionDrop(array)){
            count += 1;
        }
    }
    if(count == 0){
        for(int i = 0; i < 4; i++){
            array[curBlock.shape[i][0] + curBlock.yPosition][curBlock.shape[i][1] + curBlock.xPosition] = EMPTY;
            curBlock.shape[i][0]++;
        }
    }
    else {
        curBlock = nextBlock;
        nextBlock = blocks[rand() % 5];
    }
}

void tetrisCheck(int (*array)[COLS]){
    for(int i = 0; i < ROWS; i++){
        int total = 0;
        for(int j = 0; j < COLS; j++){
            if(array[i][j] == BLOCK){
                total++;
            }
        }
        if(total == COLS){
            score += 100 * level; //number is from single line clear based off wikipedia's tetris scoring, probably will change if i care
            for(int j = i - 1; j >= 0; j--){
                for(int k = 0; k < COLS; k++) {
                    array[j + 1][k] = array[j][k];
                }
            }
        }
    }
}

int checkCollisionLR(int (*array)[COLS], int direction /*1 for right, -1 for left*/){
    for(int i = 0; i < 4; i++){
        int curCol = curBlock.shape[i][1] + curBlock.xPosition;
        if(curCol + direction < 0 || curCol + direction >= COLS){
            return 1;
        }
        if(array[curBlock.shape[i][0] + curBlock.yPosition][curCol + direction] == BLOCK){
            int testCount = 0; //is 0 when block in the column of the desired direction is not in the current full block, is 1 when it is
            for(int j = 0; j < 4; j++){
                if(curBlock.shape[j][0] + curBlock.yPosition == curBlock.shape[i][0] + curBlock.yPosition && curBlock.shape[j][1] + curBlock.xPosition == curCol + direction){
                    testCount++;
                }
            }
            if(testCount != 1){
                return 1;
            }
        }
    }
    return 0;
}

int checkCollisionDrop(int (*array)[COLS]){
    for(int i = 0; i < 4; i++){
        int curRow = curBlock.shape[i][0] + curBlock.yPosition;
        if(curRow + 1 >= ROWS){
            return 1;
        }
        if(array[curRow + 1][curBlock.shape[i][1] + curBlock.xPosition] == BLOCK){
            int testCount = 0; //is 0 when block in the row beneath is not in the current full Block, is 1 when it is
            for(int j = 0; j < 4; j++){
                if(curBlock.shape[j][0] == curRow + 1 && curBlock.shape[j][1] + curBlock.xPosition == curBlock.shape[i][1] + curBlock.xPosition){
                    testCount++;
                }
            }
            if(testCount != 1){
                return 1;
            }
        }   
    }
    return 0;
}
void placeBlock(int (*array)[COLS]){
    //remove ghosts
    for(int i = 0; i < 4; i++){
        array[curBlock.shape[i][0] + curBlock.yPosition][curBlock.shape[i][1] + curBlock.xPosition] = BLOCK;
    }

}


void printGame(int (*array)[COLS], int score){
    system("clear");
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            printf("%d ", array[i][j]);
        }
        if(i <= 2){
            printf("    |");
            if(i == 0){
                printf("Next Block|");
            }
            else {
                printf(" ");
                int topRow[4] = {0,0,0,0};
                int bottomRow[4] = {0,0,0,0};
                for(int j = 0; j < 4; j++){
                    if(nextBlock.shape[j][0] == 0){
                        topRow[nextBlock.shape[j][1]] = 1;
                    }
                    if(nextBlock.shape[j][0] == 1){
                        bottomRow[nextBlock.shape[j][1]] = 1;
                    }
                }
                if(i == 1){
                    for(int j = 0; j < 4; j++){
                        printf("%d ", topRow[j]);
                    }
                }
                if(i == 2){
                    for(int j = 0; j < 4; j++){
                        printf("%d ", bottomRow[j]);
                    }
                }
                printf(" |");
            }
            
            
        }
        else if(i == 3){
            printf("    |__________|");
        }
        printf("\n");
    }
    for(int i = 0; i < COLS; i++){
        printf("--");
    }
    printf("\n");
    printf("Score: %d\n", score);
}


int playGame(){
    int answer;
    printf("1 to play, 0 to quit\n");
    scanf("%d", &answer);
    return answer;
}