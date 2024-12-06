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


void dropBlock(struct Block *curBlock, int (*array)[COLS]);
void tetrisCheck(int (*array)[COLS]);
void placeBlock(int (*array)[COLS], struct Block curBlock);
int checkCollisionDrop(struct Block curBlock, int (*array)[COLS]);
void printGame(int (*array)[COLS], int score);
int playGame();
int checkCollisionLR(struct Block curBlock, int (*array)[COLS], int direction /*1 for right, -1 for left*/);
void shiftBlock(struct Block *curBlock, int (*array)[COLS], int direction /*1 for right, -1 for left*/);


int main(int argc, char *argv[]){
    int xButton, circleButton, triangleButton, squareButton;
    int score = 0;
    double timePassed = 0;
    double timeToPass = 0.5;
    struct Block b1 = blocks[rand() % 5];

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
                shiftBlock(&b1, game_array, direction);
                placeBlock(game_array, b1);
                printGame(game_array, score); //all this does is print the array, dw about logiccc
                tetrisCheck(game_array);
                dropBlock(&b1, game_array);
            }
            
        }

    }

    

    return 0;
}

void shiftBlock(struct Block *curBlock, int (*array)[COLS], int direction /*1 for right, -1 for left*/){
    int count = 0;
    for(int i = 0; i < 4; i++){
        if(checkCollisionLR(*curBlock, array, direction)){
            count++;
        }
    }
    if(count == 0){
        for(int i = 0; i < 4; i++){
            array[curBlock->shape[i][0] + curBlock->yPosition][curBlock->shape[i][1] + curBlock->xPosition] = EMPTY;
            curBlock->shape[i][1] += direction;
        }
    }
}

void dropBlock(struct Block *curBlock, int (*array)[COLS]){
    int count = 0;
    for(int i = 0; i < 4; i++){
        if(checkCollisionDrop(*curBlock, array)){
            count += 1;
        }
    }
    if(count == 0){
        for(int i = 0; i < 4; i++){
            array[curBlock->shape[i][0] + curBlock->yPosition][curBlock->shape[i][1] + curBlock->xPosition] = EMPTY;
            curBlock->shape[i][0]++;
        }
    }
    else {
        *curBlock = blocks[rand() % 5];
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
            for(int j = i - 1; j >= 0; j--){
                for(int k = 0; k < COLS; k++) {
                    array[j + 1][k] = array[j][k];
                }
            }
        }
    }
}

int checkCollisionLR(struct Block curBlock, int (*array)[COLS], int direction /*1 for right, -1 for left*/){
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

int checkCollisionDrop(struct Block curBlock, int (*array)[COLS]){
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
void placeBlock(int (*array)[COLS], struct Block curBlock){
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