#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define ROWS 8
#define COLS 7
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
void printGame(int (*array)[COLS]);
int playGame();



int main(int argc, char *argv[]){
    int xButton, circleButton, triangleButton, squareButton;
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
            
            placeBlock(game_array, b1);
            printGame(game_array);
            dropBlock(&b1, game_array);
            sleep(1);
            
        }

    }

    

    return 0;
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
            for(int j = 0; j < COLS; j++){
                array[i][j] = EMPTY;
            }
            for(int y = i; y > 0; y--){
                for(int x = 0; y < COLS; x++) {
                    array[y][x] = array[y-1][x];

                }

            }
            i--;
        }
    }
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


void printGame(int (*array)[COLS]){
    system("clear");
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            printf("%d ", array[i][j]);
        }
        printf("\n");
    }
}


int playGame(){
    int answer;
    printf("1 to play, 0 to quit\n");
    scanf("%d", &answer);
    return answer;
}