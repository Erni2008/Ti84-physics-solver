#include <stdio.h>
#include <string.h>

#include "io_helpers.h"
#include "menu.h"

static void draw_menu_line(uint8_t line, bool selected, const char *text) {
    char buffer[27];
    size_t max_len = sizeof(buffer) - 1;
    size_t text_len = strlen(text);

    buffer[0] = selected ? '>' : ' ';
    buffer[1] = ' ';

    if (text_len > max_len - 2) {
        text_len = max_len - 2;
    }

    memcpy(&buffer[2], text, text_len);
    buffer[2 + text_len] = '\0';
    io_draw_wrapped_text(line, buffer, 26);
}

static menu_result_t run_menu(const char *title, const char *const *items, uint8_t item_count, uint8_t *selected_index) {
    uint8_t scroll_offset = 0;
    const uint8_t visible_rows = 7;

    while (true) {
        uint8_t i = 0;
        uint8_t action = 0;

        if (*selected_index < scroll_offset) {
            scroll_offset = *selected_index;
        }
        if (*selected_index >= scroll_offset + visible_rows) {
            scroll_offset = (uint8_t)(*selected_index - visible_rows + 1);
        }

        io_clear_screen();
        io_draw_title(title);

        for (i = 0; i < visible_rows && (uint8_t)(scroll_offset + i) < item_count; ++i) {
            uint8_t item_index = (uint8_t)(scroll_offset + i);
            draw_menu_line((uint8_t)(2 + i), item_index == *selected_index, items[item_index]);
        }

        if (item_count > visible_rows) {
            io_draw_footer("UP/DN scroll OK select");
        } else {
            io_draw_footer("UP/DN OK MODE BACK");
        }
        action = io_read_menu_key();

        if (action == IO_MENU_UP && *selected_index > 0) {
            --(*selected_index);
        } else if (action == IO_MENU_DOWN && *selected_index + 1 < item_count) {
            ++(*selected_index);
        } else if (action == IO_MENU_SELECT) {
            return MENU_STAY;
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

menu_result_t menu_select_home(uint8_t *selected_index, app_section_t *section_out) {
    static const char *labels[] = {
        "Formula Solver",
        "All Formulas",
        "Symbols"
    };
    menu_result_t result = run_menu("Physics Tools", labels, 3, selected_index);

    if (result == MENU_STAY) {
        *section_out = (app_section_t)(*selected_index);
    }
    return result;
}

menu_result_t menu_select_category(uint8_t *selected_index, const category_def_t **category_out) {
    const category_def_t *categories = NULL;
    const char *labels[CATEGORY_COUNT];
    uint8_t count = 0;
    uint8_t i = 0;
    menu_result_t result;

    categories = formula_get_categories(&count);
    for (i = 0; i < count; ++i) {
        labels[i] = categories[i].name;
    }

    result = run_menu("Physics Solver", labels, count, selected_index);
    if (result == MENU_STAY) {
        *category_out = &categories[*selected_index];
    }
    return result;
}

menu_result_t menu_select_formula(const category_def_t *category, uint8_t *selected_index) {
    uint8_t scroll_offset = 0;
    const uint8_t visible_rows = 5;

    if (*selected_index >= category->formula_count) {
        *selected_index = 0;
    }

    while (true) {
        uint8_t i = 0;
        uint8_t action = 0;

        if (*selected_index < scroll_offset) {
            scroll_offset = *selected_index;
        }
        if (*selected_index >= scroll_offset + visible_rows) {
            scroll_offset = (uint8_t)(*selected_index - visible_rows + 1);
        }

        io_clear_screen();
        io_draw_title(category->name);
        io_draw_wrapped_text(1, category->formulas[*selected_index]->expression, 26);

        for (i = 0; i < visible_rows && (uint8_t)(scroll_offset + i) < category->formula_count; ++i) {
            uint8_t item_index = (uint8_t)(scroll_offset + i);
            draw_menu_line((uint8_t)(4 + i), item_index == *selected_index, category->formulas[item_index]->name);
        }

        io_draw_footer("UP/DN OK MODE BACK");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && *selected_index > 0) {
            --(*selected_index);
        } else if (action == IO_MENU_DOWN && *selected_index + 1 < category->formula_count) {
            ++(*selected_index);
        } else if (action == IO_MENU_SELECT) {
            return MENU_STAY;
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}
