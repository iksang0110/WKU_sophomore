/* game.c */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>
#include <windows.h>
#include "raycaster.h"
#include "map.h"

void init_game_state(struct s_game_state* state)
{
    state->pos_x = 22.0;
    state->pos_y = 12.0;
    state->dir_x = -1.0;
    state->dir_y = 0.0;
    state->plane_x = 0.0;
    state->plane_y = 0.66;
    state->move_speed = 0.15;
    state->rot_speed = 0.1;

    /* 화면 초기화 */
    clear_screen(state);

    /* 콘솔 설정 */
    system("mode con cols=120 lines=40");
    hide_cursor();
}

void set_cursor_position(int x, int y)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(handle, pos);
}

void hide_cursor(void)
{
    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(handle, &cursor_info);
    cursor_info.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &cursor_info);
}

void clear_screen(struct s_game_state* state)
{
    int i, j;
    for (i = 0; i < SCREEN_HEIGHT; i++) {
        for (j = 0; j < SCREEN_WIDTH; j++) {
            state->screen[i][j] = ' ';
        }
    }
}

int handle_input(struct s_game_state* state)
{
    if (!_kbhit())
        return TRUE;

    int key = _getch();
    double old_dir_x;
    double old_plane_x;

    switch (key) {
    case 27:  /* ESC */
        return FALSE;

    case 'w': /* 전진 */
        if (!g_world_map[(int)(state->pos_x + state->dir_x * state->move_speed)][(int)state->pos_y])
            state->pos_x += state->dir_x * state->move_speed;
        if (!g_world_map[(int)state->pos_x][(int)(state->pos_y + state->dir_y * state->move_speed)])
            state->pos_y += state->dir_y * state->move_speed;
        break;

    case 's': /* 후진 */
        if (!g_world_map[(int)(state->pos_x - state->dir_x * state->move_speed)][(int)state->pos_y])
            state->pos_x -= state->dir_x * state->move_speed;
        if (!g_world_map[(int)state->pos_x][(int)(state->pos_y - state->dir_y * state->move_speed)])
            state->pos_y -= state->dir_y * state->move_speed;
        break;

    case 'a': /* 좌회전 */
        old_dir_x = state->dir_x;
        state->dir_x = state->dir_x * cos(state->rot_speed) - state->dir_y * sin(state->rot_speed);
        state->dir_y = old_dir_x * sin(state->rot_speed) + state->dir_y * cos(state->rot_speed);
        old_plane_x = state->plane_x;
        state->plane_x = state->plane_x * cos(state->rot_speed) - state->plane_y * sin(state->rot_speed);
        state->plane_y = old_plane_x * sin(state->rot_speed) + state->plane_y * cos(state->rot_speed);
        break;

    case 'd': /* 우회전 */
        old_dir_x = state->dir_x;
        state->dir_x = state->dir_x * cos(-state->rot_speed) - state->dir_y * sin(-state->rot_speed);
        state->dir_y = old_dir_x * sin(-state->rot_speed) + state->dir_y * cos(-state->rot_speed);
        old_plane_x = state->plane_x;
        state->plane_x = state->plane_x * cos(-state->rot_speed) - state->plane_y * sin(-state->rot_speed);
        state->plane_y = old_plane_x * sin(-state->rot_speed) + state->plane_y * cos(-state->rot_speed);
        break;
    }
    return TRUE;
}