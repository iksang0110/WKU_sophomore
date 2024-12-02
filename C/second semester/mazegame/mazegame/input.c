/* input.c */
#include <conio.h>
#include <math.h>
#include "raycaster.h"

int handle_input(struct s_game_state* state)
{
    if (!_kbhit())
        return TRUE;

    int key = _getch();
    double old_dir_x;
    double old_plane_x;

    /* ESC키 - 종료 */
    if (key == KEY_ESC)
        return FALSE;

    /* 이동 및 회전 처리 */
    switch (key) {
    case KEY_UP: /* 전진 */
        if (!g_world_map[(int)(state->pos_x + state->dir_x * state->move_speed)][(int)state->pos_y])
            state->pos_x += state->dir_x * state->move_speed;
        if (!g_world_map[(int)state->pos_x][(int)(state->pos_y + state->dir_y * state->move_speed)])
            state->pos_y += state->dir_y * state->move_speed;
        break;

    case KEY_DOWN: /* 후진 */
        if (!g_world_map[(int)(state->pos_x - state->dir_x * state->move_speed)][(int)state->pos_y])
            state->pos_x -= state->dir_x * state->move_speed;
        if (!g_world_map[(int)state->pos_x][(int)(state->pos_y - state->dir_y * state->move_speed)])
            state->pos_y -= state->dir_y * state->move_speed;
        break;

    case KEY_LEFT: /* 좌회전 */
        old_dir_x = state->dir_x;
        state->dir_x = state->dir_x * cos(state->rot_speed) -
            state->dir_y * sin(state->rot_speed);
        state->dir_y = old_dir_x * sin(state->rot_speed) +
            state->dir_y * cos(state->rot_speed);
        old_plane_x = state->plane_x;
        state->plane_x = state->plane_x * cos(state->rot_speed) -
            state->plane_y * sin(state->rot_speed);
        state->plane_y = old_plane_x * sin(state->rot_speed) +
            state->plane_y * cos(state->rot_speed);
        break;

    case KEY_RIGHT: /* 우회전 */
        old_dir_x = state->dir_x;
        state->dir_x = state->dir_x * cos(-state->rot_speed) -
            state->dir_y * sin(-state->rot_speed);
        state->dir_y = old_dir_x * sin(-state->rot_speed) +
            state->dir_y * cos(-state->rot_speed);
        old_plane_x = state->plane_x;
        state->plane_x = state->plane_x * cos(-state->rot_speed) -
            state->plane_y * sin(-state->rot_speed);
        state->plane_y = old_plane_x * sin(-state->rot_speed) +
            state->plane_y * cos(-state->rot_speed);
        break;
    }

    return TRUE;
}