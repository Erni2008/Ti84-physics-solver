#ifndef IO_HELPERS_H
#define IO_HELPERS_H

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>

#define IO_MENU_UP 1
#define IO_MENU_DOWN 2
#define IO_MENU_SELECT 3
#define IO_MENU_BACK 4
#define IO_MENU_EXIT 5
#define IO_MENU_LEFT 6
#define IO_MENU_RIGHT 7

bool io_wait_for_key_release(void);
void io_clear_screen(void);
void io_draw_title(const char *title);
void io_draw_footer(const char *footer);
void io_draw_wrapped_text(uint8_t line, const char *text, uint8_t max_chars_per_line);
void io_show_message(const char *title, const char *line1, const char *line2);
bool io_prompt_double(const char *title, const char *prompt, double *out_value);
bool io_prompt_text(const char *title, const char *prompt, char *buffer, size_t buffer_size);
void io_show_result(const char *formula_name, const char *target_name, double result, const char *const_note);
uint8_t io_read_menu_key(void);

#endif
