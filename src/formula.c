#include <math.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

#include <ti/getcsc.h>

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

static solve_result_t solve_vavg_from_x2_x1_t2_t1(const double *in) {
    double dt = in[2] - in[3];
    if (!math_can_divide(dt)) return error_result("t2-t1 cannot be zero.");
    return ok_result((in[0] - in[1]) / dt);
}
static solve_result_t solve_x2_from_vavg_x1_t2_t1(const double *in) {
    return ok_result(in[1] + in[0] * (in[2] - in[3]));
}
static solve_result_t solve_x1_from_vavg_x2_t2_t1(const double *in) {
    return ok_result(in[1] - in[0] * (in[2] - in[3]));
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

static solve_result_t solve_phi_deg_from_dot_vmag_amag(const double *in) {
    double denominator = in[1] * in[2];
    double c;
    if (!math_can_divide(denominator)) return error_result("|v| or |a| is zero.");
    c = in[0] / denominator;
    if (c < -1.0 || c > 1.0) return error_result("cos(phi) outside [-1,1].");
    return ok_result(acos(c) * 180.0 / PHYSICS_PI);
}
static solve_result_t solve_dot_from_phi_deg_vmag_amag(const double *in) {
    double phi_rad = in[0] * PHYSICS_PI / 180.0;
    return ok_result(in[1] * in[2] * cos(phi_rad));
}

static solve_result_t solve_d_from_xyz(const double *in) {
    return ok_result(sqrt(in[0] * in[0] + in[1] * in[1] + in[2] * in[2]));
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
static solve_result_t solve_ac_from_r_omega(const double *in) {
    return ok_result(in[0] * in[1] * in[1]);
}
static solve_result_t solve_r_from_ac_omega(const double *in) {
    double denominator = in[1] * in[1];
    if (!math_can_divide(denominator)) return error_result("omega cannot be zero.");
    return ok_result(in[0] / denominator);
}
static solve_result_t solve_omega_from_ac_r(const double *in) {
    double radicand;
    if (!math_can_divide(in[1])) return error_result("r cannot be zero.");
    radicand = in[0] / in[1];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_a_total_from_at_ac(const double *in) {
    return ok_result(sqrt(in[0] * in[0] + in[1] * in[1]));
}
static solve_result_t solve_at_from_a_total_ac(const double *in) {
    double radicand = in[0] * in[0] - in[1] * in[1];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_ac_from_a_total_at(const double *in) {
    double radicand = in[0] * in[0] - in[1] * in[1];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
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

static solve_result_t solve_n_from_omega0_alpha_t(const double *in) {
    double phi = in[0] * in[2] + 0.5 * in[1] * in[2] * in[2];
    return ok_result(phi / (2.0 * PHYSICS_PI));
}
static solve_result_t solve_omega0_from_n_alpha_t(const double *in) {
    if (!math_can_divide(in[2])) return error_result("t cannot be zero.");
    return ok_result(((2.0 * PHYSICS_PI * in[0]) - 0.5 * in[1] * in[2] * in[2]) / in[2]);
}
static solve_result_t solve_alpha_from_n_omega0_t(const double *in) {
    double denominator = in[2] * in[2];
    if (!math_can_divide(denominator)) return error_result("t cannot be zero.");
    return ok_result(2.0 * ((2.0 * PHYSICS_PI * in[0]) - in[1] * in[2]) / denominator);
}
static solve_result_t solve_t_from_n_omega0_alpha(const double *in) {
    double phi = 2.0 * PHYSICS_PI * in[0];
    return solve_t_from_phi_omega0_alpha((double[]){phi, in[1], in[2]});
}

static solve_result_t solve_force_from_dp_t(const double *in) {
    if (!math_can_divide(in[1])) return error_result("dt cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_dp_from_force_t(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_t_from_force_dp(const double *in) {
    if (!math_can_divide(in[0])) return error_result("F cannot be zero.");
    return ok_result(in[1] / in[0]);
}
static solve_result_t solve_f_from_k_time(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_k_from_f_time(const double *in) {
    if (!math_can_divide(in[1])) return error_result("t cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_t_from_f_k_time(const double *in) {
    if (!math_can_divide(in[1])) return error_result("k cannot be zero.");
    return ok_result(in[0] / in[1]);
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

static solve_result_t solve_weight_from_m_g(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_m_from_weight_g(const double *in) {
    if (!math_can_divide(in[1])) return error_result("g cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_g_from_weight_m(const double *in) {
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

static solve_result_t solve_s_stop_from_f_m_v0(const double *in) {
    if (!math_can_divide(in[0])) return error_result("F cannot be zero.");
    return ok_result((0.5 * in[1] * in[2] * in[2]) / in[0]);
}
static solve_result_t solve_f_stop_from_s_m_v0(const double *in) {
    if (!math_can_divide(in[0])) return error_result("s cannot be zero.");
    return ok_result((0.5 * in[1] * in[2] * in[2]) / in[0]);
}
static solve_result_t solve_m_stop_from_f_s_v0(const double *in) {
    double denominator = in[2] * in[2];
    if (!math_can_divide(denominator)) return error_result("v0 cannot be zero.");
    return ok_result((2.0 * in[0] * in[1]) / denominator);
}
static solve_result_t solve_v0_stop_from_f_s_m(const double *in) {
    double radicand;
    if (!math_can_divide(in[2])) return error_result("m cannot be zero.");
    radicand = (2.0 * in[0] * in[1]) / in[2];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}

static solve_result_t solve_h_from_v0_g_energy_ratio(const double *in) {
    double denominator = 3.0 * in[1];
    if (!math_can_divide(denominator)) return error_result("g cannot be zero.");
    return ok_result((in[0] * in[0]) / denominator);
}
static solve_result_t solve_v0_from_h_g_energy_ratio(const double *in) {
    double radicand = 3.0 * in[1] * in[0];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_g_from_v0_h_energy_ratio(const double *in) {
    double denominator = 3.0 * in[1];
    if (!math_can_divide(denominator)) return error_result("h cannot be zero.");
    return ok_result((in[0] * in[0]) / denominator);
}

static solve_result_t solve_v_from_vertical_height(const double *in) {
    double radicand = in[0] * in[0] - 2.0 * in[1] * in[2];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_v0_from_vertical_height(const double *in) {
    double radicand = in[0] * in[0] + 2.0 * in[1] * in[2];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_h_from_vertical_speeds(const double *in) {
    double denominator = 2.0 * in[2];
    if (!math_can_divide(denominator)) return error_result("g cannot be zero.");
    return ok_result((in[0] * in[0] - in[1] * in[1]) / denominator);
}
static solve_result_t solve_g_from_vertical_speeds_h(const double *in) {
    double denominator = 2.0 * in[2];
    if (!math_can_divide(denominator)) return error_result("h cannot be zero.");
    return ok_result((in[0] * in[0] - in[1] * in[1]) / denominator);
}

static solve_result_t solve_hmax_from_v0_g(const double *in) {
    double denominator = 2.0 * in[1];
    if (!math_can_divide(denominator)) return error_result("g cannot be zero.");
    return ok_result((in[0] * in[0]) / denominator);
}
static solve_result_t solve_v0_from_hmax_g(const double *in) {
    double radicand = 2.0 * in[1] * in[0];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_g_from_v0_hmax(const double *in) {
    double denominator = 2.0 * in[1];
    if (!math_can_divide(denominator)) return error_result("h cannot be zero.");
    return ok_result((in[0] * in[0]) / denominator);
}

static solve_result_t solve_hmax_from_h0_v0_g(const double *in) {
    double denominator = 2.0 * in[2];
    if (!math_can_divide(denominator)) return error_result("g cannot be zero.");
    return ok_result(in[0] + (in[1] * in[1]) / denominator);
}
static solve_result_t solve_h0_from_hmax_v0_g(const double *in) {
    double denominator = 2.0 * in[2];
    if (!math_can_divide(denominator)) return error_result("g cannot be zero.");
    return ok_result(in[0] - (in[1] * in[1]) / denominator);
}
static solve_result_t solve_v0_from_hmax_h0_g(const double *in) {
    double radicand = 2.0 * in[2] * (in[0] - in[1]);
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_g_from_hmax_h0_v0(const double *in) {
    double denominator = 2.0 * (in[0] - in[1]);
    if (!math_can_divide(denominator)) return error_result("hmax-h0 cannot be zero.");
    return ok_result((in[2] * in[2]) / denominator);
}

static solve_result_t solve_v_from_energy_two_heights(const double *in) {
    double radicand = in[0] * in[0] + 2.0 * in[1] * (in[2] - in[3]);
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_v0_from_energy_two_heights(const double *in) {
    double radicand = in[0] * in[0] + 2.0 * in[1] * (in[3] - in[2]);
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_h2_from_energy_two_heights(const double *in) {
    if (!math_can_divide(in[2])) return error_result("g cannot be zero.");
    return ok_result(in[3] + (in[0] * in[0] - in[1] * in[1]) / (2.0 * in[2]));
}
static solve_result_t solve_h1_from_energy_two_heights(const double *in) {
    if (!math_can_divide(in[2])) return error_result("g cannot be zero.");
    return ok_result(in[3] + (in[1] * in[1] - in[0] * in[0]) / (2.0 * in[2]));
}

static solve_result_t solve_h_from_ek_eq_ep(const double *in) {
    double denominator = 2.0 * in[1];
    if (!math_can_divide(denominator)) return error_result("g cannot be zero.");
    return ok_result((in[0] * in[0]) / denominator);
}
static solve_result_t solve_v_from_ek_eq_ep(const double *in) {
    double radicand = 2.0 * in[1] * in[0];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_g_from_ek_eq_ep(const double *in) {
    double denominator = 2.0 * in[1];
    if (!math_can_divide(denominator)) return error_result("h cannot be zero.");
    return ok_result((in[0] * in[0]) / denominator);
}

static solve_result_t solve_h_from_ek_third_ep(const double *in) {
    double denominator = 8.0 * in[1];
    if (!math_can_divide(denominator)) return error_result("g cannot be zero.");
    return ok_result((3.0 * in[0] * in[0]) / denominator);
}
static solve_result_t solve_v0_from_h_g_ek_third_ep(const double *in) {
    double radicand = (8.0 * in[1] * in[0]) / 3.0;
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}
static solve_result_t solve_g_from_v0_h_ek_third_ep(const double *in) {
    double denominator = 8.0 * in[1];
    if (!math_can_divide(denominator)) return error_result("h cannot be zero.");
    return ok_result((3.0 * in[0] * in[0]) / denominator);
}

static solve_result_t solve_i0_cylinder_from_m_r(const double *in) { return ok_result(0.5 * in[0] * in[1] * in[1]); }
static solve_result_t solve_m_from_i0_cylinder_r(const double *in) {
    double denominator = 0.5 * in[1] * in[1];
    if (!math_can_divide(denominator)) return error_result("r cannot be zero.");
    return ok_result(in[0] / denominator);
}
static solve_result_t solve_r_from_i0_cylinder_m(const double *in) {
    double radicand;
    if (!math_can_divide(in[1])) return error_result("m cannot be zero.");
    radicand = (2.0 * in[0]) / in[1];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}

static solve_result_t solve_irot_from_i0_m_axis_a(const double *in) { return ok_result(in[0] + in[1] * in[2] * in[2]); }
static solve_result_t solve_i0_from_irot_m_axis_a(const double *in) { return ok_result(in[0] - in[1] * in[2] * in[2]); }
static solve_result_t solve_m_from_irot_i0_axis_a(const double *in) {
    double denominator = in[2] * in[2];
    if (!math_can_divide(denominator)) return error_result("a cannot be zero.");
    return ok_result((in[0] - in[1]) / denominator);
}
static solve_result_t solve_axis_a_from_irot_i0_m(const double *in) {
    double radicand;
    if (!math_can_divide(in[2])) return error_result("m cannot be zero.");
    radicand = (in[0] - in[1]) / in[2];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}

static solve_result_t solve_ek_rot_from_irot_omega(const double *in) { return ok_result(0.5 * in[0] * in[1] * in[1]); }
static solve_result_t solve_irot_from_ek_rot_omega(const double *in) {
    double denominator = 0.5 * in[1] * in[1];
    if (!math_can_divide(denominator)) return error_result("omega cannot be zero.");
    return ok_result(in[0] / denominator);
}
static solve_result_t solve_omega_from_ek_rot_irot(const double *in) {
    double radicand;
    if (!math_can_divide(in[1])) return error_result("I cannot be zero.");
    radicand = (2.0 * in[0]) / in[1];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}

static solve_result_t solve_ek_rot_cylinder_offset_from_m_r_a_omega(const double *in) {
    double irot = 0.5 * in[0] * in[1] * in[1] + in[0] * in[2] * in[2];
    return ok_result(0.5 * irot * in[3] * in[3]);
}

static solve_result_t solve_torque_from_irot_alpha(const double *in) { return ok_result(in[0] * in[1]); }
static solve_result_t solve_irot_from_torque_alpha(const double *in) {
    if (!math_can_divide(in[1])) return error_result("alpha cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_alpha_from_torque_irot(const double *in) {
    if (!math_can_divide(in[1])) return error_result("I cannot be zero.");
    return ok_result(in[0] / in[1]);
}

static solve_result_t solve_w_rot_from_irot_omega_omega0(const double *in) {
    return ok_result(0.5 * in[0] * (in[1] * in[1] - in[2] * in[2]));
}
static solve_result_t solve_irot_from_w_rot_omega_omega0(const double *in) {
    double denominator = 0.5 * (in[1] * in[1] - in[2] * in[2]);
    if (!math_can_divide(denominator)) return error_result("omega terms cancel.");
    return ok_result(in[0] / denominator);
}
static solve_result_t solve_omega_from_w_rot_irot_omega0(const double *in) {
    double radicand;
    if (!math_can_divide(in[1])) return error_result("I cannot be zero.");
    radicand = (2.0 * in[0]) / in[1] + in[2] * in[2];
    if (!math_can_sqrt(radicand)) return error_result("Invalid square root.");
    return ok_result(sqrt(radicand));
}

static solve_result_t solve_alpha_from_rigidbody_task(const double *in) {
    if (!math_can_divide(in[1])) return error_result("I cannot be zero.");
    return ok_result(in[0] / in[1]);
}
static solve_result_t solve_omega_from_rigidbody_task(const double *in) {
    if (!math_can_divide(in[1])) return error_result("I cannot be zero.");
    return ok_result(in[2] + (in[0] / in[1]) * in[3]);
}
static solve_result_t solve_w_from_rigidbody_task(const double *in) {
    double omega;
    if (!math_can_divide(in[1])) return error_result("I cannot be zero.");
    omega = in[2] + (in[0] / in[1]) * in[3];
    return ok_result(0.5 * in[1] * (omega * omega - in[2] * in[2]));
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

static formula_run_result_t run_force_linear_motion_solver(const formula_def_t *formula) {
    double m = 0.0;
    double f0 = 0.0;
    double k = 0.0;
    double v0 = 0.0;
    double s0 = 0.0;
    double t = 0.0;
    double velocity = 0.0;
    double displacement = 0.0;
    char result_line[32];

    if (!io_prompt_double(formula->expression, "Enter mass m [kg]:", &m)) {
        return FORMULA_RUN_BACK;
    }
    if (!math_can_divide(m)) {
        io_show_message("Solver Error", "Mass m cannot", "be zero.");
        return FORMULA_RUN_BACK;
    }
    if (!io_prompt_double(formula->expression, "Enter force offset F0 [N]:", &f0)) {
        return FORMULA_RUN_BACK;
    }
    if (!io_prompt_double(formula->expression, "Enter force slope k [N/s]:", &k)) {
        return FORMULA_RUN_BACK;
    }
    if (!io_prompt_double(formula->expression, "Enter initial velocity v0 [m/s]:", &v0)) {
        return FORMULA_RUN_BACK;
    }
    if (!io_prompt_double(formula->expression, "Enter initial position s0 [m]:", &s0)) {
        return FORMULA_RUN_BACK;
    }
    if (!io_prompt_double(formula->expression, "Enter time t [s]:", &t)) {
        return FORMULA_RUN_BACK;
    }
    if (math_is_negative(t)) {
        io_show_message("Solver Error", "Time t must be", "non-negative.");
        return FORMULA_RUN_BACK;
    }

    velocity = v0 + (f0 / m) * t + (k / (2.0 * m)) * t * t;
    displacement = s0 + v0 * t + (f0 / (2.0 * m)) * t * t + (k / (6.0 * m)) * t * t * t;

    io_clear_screen();
    io_draw_title("Result");
    io_draw_wrapped_text(1, formula->expression, 26);
    snprintf(result_line, sizeof(result_line), "v(t) = %.8g m/s", velocity);
    io_draw_wrapped_text(3, result_line, 26);
    snprintf(result_line, sizeof(result_line), "s(t) = %.8g m", displacement);
    io_draw_wrapped_text(5, result_line, 26);
    io_draw_wrapped_text(7, formula->const_note, 26);
    io_draw_footer("ENTER/CLEAR continue");

    while (true) {
        char key = os_GetCSC();
        if (key == sk_Enter || key == sk_Clear) {
            io_wait_for_key_release();
            return FORMULA_RUN_BACK;
        }
    }
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

static const solve_option_t OPT_VAVG_X2_X1_T2_T1[] = {
    { VAR_V, 4, { VAR_X2, VAR_X1, VAR_T2, VAR_T1 }, solve_vavg_from_x2_x1_t2_t1 },
    { VAR_X2, 4, { VAR_V, VAR_X1, VAR_T2, VAR_T1 }, solve_x2_from_vavg_x1_t2_t1 },
    { VAR_X1, 4, { VAR_V, VAR_X2, VAR_T2, VAR_T1 }, solve_x1_from_vavg_x2_t2_t1 }
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

static const solve_option_t OPT_PHI_DOT_VMAG_AMAG[] = {
    { VAR_PHI_DEG, 3, { VAR_DOT_VA, VAR_V_MAG, VAR_A_MAG }, solve_phi_deg_from_dot_vmag_amag },
    { VAR_DOT_VA, 3, { VAR_PHI_DEG, VAR_V_MAG, VAR_A_MAG }, solve_dot_from_phi_deg_vmag_amag }
};

static const solve_option_t OPT_D_XYZ[] = {
    { VAR_D, 3, { VAR_X, VAR_Y, VAR_Z }, solve_d_from_xyz }
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

static const solve_option_t OPT_AC_R_OMEGA[] = {
    { VAR_AC, 2, { VAR_R, VAR_OMEGA }, solve_ac_from_r_omega },
    { VAR_R, 2, { VAR_AC, VAR_OMEGA }, solve_r_from_ac_omega },
    { VAR_OMEGA, 2, { VAR_AC, VAR_R }, solve_omega_from_ac_r }
};

static const solve_option_t OPT_A_AT_AC[] = {
    { VAR_A, 2, { VAR_AT, VAR_AC }, solve_a_total_from_at_ac },
    { VAR_AT, 2, { VAR_A, VAR_AC }, solve_at_from_a_total_ac },
    { VAR_AC, 2, { VAR_A, VAR_AT }, solve_ac_from_a_total_at }
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

static const solve_option_t OPT_N_OMEGA0_ALPHA_T[] = {
    { VAR_N, 3, { VAR_OMEGA0, VAR_ALPHA, VAR_T }, solve_n_from_omega0_alpha_t },
    { VAR_OMEGA0, 3, { VAR_N, VAR_ALPHA, VAR_T }, solve_omega0_from_n_alpha_t },
    { VAR_ALPHA, 3, { VAR_N, VAR_OMEGA0, VAR_T }, solve_alpha_from_n_omega0_t },
    { VAR_T, 3, { VAR_N, VAR_OMEGA0, VAR_ALPHA }, solve_t_from_n_omega0_alpha }
};

static const solve_option_t OPT_FORCE_DP_T[] = {
    { VAR_F, 2, { VAR_DP, VAR_T }, solve_force_from_dp_t },
    { VAR_DP, 2, { VAR_F, VAR_T }, solve_dp_from_force_t },
    { VAR_T, 2, { VAR_F, VAR_DP }, solve_t_from_force_dp }
};

static const solve_option_t OPT_F_KTIME_T[] = {
    { VAR_F, 2, { VAR_K_FORCE_TIME, VAR_T }, solve_f_from_k_time },
    { VAR_K_FORCE_TIME, 2, { VAR_F, VAR_T }, solve_k_from_f_time },
    { VAR_T, 2, { VAR_F, VAR_K_FORCE_TIME }, solve_t_from_f_k_time }
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

static const solve_option_t OPT_WEIGHT_M_G[] = {
    { VAR_WEIGHT_G, 2, { VAR_M, VAR_G }, solve_weight_from_m_g },
    { VAR_M, 2, { VAR_WEIGHT_G, VAR_G }, solve_m_from_weight_g },
    { VAR_G, 2, { VAR_WEIGHT_G, VAR_M }, solve_g_from_weight_m }
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

static const solve_option_t OPT_STOP_F_S_M_V0[] = {
    { VAR_S, 3, { VAR_F, VAR_M, VAR_V0 }, solve_s_stop_from_f_m_v0 },
    { VAR_F, 3, { VAR_S, VAR_M, VAR_V0 }, solve_f_stop_from_s_m_v0 },
    { VAR_M, 3, { VAR_F, VAR_S, VAR_V0 }, solve_m_stop_from_f_s_v0 },
    { VAR_V0, 3, { VAR_F, VAR_S, VAR_M }, solve_v0_stop_from_f_s_m }
};

static const solve_option_t OPT_H_EK_HALF_EP[] = {
    { VAR_H, 2, { VAR_V0, VAR_G }, solve_h_from_v0_g_energy_ratio },
    { VAR_V0, 2, { VAR_H, VAR_G }, solve_v0_from_h_g_energy_ratio },
    { VAR_G, 2, { VAR_V0, VAR_H }, solve_g_from_v0_h_energy_ratio }
};

static const solve_option_t OPT_VERTICAL_V2[] = {
    { VAR_V, 3, { VAR_V0, VAR_G, VAR_H }, solve_v_from_vertical_height },
    { VAR_V0, 3, { VAR_V, VAR_G, VAR_H }, solve_v0_from_vertical_height },
    { VAR_H, 3, { VAR_V0, VAR_V, VAR_G }, solve_h_from_vertical_speeds },
    { VAR_G, 3, { VAR_V0, VAR_V, VAR_H }, solve_g_from_vertical_speeds_h }
};

static const solve_option_t OPT_HMAX_V0_G[] = {
    { VAR_H, 2, { VAR_V0, VAR_G }, solve_hmax_from_v0_g },
    { VAR_V0, 2, { VAR_H, VAR_G }, solve_v0_from_hmax_g },
    { VAR_G, 2, { VAR_V0, VAR_H }, solve_g_from_v0_hmax }
};

static const solve_option_t OPT_HMAX_H0_V0_G[] = {
    { VAR_HMAX, 3, { VAR_H0, VAR_V0, VAR_G }, solve_hmax_from_h0_v0_g },
    { VAR_H0, 3, { VAR_HMAX, VAR_V0, VAR_G }, solve_h0_from_hmax_v0_g },
    { VAR_V0, 3, { VAR_HMAX, VAR_H0, VAR_G }, solve_v0_from_hmax_h0_g },
    { VAR_G, 3, { VAR_HMAX, VAR_H0, VAR_V0 }, solve_g_from_hmax_h0_v0 }
};

static const solve_option_t OPT_ENERGY_TWO_HEIGHTS[] = {
    { VAR_V, 4, { VAR_V0, VAR_G, VAR_H1, VAR_H2 }, solve_v_from_energy_two_heights },
    { VAR_V0, 4, { VAR_V, VAR_G, VAR_H1, VAR_H2 }, solve_v0_from_energy_two_heights },
    { VAR_H2, 4, { VAR_V0, VAR_V, VAR_G, VAR_H1 }, solve_h2_from_energy_two_heights },
    { VAR_H1, 4, { VAR_V0, VAR_V, VAR_G, VAR_H2 }, solve_h1_from_energy_two_heights }
};

static const solve_option_t OPT_EK_EQ_EP[] = {
    { VAR_H, 2, { VAR_V, VAR_G }, solve_h_from_ek_eq_ep },
    { VAR_V, 2, { VAR_H, VAR_G }, solve_v_from_ek_eq_ep },
    { VAR_G, 2, { VAR_V, VAR_H }, solve_g_from_ek_eq_ep }
};

static const solve_option_t OPT_EK_THIRD_EP[] = {
    { VAR_H, 2, { VAR_V0, VAR_G }, solve_h_from_ek_third_ep },
    { VAR_V0, 2, { VAR_H, VAR_G }, solve_v0_from_h_g_ek_third_ep },
    { VAR_G, 2, { VAR_V0, VAR_H }, solve_g_from_v0_h_ek_third_ep }
};

static const solve_option_t OPT_I0_CYLINDER[] = {
    { VAR_I0, 2, { VAR_M, VAR_R }, solve_i0_cylinder_from_m_r },
    { VAR_M, 2, { VAR_I0, VAR_R }, solve_m_from_i0_cylinder_r },
    { VAR_R, 2, { VAR_I0, VAR_M }, solve_r_from_i0_cylinder_m }
};

static const solve_option_t OPT_PARALLEL_AXIS[] = {
    { VAR_IROT, 3, { VAR_I0, VAR_M, VAR_AXIS_A }, solve_irot_from_i0_m_axis_a },
    { VAR_I0, 3, { VAR_IROT, VAR_M, VAR_AXIS_A }, solve_i0_from_irot_m_axis_a },
    { VAR_M, 3, { VAR_IROT, VAR_I0, VAR_AXIS_A }, solve_m_from_irot_i0_axis_a },
    { VAR_AXIS_A, 3, { VAR_IROT, VAR_I0, VAR_M }, solve_axis_a_from_irot_i0_m }
};

static const solve_option_t OPT_EK_ROT[] = {
    { VAR_EK, 2, { VAR_IROT, VAR_OMEGA }, solve_ek_rot_from_irot_omega },
    { VAR_IROT, 2, { VAR_EK, VAR_OMEGA }, solve_irot_from_ek_rot_omega },
    { VAR_OMEGA, 2, { VAR_EK, VAR_IROT }, solve_omega_from_ek_rot_irot }
};

static const solve_option_t OPT_EK_ROT_CYLINDER_OFFSET[] = {
    { VAR_EK, 4, { VAR_M, VAR_R, VAR_AXIS_A, VAR_OMEGA }, solve_ek_rot_cylinder_offset_from_m_r_a_omega }
};

static const solve_option_t OPT_TORQUE_I_ALPHA[] = {
    { VAR_TORQUE_M, 2, { VAR_IROT, VAR_ALPHA }, solve_torque_from_irot_alpha },
    { VAR_IROT, 2, { VAR_TORQUE_M, VAR_ALPHA }, solve_irot_from_torque_alpha },
    { VAR_ALPHA, 2, { VAR_TORQUE_M, VAR_IROT }, solve_alpha_from_torque_irot }
};

static const solve_option_t OPT_W_ROT_DELTA_EK[] = {
    { VAR_W, 3, { VAR_IROT, VAR_OMEGA, VAR_OMEGA0 }, solve_w_rot_from_irot_omega_omega0 },
    { VAR_IROT, 3, { VAR_W, VAR_OMEGA, VAR_OMEGA0 }, solve_irot_from_w_rot_omega_omega0 },
    { VAR_OMEGA, 3, { VAR_W, VAR_IROT, VAR_OMEGA0 }, solve_omega_from_w_rot_irot_omega0 }
};

static const solve_option_t OPT_RIGID_TORQUE_TASK[] = {
    { VAR_ALPHA, 4, { VAR_TORQUE_M, VAR_IROT, VAR_OMEGA0, VAR_T }, solve_alpha_from_rigidbody_task },
    { VAR_OMEGA, 4, { VAR_TORQUE_M, VAR_IROT, VAR_OMEGA0, VAR_T }, solve_omega_from_rigidbody_task },
    { VAR_W, 4, { VAR_TORQUE_M, VAR_IROT, VAR_OMEGA0, VAR_T }, solve_w_from_rigidbody_task }
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

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-field-initializers"

static const formula_def_t FORMULA_KIN_V = {
    "v=v0+a*t",
    "v = v0 + a*t",
    "Condition: constant a",
    ARRAY_LEN(OPT_V_V0_A_T),
    OPT_V_V0_A_T
};

static const formula_def_t FORMULA_KIN_VAVG = {
    "v=s/t",
    "v_avg = s / t",
    "Use path or displacement",
    ARRAY_LEN(OPT_V_S_T),
    OPT_V_S_T
};

static const formula_def_t FORMULA_KIN_A = {
    "a=(v-v0)/t",
    "a = (v - v0) / t",
    "Straight-line motion",
    ARRAY_LEN(OPT_A_V_V0_T),
    OPT_A_V_V0_T
};

static const formula_def_t FORMULA_KIN_S = {
    "s=s0+v*t",
    "s = s0 + v*t",
    "Condition: constant v",
    ARRAY_LEN(OPT_S_S0_V_T),
    OPT_S_S0_V_T
};

static const formula_def_t FORMULA_KIN_S2 = {
    "s=v0*t+0.5*a*t^2",
    "s = v0*t + 0.5*a*t^2",
    "Condition: constant a",
    ARRAY_LEN(OPT_S_V0_A_T2),
    OPT_S_V0_A_T2
};

static const formula_def_t FORMULA_CIRC_V = {
    "v=r*omega",
    "v = r*omega",
    "Condition: constant r",
    ARRAY_LEN(OPT_V_R_OMEGA),
    OPT_V_R_OMEGA
};

static const formula_def_t FORMULA_CIRC_PHI = {
    "phi=omega*t",
    "phi = omega*t",
    "Condition: constant omega",
    ARRAY_LEN(OPT_PHI_OMEGA_T),
    OPT_PHI_OMEGA_T
};

static const formula_def_t FORMULA_CIRC_AT = {
    "a_t=r*alpha",
    "a_t = r*alpha",
    "Condition: constant r",
    ARRAY_LEN(OPT_AT_R_ALPHA),
    OPT_AT_R_ALPHA
};

static const formula_def_t FORMULA_CIRC_AC = {
    "a_c=v^2/r",
    "a_c = v^2 / r",
    "Condition: constant r",
    ARRAY_LEN(OPT_AC_V_R),
    OPT_AC_V_R
};

static const formula_def_t FORMULA_CIRC_AC_OMEGA = {
    "a_c=r*omega^2",
    "a_c = r*omega^2",
    "Condition: constant r",
    ARRAY_LEN(OPT_AC_R_OMEGA),
    OPT_AC_R_OMEGA
};

static const formula_def_t FORMULA_CIRC_A_TOTAL = {
    "a=sqrt(a_t^2+a_c^2)",
    "a = sqrt(a_t^2 + a_c^2)",
    "Total acceleration",
    ARRAY_LEN(OPT_A_AT_AC),
    OPT_A_AT_AC
};

static const formula_def_t FORMULA_CIRC_OMEGA_F = {
    "omega=2*pi*f",
    "omega = 2*pi*f",
    "No extra condition",
    ARRAY_LEN(OPT_OMEGA_F),
    OPT_OMEGA_F
};

static const formula_def_t FORMULA_CIRC_OMEGA_T = {
    "omega=2*pi/T",
    "omega = 2*pi / T",
    "No extra condition",
    ARRAY_LEN(OPT_OMEGA_T),
    OPT_OMEGA_T
};

static const formula_def_t FORMULA_CIRC_FREQ = {
    "f=1/T",
    "f = 1 / T",
    "No extra condition",
    ARRAY_LEN(OPT_FREQ_PERIOD),
    OPT_FREQ_PERIOD
};

static const formula_def_t FORMULA_CIRC_OMEGA_ALPHA = {
    "omega=omega0+alpha*t",
    "omega = omega0 + alpha*t",
    "Condition: constant alpha",
    ARRAY_LEN(OPT_OMEGA_OMEGA0_ALPHA_T),
    OPT_OMEGA_OMEGA0_ALPHA_T
};

static const formula_def_t FORMULA_CIRC_PHI_ALPHA = {
    "phi=omega0*t+0.5*alpha*t^2",
    "phi = omega0*t + 0.5*alpha*t^2",
    "Condition: constant alpha",
    ARRAY_LEN(OPT_PHI_OMEGA0_ALPHA_T),
    OPT_PHI_OMEGA0_ALPHA_T
};

static const formula_def_t FORMULA_CIRC_N = {
    "N=phi/(2*pi)",
    "phi = 2*pi*N",
    "No extra condition",
    ARRAY_LEN(OPT_PHI_N),
    OPT_PHI_N
};

static const formula_def_t FORMULA_CIRC_N_ALPHA = {
    "N=(omega0*t+.5*alpha*t^2)/2pi",
    "N = (omega0*t + 0.5*alpha*t^2)/(2*pi)",
    "Condition: constant alpha",
    ARRAY_LEN(OPT_N_OMEGA0_ALPHA_T),
    OPT_N_OMEGA0_ALPHA_T
};

static const formula_def_t FORMULA_DYN_P = {
    "p = m*v",
    "p = m*v",
    "Condition: constant m",
    ARRAY_LEN(OPT_P_M_V),
    OPT_P_M_V
};

static const formula_def_t FORMULA_DYN_F = {
    "F = m*a",
    "F = m*a",
    "Condition: constant m",
    ARRAY_LEN(OPT_F_M_A),
    OPT_F_M_A
};

static const formula_def_t FORMULA_DYN_A = {
    "a = F / m",
    "a = F / m",
    "Condition: constant m",
    ARRAY_LEN(OPT_F_M_A),
    OPT_F_M_A
};

static const formula_def_t FORMULA_DYN_I = {
    "I = F*t",
    "I = F*t",
    "Condition: constant F",
    ARRAY_LEN(OPT_I_F_T),
    OPT_I_F_T
};

static const formula_def_t FORMULA_DYN_F_KT = {
    "F=k*t",
    "F = k*t",
    "Force grows linearly with time",
    ARRAY_LEN(OPT_F_KTIME_T),
    OPT_F_KTIME_T
};

static const formula_def_t FORMULA_DYN_WEIGHT = {
    "G = m*g",
    "G = m*g",
    "Weight force near Earth",
    ARRAY_LEN(OPT_WEIGHT_M_G),
    OPT_WEIGHT_M_G
};

static const formula_def_t FORMULA_EN_EK = {
    "Ek = 0.5*m*v^2",
    "Ek = 0.5*m*v^2",
    "Condition: constant m",
    ARRAY_LEN(OPT_EK_M_V),
    OPT_EK_M_V
};

static const formula_def_t FORMULA_EN_STOP = {
    "F*s=0.5*m*v0^2",
    "F*s = 0.5*m*v0^2",
    "Use: stopping, final v=0",
    ARRAY_LEN(OPT_STOP_F_S_M_V0),
    OPT_STOP_F_S_M_V0
};

static const formula_def_t FORMULA_EN_EK_HALF_EP_HEIGHT = {
    "Ek=0.5Ep: h=v0^2/(3g)",
    "h = v0^2 / (3*g)",
    "Use: when Ek=0.5Ep",
    ARRAY_LEN(OPT_H_EK_HALF_EP),
    OPT_H_EK_HALF_EP
};

static const formula_def_t FORMULA_VERT_V2 = {
    "v^2=v0^2-2gh",
    "v^2 = v0^2 - 2*g*h",
    "Use: upward, v not 0",
    ARRAY_LEN(OPT_VERTICAL_V2),
    OPT_VERTICAL_V2
};

static const formula_def_t FORMULA_VERT_H_FROM_V = {
    "h=(v0^2-v^2)/(2g)",
    "h = (v0^2 - v^2)/(2*g)",
    "Use: height, known v",
    ARRAY_LEN(OPT_VERTICAL_V2),
    OPT_VERTICAL_V2
};

static const formula_def_t FORMULA_VERT_HMAX = {
    "hmax=v0^2/(2g)",
    "hmax = v0^2 / (2*g)",
    "Use: max height, v=0",
    ARRAY_LEN(OPT_HMAX_V0_G),
    OPT_HMAX_V0_G
};

static const formula_def_t FORMULA_VERT_HMAX_H0 = {
    "hmax=h0+v0^2/(2g)",
    "hmax = h0 + v0^2/(2*g)",
    "Use: max height from h0",
    ARRAY_LEN(OPT_HMAX_H0_V0_G),
    OPT_HMAX_H0_V0_G
};

static const formula_def_t FORMULA_EN_TWO_HEIGHTS = {
    "0.5mv0^2+mgh1=0.5mv^2+mgh2",
    "0.5*m*v0^2 + m*g*h1 = 0.5*m*v^2 + m*g*h2",
    "Use: same body, two heights",
    ARRAY_LEN(OPT_ENERGY_TWO_HEIGHTS),
    OPT_ENERGY_TWO_HEIGHTS
};

static const formula_def_t FORMULA_EN_EK_EQ_EP = {
    "Ek=Ep: h=v^2/(2g)",
    "Ek = Ep, so h = v^2/(2*g)",
    "Use: when Ek=Ep",
    ARRAY_LEN(OPT_EK_EQ_EP),
    OPT_EK_EQ_EP
};

static const formula_def_t FORMULA_EN_EK_THIRD_EP_HEIGHT = {
    "Ek=(1/3)Ep: h=3v0^2/(8g)",
    "h = 3*v0^2 / (8*g)",
    "Use: when Ek=(1/3)Ep",
    ARRAY_LEN(OPT_EK_THIRD_EP),
    OPT_EK_THIRD_EP
};

static const formula_def_t FORMULA_ROT_I0_CYLINDER = {
    "I0=0.5*m*r^2",
    "I0 = 0.5*m*r^2",
    "Use: solid cylinder axis",
    ARRAY_LEN(OPT_I0_CYLINDER),
    OPT_I0_CYLINDER
};

static const formula_def_t FORMULA_ROT_PARALLEL_AXIS = {
    "I=I0+m*a^2",
    "I = I0 + m*a^2",
    "Use: shifted parallel axis",
    ARRAY_LEN(OPT_PARALLEL_AXIS),
    OPT_PARALLEL_AXIS
};

static const formula_def_t FORMULA_ROT_EK = {
    "Ek=0.5*I*omega^2",
    "Ek = 0.5*I*omega^2",
    "Use: rotational kinetic energy",
    ARRAY_LEN(OPT_EK_ROT),
    OPT_EK_ROT
};

static const formula_def_t FORMULA_ROT_EK_CYLINDER_OFFSET = {
    "Ek=0.5*(0.5mr^2+ma^2)*omega^2",
    "Ek = 0.5*(0.5*m*r^2 + m*a^2)*omega^2",
    "Use: cylinder, shifted axis",
    ARRAY_LEN(OPT_EK_ROT_CYLINDER_OFFSET),
    OPT_EK_ROT_CYLINDER_OFFSET
};

static const formula_def_t FORMULA_ROT_TORQUE = {
    "M=I*alpha",
    "M = I*alpha",
    "Use: torque causes angular accel",
    ARRAY_LEN(OPT_TORQUE_I_ALPHA),
    OPT_TORQUE_I_ALPHA
};

static const formula_def_t FORMULA_ROT_WORK_DELTA_EK = {
    "W=0.5I(omega^2-omega0^2)",
    "W = 0.5*I*(omega^2 - omega0^2)",
    "Use: rotational work-energy",
    ARRAY_LEN(OPT_W_ROT_DELTA_EK),
    OPT_W_ROT_DELTA_EK
};

static const formula_def_t FORMULA_ROT_TORQUE_TASK = {
    "omega=omega0+alpha*t",
    "M = I*alpha",
    "W=0.5I(omega^2-omega0^2)",
    ARRAY_LEN(OPT_RIGID_TORQUE_TASK),
    OPT_RIGID_TORQUE_TASK
};

static const formula_def_t FORMULA_EN_W = {
    "W = F*d",
    "W = F*d",
    "Constant F, F parallel d",
    ARRAY_LEN(OPT_W_F_D),
    OPT_W_F_D
};

static const formula_def_t FORMULA_EN_PAVG = {
    "P_avg = W / t",
    "P_avg = W / t",
    "No extra condition",
    ARRAY_LEN(OPT_PAVG_W_T),
    OPT_PAVG_W_T
};

static const formula_def_t FORMULA_EN_P = {
    "P = F*v",
    "P = F*v",
    "F parallel v",
    ARRAY_LEN(OPT_POWER_F_V),
    OPT_POWER_F_V
};

static const formula_def_t FORMULA_EN_EP = {
    "dEp = m*g*h",
    "delta Ep = m*g*h",
    "Condition: constant m and g",
    ARRAY_LEN(OPT_DELTA_EP_M_G_H),
    OPT_DELTA_EP_M_G_H
};

static const formula_def_t FORMULA_EN_E = {
    "E = Ep + Ek",
    "E = Ep + Ek",
    "No extra condition",
    ARRAY_LEN(OPT_E_TOTAL_EP_EK),
    OPT_E_TOTAL_EP_EK
};

static const formula_def_t FORMULA_EN_W_DEK = {
    "W = dEk",
    "W = delta Ek",
    "No extra condition",
    ARRAY_LEN(OPT_W_DELTA_EK),
    OPT_W_DELTA_EK
};

static const formula_def_t FORMULA_EN_SPRING = {
    "F = k*d",
    "F = k*d",
    "Condition: constant k",
    ARRAY_LEN(OPT_F_K_X),
    OPT_F_K_X
};

static const formula_def_t FORMULA_KIN_V2 = {
    "v^2 = v0^2 + 2*a*s",
    "v^2 = v0^2 + 2*a*s",
    "Condition: constant a",
    ARRAY_LEN(OPT_V2_V0_A_S),
    OPT_V2_V0_A_S
};

static const formula_def_t FORMULA_KIN_ANGLE_VA = {
    "cos(phi)=(v.a)/(|v||a|)",
    "cos(phi) = (v.a) / (|v|*|a|)",
    "Angle between v and a",
    ARRAY_LEN(OPT_PHI_DOT_VMAG_AMAG),
    OPT_PHI_DOT_VMAG_AMAG
};

static const formula_def_t FORMULA_KIN_R_MAG = {
    "d=|r1|",
    "d = |r1| = sqrt(x^2+y^2+z^2)",
    "Magnitude of position vector",
    ARRAY_LEN(OPT_D_XYZ),
    OPT_D_XYZ
};

static const formula_def_t FORMULA_KIN_VAVG_X = {
    "v_avg=(x2-x1)/(t2-t1)",
    "v_avg = (x2 - x1) / (t2 - t1)",
    "Average velocity",
    ARRAY_LEN(OPT_VAVG_X2_X1_T2_T1),
    OPT_VAVG_X2_X1_T2_T1
};

static const formula_def_t FORMULA_CALC_V = {
    "v = dx / dt",
    "v = dx / dt",
    "Use change in position over time",
    ARRAY_LEN(OPT_V_S_T),
    OPT_V_S_T
};

static const formula_def_t FORMULA_CALC_A = {
    "a = dv / dt",
    "a = dv / dt",
    "Use change in velocity over time",
    ARRAY_LEN(OPT_A_V_V0_T),
    OPT_A_V_V0_T
};

static const formula_def_t FORMULA_CALC_OMEGA = {
    "omega = dphi / dt",
    "omega = dphi / dt",
    "Use change in angle over time",
    ARRAY_LEN(OPT_PHI_OMEGA_T),
    OPT_PHI_OMEGA_T
};

static const formula_def_t FORMULA_CALC_ALPHA = {
    "alpha = domega / dt",
    "alpha = domega / dt",
    "Use change in omega over time",
    ARRAY_LEN(OPT_OMEGA_OMEGA0_ALPHA_T),
    OPT_OMEGA_OMEGA0_ALPHA_T
};

static const formula_def_t FORMULA_CALC_POWER = {
    "P = dW / dt",
    "P = dW / dt",
    "Use work change over time",
    ARRAY_LEN(OPT_POWER_W_T),
    OPT_POWER_W_T
};

static const formula_def_t FORMULA_CALC_FORCE = {
    "F = dp / dt",
    "F = dp / dt",
    "Use momentum change over time",
    ARRAY_LEN(OPT_FORCE_DP_T),
    OPT_FORCE_DP_T
};

static const formula_def_t FORMULA_CALC_DISP = {
    "x = S v*dt",
    "x = S v*dt",
    "Constant v over the interval",
    ARRAY_LEN(OPT_V_S_T),
    OPT_V_S_T
};

static const formula_def_t FORMULA_CALC_DV = {
    "v = S a*dt",
    "v = S a*dt",
    "Constant a over the interval",
    ARRAY_LEN(OPT_A_V_V0_T),
    OPT_A_V_V0_T
};

static const formula_def_t FORMULA_CALC_DPHI = {
    "phi = S omega*dt",
    "phi = S omega*dt",
    "Constant omega over the interval",
    ARRAY_LEN(OPT_PHI_OMEGA_T),
    OPT_PHI_OMEGA_T
};

static const formula_def_t FORMULA_CALC_DOMEGA = {
    "omega = S alpha*dt",
    "omega = S alpha*dt",
    "Constant alpha over the interval",
    ARRAY_LEN(OPT_OMEGA_OMEGA0_ALPHA_T),
    OPT_OMEGA_OMEGA0_ALPHA_T
};

static const formula_def_t FORMULA_CALC_I = {
    "I = S F*dt",
    "I = S F*dt",
    "F constant over dt",
    ARRAY_LEN(OPT_I_F_T),
    OPT_I_F_T
};

static const formula_def_t FORMULA_CALC_W = {
    "W = S F*dr",
    "W = S F*dr",
    "F constant and parallel",
    ARRAY_LEN(OPT_W_F_D),
    OPT_W_F_D
};

static const formula_def_t FORMULA_CALC_EXPR = {
    "d/dx or S polynomial",
    "Enter polynomial in x or t",
    "Examples: 10t, 3t^2+2, x^3-x",
    0,
    NULL,
    expr_calc_run
};

static const formula_def_t FORMULA_CALC_FORCE_LINEAR = {
    "F(t)->v(t),s(t)",
    "F(t) = F0 + k*t",
    "Find v(t) and s(t) for linear force",
    0,
    NULL,
    run_force_linear_motion_solver
};

#pragma clang diagnostic pop

static category_def_t CATEGORIES[CATEGORY_COUNT];

void formula_registry_init(void) {
    CATEGORIES[0].name = "1. Quick Solver";
    CATEGORIES[0].formula_count = 49;
    CATEGORIES[0].formulas[0] = &FORMULA_CALC_EXPR;
    CATEGORIES[0].formulas[1] = &FORMULA_CALC_FORCE;
    CATEGORIES[0].formulas[2] = &FORMULA_CALC_FORCE_LINEAR;
    CATEGORIES[0].formulas[3] = &FORMULA_CALC_V;
    CATEGORIES[0].formulas[4] = &FORMULA_CALC_A;
    CATEGORIES[0].formulas[5] = &FORMULA_KIN_V;
    CATEGORIES[0].formulas[6] = &FORMULA_KIN_S2;
    CATEGORIES[0].formulas[7] = &FORMULA_KIN_V2;
    CATEGORIES[0].formulas[8] = &FORMULA_KIN_ANGLE_VA;
    CATEGORIES[0].formulas[9] = &FORMULA_KIN_R_MAG;
    CATEGORIES[0].formulas[10] = &FORMULA_KIN_VAVG_X;
    CATEGORIES[0].formulas[11] = &FORMULA_CIRC_N_ALPHA;
    CATEGORIES[0].formulas[12] = &FORMULA_CIRC_OMEGA_ALPHA;
    CATEGORIES[0].formulas[13] = &FORMULA_CIRC_PHI_ALPHA;
    CATEGORIES[0].formulas[14] = &FORMULA_CIRC_V;
    CATEGORIES[0].formulas[15] = &FORMULA_CIRC_AC_OMEGA;
    CATEGORIES[0].formulas[16] = &FORMULA_CIRC_AT;
    CATEGORIES[0].formulas[17] = &FORMULA_CIRC_A_TOTAL;
    CATEGORIES[0].formulas[18] = &FORMULA_CIRC_OMEGA_F;
    CATEGORIES[0].formulas[19] = &FORMULA_CIRC_OMEGA_T;
    CATEGORIES[0].formulas[20] = &FORMULA_CIRC_FREQ;
    CATEGORIES[0].formulas[21] = &FORMULA_DYN_F;
    CATEGORIES[0].formulas[22] = &FORMULA_DYN_WEIGHT;
    CATEGORIES[0].formulas[23] = &FORMULA_DYN_F_KT;
    CATEGORIES[0].formulas[24] = &FORMULA_DYN_P;
    CATEGORIES[0].formulas[25] = &FORMULA_DYN_I;
    CATEGORIES[0].formulas[26] = &FORMULA_EN_W_DEK;
    CATEGORIES[0].formulas[27] = &FORMULA_EN_EK;
    CATEGORIES[0].formulas[28] = &FORMULA_ROT_I0_CYLINDER;
    CATEGORIES[0].formulas[29] = &FORMULA_ROT_PARALLEL_AXIS;
    CATEGORIES[0].formulas[30] = &FORMULA_ROT_EK;
    CATEGORIES[0].formulas[31] = &FORMULA_ROT_EK_CYLINDER_OFFSET;
    CATEGORIES[0].formulas[32] = &FORMULA_ROT_TORQUE;
    CATEGORIES[0].formulas[33] = &FORMULA_CIRC_OMEGA_ALPHA;
    CATEGORIES[0].formulas[34] = &FORMULA_ROT_WORK_DELTA_EK;
    CATEGORIES[0].formulas[35] = &FORMULA_ROT_TORQUE_TASK;
    CATEGORIES[0].formulas[36] = &FORMULA_EN_EP;
    CATEGORIES[0].formulas[37] = &FORMULA_EN_E;
    CATEGORIES[0].formulas[38] = &FORMULA_VERT_V2;
    CATEGORIES[0].formulas[39] = &FORMULA_VERT_H_FROM_V;
    CATEGORIES[0].formulas[40] = &FORMULA_VERT_HMAX;
    CATEGORIES[0].formulas[41] = &FORMULA_VERT_HMAX_H0;
    CATEGORIES[0].formulas[42] = &FORMULA_EN_TWO_HEIGHTS;
    CATEGORIES[0].formulas[43] = &FORMULA_EN_EK_EQ_EP;
    CATEGORIES[0].formulas[44] = &FORMULA_EN_EK_HALF_EP_HEIGHT;
    CATEGORIES[0].formulas[45] = &FORMULA_EN_EK_THIRD_EP_HEIGHT;
    CATEGORIES[0].formulas[46] = &FORMULA_EN_STOP;
    CATEGORIES[0].formulas[47] = &FORMULA_EN_W;
    CATEGORIES[0].formulas[48] = &FORMULA_EN_P;

    CATEGORIES[1].name = "2. Kinematics";
    CATEGORIES[1].formula_count = 9;
    CATEGORIES[1].formulas[0] = &FORMULA_KIN_V;
    CATEGORIES[1].formulas[1] = &FORMULA_KIN_S2;
    CATEGORIES[1].formulas[2] = &FORMULA_KIN_V2;
    CATEGORIES[1].formulas[3] = &FORMULA_KIN_S;
    CATEGORIES[1].formulas[4] = &FORMULA_KIN_VAVG;
    CATEGORIES[1].formulas[5] = &FORMULA_KIN_A;
    CATEGORIES[1].formulas[6] = &FORMULA_KIN_ANGLE_VA;
    CATEGORIES[1].formulas[7] = &FORMULA_KIN_R_MAG;
    CATEGORIES[1].formulas[8] = &FORMULA_KIN_VAVG_X;

    CATEGORIES[2].name = "3. Circular Motion";
    CATEGORIES[2].formula_count = 13;
    CATEGORIES[2].formulas[0] = &FORMULA_CIRC_V;
    CATEGORIES[2].formulas[1] = &FORMULA_CIRC_AC;
    CATEGORIES[2].formulas[2] = &FORMULA_CIRC_AC_OMEGA;
    CATEGORIES[2].formulas[3] = &FORMULA_CIRC_AT;
    CATEGORIES[2].formulas[4] = &FORMULA_CIRC_A_TOTAL;
    CATEGORIES[2].formulas[5] = &FORMULA_CIRC_OMEGA_F;
    CATEGORIES[2].formulas[6] = &FORMULA_CIRC_OMEGA_T;
    CATEGORIES[2].formulas[7] = &FORMULA_CIRC_OMEGA_ALPHA;
    CATEGORIES[2].formulas[8] = &FORMULA_CIRC_PHI_ALPHA;
    CATEGORIES[2].formulas[9] = &FORMULA_CIRC_N_ALPHA;
    CATEGORIES[2].formulas[10] = &FORMULA_CIRC_N;
    CATEGORIES[2].formulas[11] = &FORMULA_CIRC_PHI;
    CATEGORIES[2].formulas[12] = &FORMULA_CIRC_FREQ;

    CATEGORIES[3].name = "4. Dynamics";
    CATEGORIES[3].formula_count = 6;
    CATEGORIES[3].formulas[0] = &FORMULA_DYN_F;
    CATEGORIES[3].formulas[1] = &FORMULA_DYN_WEIGHT;
    CATEGORIES[3].formulas[2] = &FORMULA_DYN_F_KT;
    CATEGORIES[3].formulas[3] = &FORMULA_DYN_A;
    CATEGORIES[3].formulas[4] = &FORMULA_DYN_P;
    CATEGORIES[3].formulas[5] = &FORMULA_DYN_I;

    CATEGORIES[4].name = "5. Work / Energy / Power";
    CATEGORIES[4].formula_count = 24;
    CATEGORIES[4].formulas[0] = &FORMULA_EN_W_DEK;
    CATEGORIES[4].formulas[1] = &FORMULA_EN_EK;
    CATEGORIES[4].formulas[2] = &FORMULA_ROT_I0_CYLINDER;
    CATEGORIES[4].formulas[3] = &FORMULA_ROT_PARALLEL_AXIS;
    CATEGORIES[4].formulas[4] = &FORMULA_ROT_EK;
    CATEGORIES[4].formulas[5] = &FORMULA_ROT_EK_CYLINDER_OFFSET;
    CATEGORIES[4].formulas[6] = &FORMULA_ROT_TORQUE;
    CATEGORIES[4].formulas[7] = &FORMULA_ROT_WORK_DELTA_EK;
    CATEGORIES[4].formulas[8] = &FORMULA_ROT_TORQUE_TASK;
    CATEGORIES[4].formulas[9] = &FORMULA_EN_EP;
    CATEGORIES[4].formulas[10] = &FORMULA_EN_E;
    CATEGORIES[4].formulas[11] = &FORMULA_VERT_V2;
    CATEGORIES[4].formulas[12] = &FORMULA_VERT_H_FROM_V;
    CATEGORIES[4].formulas[13] = &FORMULA_VERT_HMAX;
    CATEGORIES[4].formulas[14] = &FORMULA_VERT_HMAX_H0;
    CATEGORIES[4].formulas[15] = &FORMULA_EN_TWO_HEIGHTS;
    CATEGORIES[4].formulas[16] = &FORMULA_EN_EK_EQ_EP;
    CATEGORIES[4].formulas[17] = &FORMULA_EN_EK_HALF_EP_HEIGHT;
    CATEGORIES[4].formulas[18] = &FORMULA_EN_EK_THIRD_EP_HEIGHT;
    CATEGORIES[4].formulas[19] = &FORMULA_EN_STOP;
    CATEGORIES[4].formulas[20] = &FORMULA_EN_W;
    CATEGORIES[4].formulas[21] = &FORMULA_EN_P;
    CATEGORIES[4].formulas[22] = &FORMULA_EN_PAVG;
    CATEGORIES[4].formulas[23] = &FORMULA_EN_SPRING;

    CATEGORIES[5].name = "6. Derivatives / Integrals";
    CATEGORIES[5].formula_count = 14;
    CATEGORIES[5].formulas[0] = &FORMULA_CALC_EXPR;
    CATEGORIES[5].formulas[1] = &FORMULA_CALC_FORCE_LINEAR;
    CATEGORIES[5].formulas[2] = &FORMULA_CALC_V;
    CATEGORIES[5].formulas[3] = &FORMULA_CALC_A;
    CATEGORIES[5].formulas[4] = &FORMULA_CALC_DISP;
    CATEGORIES[5].formulas[5] = &FORMULA_CALC_FORCE;
    CATEGORIES[5].formulas[6] = &FORMULA_CALC_POWER;
    CATEGORIES[5].formulas[7] = &FORMULA_CALC_I;
    CATEGORIES[5].formulas[8] = &FORMULA_CALC_W;
    CATEGORIES[5].formulas[9] = &FORMULA_CALC_OMEGA;
    CATEGORIES[5].formulas[10] = &FORMULA_CALC_ALPHA;
    CATEGORIES[5].formulas[11] = &FORMULA_CALC_DV;
    CATEGORIES[5].formulas[12] = &FORMULA_CALC_DPHI;
    CATEGORIES[5].formulas[13] = &FORMULA_CALC_DOMEGA;
}

const category_def_t *formula_get_categories(uint8_t *count_out) {
    *count_out = CATEGORY_COUNT;
    return CATEGORIES;
}

const char *formula_variable_name(variable_id_t id) {
    switch (id) {
        case VAR_V: return "Velocity v";
        case VAR_V0: return "Initial velocity v0";
        case VAR_A: return "Acceleration a";
        case VAR_T: return "Time t";
        case VAR_S: return "Displacement s";
        case VAR_S0: return "Initial position s0";
        case VAR_R: return "Radius r";
        case VAR_OMEGA: return "Angular speed omega";
        case VAR_ALPHA: return "Angular acceleration alpha";
        case VAR_AT: return "Tangential accel a_t";
        case VAR_AC: return "Centripetal accel a_c";
        case VAR_F: return "Force F";
        case VAR_PERIOD: return "Period T";
        case VAR_FREQ: return "Frequency f";
        case VAR_PHI: return "Angle phi";
        case VAR_OMEGA0: return "Initial ang. speed omega0";
        case VAR_N: return "Revolutions N";
        case VAR_DP: return "Change in momentum dp";
        case VAR_P: return "Momentum p";
        case VAR_M: return "Mass m";
        case VAR_I: return "Impulse I";
        case VAR_EK: return "Kinetic energy Ek";
        case VAR_W: return "Work W";
        case VAR_D: return "Distance d";
        case VAR_PAVG: return "Average power P_avg";
        case VAR_POWER: return "Power P";
        case VAR_EP_DELTA: return "Potential-energy change dEp";
        case VAR_G: return "Gravity g";
        case VAR_WEIGHT_G: return "Weight G";
        case VAR_H: return "Height h";
        case VAR_H0: return "Initial height h0";
        case VAR_H1: return "Height h1";
        case VAR_H2: return "Height h2";
        case VAR_HMAX: return "Maximum height hmax";
        case VAR_E_TOTAL: return "Mechanical energy E";
        case VAR_EP: return "Potential energy Ep";
        case VAR_DELTA_EK: return "Change in kinetic energy dEk";
        case VAR_K: return "Spring constant k";
        case VAR_DOT_VA: return "Dot product v.a";
        case VAR_V_MAG: return "Speed magnitude |v|";
        case VAR_A_MAG: return "Accel magnitude |a|";
        case VAR_PHI_DEG: return "Angle phi";
        case VAR_K_FORCE_TIME: return "Force-time slope k";
        case VAR_X: return "x component";
        case VAR_Y: return "y component";
        case VAR_Z: return "z component";
        case VAR_X1: return "Position x1";
        case VAR_X2: return "Position x2";
        case VAR_T1: return "Time t1";
        case VAR_T2: return "Time t2";
        case VAR_IROT: return "Moment of inertia I";
        case VAR_I0: return "Center-axis inertia I0";
        case VAR_AXIS_A: return "Axis offset a";
        case VAR_TORQUE_M: return "Torque M";
        default: return "?";
    }
}

const char *formula_variable_prompt(variable_id_t id) {
    switch (id) {
        case VAR_V: return "velocity v";
        case VAR_V0: return "initial velocity v0";
        case VAR_A: return "acceleration a";
        case VAR_T: return "time t";
        case VAR_S: return "displacement s";
        case VAR_S0: return "initial position s0";
        case VAR_R: return "radius r";
        case VAR_OMEGA: return "angular speed omega";
        case VAR_ALPHA: return "angular acceleration alpha";
        case VAR_AT: return "tangential acceleration a_t";
        case VAR_AC: return "centripetal acceleration a_c";
        case VAR_F: return "force F";
        case VAR_PERIOD: return "period T";
        case VAR_FREQ: return "frequency f";
        case VAR_PHI: return "angle phi";
        case VAR_OMEGA0: return "initial angular speed omega0";
        case VAR_N: return "number of revolutions N";
        case VAR_DP: return "change in momentum dp";
        case VAR_P: return "momentum p";
        case VAR_M: return "mass m";
        case VAR_I: return "impulse I";
        case VAR_EK: return "kinetic energy Ek";
        case VAR_W: return "work W";
        case VAR_D: return "distance d";
        case VAR_PAVG: return "average power P_avg";
        case VAR_POWER: return "power P";
        case VAR_EP_DELTA: return "potential-energy change dEp";
        case VAR_G: return "gravity g";
        case VAR_WEIGHT_G: return "weight G";
        case VAR_H: return "height h";
        case VAR_H0: return "initial height h0";
        case VAR_H1: return "height h1";
        case VAR_H2: return "height h2";
        case VAR_HMAX: return "maximum height hmax";
        case VAR_E_TOTAL: return "mechanical energy E";
        case VAR_EP: return "potential energy Ep";
        case VAR_DELTA_EK: return "change in kinetic energy dEk";
        case VAR_K: return "spring constant k";
        case VAR_DOT_VA: return "dot product v.a";
        case VAR_V_MAG: return "speed magnitude |v|";
        case VAR_A_MAG: return "acceleration magnitude |a|";
        case VAR_PHI_DEG: return "angle phi";
        case VAR_K_FORCE_TIME: return "force-time slope k";
        case VAR_X: return "x component";
        case VAR_Y: return "y component";
        case VAR_Z: return "z component";
        case VAR_X1: return "position x1";
        case VAR_X2: return "position x2";
        case VAR_T1: return "time t1";
        case VAR_T2: return "time t2";
        case VAR_IROT: return "moment of inertia I";
        case VAR_I0: return "center-axis inertia I0";
        case VAR_AXIS_A: return "axis offset a";
        case VAR_TORQUE_M: return "torque M";
        default: return "?";
    }
}

const char *formula_variable_unit(variable_id_t id) {
    switch (id) {
        case VAR_V: return "m/s";
        case VAR_V0: return "m/s";
        case VAR_A: return "m/s^2";
        case VAR_T: return "s";
        case VAR_S: return "m";
        case VAR_S0: return "m";
        case VAR_R: return "m";
        case VAR_OMEGA: return "rad/s";
        case VAR_ALPHA: return "rad/s^2";
        case VAR_AT: return "m/s^2";
        case VAR_AC: return "m/s^2";
        case VAR_F: return "N";
        case VAR_PERIOD: return "s";
        case VAR_FREQ: return "Hz";
        case VAR_PHI: return "rad";
        case VAR_OMEGA0: return "rad/s";
        case VAR_N: return "rev";
        case VAR_DP: return "kg*m/s";
        case VAR_P: return "kg*m/s";
        case VAR_M: return "kg";
        case VAR_I: return "N*s";
        case VAR_EK: return "J";
        case VAR_W: return "J";
        case VAR_D: return "m";
        case VAR_PAVG: return "W";
        case VAR_POWER: return "W";
        case VAR_EP_DELTA: return "J";
        case VAR_G: return "m/s^2";
        case VAR_WEIGHT_G: return "N";
        case VAR_H: return "m";
        case VAR_H0: return "m";
        case VAR_H1: return "m";
        case VAR_H2: return "m";
        case VAR_HMAX: return "m";
        case VAR_E_TOTAL: return "J";
        case VAR_EP: return "J";
        case VAR_DELTA_EK: return "J";
        case VAR_K: return "N/m";
        case VAR_DOT_VA: return "m^2/s^3";
        case VAR_V_MAG: return "m/s";
        case VAR_A_MAG: return "m/s^2";
        case VAR_PHI_DEG: return "deg";
        case VAR_K_FORCE_TIME: return "N/s";
        case VAR_X: return "m";
        case VAR_Y: return "m";
        case VAR_Z: return "m";
        case VAR_X1: return "m";
        case VAR_X2: return "m";
        case VAR_T1: return "s";
        case VAR_T2: return "s";
        case VAR_IROT: return "kg*m^2";
        case VAR_I0: return "kg*m^2";
        case VAR_AXIS_A: return "m";
        case VAR_TORQUE_M: return "N*m";
        default: return "";
    }
}

static bool formula_can_solve_variable(const formula_def_t *formula, variable_id_t variable) {
    uint8_t i;
    for (i = 0; i < formula->option_count; ++i) {
        if (formula->options[i].target == variable) {
            return true;
        }
    }
    return false;
}

static bool formula_match_exists(const formula_def_t *const *matches, uint8_t count, const formula_def_t *formula) {
    uint8_t i;
    for (i = 0; i < count; ++i) {
        if (matches[i] == formula) {
            return true;
        }
    }
    return false;
}

static uint8_t collect_formula_variables(const formula_def_t *formula, variable_id_t *variables, uint8_t max_variables) {
    uint8_t count = 0;
    uint8_t i;
    for (i = 0; i < formula->option_count; ++i) {
        uint8_t j;
        bool found = false;

        for (j = 0; j < count; ++j) {
            if (variables[j] == formula->options[i].target) {
                found = true;
                break;
            }
        }
        if (!found && count < max_variables) {
            variables[count++] = formula->options[i].target;
        }

        for (j = 0; j < formula->options[i].input_count; ++j) {
            uint8_t k;
            found = false;
            for (k = 0; k < count; ++k) {
                if (variables[k] == formula->options[i].inputs[j]) {
                    found = true;
                    break;
                }
            }
            if (!found && count < max_variables) {
                variables[count++] = formula->options[i].inputs[j];
            }
        }
    }
    return count;
}

static uint8_t collect_formulas_for_variable(variable_id_t variable, const formula_def_t **matches, uint8_t max_matches) {
    const category_def_t *categories;
    uint8_t category_count;
    uint8_t count = 0;
    uint8_t i;

    categories = formula_get_categories(&category_count);
    for (i = 0; i < category_count; ++i) {
        uint8_t j;
        for (j = 0; j < categories[i].formula_count; ++j) {
            const formula_def_t *formula = categories[i].formulas[j];
            if (formula_can_solve_variable(formula, variable) && !formula_match_exists(matches, count, formula)) {
                if (count < max_matches) {
                    matches[count++] = formula;
                }
            }
        }
    }
    return count;
}

static const solve_option_t *first_option_for_variable(const formula_def_t *formula, variable_id_t variable) {
    uint8_t i;
    for (i = 0; i < formula->option_count; ++i) {
        if (formula->options[i].target == variable) {
            return &formula->options[i];
        }
    }
    return NULL;
}

static void build_inputs_line(const solve_option_t *option, char *line, size_t line_size) {
    uint8_t i;
    size_t used = 0;

    if (option == NULL || option->input_count == 0) {
        snprintf(line, line_size, "Needs: no inputs");
        return;
    }

    used = snprintf(line, line_size, "Needs: ");
    for (i = 0; i < option->input_count; ++i) {
        const char *name = formula_variable_name(option->inputs[i]);
        used += snprintf(line + used, line_size > used ? line_size - used : 0,
                         "%s%s", i == 0 ? "" : ", ", name);
    }
}

static menu_result_t solve_formula_for_variable(variable_id_t variable, const formula_def_t *formula) {
    const solve_option_t *option = first_option_for_variable(formula, variable);
    double inputs[MAX_FORMULA_INPUTS] = {0};
    uint8_t i;

    if (option == NULL) {
        io_show_message("Solver Error", "Formula cannot solve", "this variable.");
        return MENU_BACK;
    }

    for (i = 0; i < option->input_count; ++i) {
        char prompt[64];
        const char *var_name = formula_variable_prompt(option->inputs[i]);
        const char *unit = formula_variable_unit(option->inputs[i]);

        snprintf(prompt, sizeof(prompt), "Enter %s [%s]:", var_name, unit);
        if (!io_prompt_double(formula->expression, prompt, &inputs[i])) {
            return MENU_BACK;
        }
    }

    {
        solve_result_t result = option->solve(inputs);
        char target_line[64];

        if (!result.ok) {
            io_show_message("Solver Error", result.error, "Check input values.");
            return MENU_BACK;
        }

        snprintf(target_line, sizeof(target_line), "%s [%s]", formula_variable_name(option->target), formula_variable_unit(option->target));
        io_show_result(formula->expression, target_line, result.value, formula->const_note);
    }

    return MENU_BACK;
}

static menu_result_t show_formulas_for_variable(variable_id_t variable) {
    const uint8_t max_matches = 24;
    const formula_def_t *matches[24];
    uint8_t match_count = collect_formulas_for_variable(variable, matches, max_matches);
    uint8_t selected = 0;
    uint8_t scroll_offset = 0;
    const uint8_t visible_rows = 5;
    char title[24];

    snprintf(title, sizeof(title), "For %s", formula_variable_name(variable));
    if (match_count == 0) {
        io_show_message(title, "No formulas found", "for this variable.");
        return MENU_BACK;
    }

    while (true) {
        uint8_t i;
        uint8_t action;

        if (selected < scroll_offset) {
            scroll_offset = selected;
        }
        if (selected >= scroll_offset + visible_rows) {
            scroll_offset = (uint8_t)(selected - visible_rows + 1);
        }

        io_clear_screen();
        io_draw_title(title);
        io_draw_wrapped_text(1, matches[selected]->expression, 26);
        {
            char inputs_line[64];
            build_inputs_line(first_option_for_variable(matches[selected], variable), inputs_line, sizeof(inputs_line));
            io_draw_wrapped_text(2, inputs_line, 26);
        }

        for (i = 0; i < visible_rows && (uint8_t)(scroll_offset + i) < match_count; ++i) {
            uint8_t item_index = (uint8_t)(scroll_offset + i);
            char line[27];
            snprintf(line, sizeof(line), "%c %s", item_index == selected ? '>' : ' ', matches[item_index]->name);
            io_draw_wrapped_text((uint8_t)(3 + i), line, 26);
        }

        io_draw_footer("OK solve MODE back");
        action = io_read_menu_key();
        if (action == IO_MENU_UP && selected > 0) {
            --selected;
        } else if (action == IO_MENU_DOWN && selected + 1 < match_count) {
            ++selected;
        } else if (action == IO_MENU_SELECT) {
            menu_result_t solve_result = solve_formula_for_variable(variable, matches[selected]);
            if (solve_result == MENU_EXIT_APP) {
                return MENU_EXIT_APP;
            }
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

static menu_result_t select_variable_lookup(const formula_def_t *formula) {
    variable_id_t variables[VAR_COUNT];
    uint8_t variable_count = collect_formula_variables(formula, variables, VAR_COUNT);
    uint8_t selected = 0;
    uint8_t scroll_offset = 0;
    const uint8_t visible_rows = 6;

    if (variable_count == 0) {
        io_show_message("Formulas", "No variables found", "for this formula.");
        return MENU_BACK;
    }

    while (true) {
        uint8_t i;
        uint8_t action;

        if (selected < scroll_offset) {
            scroll_offset = selected;
        }
        if (selected >= scroll_offset + visible_rows) {
            scroll_offset = (uint8_t)(selected - visible_rows + 1);
        }

        io_clear_screen();
        io_draw_title("Find formulas for");
        io_draw_wrapped_text(1, formula->expression, 26);

        for (i = 0; i < visible_rows && (uint8_t)(scroll_offset + i) < variable_count; ++i) {
            uint8_t item_index = (uint8_t)(scroll_offset + i);
            char line[27];
            snprintf(line, sizeof(line), "%c %s", item_index == selected ? '>' : ' ', formula_variable_name(variables[item_index]));
            io_draw_wrapped_text((uint8_t)(3 + i), line, 26);
        }

        io_draw_footer("OK show MODE back");
        action = io_read_menu_key();
        if (action == IO_MENU_UP && selected > 0) {
            --selected;
        } else if (action == IO_MENU_DOWN && selected + 1 < variable_count) {
            ++selected;
        } else if (action == IO_MENU_SELECT) {
            menu_result_t lookup_result = show_formulas_for_variable(variables[selected]);
            if (lookup_result == MENU_EXIT_APP) {
                return MENU_EXIT_APP;
            }
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

static menu_result_t select_solve_option(const formula_def_t *formula, uint8_t *selected_index, const solve_option_t **option_out) {
    const char *labels[8];
    uint8_t i = 0;
    uint8_t scroll_offset = 0;
    const uint8_t visible_rows = 5;
    uint8_t item_count = (uint8_t)(formula->option_count + 1);

    for (i = 0; i < formula->option_count; ++i) {
        labels[i] = formula_variable_name(formula->options[i].target);
    }

    while (true) {
        uint8_t j = 0;
        uint8_t action = 0;

        if (*selected_index < scroll_offset) {
            scroll_offset = *selected_index;
        }
        if (*selected_index >= item_count) {
            *selected_index = 0;
        }
        if (*selected_index >= scroll_offset + visible_rows) {
            scroll_offset = (uint8_t)(*selected_index - visible_rows + 1);
        }

        io_clear_screen();
        io_draw_title(formula->expression);
        io_draw_wrapped_text(1, formula->name, 26);
        io_draw_wrapped_text(2, formula->const_note, 26);

        for (j = 0; j < visible_rows && (uint8_t)(scroll_offset + j) < item_count; ++j) {
            char line[27];
            size_t label_len = 0;
            uint8_t option_index = (uint8_t)(scroll_offset + j);
            const char *label = option_index < formula->option_count ? labels[option_index] : "Find formulas...";

            line[0] = option_index == *selected_index ? '>' : ' ';
            line[1] = ' ';
            if (option_index < formula->option_count) {
                line[2] = 'F';
                line[3] = 'i';
                line[4] = 'n';
                line[5] = 'd';
                line[6] = ' ';
                label_len = strlen(label);
                if (label_len > sizeof(line) - 8) {
                    label_len = sizeof(line) - 8;
                }
                memcpy(&line[7], label, label_len);
                line[7 + label_len] = '\0';
            } else {
                label_len = strlen(label);
                if (label_len > sizeof(line) - 3) {
                    label_len = sizeof(line) - 3;
                }
                memcpy(&line[2], label, label_len);
                line[2 + label_len] = '\0';
            }

            io_draw_wrapped_text((uint8_t)(4 + j), line, 26);
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
            if (*selected_index < formula->option_count) {
                *option_out = &formula->options[*selected_index];
                return MENU_STAY;
            } else {
                menu_result_t lookup_result = select_variable_lookup(formula);
                if (lookup_result == MENU_EXIT_APP) {
                    return MENU_EXIT_APP;
                }
            }
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
