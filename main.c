#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>

#define BLOCK 4
#define EMPTY 0
#define MAX_SCORES 100
#define MAX_NAME_LENGTH 4

struct LeaderboardEntry {
    char name[MAX_NAME_LENGTH];
    int score;
    char date[11];
};
struct Block {
    int xPosition;
    int yPosition;
    int shape[4][2];
};

struct Block blocks[] = {
    {0, 0, {{0,0}, {0,1}, {0,2}, {0,3}}},
    {0, 0, {{0,0}, {1,0}, {0,1}, {1,1}}},
    {0, 0, {{0,0}, {1,0}, {1,1}, {1,2}}},
    {0, 0, {{1,0}, {1,1}, {1,2}, {0,2}}},
    {0, 0, {{0,1}, {1,0}, {1,1}, {1,2}}}
};

int ROWS = 10;
int COLS = 5;

void dropBlock(int (*array)[COLS]);
void tetrisCheck(int (*array)[COLS]);
void placeBlock(int (*array)[COLS]);
int checkCollisionDrop(int (*array)[COLS]);
void printGame(int (*array)[COLS], int score);
int Menu();
int checkCollisionLR(int (*array)[COLS], int direction /*1 for right, -1 for left*/);
void shiftBlock(int (*array)[COLS], int direction /*1 for right, -1 for left*/);
void checkGameOver(int (*array)[COLS]);
void readLeaderboard();
void writeLeaderboard();
void addLeaderboardEntry();
void printLeaderboard();
void openSettings();
void dropBlockStraight(int (*array)[COLS]);
void playGameAudio();
void stopGameAudio();
int numScores = 0;
int score;
int level;
int gameOver;
double timePassed;
double timeToPass;
double timeIncrements;
struct Block curBlock;
struct Block nextBlock;
int playAudio = 1;
struct LeaderboardEntry leaderboard[MAX_SCORES];
int main(int argc, char *argv[]){
    readLeaderboard();
    int xButton, circleButton, triangleButton, squareButton;
    curBlock = blocks[rand() % 5];
    curBlock.xPosition = COLS / 3;
    nextBlock = blocks[rand() % 5];
    nextBlock.xPosition = COLS / 3;

    while(Menu()){
        playGameAudio();
        score = 0;
        level = 1;
        gameOver = 0;
        timePassed = 0;
        timeToPass = 0.5;
        timeIncrements = 1.0;
        int game_array[ROWS][COLS];
        for(int i = 0; i < ROWS; i++){
            for(int j = 0; j < COLS; j++){
                game_array[i][j] = EMPTY;
            }
            printf("\n");
        }
        while(!gameOver){
            
            timePassed += 0.000000005;
            if(timePassed >= timeToPass){
                timeToPass += 1;
                int direction = 0;
                scanf("%d", &direction);
                if(direction == 1 || direction == 0 || direction == -1){
                    shiftBlock(game_array, direction);  
                }
                else if(direction == 2){
                    dropBlockStraight(game_array);
                }
                placeBlock(game_array);
                printGame(game_array, score); //all this does is print the array, dw about logiccc
                tetrisCheck(game_array);
                dropBlock(game_array);
            }
            
        }
        stopGameAudio();
        printf("Game Over\n");
        addLeaderboardEntry();

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
    int count = checkCollisionDrop(array);
    if(count == 0){
        for(int i = 0; i < 4; i++){
            array[curBlock.shape[i][0] + curBlock.yPosition][curBlock.shape[i][1] + curBlock.xPosition] = EMPTY;
            curBlock.shape[i][0]++;
        }
    }
    else {
        curBlock = nextBlock;
        checkGameOver(array);
        nextBlock = blocks[rand() % 5];
        nextBlock.xPosition = COLS / 3;
    }
}
void checkGameOver(int (*array)[COLS]){
    for(int i = 0; i < 4; i++){
        if(array[0][curBlock.shape[i][1] + curBlock.xPosition] == BLOCK){
            gameOver = 1;
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
            score += 100 * level; //number is from single line clear based off wikipedia's tetris scoring, probably will change if i care
            for(int j = i - 1; j >= 0; j--){
                for(int k = 0; k < COLS; k++) {
                    array[j + 1][k] = array[j][k];
                }
            }
        }
    }
}

void checkIncreaseLevel(){
    if(score >= level * 500){
        level++;
        timeIncrements *= 0.95;
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

void openSettings(){
    int answer;
    while(answer != 4){
        printf("Press 1 to change rows, 2 to change columns, 3 to toggle audio, 4 to go back\n");
        scanf("%d", &answer);
        if(answer == 1){
            printf("Enter new number of rows: ");
            scanf("%d", &ROWS);
        }
        else if(answer == 2){
            printf("Enter new number of columns: ");
            scanf("%d", &COLS);
        }
        else if(answer == 3){
            playAudio *= -1;
        }
    }
}

void dropBlockStraight(int (*array)[COLS]){
    while(checkCollisionDrop(array) == 0){
        for(int i = 0; i < 4; i++){
            array[curBlock.shape[i][0] + curBlock.yPosition][curBlock.shape[i][1] + curBlock.xPosition] = EMPTY;
            curBlock.shape[i][0]++;
        }
    }
}

int Menu(){
    int answer = 2;
    while(answer > 1){
        printf("1 to play, 2 for leaderboard, 3 for settings, 0 to quit\n");
        scanf("%d", &answer);
        if(answer == 2){
            printLeaderboard();
        }
        else if(answer == 3){
            openSettings();
        }
    }
    
    return answer;
}

void readLeaderboard() {
    FILE *file = fopen("leaderboard.csv", "r");
    if (file == NULL) {
        return;
    }

    char line[256];
    while (fgets(line, sizeof(line), file) && numScores < MAX_SCORES) {
        char name[MAX_NAME_LENGTH];
        int score;
        if (sscanf(line, "%[^,],%d", name, &score) == 2) {
            strncpy(leaderboard[numScores].name, name, MAX_NAME_LENGTH - 1);
            leaderboard[numScores].name[MAX_NAME_LENGTH - 1] = '\0';
            leaderboard[numScores].score = score;
            numScores++;
        }
    }

    fclose(file);
}

void writeLeaderboard() {
    FILE *file = fopen("leaderboard.csv", "w");
    if (file == NULL) {
        return;
    }

    for (int i = 0; i < numScores; i++) {
        fprintf(file, "%s,%d,%s\n", leaderboard[i].name, leaderboard[i].score, leaderboard[i].date);
    }

    fclose(file);
}

void addLeaderboardEntry(){
    printf("Enter your name: ");
    char name[MAX_NAME_LENGTH];
    scanf("%s", name);
    time_t t = time(NULL);
    struct tm *tm = localtime(&t);
    char date[11];
    strftime(date, sizeof(date), "%Y-%m-%d", tm);
    
    while(strlen(name) > MAX_NAME_LENGTH - 1){
        printf("Name too long, %d characters max\n", MAX_NAME_LENGTH - 1);
        scanf("%s", name);
    }
    strcpy(leaderboard[numScores].date, date);
    strncpy(leaderboard[numScores].name, name, MAX_NAME_LENGTH - 1);
    leaderboard[numScores].name[MAX_NAME_LENGTH - 1] = '\0';
    leaderboard[numScores].score = score;
    numScores++;
    writeLeaderboard();
}

void printLeaderboard() {
    for (int i = 0; i < numScores - 1; i++) {
        for (int j = 0; j < numScores - i - 1; j++) {
            if (leaderboard[j].score < leaderboard[j + 1].score) {
                struct LeaderboardEntry temp = leaderboard[j];
                leaderboard[j] = leaderboard[j + 1];
                leaderboard[j + 1] = temp;
            }
        }
    }
    printf("\nTop Scores:\n");
    printf("-----------\n");
    for (int i = 0; i < 3 && i < numScores; i++) {
        printf("%d. %s: %d (%s)\n", 
            i + 1,
            leaderboard[i].name,
            leaderboard[i].score,
            leaderboard[i].date
        );
    }
    printf("\n");
}


void stopGameAudio() {
    FILE *fp;
    char command[100];
    
    snprintf(command, sizeof(command), "pkill mpg123");
    
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to stop audio\n");
        return;
    }
    
    pclose(fp);
}

void playGameAudio() {
    if(playAudio == 0){
        return;
    }
    FILE *fp;
    char command[100];
    
    snprintf(command, sizeof(command), "mpg123 -q game_audio.mp3 > /dev/null 2>&1 &");
    
    fp = popen(command, "r");
    if (fp == NULL) {
        printf("Failed to play audio\n");
        return;
    }
    
    pclose(fp);
}
