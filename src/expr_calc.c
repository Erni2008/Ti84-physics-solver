#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "expr_calc.h"
#include "io_helpers.h"
#include "math_helpers.h"

#define EXPR_MAX_DEGREE 8
#define EXPR_BUFFER_SIZE 64

typedef struct {
    double coeffs[EXPR_MAX_DEGREE + 1];
    char variable;
    bool has_variable;
} polynomial_t;

typedef struct {
    polynomial_t i;
    polynomial_t j;
    polynomial_t k;
} vector_poly_t;

static void poly_clear(polynomial_t *poly) {
    uint8_t i;
    for (i = 0; i <= EXPR_MAX_DEGREE; ++i) {
        poly->coeffs[i] = 0.0;
    }
    poly->variable = 'x';
    poly->has_variable = false;
}

static bool parse_number(const char **p, double *out) {
    char *endptr;
    double value = strtod(*p, &endptr);
    if (endptr == *p) {
        return false;
    }
    *out = value;
    *p = endptr;
    return true;
}

static const char *skip_spaces(const char *p) {
    while (*p == ' ') {
        ++p;
    }
    return p;
}

static bool has_wrapping_parentheses(const char *expr) {
    int depth = 0;
    size_t i = 0;
    size_t len = strlen(expr);

    if (len < 2 || expr[0] != '(' || expr[len - 1] != ')') {
        return false;
    }

    for (i = 0; i < len; ++i) {
        if (expr[i] == '(') {
            ++depth;
        } else if (expr[i] == ')') {
            --depth;
            if (depth == 0 && i != len - 1) {
                return false;
            }
        }
        if (depth < 0) {
            return false;
        }
    }
    return depth == 0;
}

static bool copy_without_outer_parentheses(const char *expr, char *out, size_t out_size) {
    char temp[EXPR_BUFFER_SIZE];
    size_t len;
    const char *start = skip_spaces(expr);

    if (strlen(start) >= sizeof(temp)) {
        return false;
    }
    strcpy(temp, start);
    len = strlen(temp);
    while (len > 0 && temp[len - 1] == ' ') {
        temp[--len] = '\0';
    }

    while (has_wrapping_parentheses(temp)) {
        memmove(temp, temp + 1, len - 2);
        temp[len - 2] = '\0';
        len -= 2;
    }

    if (strlen(temp) >= out_size) {
        return false;
    }
    strcpy(out, temp);
    return true;
}

static bool parse_polynomial(const char *expr, polynomial_t *poly, char *error, size_t error_size) {
    char normalized[EXPR_BUFFER_SIZE];
    const char *p = normalized;
    poly_clear(poly);

    if (!copy_without_outer_parentheses(expr, normalized, sizeof(normalized))) {
        snprintf(error, error_size, "Expression too long.");
        return false;
    }

    while (*p != '\0') {
        double coeff = 1.0;
        int degree = 0;
        int sign = 1;
        bool has_coeff = false;
        bool has_var = false;

        p = skip_spaces(p);
        if (*p == '+') {
            ++p;
        } else if (*p == '-') {
            sign = -1;
            ++p;
        }
        p = skip_spaces(p);

        if (*p == '(') {
            char grouped[EXPR_BUFFER_SIZE];
            polynomial_t grouped_poly;
            const char *group_start = p + 1;
            const char *group_end = group_start;
            int depth = 1;
            size_t group_len;
            uint8_t idx;

            while (*group_end != '\0' && depth > 0) {
                if (*group_end == '(') {
                    ++depth;
                } else if (*group_end == ')') {
                    --depth;
                }
                if (depth > 0) {
                    ++group_end;
                }
            }
            if (depth != 0) {
                snprintf(error, error_size, "Missing ).");
                return false;
            }
            group_len = (size_t)(group_end - group_start);
            if (group_len >= sizeof(grouped)) {
                snprintf(error, error_size, "Group too long.");
                return false;
            }
            memcpy(grouped, group_start, group_len);
            grouped[group_len] = '\0';
            if (!parse_polynomial(grouped, &grouped_poly, error, error_size)) {
                return false;
            }
            for (idx = 0; idx <= EXPR_MAX_DEGREE; ++idx) {
                poly->coeffs[idx] += sign * grouped_poly.coeffs[idx];
            }
            if (!poly->has_variable && grouped_poly.has_variable) {
                poly->variable = grouped_poly.variable;
                poly->has_variable = true;
            }
            p = group_end + 1;
            p = skip_spaces(p);
            if (*p == '*') {
                ++p;
                p = skip_spaces(p);
            }
            if (*p != '\0' && *p != '+' && *p != '-') {
                snprintf(error, error_size, "Unexpected input.");
                return false;
            }
            continue;
        }

        if (isdigit((unsigned char)*p) || *p == '.') {
            if (!parse_number(&p, &coeff)) {
                snprintf(error, error_size, "Invalid number.");
                return false;
            }
            has_coeff = true;
        }

        if (*p == '*') {
            ++p;
        }

        if (*p == 'x' || *p == 't') {
            has_var = true;
            if (!poly->has_variable) {
                poly->variable = *p;
                poly->has_variable = true;
            } else if (poly->variable != *p) {
                snprintf(error, error_size, "Use only one variable.");
                return false;
            }
            degree = 1;
            ++p;
            if (*p == '^') {
                ++p;
                if (!isdigit((unsigned char)*p)) {
                    snprintf(error, error_size, "Bad exponent.");
                    return false;
                }
                degree = 0;
                while (isdigit((unsigned char)*p)) {
                    degree = degree * 10 + (*p - '0');
                    ++p;
                }
            }
        } else if (!has_coeff) {
            snprintf(error, error_size, "Use terms like 10t or 3x^2.");
            return false;
        }

        if (degree > EXPR_MAX_DEGREE) {
            snprintf(error, error_size, "Degree too high.");
            return false;
        }

        if (!has_var) {
            degree = 0;
        }
        if (!has_coeff && has_var) {
            coeff = 1.0;
        }
        poly->coeffs[degree] += sign * coeff;

        p = skip_spaces(p);
        if (*p != '\0' && *p != '+' && *p != '-') {
            snprintf(error, error_size, "Unexpected input.");
            return false;
        }
    }

    return true;
}

static double poly_eval(const polynomial_t *poly, double x) {
    int i;
    double sum = 0.0;
    for (i = 0; i <= EXPR_MAX_DEGREE; ++i) {
        if (!math_is_near_zero(poly->coeffs[i])) {
            sum += poly->coeffs[i] * pow(x, (double)i);
        }
    }
    return sum;
}

static void poly_derivative(const polynomial_t *src, polynomial_t *dst) {
    int i;
    poly_clear(dst);
    dst->variable = src->variable;
    dst->has_variable = src->has_variable;
    for (i = 1; i <= EXPR_MAX_DEGREE; ++i) {
        dst->coeffs[i - 1] = src->coeffs[i] * i;
    }
}

static bool poly_integral(const polynomial_t *src, polynomial_t *dst, char *error, size_t error_size) {
    int i;
    poly_clear(dst);
    dst->variable = src->variable;
    dst->has_variable = src->has_variable;
    for (i = 0; i <= EXPR_MAX_DEGREE; ++i) {
        if (!math_is_near_zero(src->coeffs[i])) {
            if (i + 1 > EXPR_MAX_DEGREE) {
                snprintf(error, error_size, "Integral degree too high.");
                return false;
            }
            dst->coeffs[i + 1] = src->coeffs[i] / (i + 1);
        }
    }
    return true;
}

static void poly_to_string(const polynomial_t *poly, char *buffer, size_t size, bool add_constant) {
    int i;
    bool first = true;
    size_t used = 0;
    buffer[0] = '\0';

    for (i = EXPR_MAX_DEGREE; i >= 0; --i) {
        double c = poly->coeffs[i];
        char term[32];

        if (math_is_near_zero(c)) {
            continue;
        }

        if (i == 0) {
            snprintf(term, sizeof(term), "%.4g", fabs(c));
        } else if (i == 1) {
            if (fabs(c) == 1.0) {
                snprintf(term, sizeof(term), "%c", poly->has_variable ? poly->variable : 'x');
            } else {
                snprintf(term, sizeof(term), "%.4g%c", fabs(c), poly->has_variable ? poly->variable : 'x');
            }
        } else {
            if (fabs(c) == 1.0) {
                snprintf(term, sizeof(term), "%c^%d", poly->has_variable ? poly->variable : 'x', i);
            } else {
                snprintf(term, sizeof(term), "%.4g%c^%d", fabs(c), poly->has_variable ? poly->variable : 'x', i);
            }
        }

        used += snprintf(buffer + used, size > used ? size - used : 0, "%s%s%s",
            first ? (c < 0 ? "-" : "") : (c < 0 ? " - " : " + "),
            term,
            "");
        first = false;
    }

    if (first) {
        snprintf(buffer, size, "0");
    } else if (add_constant) {
        used = strlen(buffer);
        snprintf(buffer + used, size > used ? size - used : 0, " + C");
    }
}

static void show_vector_result(const char *title, const vector_poly_t *vec, bool add_constant) {
    char line_i[32];
    char line_j[32];
    char line_k[32];
    char part[32];

    poly_to_string(&vec->i, part, sizeof(part), add_constant);
    snprintf(line_i, sizeof(line_i), "i: %s", part);
    poly_to_string(&vec->j, part, sizeof(part), add_constant);
    snprintf(line_j, sizeof(line_j), "j: %s", part);
    poly_to_string(&vec->k, part, sizeof(part), add_constant);
    snprintf(line_k, sizeof(line_k), "k: %s", part);

    io_clear_screen();
    io_draw_title(title);
    io_draw_wrapped_text(2, line_i, 26);
    io_draw_wrapped_text(4, line_j, 26);
    io_draw_wrapped_text(6, line_k, 26);
    io_draw_footer("ENTER/CLEAR continue");

    while (true) {
        uint8_t action = io_read_menu_key();
        if (action == IO_MENU_SELECT || action == IO_MENU_EXIT) {
            return;
        }
    }
}

static void show_text_result(const char *title, const char *line1, const char *line2) {
    io_clear_screen();
    io_draw_title(title);
    io_draw_wrapped_text(2, line1, 26);
    if (line2 != NULL && line2[0] != '\0') {
        io_draw_wrapped_text(5, line2, 26);
    }
    io_draw_footer("ENTER/CLEAR continue");
    while (true) {
        uint8_t action = io_read_menu_key();
        if (action == IO_MENU_SELECT || action == IO_MENU_EXIT) {
            return;
        }
    }
}

static bool prompt_vector_components(const char *title, vector_poly_t *vec, char *error, size_t error_size) {
    char input[EXPR_BUFFER_SIZE];

    if (!io_prompt_text(title, "i component, ex: 2t", input, sizeof(input))) {
        return false;
    }
    if (!parse_polynomial(input, &vec->i, error, error_size)) {
        io_show_message("Vector Error", error, "Bad i component.");
        return false;
    }

    if (!io_prompt_text(title, "j component, ex: t+4t^2", input, sizeof(input))) {
        return false;
    }
    if (!parse_polynomial(input, &vec->j, error, error_size)) {
        io_show_message("Vector Error", error, "Bad j component.");
        return false;
    }

    if (!io_prompt_text(title, "k component, ex: -(3t+t^3)", input, sizeof(input))) {
        return false;
    }
    if (!parse_polynomial(input, &vec->k, error, error_size)) {
        io_show_message("Vector Error", error, "Bad k component.");
        return false;
    }

    return true;
}

formula_run_result_t expr_calc_run(const formula_def_t *formula) {
    static const char *actions[] = {
        "Derivative formula",
        "Derivative at value",
        "Integral formula",
        "Definite integral",
        "Vector derivative",
        "Vector integral"
    };
    uint8_t selected = 0;

    while (true) {
        char expr[EXPR_BUFFER_SIZE];
        char error[40];
        polynomial_t poly;
        polynomial_t result_poly;
        uint8_t i;

        io_clear_screen();
        io_draw_title(formula->expression);
        io_draw_wrapped_text(1, "Supports polynomial terms like 10t, 3t^2+2, x^3-x.", 26);
        for (i = 0; i < 6; ++i) {
            char line[28];
            snprintf(line, sizeof(line), "%c %s", i == selected ? '>' : ' ', actions[i]);
            io_draw_wrapped_text((uint8_t)(4 + i), line, 26);
        }
        io_draw_footer("UP/DN OK MD/CLR BK");

        switch (io_read_menu_key()) {
            case IO_MENU_UP:
                if (selected > 0) --selected;
                continue;
            case IO_MENU_DOWN:
                if (selected < 5) ++selected;
                continue;
            case IO_MENU_BACK:
                return FORMULA_RUN_BACK;
            case IO_MENU_EXIT:
                return FORMULA_RUN_EXIT_APP;
            case IO_MENU_SELECT:
                break;
            default:
                continue;
        }

        if (selected >= 4) {
            vector_poly_t vec;
            vector_poly_t result_vec;

            if (!prompt_vector_components(formula->name, &vec, error, sizeof(error))) {
                continue;
            }
            if (selected == 4) {
                poly_derivative(&vec.i, &result_vec.i);
                poly_derivative(&vec.j, &result_vec.j);
                poly_derivative(&vec.k, &result_vec.k);
                show_vector_result("Vector derivative", &result_vec, false);
            } else {
                if (!poly_integral(&vec.i, &result_vec.i, error, sizeof(error)) ||
                    !poly_integral(&vec.j, &result_vec.j, error, sizeof(error)) ||
                    !poly_integral(&vec.k, &result_vec.k, error, sizeof(error))) {
                    io_show_message("Vector Error", error, "");
                    continue;
                }
                show_vector_result("Vector integral", &result_vec, true);
            }
            continue;
        }

        if (!io_prompt_text(formula->name, "Enter expression with x or t:", expr, sizeof(expr))) {
            continue;
        }
        if (!parse_polynomial(expr, &poly, error, sizeof(error))) {
            io_show_message("Expr Error", error, "Example: 10t+3t^2");
            continue;
        }

        if (selected == 0) {
            char out[64];
            poly_derivative(&poly, &result_poly);
            poly_to_string(&result_poly, out, sizeof(out), false);
            show_text_result("Derivative", out, "");
        } else if (selected == 1) {
            double value;
            char out[40];
            poly_derivative(&poly, &result_poly);
            if (!io_prompt_double(formula->name, "Enter x or t value:", &value)) {
                continue;
            }
            snprintf(out, sizeof(out), "Value = %.8g", poly_eval(&result_poly, value));
            show_text_result("Derivative at point", out, "");
        } else if (selected == 2) {
            char out[64];
            if (!poly_integral(&poly, &result_poly, error, sizeof(error))) {
                io_show_message("Expr Error", error, "");
                continue;
            }
            poly_to_string(&result_poly, out, sizeof(out), true);
            show_text_result("Integral", out, "");
        } else if (selected == 3) {
            double a, b;
            char out[40];
            if (!poly_integral(&poly, &result_poly, error, sizeof(error))) {
                io_show_message("Expr Error", error, "");
                continue;
            }
            if (!io_prompt_double(formula->name, "Lower bound:", &a)) {
                continue;
            }
            if (!io_prompt_double(formula->name, "Upper bound:", &b)) {
                continue;
            }
            snprintf(out, sizeof(out), "Value = %.8g", poly_eval(&result_poly, b) - poly_eval(&result_poly, a));
            show_text_result("Definite integral", out, "");
        }
    }
}
