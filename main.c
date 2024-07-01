#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <time.h>

#define SIZE 20
#define BRICK 3

long previousMillis = 0;   
const long interval = 250;  // refresh rate

long previousMillis_b = 0;   
const long ball_speed = 250;  // refresh rate

struct position {
    int x;
    int y;
};

struct position ball_position;

int screen[SIZE][SIZE];
int board_location = SIZE / 2;
int ball_direction = 1;
int score = 0; 
int status;
char name[100];

//      |
//  2   |   1
//      |
//------|-------
//      |
//  3   |   4
//      |
// upper_brick  4 -> 1 & 3 -> 2
// lower_brick  1 -> 4 & 2 -> 3
// right_brick  4 -> 3 & 1 -> 2 
// left_brick   3 -> 4 & 2 -> 1
// paddle       4 -> 1 & 3 -> 2
// right_wall   4 -> 3 & 1 -> 2 
// left_wall    3 -> 4 & 2 -> 1
// top_wall     1 -> 4 & 2 -> 3
// up       u
// down     d
// right    r
// left     l


void wall() {
    for (int i = 0; i < SIZE ; i++) {
        screen[i][0] = 1;
        screen[i][SIZE - 1] = 1; 
    }
}

void brick() {
    for (int i = 1; i < BRICK + 1 ; i++) {
        for (int j = 1; j < SIZE -1 ; j++) {
            screen[i][j] = 2;
        }
    }
}
//###
//#o#
//###
void board(int x) {
    for (int i = 1; i < SIZE - 1 ; i++) {
        screen[SIZE - 1][i] = 0;
    }
    screen[SIZE - 1][x - 1] = 3;
    screen[SIZE - 1][x] =     3;
    screen[SIZE - 1][x + 1] = 3;
}

void upper_wall() {
    for (int i = 1; i < SIZE - 1 ; i++) {
        screen[0][i] = 5;
    }
}

int check_brick(int x, int y) {
    switch (ball_direction)
    {
    case 1:
        if (screen[x-1][y] == 2 || screen[x][y+1] == 2) {
            return 1;
        } else {
            return 0;
        }
        break;
    case 2:
        if (screen[x-1][y] == 2 || screen[x][y-1] == 2) {
            return 1;
        } else {
            return 0;
        }
        break;
    case 3:
        if (screen[x+1][y] == 2 || screen[x][y-1] == 2) {
            return 1;
        } else {
            return 0;
        }
        break;
    case 4:
        if (screen[x+1][y] == 2 || screen[x][y+1] == 2) {
            return 1;
        } else {
            return 0;
        }
        break;
    default:
        break;
    }

}

void collapse_brick(int x, int y) {
    switch (ball_direction)
    {
    case 1:
        if (screen[x-1][y] == 2 && screen[x][y+1] == 2) {
            screen[x-1][y] = 0;
            screen[x][y+1] = 0;
            ball_direction = 3;
            score += 2;
        } else if (screen[x-1][y] == 2) {
            screen[x-1][y] = 0;
            ball_direction = 4;
            score += 1;
        } else {
            screen[x][y+1] = 0;
            ball_direction = 2;
            score += 1;
        }
        break;
    case 2:
        if (screen[x-1][y] == 2 && screen[x][y-1] == 2) {
            screen[x-1][y] = 0;
            screen[x][y-1] = 0;
            ball_direction = 4;
            score += 2;
        } else if (screen[x-1][y] == 2) {
            screen[x-1][y] = 0;
            ball_direction = 3;
            score += 1;
        } else {
            screen[x][y-1] = 0;
            ball_direction = 1;
            score += 1;
        }
        break;
    case 3:
        if (screen[x+1][y] == 2 && screen[x][y-1] == 2) {
            screen[x+1][y] = 0;
            screen[x][y-1] = 0;
            ball_direction = 1;
            score += 2;
        } else if (screen[x+1][y] == 2) {
            screen[x+1][y] = 0;
            ball_direction = 2;
            score += 1;
        } else {
            screen[x][y-1] = 0;
            ball_direction = 4;
            score += 1;
        }
        break;
    case 4:
        if (screen[x+1][y] == 2 && screen[x][y+1] == 2) {
            screen[x+1][y] = 0;
            screen[x][y+1] = 0;
            ball_direction = 2;
            score += 2;
        } else if (screen[x+1][y] == 2) {
            screen[x+1][y] = 0;
            ball_direction = 1;
            score += 1;
        } else {
            screen[x][y+1] = 0;
            ball_direction = 3;
            score += 1;
        }
        break;
    default:
        break;
    }
}

void collapse() {
    switch (screen[ball_position.x][ball_position.y])
    {
    case 1: // wall
        switch (ball_direction)
        {
        case 1:
            ball_direction = 2;
            break;
        case 2:
            ball_direction = 1;
            break;
        case 3:
            ball_direction = 4;
            break;
        case 4:
            ball_direction = 3;
            break;
        default:
            break;
        }
        break;
    case 2: // brick
        break;
    case 3: // board
        if (ball_direction == 4) {
            ball_direction = 1;
        } else if (ball_direction == 3) {
            ball_direction = 2;
        }
        break;
    case 5: // upper_wall
        if (ball_direction == 1) {
            ball_direction = 4;
        } else if (ball_direction == 2) {
            ball_direction = 3;
        }
        break;
    default:
        break;
    }
    screen[ball_position.x][ball_position.y] = 4;
}

void move_ball(int direction) {
    clock_t currentMillis_b = clock();
    if (currentMillis_b - previousMillis_b >= ball_speed) {
            int x , y;   
            x = ball_position.x;
            y = ball_position.y;
            switch (direction)
            {
            case 1:
                ball_position.x -= 1;
                ball_position.y += 1;
                break;
            case 2:
                ball_position.x -= 1;
                ball_position.y -= 1;
                break;
            case 3:
                ball_position.x += 1;
                ball_position.y -= 1;
                break;
            case 4:
                ball_position.x += 1;
                ball_position.y += 1;
                break;
            default:
                break;
            }

        previousMillis_b = currentMillis_b;
        if (check_brick(x, y)) {
            collapse_brick(x, y);
            ball_position.x = x;
            ball_position.y = y;
        } else if (screen[ball_position.x][ball_position.y] == 0) {
            screen[x][y] = 0;
            screen[ball_position.x][ball_position.y] = 4;
        }  else {
            collapse();
            ball_position.x = x;
            ball_position.y = y;
        }
    }

}


void move_board() {
    int x;
    if (kbhit())
    {
        x = getch();
        if (x == 224)
        {
            x = getch();
        }
        switch (x)
        {
        case 77: // right
            board_location++;
            break;
        case 75: // left
            board_location--;
            break;
        }
    }
    if (board_location < 2) {
        board_location = 2;
    } else if (board_location > SIZE - 3) {
        board_location = SIZE - 3;
    } else {
        //....
    }
}

void game_mechanic() {
    move_board();
    move_ball(ball_direction);
}

void init() {
    brick();
}

void load() {
    upper_wall();
    wall();
    board(board_location);
}

void show() {
    load();
    for (int i = 0; i < SIZE ; i++ ) {
        for (int j = 0; j < SIZE ; j++) {
            switch (screen[i][j])
            {
            case 0:
                printf(" ");
                break;
            case 1:
                printf("|");
                break;
            case 2:
                printf("#");
                break;
            case 3:
                printf("_");
                break;
            case 4:
                printf("o");
                break;
            case 5:
                printf("-");
                break;
            default:
                break;
            }
        }
        printf("\n");
    }
}

void clear() {
    system("cls");
}

int check_game() {
    if (ball_position.x > SIZE) {
        return -1;
    } else if (score == (SIZE - 2) * BRICK) {
        return 1;
    } 
    return 0;
}

int main () {
    init();
    ball_position.x = SIZE - 1;
    ball_position.y = 7;
    status = 0;
    while(status == 0) {
        clock_t currentMillis = clock();
        game_mechanic();     
        if (currentMillis - previousMillis >= interval) {
            clear();
            show();    
            previousMillis = currentMillis;
        }
        status = check_game();
    }
    
    FILE *inputFile;

    if (status == 1) {
        printf("yoy win\n");
    } else {
        printf("you loose\n");
    }
    printf("ur score was: %d \nenter ur name: ", score);
    scanf("%s", &name);
    inputFile = fopen("scores.txt", "w+");
    fprintf(inputFile,"%s : %d", name, score);
    fclose(inputFile);
    system("pause");
    return 0;
}