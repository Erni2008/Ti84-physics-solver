#ifndef MENU_H
#define MENU_H

#include <stdint.h>

#include "formula.h"

typedef enum {
    MENU_STAY = 0,
    MENU_BACK,
    MENU_EXIT_APP
} menu_result_t;

typedef enum {
    APP_SECTION_SOLVER = 0,
    APP_SECTION_REFERENCE,
    APP_SECTION_SYMBOLS
} app_section_t;

menu_result_t menu_select_home(uint8_t *selected_index, app_section_t *section_out);
menu_result_t menu_select_category(uint8_t *selected_index, const category_def_t **category_out);
menu_result_t menu_select_formula(const category_def_t *category, uint8_t *selected_index);

#endif
