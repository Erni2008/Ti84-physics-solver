#include <tice.h>

#include "formula.h"
#include "menu.h"
#include "reference.h"
#include "symbols.h"
#include "theory.h"

static void init_app(void) {
}

static void close_app(void) {
}

int main(void) {
    uint8_t home_index = 0;
    uint8_t category_index = 0;
    uint8_t formula_index = 0;

    init_app();
    formula_registry_init();

    while (true) {
        app_section_t section = APP_SECTION_SOLVER;
        menu_result_t home_result = menu_select_home(&home_index, &section);

        if (home_result == MENU_EXIT_APP || home_result == MENU_BACK) {
            break;
        }

        if (section == APP_SECTION_REFERENCE) {
            if (reference_run() == MENU_EXIT_APP) {
                break;
            }
            continue;
        }

        if (section == APP_SECTION_SYMBOLS) {
            if (symbols_run() == MENU_EXIT_APP) {
                break;
            }
            continue;
        }

        if (section == APP_SECTION_THEORY) {
            if (theory_run() == MENU_EXIT_APP) {
                break;
            }
            continue;
        }

        if (section == APP_SECTION_PHYSEX) {
            const category_def_t *categories = NULL;
            uint8_t category_count = 0;
            const category_def_t *category = NULL;

            categories = formula_get_categories(&category_count);
            if (category_count <= 6) {
                continue;
            }
            category = &categories[6];

            formula_index = 0;
            while (true) {
                menu_result_t formula_result = menu_select_formula(category, &formula_index);
                if (formula_result == MENU_BACK) {
                    break;
                }
                if (formula_result == MENU_EXIT_APP) {
                    close_app();
                    return 0;
                }

                if (formula_run(category->formulas[formula_index]) == FORMULA_RUN_EXIT_APP) {
                    close_app();
                    return 0;
                }
            }
            continue;
        }

        while (true) {
            const category_def_t *category = NULL;
            menu_result_t category_result = menu_select_category(&category_index, &category);

            if (category_result == MENU_BACK) {
                break;
            }
            if (category_result == MENU_EXIT_APP) {
                close_app();
                return 0;
            }

            formula_index = 0;
            while (true) {
                menu_result_t formula_result = menu_select_formula(category, &formula_index);
                if (formula_result == MENU_BACK) {
                    break;
                }
                if (formula_result == MENU_EXIT_APP) {
                    close_app();
                    return 0;
                }

                if (formula_run(category->formulas[formula_index]) == FORMULA_RUN_EXIT_APP) {
                    close_app();
                    return 0;
                }
            }
        }
    }

    close_app();
    return 0;
}
