#include <stdio.h>

#define ROWS 8
#define COLS 4
#define BLOCK 4
#define EMPTY 0
struct Block {
    int xPosition;
    int yPosition;
    int shape[4][2];
};

void dropBlock(struct Block *curBlock);
void tetrisCheck(int (*array)[COLS]);
void placeBlock(int (*array)[COLS], struct Block curBlock);
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
        struct Block b1 = {0, 0, {{0,0},{0,1},{0,2},{0,3}}};
        while(1){
            
            placeBlock(my_array, b1);
            printGame(my_array);
            dropBlock(&b1);
            sleep(1);
            
        }

    }

    

    return 0;
}


void dropBlock(struct Block *curBlock){
    for(int i = 0; i < 4; i++){
    curBlock->shape[i][0]++;
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


void placeBlock(int (*array)[COLS], struct Block curBlock){
    for(int i = 0; i < ROWS; i++){
        for(int j = 0; j < COLS; j++){
            array[i][j] = EMPTY;
        }
    }
    for(int i = 0; i < 4; i++){
        array[curBlock.shape[i][0]][curBlock.shape[i][1]] = BLOCK;
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