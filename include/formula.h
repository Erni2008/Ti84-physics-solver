#ifndef FORMULA_H
#define FORMULA_H

#include <stdbool.h>
#include <stdint.h>

#define MAX_FORMULA_INPUTS 4
#define MAX_FORMULAS_PER_CATEGORY 49
#define CATEGORY_COUNT 6

typedef enum {
    VAR_V,
    VAR_V0,
    VAR_A,
    VAR_T,
    VAR_S,
    VAR_S0,
    VAR_R,
    VAR_OMEGA,
    VAR_ALPHA,
    VAR_AT,
    VAR_AC,
    VAR_F,
    VAR_PERIOD,
    VAR_FREQ,
    VAR_PHI,
    VAR_OMEGA0,
    VAR_N,
    VAR_DP,
    VAR_P,
    VAR_M,
    VAR_I,
    VAR_EK,
    VAR_W,
    VAR_D,
    VAR_PAVG,
    VAR_POWER,
    VAR_EP_DELTA,
    VAR_G,
    VAR_WEIGHT_G,
    VAR_H,
    VAR_H0,
    VAR_H1,
    VAR_H2,
    VAR_HMAX,
    VAR_E_TOTAL,
    VAR_EP,
    VAR_DELTA_EK,
    VAR_K,
    VAR_DOT_VA,
    VAR_V_MAG,
    VAR_A_MAG,
    VAR_PHI_DEG,
    VAR_K_FORCE_TIME,
    VAR_X,
    VAR_Y,
    VAR_Z,
    VAR_X1,
    VAR_X2,
    VAR_T1,
    VAR_T2,
    VAR_IROT,
    VAR_I0,
    VAR_AXIS_A,
    VAR_TORQUE_M,
    VAR_COUNT
} variable_id_t;

typedef struct {
    bool ok;
    double value;
    const char *error;
} solve_result_t;

typedef solve_result_t (*solver_fn_t)(const double *inputs);

typedef struct {
    variable_id_t target;
    uint8_t input_count;
    variable_id_t inputs[MAX_FORMULA_INPUTS];
    solver_fn_t solve;
} solve_option_t;

typedef enum {
    FORMULA_RUN_BACK = 0,
    FORMULA_RUN_EXIT_APP
} formula_run_result_t;

typedef struct formula_def_t formula_def_t;

struct formula_def_t {
    const char *name;
    const char *expression;
    const char *const_note;
    uint8_t option_count;
    const solve_option_t *options;
    formula_run_result_t (*custom_run)(const formula_def_t *formula);
};

typedef struct {
    const char *name;
    uint8_t formula_count;
    const formula_def_t *formulas[MAX_FORMULAS_PER_CATEGORY];
} category_def_t;

typedef struct {
    const formula_def_t *formula;
    const solve_option_t *option;
} solve_context_t;

void formula_registry_init(void);
const category_def_t *formula_get_categories(uint8_t *count_out);
const char *formula_variable_name(variable_id_t id);
const char *formula_variable_prompt(variable_id_t id);
const char *formula_variable_unit(variable_id_t id);
formula_run_result_t formula_run(const formula_def_t *formula);

#endif
