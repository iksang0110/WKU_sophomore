#pragma once
/* raycaster.h */
#ifndef RAYCASTER_H
#define RAYCASTER_H

/* 기본 상수 정의 */
#define MAP_WIDTH 24
#define MAP_HEIGHT 24
#define SCREEN_WIDTH 120   /* 콘솔 화면 너비 */
#define SCREEN_HEIGHT 40   /* 콘솔 화면 높이 */
#define TRUE 1
#define FALSE 0

/* 방향키 상수 */
#define KEY_UP 'w'
#define KEY_DOWN 's'
#define KEY_LEFT 'a'
#define KEY_RIGHT 'd'
#define KEY_ESC 27

/* 맵 타일 타입 */
#define TILE_EMPTY 0
#define TILE_WALL_1 1
#define TILE_WALL_2 2
#define TILE_WALL_3 3
#define TILE_WALL_4 4
#define TILE_WALL_5 5

/* 게임 상태 구조체 */
struct s_game_state {
    /* 플레이어 위치 */
    double pos_x;
    double pos_y;

    /* 플레이어 방향 */
    double dir_x;
    double dir_y;

    /* 카메라 평면 */
    double plane_x;
    double plane_y;

    /* 이동 관련 */
    double move_speed;
    double rot_speed;

    /* 화면 버퍼 */
    char screen[SCREEN_HEIGHT][SCREEN_WIDTH];
};

/* 초기화 함수 */
void init_game_state(struct s_game_state* state);
void setup_console(void);

/* 화면 관련 함수 */
void clear_screen(struct s_game_state* state);
void set_cursor_position(int x, int y);
void hide_cursor(void);

/* 렌더링 함수 */
void render(struct s_game_state* state);
void draw_screen(struct s_game_state* state);

/* 입력 처리 함수 */
int handle_input(struct s_game_state* state);

/* 유틸리티 함수 */
double get_time(void);
void delay(int milliseconds);

/* 맵 관련 전역 변수 선언 */
extern const int g_world_map[MAP_WIDTH][MAP_HEIGHT];

#endif /* RAYCASTER_H */