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
    {COLS / 2, 0, {{0,0}, {0,1}, {0,2}, {0,3}}},
    {COLS / 2, 0, {{0,0}, {1,0}, {0,1}, {1,1}}},
    {COLS / 2, 0, {{0,0}, {1,0}, {1,1}, {1,2}}},
    {COLS / 2, 0, {{1,0}, {1,1}, {1,2}, {0,2}}},
    {COLS / 2, 0, {{0,1}, {1,0}, {1,1}, {1,2}}}
};


void dropBlock(struct Block *curBlock, int (*array)[COLS]);
void tetrisCheck(int (*array)[COLS]);
void placeBlock(int (*array)[COLS], struct Block curBlock);
int checkCollision(int (*array)[COLS], struct Block curBlock);
void printGame(int (*array)[COLS]);
int playGame();



int main(int argc, char *argv[]){
    int xButton, circleButton, triangleButton, squareButton;

    if(playGame()){
        int my_array[ROWS][COLS];
        for(int i = 0; i < ROWS; i++){
            for(int j = 0; j < COLS; j++){
                my_array[i][j] = EMPTY;
            }
            printf("\n");
        }
        struct Block b1 = blocks[1];
        while(1){
            
            placeBlock(my_array, b1);
            printGame(my_array);
            dropBlock(&b1, my_array);
            sleep(1);
            
        }

    }

    

    return 0;
}


void dropBlock(struct Block *curBlock, int (*array)[COLS]){
    int count = 0;
    for(int i = 0; i < 4; i++){
        if(checkCollision(array, *curBlock)){
            count += 1;
        }
    }
    if(count == 0){
        for(int i = 0; i < 4; i++){
            curBlock->shape[i][0]++;
        }
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

int checkCollision(int (*array)[COLS], struct Block curBlock){
    for(int i = 0; i < 4; i++){
        int curRow = curBlock.shape[i][0] + curBlock.yPosition;
        int curCol = curBlock.shape[i][1] + curBlock.xPosition;
        if(curRow + 1 >= ROWS || curCol + 1 >= COLS || curCol < 0){
            return 1;
        }
    }
    return 0;
}
void placeBlock(int (*array)[COLS], struct Block curBlock){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            array[i][j] = EMPTY;
        }
    }
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