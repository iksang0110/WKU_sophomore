/* render.c */
#include <math.h>
#include <stdio.h>
#include "raycaster.h"

/* 벽 렌더링에 사용할 문자 배열 */
static const char WALL_CHARS[] = { ' ', '#', '@', '$', '=', '*' };
static const char SHADED_WALL_CHARS[] = { ' ', '.', ',', '\'', '-', '+' };

/* 화면을 그리는 함수 */
void draw_screen(struct s_game_state* state)
{
    int i, j;

    /* 커서를 화면 왼쪽 상단으로 이동 */
    set_cursor_position(0, 0);

    /* 버퍼의 내용을 화면에 출력 */
    for (i = 0; i < SCREEN_HEIGHT; i++) {
        for (j = 0; j < SCREEN_WIDTH; j++) {
            putchar(state->screen[i][j]);
        }
        putchar('\n');
    }
}

/* 레이캐스팅 렌더링 수행 */
void render(struct s_game_state* state)
{
    int x;

    /* 화면 버퍼 초기화 */
    clear_screen(state);

    /* 각 수직선에 대해 레이캐스팅 수행 */
    for (x = 0; x < SCREEN_WIDTH; x++) {
        /* 카메라 평면상의 x좌표 계산 (-1.0 ~ 1.0) */
        double camera_x = 2.0 * x / (double)SCREEN_WIDTH - 1.0;

        /* 광선의 방향 벡터 계산 */
        double ray_dir_x = state->dir_x + state->plane_x * camera_x;
        double ray_dir_y = state->dir_y + state->plane_y * camera_x;

        /* 현재 맵 위치 */
        int map_x = (int)state->pos_x;
        int map_y = (int)state->pos_y;

        /* DDA 알고리즘을 위한 변수들 */
        double delta_dist_x = fabs(ray_dir_x) < 1e-10 ? 1e10 : fabs(1.0 / ray_dir_x);
        double delta_dist_y = fabs(ray_dir_y) < 1e-10 ? 1e10 : fabs(1.0 / ray_dir_y);

        double side_dist_x, side_dist_y;
        int step_x, step_y;
        int hit = 0;  /* 벽과 충돌 여부 */
        int side;     /* NS 또는 EW 벽과 충돌 */

        /* 단계 방향과 초기 사이드 거리 계산 */
        if (ray_dir_x < 0) {
            step_x = -1;
            side_dist_x = (state->pos_x - map_x) * delta_dist_x;
        }
        else {
            step_x = 1;
            side_dist_x = (map_x + 1.0 - state->pos_x) * delta_dist_x;
        }

        if (ray_dir_y < 0) {
            step_y = -1;
            side_dist_y = (state->pos_y - map_y) * delta_dist_y;
        }
        else {
            step_y = 1;
            side_dist_y = (map_y + 1.0 - state->pos_y) * delta_dist_y;
        }

        /* DDA 알고리즘 실행 */
        while (hit == 0) {
            /* x면과 y면 중 가까운 쪽으로 이동 */
            if (side_dist_x < side_dist_y) {
                side_dist_x += delta_dist_x;
                map_x += step_x;
                side = 0;
            }
            else {
                side_dist_y += delta_dist_y;
                map_y += step_y;
                side = 1;
            }

            /* 벽과 충돌 체크 */
            if (g_world_map[map_x][map_y] > 0)
                hit = 1;
        }

        /* 벽까지의 수직 거리 계산 */
        double perp_wall_dist;
        if (side == 0)
            perp_wall_dist = (map_x - state->pos_x + (1 - step_x) / 2) / ray_dir_x;
        else
            perp_wall_dist = (map_y - state->pos_y + (1 - step_y) / 2) / ray_dir_y;

        /* 화면에 그릴 수직선의 높이 계산 */
        int line_height = (int)(SCREEN_HEIGHT / perp_wall_dist);

        /* 수직선의 시작점과 끝점 계산 */
        int draw_start = -line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_start < 0)
            draw_start = 0;

        int draw_end = line_height / 2 + SCREEN_HEIGHT / 2;
        if (draw_end >= SCREEN_HEIGHT)
            draw_end = SCREEN_HEIGHT - 1;

        /* 벽 타입에 따른 문자 선택 */
        int wall_type = g_world_map[map_x][map_y];
        char wall_char = (side == 1) ?
            SHADED_WALL_CHARS[wall_type] : WALL_CHARS[wall_type];

        /* 수직선 그리기 */
        int y;
        for (y = draw_start; y <= draw_end; y++) {
            state->screen[y][x] = wall_char;
        }

        /* 바닥과 천장 그리기 */
        for (y = 0; y < draw_start; y++)
            state->screen[y][x] = ' ';  /* 천장 */
        for (y = draw_end + 1; y < SCREEN_HEIGHT; y++)
            state->screen[y][x] = '.';  /* 바닥 */
    }
}