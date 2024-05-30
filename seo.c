//1-1:부산헹 2.3이동까지
//1-2. 좀비, 마동석 행동 수정
//1-3. aggro 음수 나오는 거 수정
//마무리
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
//파라미터
#define LEN_MIN 15
#define LEN_MAX 50
#define STM_MIN 0
#define STM_MAX 5
#define PROB_MIN 10
#define PROB_MAX 90
#define AGGRO_MIN 0
#define AGGRO_MAX 5
// 마동석 이동 방향
#define MOVE_LEFT 1
#define MOVE_STAY 0
//좀비의 공격 대상
#define ATK_NONE 0
#define ATK_CITIZEN 1
#define ATK_DONGSEOK 2
//마동석 행동
#define ACTION_REST 0
#define ACTION_PROVOKE 1
#define ACTION_PULL 2

// 전역 변수
int length, p, stamina;
int citizen_aggro = 1;
int dongseok_aggro = 1;
int C, Z, M, turn;
int before_C, before_Z, before_M;

void print_intro() {
    printf(" ____  ____  ____  ____  ____  _________  ____  ____  ____  ____   \n");
    printf("||b ||||u ||||s ||||a ||||n ||||       ||||h ||||e ||||n ||||g || \n");
    printf("||__||||__||||__||||__||||__||||_______||||__||||__||||__||||__|| \n");
    printf("|/__\\||/__\\||/__\\||/__\\||/__\\||/_______\\||/__\\||/__\\||/__\\||/__\\| \n\n");
}

int get_valid_input(int min, int max, const char* prompt) {
    int value;
    while (1) {
        printf("%s(%d~%d)>> ", prompt, min, max);
        scanf_s("%d", &value);
        if (value >= min && value <= max) break;
        printf("잘못된 입력.\n");
    }
    return value;
}

void get_game_input() {
    length = get_valid_input(LEN_MIN, LEN_MAX, "train length");
    stamina = get_valid_input(STM_MIN, STM_MAX, "madongseok stamina");
    p = get_valid_input(PROB_MIN, PROB_MAX, "percentile probability 'p'");
}

void init_game() {
    srand((unsigned int)time(NULL));
    C = length - 6;
    before_C = C;
    Z = length - 3;
    M = length - 2;
    turn = 0;
}

void print_train() {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < length; j++) {
            if (i == 0 || i == 2) {
                printf("#");
            }
            else {
                if (j == M) {
                    printf("M");
                }
                else if (j == Z) {
                    printf("Z");
                }
                else if (j == C) {
                    printf("C");
                }
                else if (j == 0 || j == length - 1) {
                    printf("#");
                }
                else {
                    printf(" ");
                }
            }
        }
        printf("\n");
    }
    printf(" \n");
}

void move_citizen() {
    before_C = C;
    int c_move_num = rand() % 100;

    if (c_move_num <= 100 - p) {
        if (C > 1 && C - 1 != Z && C - 1 != M) {
            --C;
            citizen_aggro = (citizen_aggro < AGGRO_MAX) ? citizen_aggro + 1 : AGGRO_MAX;
        }
    }
    else {
        citizen_aggro = (citizen_aggro > AGGRO_MIN) ? citizen_aggro - 1 : AGGRO_MIN;
    }
}

void print_citizen_status() {
    if (before_C != C) {
        printf("citizen: %d -> %d (aggro: %d -> %d)\n", before_C, C, citizen_aggro + ((before_C > C) ? -1 : 1), citizen_aggro);
    }
    else {
        printf("citizen: stay %d (aggro: %d)\n", C, citizen_aggro);
    }
}

void move_zombie() {
    before_Z = Z;
    if (turn % 2 == 1) {
        if (dongseok_aggro >= citizen_aggro) {
            if (Z > M && Z != M + 1) {
                --Z;
            }
        }
        else {
            if (Z > C && Z != C + 1) {
                --Z;
            }
        }
    }
}

void zombie_attacked() {
    if (Z == C + 1) {
        printf("Zombie attacked citizen!\n");
        printf("GAME OVER!!!\n");
        exit(0);
    }
    else if (Z == M + 1) {
        printf("Zombie attacked madongseok! (stamina: %d -> %d)\n", stamina, stamina - 1);
        stamina--;
    }
    else {
        printf("Zombie moves towards citizen instead of attacking madongseok.\n");
        if (Z > C) {
            --Z;
        }
    }
}

void print_zombie_status() {
    if (before_Z != Z) {
        printf("zombie: %d -> %d\n", before_Z, Z);
    }
    else {
        printf("zombie: stay %d\n", Z);
    }
}

void move_dongseok() {
    int move;
    before_M = M;
    while (1) {
        printf("madongseok move(0:stay, 1:left)>> ");
        scanf_s("%d", &move);
        if (move == MOVE_LEFT || move == MOVE_STAY) break;
        printf("잘못된 입력.\n");
    }
    if (move == MOVE_LEFT) {
        if (M > 1 && M - 1 != Z) {
            --M;
            dongseok_aggro = (dongseok_aggro < AGGRO_MAX) ? dongseok_aggro + 1 : AGGRO_MAX;
        }
        else if (M - 1 == Z) {
            printf("Madongseok can't move to the zombie's position. Zombie will attack or move.\n");
            zombie_attacked();
        }
    }
    else {
        dongseok_aggro = (dongseok_aggro > AGGRO_MIN) ? dongseok_aggro - 1 : AGGRO_MIN;
        if (dongseok_aggro < AGGRO_MIN) dongseok_aggro = AGGRO_MIN;
    }
}

void print_dongseok_status() {
    if (before_M != M) {
        printf("madongseok: %d -> %d (aggro: %d, stamina: %d)\n", before_M, M, dongseok_aggro, stamina);
    }
    else {
        printf("madongseok: stay %d (aggro: %d, stamina: %d)\n", M, dongseok_aggro, stamina);
    }
}

void print_game_status() {
    printf("citizen: %d (aggro: %d)\n", C, citizen_aggro);
    printf("zombie: %d\n", Z);
    printf("madongseok: %d (aggro: %d, stamina: %d)\n", M, dongseok_aggro, stamina);
}

void dongseok_action() {
    int action;
    while (1) {
        printf("madongseok action(0.rest, 1.provoke, 2.pull)>> ");
        scanf_s("%d", &action);
        if (action == ACTION_REST || action == ACTION_PROVOKE || action == ACTION_PULL) break;
        printf("잘못된 입력.\n");
    }
    int before_aggro = dongseok_aggro;
    int before_stamina = stamina;
    switch (action) {
    case ACTION_REST:
        stamina = (stamina < STM_MAX) ? stamina + 1 : STM_MAX;
        dongseok_aggro = (dongseok_aggro > AGGRO_MIN) ? dongseok_aggro - 1 : AGGRO_MIN;
        if (dongseok_aggro < AGGRO_MIN) dongseok_aggro = AGGRO_MIN;
        printf("madongseok rests...\n");
        break;
    case ACTION_PROVOKE:
        dongseok_aggro = AGGRO_MAX;
        printf("madongseok provoked zombie... (aggro: %d)\n", dongseok_aggro);
        break;
    case ACTION_PULL:
        stamina = (stamina > STM_MIN) ? stamina - 1 : STM_MIN;
        dongseok_aggro = (dongseok_aggro < AGGRO_MAX) ? dongseok_aggro + 2 : AGGRO_MAX;
        int pull_success = rand() % 100;
        if (pull_success <= 100 - p) {
            printf("madongseok pulled zombie... Next turn, it can't move\n");
            Z = before_Z;
        }
        else {
            printf("madongseok failed to pull zombie\n");
        }
        break;
    }
    printf("madongseok: %d (aggro: %d -> %d, stamina: %d -> %d)\n", M, before_aggro, dongseok_aggro, before_stamina, stamina);
}

int check_game_over() {
    if (C == 1) {
        printf("\nYOU WIN!!!!!!!");
        return 1;
    }
    if (Z == C + 1) {
        printf("\nGAME OVER!!!!!!!");
        return 1;
    }
    if (stamina == STM_MIN) {
        printf("\nGAME OVER!!!!!!!");
        return 1;
    }
    return 0;
}

int main(void) {
    print_intro();
    get_game_input();
    init_game();
    print_train();
    while (1) {
        ++turn;
        move_citizen();
        move_zombie();
        print_train();
        print_citizen_status();
        print_zombie_status();
        move_dongseok();
        print_train();
        print_dongseok_status();
        dongseok_action();
        print_train();
        if (check_game_over()) break;
        printf(" \n");
    }
    return 0;
}
