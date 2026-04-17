#include <math.h>
#include <stddef.h>
#include <stdio.h>

#include "expr_calc.h"
#include "formula.h"
#include "io_helpers.h"
#include "math_helpers.h"
#include "menu.h"

#define ARRAY_LEN(array) ((uint8_t)(sizeof(array) / sizeof((array)[0])))

static solve_result_t ok_result(double value) {
    solve_result_t result = { true, value, NULL };
    return result;
}

static solve_result_t error_result(const char *message) {
    solve_result_t result = { false, 0.0, message };
    return result;
}

static solve_result_t solve_v_from_v0_a_t(const double *in) { return ok_result(in[0] + in[1] * in[2]); }
static solve_result_t solve_v0_from_v_a_t(const double *in) { return ok_result(in[0] - in[1] * in[2]); }
static solve_result_t solve_a_from_v_v0_t(const double *in) {
    if (!math_can_divide(in[2])) return error_result("t cannot be zero.");
    return ok_result((in[0] - in[1]) / in[2]);
}
static solve_result_t solve_t_from_v_v0_a(const double *in) {
    if (!math_can_divide(in[2])) return error_result("a cannot be zero.");
    return ok_result((in[0] - in[1]) / in[2]);
}

static solve_result_t solve_s_from_s0_v_t(const double *in) { return ok_result(in[0] + in[1] * in[2]); }
static solve_result_t solve_s0_from_s_v_t(const double *in) { return ok_result(in[0] - in[1] * in[2]); }
static solve_result_t solve_v_from_s_s0_t(const double *in) {
    if (!math_can_divide(in[2])) return error_result("t cannot be zero.");
    return ok_result((in[0] - in[1]) / in[2]);
}
static solve_result_t solve_t_from_s_s0_v(const double *in) {
    if (!math_can_divide(in[2])) return error_result("v cannot be zero.");
    return ok_result((in[0] - in[1]) / in[2]);
}

static solve_result_t solve_s_from_v0_t_a(const double *in) { return ok_result(in[0] * in[1] + 0.5 * in[2] * in[1] * in[1]); }
static solve_result_t solve_v0_from_s_t_a(const double *in) {
    if (!math_can_divide(in[1])) return error_result("t cannot be zero.");
    return ok_result((in[0] - 0.5 * in[2] * in[1] * in[1]) / in[1]);
}
static solve_result_t solve_a_from_s_v0_t(const double *in) {
    double denominator = in[2] * in[2];
    if (!math_can_divide(denominator)) return error_result("t cannot be zero.");
    return ok_result(2.0 * (in[0] - in[1] * in[2]) / denominator);
}
static solve_result_t solve_t_from_s_v0_a(const double *in) {
    double a = in[2];
    double v0 = in[1];
    double s = in[0];
    double discriminant = v0 * v0 + 2.0 * a * s;

    if (math_is_near_zero(a)) {
        if (!math_can_divide(v0)) return error_result("No unique t value.");
        return ok_result(s / v0);
    }
    if (!math_can_sqrt(discriminant)) return error_result("Invalid square root.");

    double root = sqrt(discriminant);
    double t1 = (-v0 + root) / a;
    double t2 = (-v0 - root) / a;
    double best = math_is_negative(t1) ? t2 : t1;

    if (math_is_negative(best)) return error_result("No non-negative time result.");
    return ok_result(best);
}

static solve_result_t solve_v_from_s_t(const double *in) {
    if (!math_can_divide(in[1])) return error_result("t cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_s_from_v_t(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_t_from_s_v(const double *in) {
    if (!math_can_divide(in[1])) return error_result("v cannot be zero.");
    return ok_result(in[0] / in[1]);
}

static solve_result_t solve_a_from_v_v0_dt(const double *in) {
    if (!math_can_divide(in[2])) return error_result("t cannot be zero.");
    return ok_result((in[0] - in[1]) / in[2]);
}

static solve_result_t solve_v_from_v0_a_s(const double *in) {
    double radicand = in[0] * in[0] + 2.0 * in[1] * in[2];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_v0_from_v_a_s(const double *in) {
    double radicand = in[0] * in[0] - 2.0 * in[1] * in[2];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_a_from_v_v0_s(const double *in) {
    if (!math_can_divide(2.0 * in[2])) return error_result("s cannot be zero.");
    return ok_result((in[0] * in[0] - in[1] * in[1]) / (2.0 * in[2]));
}
static solve_result_t solve_s_from_v_v0_a(const double *in) {
    if (!math_can_divide(2.0 * in[2])) return error_result("a cannot be zero.");
    return ok_result((in[0] * in[0] - in[1] * in[1]) / (2.0 * in[2]));
}

static solve_result_t solve_v_from_r_omega(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_r_from_v_omega(const double *in) {
    if (!math_can_divide(in[1])) return error_result("omega cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_omega_from_v_r(const double *in) {
    if (!math_can_divide(in[1])) return error_result("r cannot be zero.");
    return ok_result(in[0] / in[1]);
}

static solve_result_t solve_at_from_r_alpha(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_r_from_at_alpha(const double *in) {
    if (!math_can_divide(in[1])) return error_result("alpha cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_alpha_from_at_r(const double *in) {
    if (!math_can_divide(in[1])) return error_result("r cannot be zero.");
    return ok_result(in[0] / in[1]);
}

static solve_result_t solve_ac_from_v_r(const double *in) {
    if (!math_can_divide(in[1])) return error_result("r cannot be zero.");
    return ok_result((in[0] * in[0]) / in[1]);
}
static solve_result_t solve_v_from_ac_r(const double *in) {
    double radicand = in[0] * in[1];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_r_from_ac_v(const double *in) {
    if (!math_can_divide(in[0])) return error_result("ac cannot be zero.");
    return ok_result((in[1] * in[1]) / in[0]);
}

static solve_result_t solve_omega_from_f(const double *in) { return ok_result(2.0 * PHYSICS_PI * in[0]); }
static solve_result_t solve_f_from_omega(const double *in) { return ok_result(in[0] / (2.0 * PHYSICS_PI)); }

static solve_result_t solve_omega_from_t_period(const double *in) {
    if (!math_can_divide(in[0])) return error_result("T cannot be zero.");
    return ok_result(2.0 * PHYSICS_PI / in[0]);
}
static solve_result_t solve_period_from_omega(const double *in) {
    if (!math_can_divide(in[0])) return error_result("omega cannot be zero.");
    return ok_result(2.0 * PHYSICS_PI / in[0]);
}

static solve_result_t solve_freq_from_period(const double *in) {
    if (!math_can_divide(in[0])) return error_result("T cannot be zero.");
    return ok_result(1.0 / in[0]);
}
static solve_result_t solve_period_from_freq(const double *in) {
    if (!math_can_divide(in[0])) return error_result("f cannot be zero.");
    return ok_result(1.0 / in[0]);
}

static solve_result_t solve_phi_from_omega_t(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_omega_from_phi_t(const double *in) {
    if (!math_can_divide(in[1])) return error_result("t cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_t_from_phi_omega(const double *in) {
    if (!math_can_divide(in[1])) return error_result("omega cannot be zero.");
    return ok_result(in[0] / in[1]);
}

static solve_result_t solve_omega_from_omega0_alpha_t(const double *in) { return ok_result(in[0] + in[1] * in[2]); }
static solve_result_t solve_omega0_from_omega_alpha_t(const double *in) { return ok_result(in[0] - in[1] * in[2]); }
static solve_result_t solve_alpha_from_omega_omega0_t(const double *in) {
    if (!math_can_divide(in[2])) return error_result("t cannot be zero.");
    return ok_result((in[0] - in[1]) / in[2]);
}
static solve_result_t solve_t_from_omega_omega0_alpha(const double *in) {
    if (!math_can_divide(in[2])) return error_result("alpha cannot be zero.");
    return ok_result((in[0] - in[1]) / in[2]);
}

static solve_result_t solve_phi_from_omega0_alpha_t(const double *in) {
    return ok_result(in[0] * in[2] + 0.5 * in[1] * in[2] * in[2]);
}
static solve_result_t solve_omega0_from_phi_alpha_t(const double *in) {
    if (!math_can_divide(in[2])) return error_result("t cannot be zero.");
    return ok_result((in[0] - 0.5 * in[1] * in[2] * in[2]) / in[2]);
}
static solve_result_t solve_alpha_from_phi_omega0_t(const double *in) {
    double denominator = in[2] * in[2];
    if (!math_can_divide(denominator)) return error_result("t cannot be zero.");
    return ok_result(2.0 * (in[0] - in[1] * in[2]) / denominator);
}
static solve_result_t solve_t_from_phi_omega0_alpha(const double *in) {
    double alpha = in[2];
    double omega0 = in[1];
    double phi = in[0];
    double discriminant = omega0 * omega0 + 2.0 * alpha * phi;

    if (math_is_near_zero(alpha)) {
        if (!math_can_divide(omega0)) return error_result("No unique t value.");
        return ok_result(phi / omega0);
    }
    if (!math_can_sqrt(discriminant)) return error_result("Invalid square root.");
    {
        double root = sqrt(discriminant);
        double t1 = (-omega0 + root) / alpha;
        double t2 = (-omega0 - root) / alpha;
        double best = math_is_negative(t1) ? t2 : t1;
        if (math_is_negative(best)) return error_result("No non-negative time result.");
        return ok_result(best);
    }
}

static solve_result_t solve_phi_from_n(const double *in) { return ok_result(2.0 * PHYSICS_PI * in[0]); }
static solve_result_t solve_n_from_phi(const double *in) { return ok_result(in[0] / (2.0 * PHYSICS_PI)); }

static solve_result_t solve_force_from_dp_t(const double *in) {
    if (!math_can_divide(in[1])) return error_result("dt cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_dp_from_force_t(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_t_from_force_dp(const double *in) {
    if (!math_can_divide(in[0])) return error_result("F cannot be zero.");
    return ok_result(in[1] / in[0]);
}

static solve_result_t solve_p_from_m_v(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_m_from_p_v(const double *in) {
    if (!math_can_divide(in[1])) return error_result("v cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_v_from_p_m(const double *in) {
    if (!math_can_divide(in[1])) return error_result("m cannot be zero.");
    return ok_result(in[0] / in[1]);
}

static solve_result_t solve_f_from_m_a(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_m_from_f_a(const double *in) {
    if (!math_can_divide(in[1])) return error_result("a cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_a_from_f_m(const double *in) {
    if (!math_can_divide(in[1])) return error_result("m cannot be zero.");
    return ok_result(in[0] / in[1]);
}

static solve_result_t solve_i_from_f_t(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_f_from_i_t(const double *in) {
    if (!math_can_divide(in[1])) return error_result("t cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_t_from_i_f(const double *in) {
    if (!math_can_divide(in[1])) return error_result("F cannot be zero.");
    return ok_result(in[0] / in[1]);
}

static solve_result_t solve_ek_from_m_v(const double *in) { return ok_result(0.5 * in[0] * in[1] * in[1]); }
static solve_result_t solve_m_from_ek_v(const double *in) {
    double denominator = in[1] * in[1];
    if (!math_can_divide(denominator)) return error_result("v cannot be zero.");
    return ok_result((2.0 * in[0]) / denominator);
}
static solve_result_t solve_v_from_ek_m(const double *in) {
    if (!math_can_divide(in[1])) return error_result("m cannot be zero.");
    double radicand = (2.0 * in[0]) / in[1];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}

static solve_result_t solve_w_from_f_d(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_f_from_w_d(const double *in) {
    if (!math_can_divide(in[1])) return error_result("d cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_d_from_w_f(const double *in) {
    if (!math_can_divide(in[1])) return error_result("F cannot be zero.");
    return ok_result(in[0] / in[1]);
}

static solve_result_t solve_pavg_from_w_t(const double *in) {
    if (!math_can_divide(in[1])) return error_result("t cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_w_from_pavg_t(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_t_from_pavg_w(const double *in) {
    if (!math_can_divide(in[0])) return error_result("Pavg cannot be zero.");
    return ok_result(in[1] / in[0]);
}

static solve_result_t solve_power_from_f_v(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_f_from_power_v(const double *in) {
    if (!math_can_divide(in[1])) return error_result("v cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_v_from_power_f(const double *in) {
    if (!math_can_divide(in[1])) return error_result("F cannot be zero.");
    return ok_result(in[0] / in[1]);
}

static solve_result_t solve_power_from_work_t(const double *in) {
    if (!math_can_divide(in[1])) return error_result("dt cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_work_from_power_t(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_t_from_power_work(const double *in) {
    if (!math_can_divide(in[0])) return error_result("P cannot be zero.");
    return ok_result(in[1] / in[0]);
}

static solve_result_t solve_delta_ep_from_m_g_h(const double *in) { return ok_result(in[0] * in[1] * in[2]); }
static solve_result_t solve_m_from_delta_ep_g_h(const double *in) {
    double denominator = in[1] * in[2];
    if (!math_can_divide(denominator)) return error_result("g*h cannot be zero.");
    return ok_result(in[0] / denominator);
}
static solve_result_t solve_g_from_delta_ep_m_h(const double *in) {
    double denominator = in[1] * in[2];
    if (!math_can_divide(denominator)) return error_result("m*h cannot be zero.");
    return ok_result(in[0] / denominator);
}
static solve_result_t solve_h_from_delta_ep_m_g(const double *in) {
    double denominator = in[1] * in[2];
    if (!math_can_divide(denominator)) return error_result("m*g cannot be zero.");
    return ok_result(in[0] / denominator);
}

static solve_result_t solve_e_total_from_ep_ek(const double *in) { return ok_result(in[0] + in[1]); }
static solve_result_t solve_ep_from_e_total_ek(const double *in) { return ok_result(in[0] - in[1]); }
static solve_result_t solve_ek_from_e_total_ep(const double *in) { return ok_result(in[0] - in[1]); }

static solve_result_t solve_w_from_delta_ek(const double *in) { return ok_result(in[0]); }
static solve_result_t solve_delta_ek_from_w(const double *in) { return ok_result(in[0]); }

static solve_result_t solve_f_from_k_x(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_k_from_f_x(const double *in) {
    if (!math_can_divide(in[1])) return error_result("x cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_x_from_f_k(const double *in) {
    if (!math_can_divide(in[1])) return error_result("k cannot be zero.");
    return ok_result(in[0] / in[1]);
}

static const solve_option_t OPT_V_V0_A_T[] = {
    { VAR_V, 3, { VAR_V0, VAR_A, VAR_T }, solve_v_from_v0_a_t },
    { VAR_V0, 3, { VAR_V, VAR_A, VAR_T }, solve_v0_from_v_a_t },
    { VAR_A, 3, { VAR_V, VAR_V0, VAR_T }, solve_a_from_v_v0_t },
    { VAR_T, 3, { VAR_V, VAR_V0, VAR_A }, solve_t_from_v_v0_a }
};

static const solve_option_t OPT_S_S0_V_T[] = {
    { VAR_S, 3, { VAR_S0, VAR_V, VAR_T }, solve_s_from_s0_v_t },
    { VAR_S0, 3, { VAR_S, VAR_V, VAR_T }, solve_s0_from_s_v_t },
    { VAR_V, 3, { VAR_S, VAR_S0, VAR_T }, solve_v_from_s_s0_t },
    { VAR_T, 3, { VAR_S, VAR_S0, VAR_V }, solve_t_from_s_s0_v }
};

static const solve_option_t OPT_S_V0_A_T2[] = {
    { VAR_S, 3, { VAR_V0, VAR_T, VAR_A }, solve_s_from_v0_t_a },
    { VAR_V0, 3, { VAR_S, VAR_T, VAR_A }, solve_v0_from_s_t_a },
    { VAR_A, 3, { VAR_S, VAR_V0, VAR_T }, solve_a_from_s_v0_t },
    { VAR_T, 3, { VAR_S, VAR_V0, VAR_A }, solve_t_from_s_v0_a }
};

static const solve_option_t OPT_V_S_T[] = {
    { VAR_V, 2, { VAR_S, VAR_T }, solve_v_from_s_t },
    { VAR_S, 2, { VAR_V, VAR_T }, solve_s_from_v_t },
    { VAR_T, 2, { VAR_S, VAR_V }, solve_t_from_s_v }
};

static const solve_option_t OPT_A_V_V0_T[] = {
    { VAR_A, 3, { VAR_V, VAR_V0, VAR_T }, solve_a_from_v_v0_dt },
    { VAR_V, 3, { VAR_V0, VAR_A, VAR_T }, solve_v_from_v0_a_t },
    { VAR_V0, 3, { VAR_V, VAR_A, VAR_T }, solve_v0_from_v_a_t },
    { VAR_T, 3, { VAR_V, VAR_V0, VAR_A }, solve_t_from_v_v0_a }
};

static const solve_option_t OPT_V2_V0_A_S[] = {
    { VAR_V, 3, { VAR_V0, VAR_A, VAR_S }, solve_v_from_v0_a_s },
    { VAR_V0, 3, { VAR_V, VAR_A, VAR_S }, solve_v0_from_v_a_s },
    { VAR_A, 3, { VAR_V, VAR_V0, VAR_S }, solve_a_from_v_v0_s },
    { VAR_S, 3, { VAR_V, VAR_V0, VAR_A }, solve_s_from_v_v0_a }
};

static const solve_option_t OPT_V_R_OMEGA[] = {
    { VAR_V, 2, { VAR_R, VAR_OMEGA }, solve_v_from_r_omega },
    { VAR_R, 2, { VAR_V, VAR_OMEGA }, solve_r_from_v_omega },
    { VAR_OMEGA, 2, { VAR_V, VAR_R }, solve_omega_from_v_r }
};

static const solve_option_t OPT_AT_R_ALPHA[] = {
    { VAR_AT, 2, { VAR_R, VAR_ALPHA }, solve_at_from_r_alpha },
    { VAR_R, 2, { VAR_AT, VAR_ALPHA }, solve_r_from_at_alpha },
    { VAR_ALPHA, 2, { VAR_AT, VAR_R }, solve_alpha_from_at_r }
};

static const solve_option_t OPT_AC_V_R[] = {
    { VAR_AC, 2, { VAR_V, VAR_R }, solve_ac_from_v_r },
    { VAR_V, 2, { VAR_AC, VAR_R }, solve_v_from_ac_r },
    { VAR_R, 2, { VAR_AC, VAR_V }, solve_r_from_ac_v }
};

static const solve_option_t OPT_OMEGA_F[] = {
    { VAR_OMEGA, 1, { VAR_FREQ }, solve_omega_from_f },
    { VAR_FREQ, 1, { VAR_OMEGA }, solve_f_from_omega }
};

static const solve_option_t OPT_OMEGA_T[] = {
    { VAR_OMEGA, 1, { VAR_PERIOD }, solve_omega_from_t_period },
    { VAR_PERIOD, 1, { VAR_OMEGA }, solve_period_from_omega }
};

static const solve_option_t OPT_FREQ_PERIOD[] = {
    { VAR_FREQ, 1, { VAR_PERIOD }, solve_freq_from_period },
    { VAR_PERIOD, 1, { VAR_FREQ }, solve_period_from_freq }
};

static const solve_option_t OPT_PHI_OMEGA_T[] = {
    { VAR_PHI, 2, { VAR_OMEGA, VAR_T }, solve_phi_from_omega_t },
    { VAR_OMEGA, 2, { VAR_PHI, VAR_T }, solve_omega_from_phi_t },
    { VAR_T, 2, { VAR_PHI, VAR_OMEGA }, solve_t_from_phi_omega }
};

static const solve_option_t OPT_OMEGA_OMEGA0_ALPHA_T[] = {
    { VAR_OMEGA, 3, { VAR_OMEGA0, VAR_ALPHA, VAR_T }, solve_omega_from_omega0_alpha_t },
    { VAR_OMEGA0, 3, { VAR_OMEGA, VAR_ALPHA, VAR_T }, solve_omega0_from_omega_alpha_t },
    { VAR_ALPHA, 3, { VAR_OMEGA, VAR_OMEGA0, VAR_T }, solve_alpha_from_omega_omega0_t },
    { VAR_T, 3, { VAR_OMEGA, VAR_OMEGA0, VAR_ALPHA }, solve_t_from_omega_omega0_alpha }
};

static const solve_option_t OPT_PHI_OMEGA0_ALPHA_T[] = {
    { VAR_PHI, 3, { VAR_OMEGA0, VAR_ALPHA, VAR_T }, solve_phi_from_omega0_alpha_t },
    { VAR_OMEGA0, 3, { VAR_PHI, VAR_ALPHA, VAR_T }, solve_omega0_from_phi_alpha_t },
    { VAR_ALPHA, 3, { VAR_PHI, VAR_OMEGA0, VAR_T }, solve_alpha_from_phi_omega0_t },
    { VAR_T, 3, { VAR_PHI, VAR_OMEGA0, VAR_ALPHA }, solve_t_from_phi_omega0_alpha }
};

static const solve_option_t OPT_PHI_N[] = {
    { VAR_PHI, 1, { VAR_N }, solve_phi_from_n },
    { VAR_N, 1, { VAR_PHI }, solve_n_from_phi }
};

static const solve_option_t OPT_FORCE_DP_T[] = {
    { VAR_F, 2, { VAR_DP, VAR_T }, solve_force_from_dp_t },
    { VAR_DP, 2, { VAR_F, VAR_T }, solve_dp_from_force_t },
    { VAR_T, 2, { VAR_F, VAR_DP }, solve_t_from_force_dp }
};

static const solve_option_t OPT_P_M_V[] = {
    { VAR_P, 2, { VAR_M, VAR_V }, solve_p_from_m_v },
    { VAR_M, 2, { VAR_P, VAR_V }, solve_m_from_p_v },
    { VAR_V, 2, { VAR_P, VAR_M }, solve_v_from_p_m }
};

static const solve_option_t OPT_F_M_A[] = {
    { VAR_F, 2, { VAR_M, VAR_A }, solve_f_from_m_a },
    { VAR_M, 2, { VAR_F, VAR_A }, solve_m_from_f_a },
    { VAR_A, 2, { VAR_F, VAR_M }, solve_a_from_f_m }
};

static const solve_option_t OPT_I_F_T[] = {
    { VAR_I, 2, { VAR_F, VAR_T }, solve_i_from_f_t },
    { VAR_F, 2, { VAR_I, VAR_T }, solve_f_from_i_t },
    { VAR_T, 2, { VAR_I, VAR_F }, solve_t_from_i_f }
};

static const solve_option_t OPT_EK_M_V[] = {
    { VAR_EK, 2, { VAR_M, VAR_V }, solve_ek_from_m_v },
    { VAR_M, 2, { VAR_EK, VAR_V }, solve_m_from_ek_v },
    { VAR_V, 2, { VAR_EK, VAR_M }, solve_v_from_ek_m }
};

static const solve_option_t OPT_W_F_D[] = {
    { VAR_W, 2, { VAR_F, VAR_D }, solve_w_from_f_d },
    { VAR_F, 2, { VAR_W, VAR_D }, solve_f_from_w_d },
    { VAR_D, 2, { VAR_W, VAR_F }, solve_d_from_w_f }
};

static const solve_option_t OPT_PAVG_W_T[] = {
    { VAR_PAVG, 2, { VAR_W, VAR_T }, solve_pavg_from_w_t },
    { VAR_W, 2, { VAR_PAVG, VAR_T }, solve_w_from_pavg_t },
    { VAR_T, 2, { VAR_PAVG, VAR_W }, solve_t_from_pavg_w }
};

static const solve_option_t OPT_POWER_F_V[] = {
    { VAR_POWER, 2, { VAR_F, VAR_V }, solve_power_from_f_v },
    { VAR_F, 2, { VAR_POWER, VAR_V }, solve_f_from_power_v },
    { VAR_V, 2, { VAR_POWER, VAR_F }, solve_v_from_power_f }
};

static const solve_option_t OPT_POWER_W_T[] = {
    { VAR_POWER, 2, { VAR_W, VAR_T }, solve_power_from_work_t },
    { VAR_W, 2, { VAR_POWER, VAR_T }, solve_work_from_power_t },
    { VAR_T, 2, { VAR_POWER, VAR_W }, solve_t_from_power_work }
};

static const solve_option_t OPT_DELTA_EP_M_G_H[] = {
    { VAR_EP_DELTA, 3, { VAR_M, VAR_G, VAR_H }, solve_delta_ep_from_m_g_h },
    { VAR_M, 3, { VAR_EP_DELTA, VAR_G, VAR_H }, solve_m_from_delta_ep_g_h },
    { VAR_G, 3, { VAR_EP_DELTA, VAR_M, VAR_H }, solve_g_from_delta_ep_m_h },
    { VAR_H, 3, { VAR_EP_DELTA, VAR_M, VAR_G }, solve_h_from_delta_ep_m_g }
};

static const solve_option_t OPT_E_TOTAL_EP_EK[] = {
    { VAR_E_TOTAL, 2, { VAR_EP, VAR_EK }, solve_e_total_from_ep_ek },
    { VAR_EP, 2, { VAR_E_TOTAL, VAR_EK }, solve_ep_from_e_total_ek },
    { VAR_EK, 2, { VAR_E_TOTAL, VAR_EP }, solve_ek_from_e_total_ep }
};

static const solve_option_t OPT_W_DELTA_EK[] = {
    { VAR_W, 1, { VAR_DELTA_EK }, solve_w_from_delta_ek },
    { VAR_DELTA_EK, 1, { VAR_W }, solve_delta_ek_from_w }
};

static const solve_option_t OPT_F_K_X[] = {
    { VAR_F, 2, { VAR_K, VAR_D }, solve_f_from_k_x },
    { VAR_K, 2, { VAR_F, VAR_D }, solve_k_from_f_x },
    { VAR_D, 2, { VAR_F, VAR_K }, solve_x_from_f_k }
};

static const formula_def_t FORMULA_KIN_V = {
    "Velocity after time",
    "v = v0 + a*t",
    "Condition: constant a",
    ARRAY_LEN(OPT_V_V0_A_T),
    OPT_V_V0_A_T
};

static const formula_def_t FORMULA_KIN_VAVG = {
    "Average speed",
    "v_avg = s / t",
    "Use path or displacement",
    ARRAY_LEN(OPT_V_S_T),
    OPT_V_S_T
};

static const formula_def_t FORMULA_KIN_A = {
    "Acceleration from velocity change",
    "a = (v - v0) / t",
    "Straight-line motion",
    ARRAY_LEN(OPT_A_V_V0_T),
    OPT_A_V_V0_T
};

static const formula_def_t FORMULA_KIN_S = {
    "Position at constant velocity",
    "s = s0 + v*t",
    "Condition: constant v",
    ARRAY_LEN(OPT_S_S0_V_T),
    OPT_S_S0_V_T
};

static const formula_def_t FORMULA_KIN_S2 = {
    "Position with acceleration",
    "s = v0*t + 0.5*a*t^2",
    "Condition: constant a",
    ARRAY_LEN(OPT_S_V0_A_T2),
    OPT_S_V0_A_T2
};

static const formula_def_t FORMULA_CIRC_V = {
    "Tangential speed",
    "v = r*omega",
    "Condition: constant r",
    ARRAY_LEN(OPT_V_R_OMEGA),
    OPT_V_R_OMEGA
};

static const formula_def_t FORMULA_CIRC_PHI = {
    "Angular displacement",
    "phi = omega*t",
    "Condition: constant omega",
    ARRAY_LEN(OPT_PHI_OMEGA_T),
    OPT_PHI_OMEGA_T
};

static const formula_def_t FORMULA_CIRC_AT = {
    "Tangential acceleration",
    "a_t = r*alpha",
    "Condition: constant r",
    ARRAY_LEN(OPT_AT_R_ALPHA),
    OPT_AT_R_ALPHA
};

static const formula_def_t FORMULA_CIRC_AC = {
    "Centripetal acceleration",
    "a_c = v^2 / r",
    "Condition: constant r",
    ARRAY_LEN(OPT_AC_V_R),
    OPT_AC_V_R
};

static const formula_def_t FORMULA_CIRC_OMEGA_F = {
    "Angular speed from frequency",
    "omega = 2*pi*f",
    "No extra condition",
    ARRAY_LEN(OPT_OMEGA_F),
    OPT_OMEGA_F
};

static const formula_def_t FORMULA_CIRC_OMEGA_T = {
    "Angular speed from period",
    "omega = 2*pi / T",
    "No extra condition",
    ARRAY_LEN(OPT_OMEGA_T),
    OPT_OMEGA_T
};

static const formula_def_t FORMULA_CIRC_FREQ = {
    "Frequency and period",
    "f = 1 / T",
    "No extra condition",
    ARRAY_LEN(OPT_FREQ_PERIOD),
    OPT_FREQ_PERIOD
};

static const formula_def_t FORMULA_CIRC_OMEGA_ALPHA = {
    "Angular velocity change",
    "omega = omega0 + alpha*t",
    "Condition: constant alpha",
    ARRAY_LEN(OPT_OMEGA_OMEGA0_ALPHA_T),
    OPT_OMEGA_OMEGA0_ALPHA_T
};

static const formula_def_t FORMULA_CIRC_PHI_ALPHA = {
    "Angular displacement with alpha",
    "phi = omega0*t + 0.5*alpha*t^2",
    "Condition: constant alpha",
    ARRAY_LEN(OPT_PHI_OMEGA0_ALPHA_T),
    OPT_PHI_OMEGA0_ALPHA_T
};

static const formula_def_t FORMULA_CIRC_N = {
    "Revolutions",
    "phi = 2*pi*N",
    "No extra condition",
    ARRAY_LEN(OPT_PHI_N),
    OPT_PHI_N
};

static const formula_def_t FORMULA_DYN_P = {
    "Momentum",
    "p = m*v",
    "Condition: constant m",
    ARRAY_LEN(OPT_P_M_V),
    OPT_P_M_V
};

static const formula_def_t FORMULA_DYN_F = {
    "Newton's second law",
    "F = m*a",
    "Condition: constant m",
    ARRAY_LEN(OPT_F_M_A),
    OPT_F_M_A
};

static const formula_def_t FORMULA_DYN_A = {
    "Acceleration from force",
    "a = F / m",
    "Condition: constant m",
    ARRAY_LEN(OPT_F_M_A),
    OPT_F_M_A
};

static const formula_def_t FORMULA_DYN_I = {
    "Impulse",
    "I = F*t",
    "Condition: constant F",
    ARRAY_LEN(OPT_I_F_T),
    OPT_I_F_T
};

static const formula_def_t FORMULA_EN_EK = {
    "Kinetic energy",
    "Ek = 0.5*m*v^2",
    "Condition: constant m",
    ARRAY_LEN(OPT_EK_M_V),
    OPT_EK_M_V
};

static const formula_def_t FORMULA_EN_W = {
    "Work",
    "W = F*d",
    "Constant F, F parallel d",
    ARRAY_LEN(OPT_W_F_D),
    OPT_W_F_D
};

static const formula_def_t FORMULA_EN_PAVG = {
    "Average power",
    "P_avg = W / t",
    "No extra condition",
    ARRAY_LEN(OPT_PAVG_W_T),
    OPT_PAVG_W_T
};

static const formula_def_t FORMULA_EN_P = {
    "Instantaneous power",
    "P = F*v",
    "F parallel v",
    ARRAY_LEN(OPT_POWER_F_V),
    OPT_POWER_F_V
};

static const formula_def_t FORMULA_EN_EP = {
    "Potential energy change",
    "delta Ep = m*g*h",
    "Condition: constant m and g",
    ARRAY_LEN(OPT_DELTA_EP_M_G_H),
    OPT_DELTA_EP_M_G_H
};

static const formula_def_t FORMULA_EN_E = {
    "Mechanical energy",
    "E = Ep + Ek",
    "No extra condition",
    ARRAY_LEN(OPT_E_TOTAL_EP_EK),
    OPT_E_TOTAL_EP_EK
};

static const formula_def_t FORMULA_EN_W_DEK = {
    "Work-energy theorem",
    "W = delta Ek",
    "No extra condition",
    ARRAY_LEN(OPT_W_DELTA_EK),
    OPT_W_DELTA_EK
};

static const formula_def_t FORMULA_EN_SPRING = {
    "Hooke's law",
    "F = k*d",
    "Condition: constant k",
    ARRAY_LEN(OPT_F_K_X),
    OPT_F_K_X
};

static const formula_def_t FORMULA_KIN_V2 = {
    "Velocity-displacement relation",
    "v^2 = v0^2 + 2*a*s",
    "Condition: constant a",
    ARRAY_LEN(OPT_V2_V0_A_S),
    OPT_V2_V0_A_S
};

static const formula_def_t FORMULA_CALC_V = {
    "Find velocity from dx and dt",
    "v = dx / dt",
    "Use change in position over time",
    ARRAY_LEN(OPT_V_S_T),
    OPT_V_S_T
};

static const formula_def_t FORMULA_CALC_A = {
    "Find acceleration from dv and dt",
    "a = dv / dt",
    "Use change in velocity over time",
    ARRAY_LEN(OPT_A_V_V0_T),
    OPT_A_V_V0_T
};

static const formula_def_t FORMULA_CALC_OMEGA = {
    "Find angular speed from dphi and dt",
    "omega = dphi / dt",
    "Use change in angle over time",
    ARRAY_LEN(OPT_PHI_OMEGA_T),
    OPT_PHI_OMEGA_T
};

static const formula_def_t FORMULA_CALC_ALPHA = {
    "Find angular accel from domega,dt",
    "alpha = domega / dt",
    "Use change in omega over time",
    ARRAY_LEN(OPT_OMEGA_OMEGA0_ALPHA_T),
    OPT_OMEGA_OMEGA0_ALPHA_T
};

static const formula_def_t FORMULA_CALC_POWER = {
    "Find power from dW and dt",
    "P = dW / dt",
    "Use work change over time",
    ARRAY_LEN(OPT_POWER_W_T),
    OPT_POWER_W_T
};

static const formula_def_t FORMULA_CALC_FORCE = {
    "Find force from dp and dt",
    "F = dp / dt",
    "Use momentum change over time",
    ARRAY_LEN(OPT_FORCE_DP_T),
    OPT_FORCE_DP_T
};

static const formula_def_t FORMULA_CALC_DISP = {
    "Find dx from velocity and dt",
    "dx = v * dt",
    "Constant v over time interval",
    ARRAY_LEN(OPT_V_S_T),
    OPT_V_S_T
};

static const formula_def_t FORMULA_CALC_DV = {
    "Find dv from acceleration, dt",
    "dv = a * dt",
    "Constant a over time interval",
    ARRAY_LEN(OPT_A_V_V0_T),
    OPT_A_V_V0_T
};

static const formula_def_t FORMULA_CALC_DPHI = {
    "Find dphi from omega, dt",
    "dphi = omega * dt",
    "Constant omega over time",
    ARRAY_LEN(OPT_PHI_OMEGA_T),
    OPT_PHI_OMEGA_T
};

static const formula_def_t FORMULA_CALC_DOMEGA = {
    "Find domega from alpha, dt",
    "domega = alpha * dt",
    "Constant alpha over time",
    ARRAY_LEN(OPT_OMEGA_OMEGA0_ALPHA_T),
    OPT_OMEGA_OMEGA0_ALPHA_T
};

static const formula_def_t FORMULA_CALC_I = {
    "Find impulse from force, dt",
    "I = integral(F dt)",
    "F constant over dt",
    ARRAY_LEN(OPT_I_F_T),
    OPT_I_F_T
};

static const formula_def_t FORMULA_CALC_W = {
    "Find work from force and dr",
    "W = integral(F dr)",
    "F constant and parallel",
    ARRAY_LEN(OPT_W_F_D),
    OPT_W_F_D
};

static const formula_def_t FORMULA_CALC_EXPR = {
    "Derivative / integral of expression",
    "Enter polynomial in x or t",
    "Examples: 10t, 3t^2+2, x^3-x",
    0,
    NULL,
    expr_calc_run
};

static category_def_t CATEGORIES[CATEGORY_COUNT];

void formula_registry_init(void) {
    CATEGORIES[0].name = "1. Kinematics";
    CATEGORIES[0].formula_count = 6;
    CATEGORIES[0].formulas[0] = &FORMULA_KIN_VAVG;
    CATEGORIES[0].formulas[1] = &FORMULA_KIN_A;
    CATEGORIES[0].formulas[2] = &FORMULA_KIN_V;
    CATEGORIES[0].formulas[3] = &FORMULA_KIN_S;
    CATEGORIES[0].formulas[4] = &FORMULA_KIN_S2;
    CATEGORIES[0].formulas[5] = &FORMULA_KIN_V2;

    CATEGORIES[1].name = "2. Circular Motion";
    CATEGORIES[1].formula_count = 10;
    CATEGORIES[1].formulas[0] = &FORMULA_CIRC_PHI;
    CATEGORIES[1].formulas[1] = &FORMULA_CIRC_V;
    CATEGORIES[1].formulas[2] = &FORMULA_CIRC_AT;
    CATEGORIES[1].formulas[3] = &FORMULA_CIRC_AC;
    CATEGORIES[1].formulas[4] = &FORMULA_CIRC_FREQ;
    CATEGORIES[1].formulas[5] = &FORMULA_CIRC_OMEGA_F;
    CATEGORIES[1].formulas[6] = &FORMULA_CIRC_OMEGA_T;
    CATEGORIES[1].formulas[7] = &FORMULA_CIRC_OMEGA_ALPHA;
    CATEGORIES[1].formulas[8] = &FORMULA_CIRC_PHI_ALPHA;
    CATEGORIES[1].formulas[9] = &FORMULA_CIRC_N;

    CATEGORIES[2].name = "3. Dynamics";
    CATEGORIES[2].formula_count = 4;
    CATEGORIES[2].formulas[0] = &FORMULA_DYN_P;
    CATEGORIES[2].formulas[1] = &FORMULA_DYN_F;
    CATEGORIES[2].formulas[2] = &FORMULA_DYN_A;
    CATEGORIES[2].formulas[3] = &FORMULA_DYN_I;

    CATEGORIES[3].name = "4. Work / Energy / Power";
    CATEGORIES[3].formula_count = 8;
    CATEGORIES[3].formulas[0] = &FORMULA_EN_EK;
    CATEGORIES[3].formulas[1] = &FORMULA_EN_W;
    CATEGORIES[3].formulas[2] = &FORMULA_EN_PAVG;
    CATEGORIES[3].formulas[3] = &FORMULA_EN_P;
    CATEGORIES[3].formulas[4] = &FORMULA_EN_EP;
    CATEGORIES[3].formulas[5] = &FORMULA_EN_E;
    CATEGORIES[3].formulas[6] = &FORMULA_EN_W_DEK;
    CATEGORIES[3].formulas[7] = &FORMULA_EN_SPRING;

    CATEGORIES[4].name = "5. Derivatives / Integrals";
    CATEGORIES[4].formula_count = 13;
    CATEGORIES[4].formulas[0] = &FORMULA_CALC_EXPR;
    CATEGORIES[4].formulas[1] = &FORMULA_CALC_V;
    CATEGORIES[4].formulas[2] = &FORMULA_CALC_A;
    CATEGORIES[4].formulas[3] = &FORMULA_CALC_FORCE;
    CATEGORIES[4].formulas[4] = &FORMULA_CALC_OMEGA;
    CATEGORIES[4].formulas[5] = &FORMULA_CALC_ALPHA;
    CATEGORIES[4].formulas[6] = &FORMULA_CALC_POWER;
    CATEGORIES[4].formulas[7] = &FORMULA_CALC_DISP;
    CATEGORIES[4].formulas[8] = &FORMULA_CALC_DV;
    CATEGORIES[4].formulas[9] = &FORMULA_CALC_DPHI;
    CATEGORIES[4].formulas[10] = &FORMULA_CALC_DOMEGA;
    CATEGORIES[4].formulas[11] = &FORMULA_CALC_I;
    CATEGORIES[4].formulas[12] = &FORMULA_CALC_W;
}

const category_def_t *formula_get_categories(uint8_t *count_out) {
    *count_out = CATEGORY_COUNT;
    return CATEGORIES;
}

const char *formula_variable_name(variable_id_t id) {
    static const char *names[VAR_COUNT] = {
        "Velocity v", "Initial velocity v0", "Acceleration a", "Time t",
        "Displacement s", "Initial position s0", "Radius r", "Angular speed omega",
        "Angular acceleration alpha", "Tangential accel a_t", "Centripetal accel a_c",
        "Force F", "Period T", "Frequency f", "Angle phi", "Initial ang. speed omega0",
        "Revolutions N", "Change in momentum dp", "Momentum p", "Mass m", "Impulse I", "Kinetic energy Ek",
        "Work W", "Distance d", "Average power P_avg", "Power P",
        "Potential-energy change dEp", "Gravity g", "Height h", "Mechanical energy E",
        "Potential energy Ep", "Change in kinetic energy dEk", "Spring constant k"
    };
    return names[id];
}

const char *formula_variable_prompt(variable_id_t id) {
    static const char *prompts[VAR_COUNT] = {
        "velocity v", "initial velocity v0", "acceleration a", "time t",
        "displacement s", "initial position s0", "radius r", "angular speed omega",
        "angular acceleration alpha", "tangential acceleration a_t",
        "centripetal acceleration a_c", "force F", "period T", "frequency f",
        "angle phi", "initial angular speed omega0", "number of revolutions N",
        "change in momentum dp", "momentum p", "mass m", "impulse I", "kinetic energy Ek", "work W",
        "distance d", "average power P_avg", "power P", "potential-energy change dEp",
        "gravity g", "height h", "mechanical energy E", "potential energy Ep",
        "change in kinetic energy dEk", "spring constant k"
    };
    return prompts[id];
}

const char *formula_variable_unit(variable_id_t id) {
    static const char *units[VAR_COUNT] = {
        "m/s", "m/s", "m/s^2", "s",
        "m", "m", "m", "rad/s",
        "rad/s^2", "m/s^2", "m/s^2",
        "N", "s", "Hz", "rad", "rad/s",
        "rev", "kg*m/s", "kg*m/s", "kg", "N*s", "J",
        "J", "m", "W", "W",
        "J", "m/s^2", "m", "J",
        "J", "J", "N/m"
    };
    return units[id];
}

static menu_result_t select_solve_option(const formula_def_t *formula, uint8_t *selected_index, const solve_option_t **option_out) {
    const char *labels[8];
    uint8_t i = 0;
    uint8_t scroll_offset = 0;
    const uint8_t visible_rows = 5;

    for (i = 0; i < formula->option_count; ++i) {
        labels[i] = formula_variable_name(formula->options[i].target);
    }

    while (true) {
        uint8_t j = 0;
        uint8_t action = 0;

        if (*selected_index < scroll_offset) {
            scroll_offset = *selected_index;
        }
        if (*selected_index >= scroll_offset + visible_rows) {
            scroll_offset = (uint8_t)(*selected_index - visible_rows + 1);
        }

        io_clear_screen();
        io_draw_title(formula->expression);
        io_draw_wrapped_text(1, formula->name, 26);
        io_draw_wrapped_text(2, formula->const_note, 26);

        for (j = 0; j < visible_rows && (uint8_t)(scroll_offset + j) < formula->option_count; ++j) {
            char line[28];
            uint8_t option_index = (uint8_t)(scroll_offset + j);

            if (option_index == *selected_index) {
                snprintf(line, sizeof(line), "> Find %s", labels[option_index]);
            } else {
                snprintf(line, sizeof(line), "  Find %s", labels[option_index]);
            }
            io_draw_wrapped_text((uint8_t)(4 + j), line, 26);
        }

        if (formula->option_count > visible_rows) {
            io_draw_footer("UP/DN scroll OK select");
        } else {
            io_draw_footer("UP/DN OK MODE BACK");
        }
        action = io_read_menu_key();
        if (action == IO_MENU_UP && *selected_index > 0) {
            --(*selected_index);
        } else if (action == IO_MENU_DOWN && *selected_index + 1 < formula->option_count) {
            ++(*selected_index);
        } else if (action == IO_MENU_SELECT) {
            *option_out = &formula->options[*selected_index];
            return MENU_STAY;
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

formula_run_result_t formula_run(const formula_def_t *formula) {
    uint8_t selected_option = 0;

    if (formula->custom_run != NULL) {
        return formula->custom_run(formula);
    }

    while (true) {
        const solve_option_t *option = NULL;
        double inputs[MAX_FORMULA_INPUTS] = {0};
        uint8_t i = 0;
        menu_result_t selection = select_solve_option(formula, &selected_option, &option);

        if (selection == MENU_BACK) {
            return FORMULA_RUN_BACK;
        }
        if (selection == MENU_EXIT_APP) {
            return FORMULA_RUN_EXIT_APP;
        }

        for (i = 0; i < option->input_count; ++i) {
            char prompt[64];
            const char *var_name = formula_variable_prompt(option->inputs[i]);
            const char *unit = formula_variable_unit(option->inputs[i]);

            prompt[0] = '\0';
            snprintf(prompt, sizeof(prompt), "Enter %s [%s]:", var_name, unit);
            if (!io_prompt_double(formula->expression, prompt, &inputs[i])) {
                break;
            }
        }

        if (i != option->input_count) {
            continue;
        }

        solve_result_t result = option->solve(inputs);
        if (!result.ok) {
            io_show_message("Solver Error", result.error, "Check your input values.");
            continue;
        }

        {
            char target_line[64];
            snprintf(target_line, sizeof(target_line), "%s [%s]", formula_variable_name(option->target), formula_variable_unit(option->target));
            io_show_result(formula->expression, target_line, result.value, formula->const_note);
        }
    }
}
