#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <tice.h>
#include <ti/getcsc.h>
#include <ti/screen.h>

#include "io_helpers.h"

#define INPUT_BUFFER_SIZE 24

static char read_key(void) {
    while (true) {
        char key = os_GetCSC();
        if (key != 0) {
            return key;
        }
    }
}

bool io_wait_for_key_release(void) {
    while (os_GetCSC() != 0) {
    }
    return true;
}

void io_clear_screen(void) {
    os_ClrHome();
}

static void io_put_line(uint8_t line, const char *text) {
    os_SetCursorPos(line, 0);
    os_PutStrFull(text);
}

void io_draw_title(const char *title) {
    io_put_line(0, title);
}

void io_draw_footer(const char *footer) {
    io_put_line(9, footer);
}

void io_draw_wrapped_text(uint8_t line, const char *text, uint8_t max_chars_per_line) {
    char row[28];
    uint8_t row_len = 0;
    uint8_t current_line = line;

    while (*text != '\0' && current_line < 9) {
        if (*text == '\n' || row_len >= max_chars_per_line) {
            row[row_len] = '\0';
            io_put_line(current_line++, row);
            row_len = 0;

            if (*text == '\n') {
                ++text;
            }
            continue;
        }

        row[row_len++] = *text++;
    }

    if (row_len > 0 && current_line < 9) {
        row[row_len] = '\0';
        io_put_line(current_line, row);
    }
}

void io_show_message(const char *title, const char *line1, const char *line2) {
    io_clear_screen();
    io_draw_title(title);
    io_draw_wrapped_text(2, line1, 26);
    if (line2 != NULL && line2[0] != '\0') {
        io_draw_wrapped_text(5, line2, 26);
    }
    io_draw_footer("ENTER/CLEAR continue");

    while (true) {
        char key = read_key();
        if (key == sk_Enter || key == sk_Clear) {
            io_wait_for_key_release();
            return;
        }
    }
}

static bool append_char_limited(char *buffer, uint8_t *length, size_t capacity, char c) {
    if (*length >= capacity - 1) {
        return false;
    }

    buffer[*length] = c;
    buffer[*length + 1] = '\0';
    ++(*length);
    return true;
}

static bool append_char(char *buffer, uint8_t *length, char c) {
    return append_char_limited(buffer, length, INPUT_BUFFER_SIZE, c);
}

static bool handle_input_key(char key, char *buffer, uint8_t *length, bool *is_back) {
    switch (key) {
        case sk_0:
            return append_char(buffer, length, '0');
        case sk_1:
            return append_char(buffer, length, '1');
        case sk_2:
            return append_char(buffer, length, '2');
        case sk_3:
            return append_char(buffer, length, '3');
        case sk_4:
            return append_char(buffer, length, '4');
        case sk_5:
            return append_char(buffer, length, '5');
        case sk_6:
            return append_char(buffer, length, '6');
        case sk_7:
            return append_char(buffer, length, '7');
        case sk_8:
            return append_char(buffer, length, '8');
        case sk_9:
            return append_char(buffer, length, '9');
        case sk_DecPnt:
            if (strchr(buffer, '.') == NULL) {
                return append_char(buffer, length, '.');
            }
            return false;
        case sk_Chs:
            if (*length == 0) {
                return append_char(buffer, length, '-');
            }
            return false;
        case sk_Del:
            if (*length > 0) {
                buffer[*length - 1] = '\0';
                --(*length);
            }
            return true;
        case sk_Mode:
            *is_back = true;
            return true;
        default:
            return false;
    }
}

bool io_prompt_double(const char *title, const char *prompt, double *out_value) {
    char buffer[INPUT_BUFFER_SIZE] = {0};
    uint8_t length = 0;

    while (true) {
        bool is_back = false;
        char line[32];
        char key;

        io_clear_screen();
        io_draw_title(title);
        io_draw_wrapped_text(2, prompt, 26);
        snprintf(line, sizeof(line), "> %s", buffer);
        io_put_line(5, line);
        io_draw_footer("ENTER ok MODE back");

        key = read_key();
        if (key == sk_Enter) {
            double parsed = 0.0;
            char *endptr = NULL;

            if (length == 0 || (length == 1 && buffer[0] == '-')) {
                io_show_message("Input Error", "Please enter a", "valid number.");
                continue;
            }

            parsed = strtod(buffer, &endptr);
            if (endptr == buffer || *endptr != '\0') {
                io_show_message("Input Error", "Number format", "not supported.");
                continue;
            }

            *out_value = parsed;
            io_wait_for_key_release();
            return true;
        }

        handle_input_key(key, buffer, &length, &is_back);
        io_wait_for_key_release();

        if (is_back) {
            return false;
        }
    }
}

bool io_prompt_text(const char *title, const char *prompt, char *buffer, size_t buffer_size) {
    uint8_t length = 0;
    bool alpha_mode = false;

    buffer[0] = '\0';

    while (true) {
        char line[32];
        char key;

        io_clear_screen();
        io_draw_title(title);
        io_draw_wrapped_text(1, prompt, 26);
        io_draw_wrapped_text(3, "X,T key=t  ALPHA+X,T=x", 26);
        snprintf(line, sizeof(line), "> %s", buffer);
        io_put_line(6, line);
        io_draw_footer("ENTER ok DEL MODE back");

        key = read_key();
        if (key == sk_Enter) {
            io_wait_for_key_release();
            return length > 0;
        }
        if (key == sk_Mode || key == sk_Clear) {
            io_wait_for_key_release();
            return false;
        }
        if (key == sk_Alpha) {
            alpha_mode = !alpha_mode;
            io_wait_for_key_release();
            continue;
        }
        if (key == sk_Del) {
            if (length > 0) {
                buffer[length - 1] = '\0';
                --length;
            }
            io_wait_for_key_release();
            continue;
        }

        if (key == sk_GraphVar) {
            append_char_limited(buffer, &length, buffer_size, alpha_mode ? 'x' : 't');
            alpha_mode = false;
        } else if (key == sk_Add) {
            append_char_limited(buffer, &length, buffer_size, '+');
        } else if (key == sk_Sub) {
            append_char_limited(buffer, &length, buffer_size, '-');
        } else if (key == sk_Mul) {
            append_char_limited(buffer, &length, buffer_size, '*');
        } else if (key == sk_Div) {
            append_char_limited(buffer, &length, buffer_size, '/');
        } else if (key == sk_Power) {
            append_char_limited(buffer, &length, buffer_size, '^');
        } else if (key == sk_Square) {
            if (append_char_limited(buffer, &length, buffer_size, '^')) {
                append_char_limited(buffer, &length, buffer_size, '2');
            }
        } else if (key == sk_DecPnt) {
            append_char_limited(buffer, &length, buffer_size, '.');
        } else if (key == sk_Chs) {
            append_char_limited(buffer, &length, buffer_size, '-');
        } else if (key == sk_0) {
            append_char_limited(buffer, &length, buffer_size, '0');
        } else if (key == sk_1) {
            append_char_limited(buffer, &length, buffer_size, '1');
        } else if (key == sk_2) {
            append_char_limited(buffer, &length, buffer_size, '2');
        } else if (key == sk_3) {
            append_char_limited(buffer, &length, buffer_size, '3');
        } else if (key == sk_4) {
            append_char_limited(buffer, &length, buffer_size, '4');
        } else if (key == sk_5) {
            append_char_limited(buffer, &length, buffer_size, '5');
        } else if (key == sk_6) {
            append_char_limited(buffer, &length, buffer_size, '6');
        } else if (key == sk_7) {
            append_char_limited(buffer, &length, buffer_size, '7');
        } else if (key == sk_8) {
            append_char_limited(buffer, &length, buffer_size, '8');
        } else if (key == sk_9) {
            append_char_limited(buffer, &length, buffer_size, '9');
        } else {
            alpha_mode = false;
        }

        io_wait_for_key_release();
    }
}

void io_show_result(const char *formula_name, const char *target_name, double result, const char *const_note) {
    char result_line[32];

    snprintf(result_line, sizeof(result_line), "%s = %.8g", target_name, result);

    io_clear_screen();
    io_draw_title("Result");
    io_draw_wrapped_text(2, formula_name, 26);
    io_draw_wrapped_text(4, result_line, 26);
    io_draw_wrapped_text(6, const_note, 26);
    io_draw_footer("ENTER/CLEAR continue");

    while (true) {
        char key = read_key();
        if (key == sk_Enter || key == sk_Clear) {
            io_wait_for_key_release();
            return;
        }
    }
}

uint8_t io_read_menu_key(void) {
    char key = read_key();

    io_wait_for_key_release();

    if (key == sk_Up) {
        return IO_MENU_UP;
    }
    if (key == sk_Down) {
        return IO_MENU_DOWN;
    }
    if (key == sk_Enter) {
        return IO_MENU_SELECT;
    }
    if (key == sk_Mode) {
        return IO_MENU_BACK;
    }
    if (key == sk_Clear) {
        return IO_MENU_EXIT;
    }

    return 0;
}
