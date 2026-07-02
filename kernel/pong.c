#include "pong.h"
#include "framebuffer.h"
#include "keyboard.h"
#include "mouse.h"
#include "font.h"
#include "renderer.h"
#include "timer.h"

typedef struct
{
    int32_t paddle_left_y;
    int32_t paddle_right_y;
    int32_t ball_x;
    int32_t ball_y;
    int32_t ball_dx;
    int32_t ball_dy;
    int32_t paddle_height;
    int32_t paddle_width;
    int32_t paddle_speed;
    int32_t ball_size;
    int32_t score_left;
    int32_t score_right;
} pong_state_t;

static pong_state_t g_pong = {0};

static void pong_reset_ball(void)
{
    g_pong.ball_x = framebuffer_width() / 2;
    g_pong.ball_y = framebuffer_height() / 2;
    g_pong.ball_dx = 6;
    g_pong.ball_dy = 3;
}
static void pong_draw(void)
{
    renderer_begin(0x00000000);

    draw_rect(10, g_pong.paddle_left_y, g_pong.paddle_width, g_pong.paddle_height, 0xFF0000);
    fill_rect(10, g_pong.paddle_left_y, g_pong.paddle_width, g_pong.paddle_height, 0xFF0000);

    draw_rect(framebuffer_width() - 22, g_pong.paddle_right_y, g_pong.paddle_width, g_pong.paddle_height, 0x0000FF);
    fill_rect(framebuffer_width() - 22, g_pong.paddle_right_y, g_pong.paddle_width, g_pong.paddle_height, 0x0000FF);

    draw_line(framebuffer_width() / 2, 0, framebuffer_width() / 2, framebuffer_height(), 0xFFFFFF);
    fill_rect(g_pong.ball_x, g_pong.ball_y, g_pong.ball_size, g_pong.ball_size, 0xFFFFFF);

    fill_rect(mouse_x, mouse_y, 5, 5,0xFFFFFF);

    font_zoom_cstr_render((uint8_t*)"Hello World!",12, 10, 10, 4,0xFFFFFF);

    renderer_present();
}

void pong_init(void)
{
    g_pong.paddle_height = 80;
    g_pong.paddle_width = 12;
    g_pong.paddle_speed = 8;
    g_pong.ball_size = 8;
    g_pong.paddle_left_y = framebuffer_height() / 2 - g_pong.paddle_height / 2;
    g_pong.paddle_right_y = framebuffer_height() / 2 - g_pong.paddle_height / 2;
    g_pong.score_left = 0;
    g_pong.score_right = 0;
    pong_reset_ball();
}
void pong_run(void)
{
    while (1)
    {
        timer_tick();
        keyboard_update();
        mouse_update();

        // mouse_is_button_down(1) 

        if (keyboard_is_key_down(KEY_W) && g_pong.paddle_left_y > 0)
        {
            g_pong.paddle_left_y -= g_pong.paddle_speed;
        }
        if (keyboard_is_key_down(KEY_S) && (g_pong.paddle_left_y + g_pong.paddle_height) < (int32_t)framebuffer_height())
        {
            g_pong.paddle_left_y += g_pong.paddle_speed;
        }

        if (keyboard_is_key_down(KEY_UP) && g_pong.paddle_right_y > 0)
        {
            g_pong.paddle_right_y -= g_pong.paddle_speed;
        }
        if (keyboard_is_key_down(KEY_DOWN) && g_pong.paddle_right_y + g_pong.paddle_height < (int32_t)framebuffer_height())
        {
            g_pong.paddle_right_y += g_pong.paddle_speed;
        }

        g_pong.ball_x += g_pong.ball_dx;
        g_pong.ball_y += g_pong.ball_dy;

        if (g_pong.ball_y <= 0 || g_pong.ball_y + g_pong.ball_size >= (int32_t)framebuffer_height())
        {
            g_pong.ball_dy = -g_pong.ball_dy;
            g_pong.ball_y += g_pong.ball_dy;
        }

        if (g_pong.ball_x <= 24 &&
            g_pong.ball_y + g_pong.ball_size >= g_pong.paddle_left_y &&
            g_pong.ball_y <= g_pong.paddle_left_y + g_pong.paddle_height)
        {
            g_pong.ball_dx = -g_pong.ball_dx;
            g_pong.ball_x = 24;
        }

        if (g_pong.ball_x + g_pong.ball_size >= (int32_t)framebuffer_width() - 24 &&
            g_pong.ball_y + g_pong.ball_size >= g_pong.paddle_right_y &&
            g_pong.ball_y <= g_pong.paddle_right_y + g_pong.paddle_height)
        {
            g_pong.ball_dx = -g_pong.ball_dx;
            g_pong.ball_x = (int32_t)framebuffer_width() - 24 - g_pong.ball_size;
        }

        if (g_pong.ball_x < 0)
        {
            g_pong.score_right++;
            pong_reset_ball();
        }

        if (g_pong.ball_x + g_pong.ball_size > (int32_t)framebuffer_width())
        {
            g_pong.score_left++;
            pong_reset_ball();
        }

        pong_draw();

        for (volatile int i = 0; i < 10000000; i++)
        {
        }
    }
}
