#include <stdio.h>

#include "io_helpers.h"
#include "reference.h"

typedef struct {
    const char *title;
    const char *lines[6];
} reference_entry_t;

typedef struct {
    const char *name;
    uint8_t entry_count;
    const reference_entry_t *entries;
} reference_category_t;

static const reference_entry_t KINEMATICS_ENTRIES[] = {
    {
        "Vector definitions",
        {
            "r = x*i + y*j + z*k",
            "v = dr / dt",
            "a = dv / dt",
            "dr = r2 - r1",
            "v_avg = dr / dt",
            "a_avg = dv / dt"
        }
    },
    {
        "Scalars and magnitudes",
        {
            "speed avg: v = ds / dt",
            "instant speed: v = ds / dt",
            "v = vx*i + vy*j + vz*k",
            "a = ax*i + ay*j + az*k",
            "|v| = sqrt(vx^2 + vy^2 + vz^2)",
            "|a| = sqrt(ax^2 + ay^2 + az^2)"
        }
    },
    {
        "Uniform motion",
        {
            "a = 0",
            "v = const",
            "s = s0 + v*t",
            "ds = v*dt",
            "r = r0 + v*t",
            "v = ds / dt"
        }
    },
    {
        "UAM / UDM",
        {
            "a = const",
            "v = v0 + a*t",
            "s = s0 + v0*t + 0.5*a*t^2",
            "UDM: v = v0 - a*t",
            "UDM: s = s0 + v0*t - 0.5*a*t^2",
            ""
        }
    },
    {
        "No-time relation",
        {
            "v^2 = v0^2 + 2*a*(s - s0)",
            "UDM stop: 0 = v0^2 - 2*a*ds",
            "meeting: s1 + s2 = distance",
            "solve kinematics together",
            "",
            ""
        }
    },
    {
        "Angle between vectors",
        {
            "cos(phi) = (u.v) / (|u||v|)",
            "u.v = ux*vx + uy*vy + uz*vz",
            "For test tasks use:",
            "cos(phi) = (v.a) / (|v||a|)",
            "phi = arccos(...)",
            ""
        }
    }
};

static const reference_entry_t CIRCULAR_ENTRIES[] = {
    {
        "Angular basics",
        {
            "omega = dphi / dt",
            "alpha = domega / dt",
            "phi = 2*pi*N",
            "f = 1/T",
            "omega = 2*pi*f",
            "omega = 2*pi/T"
        }
    },
    {
        "Linear/angular",
        {
            "ds = r*dphi",
            "v = r*omega",
            "at = dv/dt = r*alpha",
            "ac = v^2/r = r*omega^2",
            "a = sqrt(at^2 + ac^2)",
            ""
        }
    },
    {
        "Uniform circular",
        {
            "alpha = 0",
            "omega = const",
            "phi = omega*t",
            "v = const size",
            "ac = v^2/r",
            "T = 2*pi/omega"
        }
    },
    {
        "Angular accel",
        {
            "alpha = const",
            "omega = omega0 + alpha*t",
            "phi = phi0 + omega0*t",
            "phi = phi0 + 0.5*alpha*t^2",
            "UDCM: omega = omega0-alpha*t",
            ""
        }
    },
    {
        "Revolutions",
        {
            "N = phi / (2*pi)",
            "phi = 2*pi*N",
            "T = 1 / f",
            "f = 1 / T",
            "omega = 2*pi*f = 2*pi/T",
            ""
        }
    }
};

static const reference_entry_t DYNAMICS_ENTRIES[] = {
    {
        "Newton laws",
        {
            "1) Fnet = 0 => rest/UM",
            "2) F = dp / dt",
            "   F = m*a for m const",
            "3) F21 = -F12",
            "inertia measure: mass m",
            ""
        }
    },
    {
        "Momentum/impulse",
        {
            "p = m*v",
            "F = dp / dt",
            "I = integral(F dt)",
            "F const => I = F*t",
            "I = dp = p2 - p1",
            ""
        }
    },
    {
        "Vector forms",
        {
            "p = m*v (vector form)",
            "F = dp/dt (vector form)",
            "If m = const => F = m*a",
            "F21 = -F12",
            "action and reaction pair",
            ""
        }
    }
};

static const reference_entry_t ENERGY_ENTRIES[] = {
    {
        "Work",
        {
            "W = F*d if F || d",
            "W = F*d*cos(phi)",
            "W = integral(F.dr)",
            "dW = F.dr",
            "unit: J = N*m",
            ""
        }
    },
    {
        "Energy",
        {
            "Ek = 0.5*m*v^2",
            "dEp = m*g*h",
            "E = Ep + Ek",
            "Work-energy: W = dEk",
            "mech E const (cons forces)",
            ""
        }
    },
    {
        "Power/springs",
        {
            "Pavg = W / t",
            "P = dW/dt = F*v",
            "Vector form: P = F . v",
            "Hooke: F = k*d",
            "spring work = 0.5*k*d^2",
            "k = F / d"
        }
    },
    {
        "Energy conservation",
        {
            "E = Ep + Ek = const",
            "Ep1 + Ek1 = Ep2 + Ek2",
            "dEp = -dEk",
            "cons. forces preserve E",
            "non-cons: friction losses",
            ""
        }
    },
    {
        "Key theorem forms",
        {
            "I = dp = m*(v2-v1)",
            "W = dEk = 0.5*m*(v2^2-v1^2)",
            "dEp = mgh",
            "s = m*(v^2-v0^2)/(2*F)",
            "t = m*(v-v0)/F",
            ""
        }
    }
};

static const reference_entry_t CALCULUS_ENTRIES[] = {
    {
        "Physics derivatives",
        {
            "Velocity: v = dx / dt",
            "Acceleration: a = dv / dt",
            "Angular speed: omega=dphi/dt",
            "Angular accel: alpha=domega/dt",
            "Power: P = dW / dt",
            "Force: F = dp / dt"
        }
    },
    {
        "Physics integrals",
        {
            "Velocity change: v = int a dt",
            "Position: r = int v dt",
            "Angular speed: omega=int alpha dt",
            "Impulse: I = int F dt",
            "Work: W = int F.dr",
            "Angle: phi = int omega dt"
        }
    },
    {
        "Test formulas",
        {
            "v = dr/dt, a = dv/dt",
            "v = int a dt, r = int v dt",
            "F = dp/dt, I = int F dt",
            "P = dW/dt, W = int F.dr",
            "omega=dphi/dt",
            "phi = int omega dt"
        }
    }
};

static const reference_category_t REFERENCE_CATEGORIES[] = {
    { "Kinematics", (uint8_t)(sizeof(KINEMATICS_ENTRIES) / sizeof(KINEMATICS_ENTRIES[0])), KINEMATICS_ENTRIES },
    { "Circular Motion", (uint8_t)(sizeof(CIRCULAR_ENTRIES) / sizeof(CIRCULAR_ENTRIES[0])), CIRCULAR_ENTRIES },
    { "Dynamics", (uint8_t)(sizeof(DYNAMICS_ENTRIES) / sizeof(DYNAMICS_ENTRIES[0])), DYNAMICS_ENTRIES },
    { "Work / Energy / Power", (uint8_t)(sizeof(ENERGY_ENTRIES) / sizeof(ENERGY_ENTRIES[0])), ENERGY_ENTRIES },
    { "Physics Calculus", (uint8_t)(sizeof(CALCULUS_ENTRIES) / sizeof(CALCULUS_ENTRIES[0])), CALCULUS_ENTRIES }
};

static menu_result_t reference_select_category(uint8_t *selected_index, const reference_category_t **category_out) {
    uint8_t i;
    uint8_t action;

    while (true) {
        io_clear_screen();
        io_draw_title("All Formulas");

        for (i = 0; i < 5; ++i) {
            char line[28];
            snprintf(line, sizeof(line), "%c %s", i == *selected_index ? '>' : ' ', REFERENCE_CATEGORIES[i].name);
            io_draw_wrapped_text((uint8_t)(2 + i), line, 26);
        }

        io_draw_footer("UP/DN OK MODE BACK");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && *selected_index > 0) {
            --(*selected_index);
        } else if (action == IO_MENU_DOWN && *selected_index < 4) {
            ++(*selected_index);
        } else if (action == IO_MENU_SELECT) {
            *category_out = &REFERENCE_CATEGORIES[*selected_index];
            return MENU_STAY;
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

static menu_result_t reference_show_entries(const reference_category_t *category) {
    uint8_t index = 0;

    while (true) {
        const reference_entry_t *entry = &category->entries[index];
        uint8_t action;
        uint8_t line = 2;
        uint8_t i;
        char header[28];

        io_clear_screen();
        snprintf(header, sizeof(header), "%s %u/%u", category->name, (unsigned)(index + 1), (unsigned)category->entry_count);
        io_draw_title(header);
        io_draw_wrapped_text(1, entry->title, 26);

        for (i = 0; i < 6 && line < 9; ++i) {
            if (entry->lines[i] != NULL && entry->lines[i][0] != '\0') {
                io_draw_wrapped_text(line++, entry->lines[i], 26);
            }
        }

        io_draw_footer("UP/DN PG ENTER NEXT");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && index > 0) {
            --index;
        } else if (action == IO_MENU_DOWN && index + 1 < category->entry_count) {
            ++index;
        } else if (action == IO_MENU_SELECT && index + 1 < category->entry_count) {
            ++index;
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

menu_result_t reference_run(void) {
    uint8_t selected_category = 0;

    while (true) {
        const reference_category_t *category = NULL;
        menu_result_t result = reference_select_category(&selected_category, &category);

        if (result != MENU_STAY) {
            return result;
        }

        result = reference_show_entries(category);
        if (result == MENU_EXIT_APP) {
            return MENU_EXIT_APP;
        }
    }
}
