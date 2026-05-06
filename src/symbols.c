#include <stdio.h>
#include <stdbool.h>
#include <string.h>

#include "io_helpers.h"
#include "symbols.h"
#include "theory.h"

typedef struct {
    const char *title;
    const char *lines[6];
    const char *definitions[20];
    const uint8_t *related_topics;
    uint8_t related_count;
} symbol_entry_t;

static const uint8_t BASIC_MOTION_TOPICS[] = { 0, 1, 2, 3, 4, 8, 9 };
static const uint8_t CIRCULAR_MOTION_TOPICS[] = { 15, 16, 17, 18, 19 };
static const uint8_t CIRCULAR_ACCEL_TOPICS[] = { 15, 16, 17, 19, 20 };
static const uint8_t DYNAMICS_TOPICS[] = { 8, 9, 10, 11, 12, 13 };
static const uint8_t ENERGY_TOPICS[] = { 4, 5, 6, 7, 20, 21, 31 };
static const uint8_t MORE_SYMBOLS_TOPICS[] = { 14, 18, 20, 21, 22, 25, 26, 27, 28, 29, 30, 31 };

static const symbol_entry_t SYMBOL_ENTRIES[] = {
    {
        "Kinematics 1",
        {
            "s = displacement [m]",
            "s0 = initial position [m]",
            "t = time [s]",
            "v = velocity / speed [m/s]",
            "v0 = initial velocity [m/s]",
            "a = acceleration [m/s^2]"
        },
        {
            "v_avg = ds/dt avg speed",
            "for time interval.",
            "v = ds/dt instant vel",
            "at exact time.",
            "a = dv/dt accel",
            "rate of vel change.",
            "Units:",
            "s,m [m], t [s]",
            "v,v0 [m/s]",
            "a [m/s^2]",
            NULL
        },
        BASIC_MOTION_TOPICS,
        (uint8_t)(sizeof(BASIC_MOTION_TOPICS) / sizeof(BASIC_MOTION_TOPICS[0]))
    },
    {
        "Kinematics 2",
        {
            "r = position vector [m]",
            "dr = change of r [m]",
            "d = distance [m]",
            "x,y,z = coordinates [m]",
            "vx,vy,vz = vel comps [m/s]",
            "ax,ay,az = acc comps [m/s^2]"
        },
        {
            "r = position vector",
            "of body in space.",
            "x,y,z coordinates",
            "describe position.",
            "dr = change of r.",
            "d = traveled distance.",
            "vx,vy,vz velocity",
            "components.",
            "ax,ay,az accel",
            "components.",
            NULL
        },
        BASIC_MOTION_TOPICS,
        (uint8_t)(sizeof(BASIC_MOTION_TOPICS) / sizeof(BASIC_MOTION_TOPICS[0]))
    },
    {
        "Circular 1",
        {
            "r = radius [m]",
            "phi = angle [rad]",
            "omega = angular speed [rad/s]",
            "omega0 = init ang speed [rad/s]",
            "alpha = ang accel [rad/s^2]",
            "N = revolutions [rev]"
        },
        {
            "omega = dphi/dt",
            "angular velocity.",
            "It shows how fast",
            "body rotates.",
            "alpha = domega/dt",
            "angular accel.",
            "It shows how fast",
            "omega changes.",
            "phi angle [rad]",
            "N revolutions.",
            NULL
        },
        CIRCULAR_MOTION_TOPICS,
        (uint8_t)(sizeof(CIRCULAR_MOTION_TOPICS) / sizeof(CIRCULAR_MOTION_TOPICS[0]))
    },
    {
        "Circular 2",
        {
            "a_t = tangential accel [m/s^2]",
            "a_c = centripetal accel [m/s^2]",
            "T = period [s]",
            "f = frequency [Hz]",
            "beta = angle [rad or deg]",
            "pi = 3.14159..."
        },
        {
            "a_t = r*alpha",
            "changes speed.",
            "a_c = v^2/r=r*omega^2",
            "changes direction.",
            "T = one full period.",
            "f = 1/T frequency.",
            "beta = angle between",
            "total accel and vel.",
            "pi = 3.14159...",
            NULL
        },
        CIRCULAR_ACCEL_TOPICS,
        (uint8_t)(sizeof(CIRCULAR_ACCEL_TOPICS) / sizeof(CIRCULAR_ACCEL_TOPICS[0]))
    },
    {
        "Dynamics 1",
        {
            "F = force [N]",
            "m = mass [kg]",
            "p = momentum [kg*m/s]",
            "I = impulse [N*s]",
            "g = grav accel [m/s^2]",
            "N = normal force [N]"
        },
        {
            "F = net force.",
            "m = mass, measure",
            "of inertia.",
            "p = m*v momentum.",
            "I = integral F dt",
            "impulse.",
            "g grav accel near",
            "Earth [m/s^2].",
            "N = normal force",
            "from support.",
            NULL
        },
        DYNAMICS_TOPICS,
        (uint8_t)(sizeof(DYNAMICS_TOPICS) / sizeof(DYNAMICS_TOPICS[0]))
    },
    {
        "Dynamics 2",
        {
            "Fext = ext force [N]",
            "Fnet = net force [N]",
            "F12,F21 = action/reac [N]",
            "k = force slope [N/s]",
            "dp = change momentum",
            "dt = time interval [s]"
        },
        {
            "Fext = external force",
            "on system.",
            "Fnet = vector sum of",
            "all forces.",
            "F21=-F12 Newton 3.",
            "k in F=F0+k*t is",
            "rate of force change.",
            "dp change in p,",
            "dt time interval.",
            NULL
        },
        DYNAMICS_TOPICS,
        (uint8_t)(sizeof(DYNAMICS_TOPICS) / sizeof(DYNAMICS_TOPICS[0]))
    },
    {
        "Energy 1",
        {
            "W = work [J]",
            "P = power [W]",
            "Pavg = avg power [W]",
            "Ek = kinetic energy [J]",
            "Ep = potential energy [J]",
            "E = mechanical energy [J]"
        },
        {
            "W = work by force.",
            "P = dW/dt power.",
            "Pavg = W/t avg power.",
            "Ek = 0.5*m*v^2",
            "kinetic energy.",
            "Ep = potential energy.",
            "E = Ek + Ep mech",
            "energy.",
            "Units: J, W.",
            NULL
        },
        ENERGY_TOPICS,
        (uint8_t)(sizeof(ENERGY_TOPICS) / sizeof(ENERGY_TOPICS[0]))
    },
    {
        "Energy 2",
        {
            "dEp = change in Ep [J]",
            "dEk = change in Ek [J]",
            "h = height [m]",
            "h0,h1,h2 = heights [m]",
            "hmax = max height [m]",
            "phi = angle force/disp"
        },
        {
            "dEp = change in",
            "potential energy.",
            "dEk = change in",
            "kinetic energy.",
            "h heights in vertical",
            "motion.",
            "hmax = max height.",
            "phi is angle between",
            "force and displacement.",
            NULL
        },
        ENERGY_TOPICS,
        (uint8_t)(sizeof(ENERGY_TOPICS) / sizeof(ENERGY_TOPICS[0]))
    },
    {
        "Rigid body",
        {
            "M = torque [N*m]",
            "L = ang momentum",
            "I = inertia [kg*m^2]",
            "I0 = base inertia",
            "l = rod length [m]",
            "a = axis offset [m]"
        },
        {
            "M = torque, rotational",
            "effect of force.",
            "L = I*omega angular",
            "momentum.",
            "I inertia resists",
            "rotation.",
            "I0 base inertia about",
            "geom axis.",
            "l length, a offset.",
            NULL
        },
        MORE_SYMBOLS_TOPICS,
        (uint8_t)(sizeof(MORE_SYMBOLS_TOPICS) / sizeof(MORE_SYMBOLS_TOPICS[0]))
    },
    {
        "SHM 1",
        {
            "A = amplitude [m]",
            "x = displacement [m]",
            "k = spring const [N/m]",
            "omega = ang freq [rad/s]",
            "phi = phase const [rad]",
            "T = period [s]"
        },
        {
            "A = max displacement.",
            "x = current disp from",
            "equilibrium.",
            "k spring constant.",
            "omega = ang freq",
            "of SHM.",
            "phi = initial phase.",
            "T = one oscillation.",
            NULL
        },
        MORE_SYMBOLS_TOPICS,
        (uint8_t)(sizeof(MORE_SYMBOLS_TOPICS) / sizeof(MORE_SYMBOLS_TOPICS[0]))
    },
    {
        "SHM 2",
        {
            "f = frequency [Hz]",
            "vmax = max speed [m/s]",
            "amax = max accel [m/s^2]",
            "Fmax = max force [N]",
            "Ek = kin energy [J]",
            "Ep = pot energy [J]"
        },
        {
            "f = oscillations per s",
            "vmax = A*omega",
            "max SHM speed.",
            "amax = A*omega^2",
            "max SHM accel.",
            "Fmax = k*A or",
            "m*omega^2*A.",
            "Ek, Ep are SHM",
            "energy parts.",
            NULL
        },
        MORE_SYMBOLS_TOPICS,
        (uint8_t)(sizeof(MORE_SYMBOLS_TOPICS) / sizeof(MORE_SYMBOLS_TOPICS[0]))
    },
    {
        "Calculus",
        {
            "dx/dt = derivative",
            "dr/dt = vel deriv",
            "dv/dt = accel deriv",
            "int = integral sign",
            "S a dt = integral a",
            "S F dr = work integral"
        },
        {
            "d/dt means derivative",
            "with respect to time.",
            "dx/dt instant vel",
            "in 1D motion.",
            "dv/dt accel.",
            "Integral sums small",
            "changes over interval.",
            "S a dt gives v,",
            "S F dr gives work.",
            NULL
        },
        MORE_SYMBOLS_TOPICS,
        (uint8_t)(sizeof(MORE_SYMBOLS_TOPICS) / sizeof(MORE_SYMBOLS_TOPICS[0]))
    },
    {
        "Common units",
        {
            "m = meter",
            "s = second",
            "N = newton",
            "J = joule",
            "W = watt",
            "Hz = s^-1"
        },
        {
            "m meter, s second.",
            "N newton force unit.",
            "J joule energy unit.",
            "W watt power unit.",
            "Hz = 1/s frequency.",
            "rad = angle unit.",
            NULL
        },
        MORE_SYMBOLS_TOPICS,
        (uint8_t)(sizeof(MORE_SYMBOLS_TOPICS) / sizeof(MORE_SYMBOLS_TOPICS[0]))
    }
};

static const char *theory_title_from_index(uint8_t index) {
    switch (index) {
        case 0: return "Define avg/inst v";
        case 1: return "Define accel";
        case 2: return "Const accel motion";
        case 3: return "Free fall";
        case 4: return "Upward energy";
        case 5: return "Cons/noncons force";
        case 6: return "Define Ek";
        case 7: return "Define Ep";
        case 8: return "What is Fnet";
        case 9: return "Newton 1 law";
        case 10: return "Newton 3 law";
        case 11: return "Mass vs weight";
        case 12: return "Friction force";
        case 13: return "Hooke law";
        case 14: return "Spring energy";
        case 15: return "Define angle phi";
        case 16: return "Find revolutions";
        case 17: return "Rot motion eqs";
        case 18: return "v and omega";
        case 19: return "at and alpha";
        case 20: return "Work by torque";
        case 21: return "Rotational power";
        case 22: return "Inertia and axis";
        case 23: return "Centre of mass";
        case 24: return "Equilibrium cond";
        case 25: return "SHM max speed";
        case 26: return "SHM max accel";
        case 27: return "SHM speed by x";
        case 28: return "SHM phase";
        case 29: return "Damped SHM";
        case 30: return "More units";
        case 31: return "SHM energy pos";
        default: return "Theory topic";
    }
}

static menu_result_t symbols_show_related(const symbol_entry_t *entry) {
    uint8_t selected = 0;

    while (true) {
        uint8_t start = 0;
        uint8_t i = 0;
        uint8_t line = 2;
        uint8_t action;

        if (selected >= 6) {
            start = selected - 5;
        }

        io_clear_screen();
        io_draw_title("Related theory");
        io_draw_wrapped_text(1, entry->title, 26);

        for (i = start; i < entry->related_count && line < 9; ++i, ++line) {
            char row[28];
            snprintf(row, sizeof(row), "%c %s", i == selected ? '>' : ' ', theory_title_from_index(entry->related_topics[i]));
            io_draw_wrapped_text(line, row, 26);
        }

        io_draw_footer("UP/DN OK MD/CLR BK");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && selected > 0) {
            --selected;
        } else if (action == IO_MENU_DOWN && selected + 1 < entry->related_count) {
            ++selected;
        } else if (action == IO_MENU_SELECT) {
            menu_result_t result = theory_show_topic_by_index(entry->related_topics[selected]);
            if (result == MENU_EXIT_APP) {
                return MENU_EXIT_APP;
            }
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

static void copy_trimmed(char *dst, size_t dst_size, const char *start, size_t len) {
    size_t begin = 0;
    size_t end = len;
    size_t out = 0;

    while (begin < len && start[begin] == ' ') {
        ++begin;
    }
    while (end > begin && start[end - 1] == ' ') {
        --end;
    }

    while (begin < end && out + 1 < dst_size) {
        dst[out++] = start[begin++];
    }
    dst[out] = '\0';
}

static void append_wrapped_definition(char lines[40][96], uint8_t *line_count, const char *text) {
    size_t len = strlen(text);
    size_t start = 0;

    if (text[0] == '\0' || *line_count >= 40) {
        return;
    }

    while (start < len && *line_count < 40) {
        size_t chunk = len - start;
        size_t cut = 26;
        size_t end;

        if (chunk <= 26) {
            snprintf(lines[(*line_count)++], 96, "%s", text + start);
            return;
        }

        end = start + cut;
        while (end > start && text[end] != ' ') {
            --end;
        }
        if (end == start) {
            end = start + cut;
        }

        copy_trimmed(lines[*line_count], 96, text + start, end - start);
        ++(*line_count);

        start = end;
        while (start < len && text[start] == ' ') {
            ++start;
        }
    }
}

static void append_formula_line(char lines[40][96], uint8_t *line_count, const char *text) {
    append_wrapped_definition(lines, line_count, text);
}

static void parse_symbol_label(const char *label, char *symbol, size_t symbol_size,
                               char *meaning, size_t meaning_size,
                               char *unit, size_t unit_size) {
    const char *eq = strchr(label, '=');
    const char *lb = strchr(label, '[');
    const char *rb = strchr(label, ']');

    symbol[0] = '\0';
    meaning[0] = '\0';
    unit[0] = '\0';

    if (eq != NULL) {
        copy_trimmed(symbol, symbol_size, label, (size_t)(eq - label));
        if (lb != NULL && lb > eq) {
            copy_trimmed(meaning, meaning_size, eq + 1, (size_t)(lb - (eq + 1)));
        } else {
            copy_trimmed(meaning, meaning_size, eq + 1, strlen(eq + 1));
        }
    } else {
        copy_trimmed(symbol, symbol_size, label, strlen(label));
    }

    if (lb != NULL && rb != NULL && rb > lb) {
        copy_trimmed(unit, unit_size, lb + 1, (size_t)(rb - (lb + 1)));
    }
}

static void get_symbol_question(const char *symbol, const char *meaning_in,
                                char *question, size_t question_size) {
    snprintf(question, question_size, "Define %s. Give formula, meaning and unit.",
             symbol[0] ? symbol : "this quantity");

    if (strcmp(symbol, "omega") == 0) {
        snprintf(question, question_size,
                 "Define angular velocity. Write formula, explain quantities and unit.");
    } else if (strcmp(symbol, "alpha") == 0) {
        snprintf(question, question_size,
                 "Define angular acceleration. Write formula, explain quantities and unit.");
    } else if (strcmp(symbol, "a_t") == 0) {
        snprintf(question, question_size,
                 "Define tangential acceleration. Write formula, meaning and unit.");
    } else if (strcmp(symbol, "a_c") == 0) {
        snprintf(question, question_size,
                 "Define centripetal acceleration. Write formula, meaning and unit.");
    } else if (strcmp(symbol, "F") == 0) {
        snprintf(question, question_size,
                 "Define force. Write formula, explain quantities and unit.");
    } else if (strcmp(symbol, "p") == 0) {
        snprintf(question, question_size,
                 "Define linear momentum. Write formula, meaning and unit.");
    } else if (strcmp(symbol, "I") == 0 && strstr(meaning_in, "impulse") != NULL) {
        snprintf(question, question_size,
                 "Define impulse. Write formula, relation to momentum and unit.");
    } else if (strcmp(symbol, "I") == 0) {
        snprintf(question, question_size,
                 "Define moment of inertia. Explain meaning and unit.");
    } else if (strcmp(symbol, "W") == 0) {
        snprintf(question, question_size,
                 "Define work. Write formula, explain quantities and unit.");
    } else if (strcmp(symbol, "P") == 0 || strcmp(symbol, "Pavg") == 0) {
        snprintf(question, question_size,
                 "Define power. Write formula, explain quantities and unit.");
    } else if (strcmp(symbol, "Ek") == 0) {
        snprintf(question, question_size,
                 "Define kinetic energy. Write formula, meaning and unit.");
    } else if (strcmp(symbol, "Ep") == 0) {
        snprintf(question, question_size,
                 "Define potential energy. Write formula, meaning and unit.");
    } else if (strcmp(symbol, "E") == 0) {
        snprintf(question, question_size,
                 "Define mechanical energy. Write formula, meaning and unit.");
    } else if (strcmp(symbol, "M") == 0) {
        snprintf(question, question_size,
                 "Define torque. Write formula, explain quantities and unit.");
    } else if (strcmp(symbol, "L") == 0) {
        snprintf(question, question_size,
                 "Define angular momentum. Write formula, meaning and unit.");
    } else if (strcmp(symbol, "A") == 0) {
        snprintf(question, question_size,
                 "Define amplitude. Explain meaning and unit.");
    } else if (strcmp(symbol, "x") == 0) {
        snprintf(question, question_size,
                 "Define displacement x. Explain meaning and unit.");
    } else if (strcmp(symbol, "T") == 0) {
        snprintf(question, question_size,
                 "Define period. Write relation to f and omega, and unit.");
    } else if (strcmp(symbol, "f") == 0) {
        snprintf(question, question_size,
                 "Define frequency. Write relation to T and omega, and unit.");
    } else if (strcmp(symbol, "phi") == 0) {
        snprintf(question, question_size,
                 "Define angular displacement / phase phi. Explain meaning and unit.");
    } else if (strcmp(symbol, "N") == 0 && strstr(meaning_in, "normal force") != NULL) {
        snprintf(question, question_size,
                 "Define normal force. Write formula, meaning and unit.");
    } else if (strcmp(symbol, "N") == 0) {
        snprintf(question, question_size,
                 "Define number of revolutions. Write relation to phi and unit.");
    } else if (strcmp(symbol, "m") == 0) {
        snprintf(question, question_size,
                 "Define mass. Explain meaning and unit.");
    } else if (strcmp(symbol, "g") == 0) {
        snprintf(question, question_size,
                 "Define gravitational acceleration. Explain meaning and unit.");
    } else if (strcmp(symbol, "k") == 0 && strstr(meaning_in, "spring") != NULL) {
        snprintf(question, question_size,
                 "Define spring constant. Write formula, meaning and unit.");
    } else if (strcmp(symbol, "k") == 0) {
        snprintf(question, question_size,
                 "Define coefficient / rate k. Explain meaning and unit.");
    }
}

static bool get_exact_label_definition(const char *selected_label,
                                       char *question, size_t question_size,
                                       char *formula, size_t formula_size,
                                       char *unit, size_t unit_size,
                                       char *meaning, size_t meaning_size,
                                       char *conclusion, size_t conclusion_size) {
    if (strcmp(selected_label, "s0 = initial position [m]") == 0) {
        snprintf(question, question_size, "Define initial position s0. Explain meaning and unit.");
        snprintf(formula, formula_size, "s = s0 + v0*t + 0.5*a*t^2");
        snprintf(unit, unit_size, "m");
        snprintf(meaning, meaning_size, "s0 is the initial position of a body at the starting moment of motion.");
        snprintf(conclusion, conclusion_size, "s0 shows where the motion starts.");
        return true;
    }
    if (strcmp(selected_label, "dr = change of r [m]") == 0) {
        snprintf(question, question_size, "Define change of position vector dr. Explain meaning and unit.");
        snprintf(formula, formula_size, "dr = r2 - r1");
        snprintf(unit, unit_size, "m");
        snprintf(meaning, meaning_size, "dr is the change of position vector between two positions of a body.");
        snprintf(conclusion, conclusion_size, "dr shows how the position vector changes.");
        return true;
    }
    if (strcmp(selected_label, "d = distance [m]") == 0) {
        snprintf(question, question_size, "Define distance d. Explain meaning and unit.");
        snprintf(formula, formula_size, "d = |r| or path length");
        snprintf(unit, unit_size, "m");
        snprintf(meaning, meaning_size, "Distance is the total path length travelled by a body.");
        snprintf(conclusion, conclusion_size, "Distance is scalar and depends on the path.");
        return true;
    }
    if (strcmp(selected_label, "x,y,z = coordinates [m]") == 0) {
        snprintf(question, question_size, "Define coordinates x, y, z. Explain meaning and unit.");
        snprintf(formula, formula_size, "r = x*i + y*j + z*k");
        snprintf(unit, unit_size, "m");
        snprintf(meaning, meaning_size, "x, y and z are coordinates that describe the position of a body in space.");
        snprintf(conclusion, conclusion_size, "Coordinates specify location relative to the axes.");
        return true;
    }
    if (strcmp(selected_label, "vx,vy,vz = vel comps [m/s]") == 0) {
        snprintf(question, question_size, "Define velocity components vx, vy, vz. Explain meaning and unit.");
        snprintf(formula, formula_size, "|v| = sqrt(vx^2+vy^2+vz^2)");
        snprintf(unit, unit_size, "m/s");
        snprintf(meaning, meaning_size, "vx, vy and vz are components of the velocity vector along the coordinate axes.");
        snprintf(conclusion, conclusion_size, "Velocity components combine to give the full velocity vector.");
        return true;
    }
    if (strcmp(selected_label, "ax,ay,az = acc comps [m/s^2]") == 0) {
        snprintf(question, question_size, "Define acceleration components ax, ay, az. Explain meaning and unit.");
        snprintf(formula, formula_size, "|a| = sqrt(ax^2+ay^2+az^2)");
        snprintf(unit, unit_size, "m/s^2");
        snprintf(meaning, meaning_size, "ax, ay and az are components of the acceleration vector along the coordinate axes.");
        snprintf(conclusion, conclusion_size, "Acceleration components combine to give the full acceleration vector.");
        return true;
    }
    if (strcmp(selected_label, "omega0 = init ang speed [rad/s]") == 0) {
        snprintf(question, question_size, "Define initial angular velocity omega0. Explain meaning and unit.");
        snprintf(formula, formula_size, "omega = omega0 + alpha*t");
        snprintf(unit, unit_size, "rad/s or s^-1");
        snprintf(meaning, meaning_size, "omega0 is the angular velocity of a body at the initial moment t = 0.");
        snprintf(conclusion, conclusion_size, "omega0 gives the starting rotation rate.");
        return true;
    }
    if (strcmp(selected_label, "beta = angle [rad or deg]") == 0) {
        snprintf(question, question_size, "Define angle beta. Write relation, meaning and unit.");
        snprintf(formula, formula_size, "tan(beta) = a_c/a_t");
        snprintf(unit, unit_size, "rad or deg");
        snprintf(meaning, meaning_size, "beta is the angle between total acceleration and linear velocity in circular motion.");
        snprintf(conclusion, conclusion_size, "beta describes the direction of total acceleration.");
        return true;
    }
    if (strcmp(selected_label, "pi = 3.14159...") == 0) {
        snprintf(question, question_size, "Define pi. Explain meaning and use.");
        snprintf(formula, formula_size, "pi = 3.14159...");
        snprintf(unit, unit_size, "dimensionless");
        snprintf(meaning, meaning_size, "pi is the mathematical constant equal to the ratio of a circle circumference to its diameter.");
        snprintf(conclusion, conclusion_size, "pi appears in circular motion and oscillation formulas.");
        return true;
    }
    if (strcmp(selected_label, "Fext = ext force [N]") == 0) {
        snprintf(question, question_size, "Define external force Fext. Explain meaning and unit.");
        snprintf(formula, formula_size, "If Fext = 0, then P = const");
        snprintf(unit, unit_size, "N");
        snprintf(meaning, meaning_size, "Fext is the net external force acting on a system from outside the system.");
        snprintf(conclusion, conclusion_size, "If external force is zero, total momentum is conserved.");
        return true;
    }
    if (strcmp(selected_label, "Fnet = net force [N]") == 0) {
        snprintf(question, question_size, "Define net force Fnet. Explain meaning and unit.");
        snprintf(formula, formula_size, "Fnet = vector sum of all forces");
        snprintf(unit, unit_size, "N");
        snprintf(meaning, meaning_size, "Fnet is the vector sum of all forces acting on a body.");
        snprintf(conclusion, conclusion_size, "Net force determines the acceleration of a body.");
        return true;
    }
    if (strcmp(selected_label, "F12,F21 = action/reac [N]") == 0) {
        snprintf(question, question_size, "Define action and reaction forces F12 and F21.");
        snprintf(formula, formula_size, "F21 = -F12");
        snprintf(unit, unit_size, "N");
        snprintf(meaning, meaning_size, "F12 and F21 are the pair of forces from Newton's third law acting on different bodies.");
        snprintf(conclusion, conclusion_size, "They are equal in magnitude and opposite in direction.");
        return true;
    }
    if (strcmp(selected_label, "k = force slope [N/s]") == 0) {
        snprintf(question, question_size, "Define force slope k. Explain meaning and unit.");
        snprintf(formula, formula_size, "F = F0 + k*t");
        snprintf(unit, unit_size, "N/s");
        snprintf(meaning, meaning_size, "k is the rate at which force changes with time.");
        snprintf(conclusion, conclusion_size, "Larger k means the force grows faster.");
        return true;
    }
    if (strcmp(selected_label, "dp = change momentum") == 0) {
        snprintf(question, question_size, "Define change in momentum dp. Explain meaning and unit.");
        snprintf(formula, formula_size, "dp = p2 - p1");
        snprintf(unit, unit_size, "kg*m/s");
        snprintf(meaning, meaning_size, "dp is the change in linear momentum between two states.");
        snprintf(conclusion, conclusion_size, "Impulse is equal to dp.");
        return true;
    }
    if (strcmp(selected_label, "dt = time interval [s]") == 0) {
        snprintf(question, question_size, "Define time interval dt. Explain meaning and unit.");
        snprintf(formula, formula_size, "Used in d/dt and integral ... dt");
        snprintf(unit, unit_size, "s");
        snprintf(meaning, meaning_size, "dt is a very small time interval used in derivatives and integrals.");
        snprintf(conclusion, conclusion_size, "dt helps describe continuous change with time.");
        return true;
    }
    if (strcmp(selected_label, "dEp = change in Ep [J]") == 0) {
        snprintf(question, question_size, "Define change in potential energy dEp. Explain meaning and unit.");
        snprintf(formula, formula_size, "dEp = Ep2 - Ep1");
        snprintf(unit, unit_size, "J");
        snprintf(meaning, meaning_size, "dEp is the change in potential energy between two positions or states.");
        snprintf(conclusion, conclusion_size, "Potential energy change tracks how position energy varies.");
        return true;
    }
    if (strcmp(selected_label, "dEk = change in Ek [J]") == 0) {
        snprintf(question, question_size, "Define change in kinetic energy dEk. Explain meaning and unit.");
        snprintf(formula, formula_size, "dEk = Ek2 - Ek1");
        snprintf(unit, unit_size, "J");
        snprintf(meaning, meaning_size, "dEk is the change in kinetic energy between two states of motion.");
        snprintf(conclusion, conclusion_size, "By the work-energy theorem, net work equals dEk.");
        return true;
    }
    if (strcmp(selected_label, "h = height [m]") == 0) {
        snprintf(question, question_size, "Define height h. Explain meaning and unit.");
        snprintf(formula, formula_size, "Ep = m*g*h");
        snprintf(unit, unit_size, "m");
        snprintf(meaning, meaning_size, "h is the vertical position of a body relative to a chosen reference level.");
        snprintf(conclusion, conclusion_size, "Greater height gives greater gravitational potential energy.");
        return true;
    }
    if (strcmp(selected_label, "h0,h1,h2 = heights [m]") == 0) {
        snprintf(question, question_size, "Define heights h0, h1, h2. Explain meaning and unit.");
        snprintf(formula, formula_size, "Ep = m*g*h");
        snprintf(unit, unit_size, "m");
        snprintf(meaning, meaning_size, "h0, h1 and h2 are heights of a body at different moments or positions.");
        snprintf(conclusion, conclusion_size, "These heights are used when comparing energy at different points.");
        return true;
    }
    if (strcmp(selected_label, "hmax = max height [m]") == 0) {
        snprintf(question, question_size, "Define maximum height hmax. Explain meaning and unit.");
        snprintf(formula, formula_size, "hmax = v0^2/(2*g)");
        snprintf(unit, unit_size, "m");
        snprintf(meaning, meaning_size, "hmax is the greatest height reached by a body in vertical upward motion.");
        snprintf(conclusion, conclusion_size, "At maximum height, velocity becomes zero.");
        return true;
    }
    if (strcmp(selected_label, "phi = angle force/disp") == 0) {
        snprintf(question, question_size, "Define angle phi in work formula. Explain meaning and unit.");
        snprintf(formula, formula_size, "W = F*s*cos(phi)");
        snprintf(unit, unit_size, "rad or deg");
        snprintf(meaning, meaning_size, "phi is the angle between force and displacement.");
        snprintf(conclusion, conclusion_size, "The sign and value of work depend on this angle.");
        return true;
    }
    if (strcmp(selected_label, "I0 = base inertia") == 0) {
        snprintf(question, question_size, "Define base moment of inertia I0. Explain meaning and unit.");
        snprintf(formula, formula_size, "I = I0 + m*a^2");
        snprintf(unit, unit_size, "kg*m^2");
        snprintf(meaning, meaning_size, "I0 is the moment of inertia about the main or central axis before using the parallel-axis theorem.");
        snprintf(conclusion, conclusion_size, "I0 is the starting inertia value for shifted-axis problems.");
        return true;
    }
    if (strcmp(selected_label, "l = rod length [m]") == 0) {
        snprintf(question, question_size, "Define rod length l. Explain meaning and unit.");
        snprintf(formula, formula_size, "Irod = (1/12)*m*l^2");
        snprintf(unit, unit_size, "m");
        snprintf(meaning, meaning_size, "l is the length of a rod used in rotational inertia formulas.");
        snprintf(conclusion, conclusion_size, "Changing rod length changes the moment of inertia.");
        return true;
    }
    if (strcmp(selected_label, "a = axis offset [m]") == 0) {
        snprintf(question, question_size, "Define axis offset a. Explain meaning and unit.");
        snprintf(formula, formula_size, "I = I0 + m*a^2");
        snprintf(unit, unit_size, "m");
        snprintf(meaning, meaning_size, "a is the distance between two parallel axes in the parallel-axis theorem.");
        snprintf(conclusion, conclusion_size, "Larger axis offset gives larger moment of inertia.");
        return true;
    }
    if (strcmp(selected_label, "dx/dt = derivative") == 0) {
        snprintf(question, question_size, "Define dx/dt. Explain meaning and unit.");
        snprintf(formula, formula_size, "v = dx/dt");
        snprintf(unit, unit_size, "m/s");
        snprintf(meaning, meaning_size, "dx/dt is the derivative of position with respect to time and gives instantaneous velocity.");
        snprintf(conclusion, conclusion_size, "Derivative gives the exact rate of change at one moment.");
        return true;
    }
    if (strcmp(selected_label, "dr/dt = vel deriv") == 0) {
        snprintf(question, question_size, "Define dr/dt. Explain meaning and unit.");
        snprintf(formula, formula_size, "v = dr/dt");
        snprintf(unit, unit_size, "m/s");
        snprintf(meaning, meaning_size, "dr/dt is the derivative of position vector and gives vector velocity.");
        snprintf(conclusion, conclusion_size, "Use dr/dt in vector kinematics.");
        return true;
    }
    if (strcmp(selected_label, "dv/dt = accel deriv") == 0) {
        snprintf(question, question_size, "Define dv/dt. Explain meaning and unit.");
        snprintf(formula, formula_size, "a = dv/dt");
        snprintf(unit, unit_size, "m/s^2");
        snprintf(meaning, meaning_size, "dv/dt is the derivative of velocity with respect to time and gives acceleration.");
        snprintf(conclusion, conclusion_size, "Use dv/dt when velocity changes with time.");
        return true;
    }
    if (strcmp(selected_label, "int = integral sign") == 0) {
        snprintf(question, question_size, "Define the integral sign. Explain meaning.");
        snprintf(formula, formula_size, "integral sums small contributions");
        snprintf(unit, unit_size, "depends on quantity");
        snprintf(meaning, meaning_size, "The integral sign means summation of infinitely small contributions over an interval.");
        snprintf(conclusion, conclusion_size, "Integrals are used for variable force, acceleration, work and similar quantities.");
        return true;
    }
    if (strcmp(selected_label, "S a dt = integral a") == 0) {
        snprintf(question, question_size, "Explain integral of acceleration with respect to time.");
        snprintf(formula, formula_size, "v = integral(a dt) + C");
        snprintf(unit, unit_size, "m/s");
        snprintf(meaning, meaning_size, "Integrating acceleration over time gives velocity.");
        snprintf(conclusion, conclusion_size, "Use initial conditions to find the constant C.");
        return true;
    }
    if (strcmp(selected_label, "S F dr = work integral") == 0) {
        snprintf(question, question_size, "Explain the work integral. Write formula and meaning.");
        snprintf(formula, formula_size, "W = integral(F dr)");
        snprintf(unit, unit_size, "J");
        snprintf(meaning, meaning_size, "The work integral is used when force changes with position or acts along a curved path.");
        snprintf(conclusion, conclusion_size, "Integrating force along displacement gives total work.");
        return true;
    }
    if (strcmp(selected_label, "m = meter") == 0) {
        snprintf(question, question_size, "Define meter. Explain what quantity it measures.");
        snprintf(formula, formula_size, "Used for distance, displacement, radius, height");
        snprintf(unit, unit_size, "m");
        snprintf(meaning, meaning_size, "Meter is the SI unit of length.");
        snprintf(conclusion, conclusion_size, "Length quantities in mechanics are usually measured in meters.");
        return true;
    }
    if (strcmp(selected_label, "s = second") == 0) {
        snprintf(question, question_size, "Define second. Explain what quantity it measures.");
        snprintf(formula, formula_size, "Used for time, period and intervals");
        snprintf(unit, unit_size, "s");
        snprintf(meaning, meaning_size, "Second is the SI unit of time.");
        snprintf(conclusion, conclusion_size, "Time quantities in mechanics are measured in seconds.");
        return true;
    }
    if (strcmp(selected_label, "N = newton") == 0) {
        snprintf(question, question_size, "Define newton. Explain what quantity it measures.");
        snprintf(formula, formula_size, "1 N = 1 kg*m/s^2");
        snprintf(unit, unit_size, "N");
        snprintf(meaning, meaning_size, "Newton is the SI unit of force.");
        snprintf(conclusion, conclusion_size, "Forces such as weight, normal force and friction are measured in newtons.");
        return true;
    }
    if (strcmp(selected_label, "J = joule") == 0) {
        snprintf(question, question_size, "Define joule. Explain what quantity it measures.");
        snprintf(formula, formula_size, "1 J = 1 N*m");
        snprintf(unit, unit_size, "J");
        snprintf(meaning, meaning_size, "Joule is the SI unit of work and energy.");
        snprintf(conclusion, conclusion_size, "Work, kinetic energy and potential energy are measured in joules.");
        return true;
    }
    if (strcmp(selected_label, "W = watt") == 0) {
        snprintf(question, question_size, "Define watt. Explain what quantity it measures.");
        snprintf(formula, formula_size, "1 W = 1 J/s");
        snprintf(unit, unit_size, "W");
        snprintf(meaning, meaning_size, "Watt is the SI unit of power.");
        snprintf(conclusion, conclusion_size, "Power tells how much work is done per second.");
        return true;
    }
    if (strcmp(selected_label, "Hz = s^-1") == 0) {
        snprintf(question, question_size, "Define hertz. Explain what quantity it measures.");
        snprintf(formula, formula_size, "1 Hz = 1/s");
        snprintf(unit, unit_size, "Hz");
        snprintf(meaning, meaning_size, "Hertz is the SI unit of frequency.");
        snprintf(conclusion, conclusion_size, "Frequency tells how many oscillations or revolutions happen per second.");
        return true;
    }
    return false;
}

static void get_symbol_formula_and_meaning(const char *symbol, const char *meaning_in,
                                           char *formula, size_t formula_size,
                                           char *meaning, size_t meaning_size,
                                           char *conclusion, size_t conclusion_size) {
    snprintf(formula, formula_size, "No single formula.");
    snprintf(meaning, meaning_size, "%s.", meaning_in[0] ? meaning_in : "Main physics quantity");
    snprintf(conclusion, conclusion_size, "Use related theory/formulas.");

    if (strcmp(symbol, "v") == 0) {
        snprintf(formula, formula_size, "v = ds/dt or dr/dt");
        snprintf(meaning, meaning_size, "Velocity is the rate of change of position with time.");
        snprintf(conclusion, conclusion_size, "Velocity gives speed and direction of motion.");
    } else if (strcmp(symbol, "v0") == 0) {
        snprintf(formula, formula_size, "v = v0 + a*t");
        snprintf(meaning, meaning_size, "v0 is initial velocity at t = 0.");
        snprintf(conclusion, conclusion_size, "v0 is the starting velocity.");
    } else if (strcmp(symbol, "a") == 0) {
        snprintf(formula, formula_size, "a = dv/dt");
        snprintf(meaning, meaning_size, "Acceleration shows how quickly velocity changes.");
        snprintf(conclusion, conclusion_size, "If velocity is constant, a = 0.");
    } else if (strcmp(symbol, "s") == 0) {
        snprintf(formula, formula_size, "v_avg = delta s / delta t");
        snprintf(meaning, meaning_size, "s is displacement / position along the path.");
        snprintf(conclusion, conclusion_size, "s tells where the body is.");
    } else if (strcmp(symbol, "s0") == 0) {
        snprintf(formula, formula_size, "s = s0 + v0*t + ...");
        snprintf(meaning, meaning_size, "s0 is initial position.");
        snprintf(conclusion, conclusion_size, "s0 is the starting point.");
    } else if (strcmp(symbol, "t") == 0) {
        snprintf(formula, formula_size, "Used in all motion formulas");
        snprintf(meaning, meaning_size, "t is time.");
        snprintf(conclusion, conclusion_size, "Time tells when the motion is observed.");
    } else if (strcmp(symbol, "r") == 0 && strstr(meaning_in, "radius") != NULL) {
        snprintf(formula, formula_size, "v = r*omega");
        snprintf(meaning, meaning_size, "r is radius, the distance from the centre to the point.");
        snprintf(conclusion, conclusion_size, "For the same omega, larger radius gives larger linear speed.");
    } else if (strcmp(symbol, "r") == 0) {
        snprintf(formula, formula_size, "r = x*i + y*j + z*k");
        snprintf(meaning, meaning_size, "r is the position vector of a body in space.");
        snprintf(conclusion, conclusion_size, "The position vector shows location relative to the origin.");
    } else if (strcmp(symbol, "dr") == 0) {
        snprintf(formula, formula_size, "dr = r2 - r1");
        snprintf(meaning, meaning_size, "dr is a small change in position vector.");
        snprintf(conclusion, conclusion_size, "dr is used in derivatives and work integrals.");
    } else if (strcmp(symbol, "d") == 0) {
        snprintf(formula, formula_size, "W = F*d");
        snprintf(meaning, meaning_size, "d is distance or displacement magnitude.");
        snprintf(conclusion, conclusion_size, "Use context to distinguish distance and displacement.");
    } else if (strcmp(symbol, "x,y,z") == 0) {
        snprintf(formula, formula_size, "r = x*i + y*j + z*k");
        snprintf(meaning, meaning_size, "x, y, z are spatial coordinates.");
        snprintf(conclusion, conclusion_size, "Coordinates describe position in space.");
    } else if (strcmp(symbol, "vx,vy,vz") == 0) {
        snprintf(formula, formula_size, "|v| = sqrt(vx^2+vy^2+vz^2)");
        snprintf(meaning, meaning_size, "vx, vy, vz are velocity components.");
        snprintf(conclusion, conclusion_size, "Components combine to give full velocity.");
    } else if (strcmp(symbol, "ax,ay,az") == 0) {
        snprintf(formula, formula_size, "|a| = sqrt(ax^2+ay^2+az^2)");
        snprintf(meaning, meaning_size, "ax, ay, az are acceleration components.");
        snprintf(conclusion, conclusion_size, "Components combine to give full acceleration.");
    } else if (strcmp(symbol, "phi") == 0) {
        snprintf(formula, formula_size, "phi = 2*pi*N");
        snprintf(meaning, meaning_size, "Angular displacement is the angle through which a body rotates.");
        snprintf(conclusion, conclusion_size, "phi shows how far a body has rotated.");
    } else if (strcmp(symbol, "omega") == 0) {
        snprintf(formula, formula_size, "omega = dphi/dt");
        snprintf(meaning, meaning_size, "Angular velocity is the rate of change of angular displacement with time.");
        snprintf(conclusion, conclusion_size, "omega describes the rotation rate of a body.");
    } else if (strcmp(symbol, "omega0") == 0) {
        snprintf(formula, formula_size, "omega = omega0 + alpha*t");
        snprintf(meaning, meaning_size, "omega0 is initial angular velocity.");
        snprintf(conclusion, conclusion_size, "omega0 is angular speed at t = 0.");
    } else if (strcmp(symbol, "alpha") == 0) {
        snprintf(formula, formula_size, "alpha = domega/dt");
        snprintf(meaning, meaning_size, "Angular acceleration is the rate of change of angular velocity with time.");
        snprintf(conclusion, conclusion_size, "alpha describes change of the rotation rate.");
    } else if (strcmp(symbol, "N") == 0 && strstr(meaning_in, "normal force") != NULL) {
        snprintf(formula, formula_size, "For body at rest: N = m*g");
        snprintf(meaning, meaning_size, "Normal force is the force exerted by a support on a body.");
        snprintf(conclusion, conclusion_size, "For a body at rest on a horizontal surface, normal force equals weight.");
    } else if (strcmp(symbol, "N") == 0) {
        snprintf(formula, formula_size, "N = phi/(2*pi)");
        snprintf(meaning, meaning_size, "N is the number of revolutions or full turns.");
        snprintf(conclusion, conclusion_size, "One full revolution corresponds to 2*pi radians.");
    } else if (strcmp(symbol, "a_t") == 0) {
        snprintf(formula, formula_size, "a_t = r*alpha");
        snprintf(meaning, meaning_size, "Tangential acceleration is directed along the tangent and changes speed.");
        snprintf(conclusion, conclusion_size, "If alpha = 0, then tangential acceleration is zero.");
    } else if (strcmp(symbol, "a_c") == 0) {
        snprintf(formula, formula_size, "a_c = v^2/r = r*omega^2");
        snprintf(meaning, meaning_size, "Centripetal acceleration is directed toward the centre and changes velocity direction.");
        snprintf(conclusion, conclusion_size, "a_c always points to the centre of the circle.");
    } else if (strcmp(symbol, "T") == 0) {
        snprintf(formula, formula_size, "T = 1/f = 2*pi/omega");
        snprintf(meaning, meaning_size, "Period is the time of one complete oscillation or revolution.");
        snprintf(conclusion, conclusion_size, "T gives the duration of one cycle.");
    } else if (strcmp(symbol, "f") == 0) {
        snprintf(formula, formula_size, "f = 1/T = omega/(2*pi)");
        snprintf(meaning, meaning_size, "Frequency is the number of oscillations or revolutions per second.");
        snprintf(conclusion, conclusion_size, "f shows how often the motion repeats.");
    } else if (strcmp(symbol, "beta") == 0) {
        snprintf(formula, formula_size, "tan(beta) = a_c/a_t");
        snprintf(meaning, meaning_size, "beta is the angle between total acceleration and velocity.");
        snprintf(conclusion, conclusion_size, "beta describes acceleration direction.");
    } else if (strcmp(symbol, "pi") == 0) {
        snprintf(formula, formula_size, "pi = 3.14159...");
        snprintf(meaning, meaning_size, "pi is the circle constant.");
        snprintf(conclusion, conclusion_size, "pi appears in angular and circular formulas.");
    } else if (strcmp(symbol, "F") == 0) {
        snprintf(formula, formula_size, "F = dp/dt or F = m*a");
        snprintf(meaning, meaning_size, "Force is the physical quantity that changes the motion of a body.");
        snprintf(conclusion, conclusion_size, "Net force determines acceleration and momentum change.");
    } else if (strcmp(symbol, "m") == 0) {
        snprintf(formula, formula_size, "p = m*v");
        snprintf(meaning, meaning_size, "Mass is the amount of matter and a measure of inertia.");
        snprintf(conclusion, conclusion_size, "Mass resists changes in motion.");
    } else if (strcmp(symbol, "p") == 0) {
        snprintf(formula, formula_size, "p = m*v");
        snprintf(meaning, meaning_size, "Linear momentum describes the quantity of motion of a body.");
        snprintf(conclusion, conclusion_size, "Momentum is a vector in the direction of velocity.");
    } else if (strcmp(symbol, "I") == 0) {
        if (strstr(meaning_in, "impulse") != NULL) {
            snprintf(formula, formula_size, "I = integral F dt");
            snprintf(meaning, meaning_size, "Impulse is the time effect of force.");
            snprintf(conclusion, conclusion_size, "Impulse equals the change in momentum.");
        } else {
            snprintf(formula, formula_size, "L = I*omega");
            snprintf(meaning, meaning_size, "Moment of inertia is resistance to rotational motion.");
            snprintf(conclusion, conclusion_size, "Moment of inertia is rotational analogue of mass.");
        }
    } else if (strcmp(symbol, "g") == 0) {
        snprintf(formula, formula_size, "Ep = m*g*h");
        snprintf(meaning, meaning_size, "g is gravitational acceleration.");
        snprintf(conclusion, conclusion_size, "Near Earth, g is about 9.81 m/s^2.");
    } else if (strcmp(symbol, "Fext") == 0) {
        snprintf(formula, formula_size, "If Fext = 0, P = const");
        snprintf(meaning, meaning_size, "Fext is net external force on a system.");
        snprintf(conclusion, conclusion_size, "Zero external force gives momentum conservation.");
    } else if (strcmp(symbol, "Fnet") == 0) {
        snprintf(formula, formula_size, "Fnet = sum of all forces");
        snprintf(meaning, meaning_size, "Fnet is net force.");
        snprintf(conclusion, conclusion_size, "If Fnet = 0, then a = 0.");
    } else if (strcmp(symbol, "F12,F21") == 0) {
        snprintf(formula, formula_size, "F21 = -F12");
        snprintf(meaning, meaning_size, "Action and reaction force pair.");
        snprintf(conclusion, conclusion_size, "They act on different bodies.");
    } else if (strcmp(symbol, "k") == 0) {
        if (strstr(meaning_in, "spring") != NULL) {
            snprintf(formula, formula_size, "F = -k*x");
            snprintf(meaning, meaning_size, "k is spring constant.");
            snprintf(conclusion, conclusion_size, "Larger k means stiffer spring.");
        } else if (strstr(meaning_in, "force slope") != NULL) {
            snprintf(formula, formula_size, "F = F0 + k*t");
            snprintf(meaning, meaning_size, "k is the rate of change of force with time.");
            snprintf(conclusion, conclusion_size, "Larger k means force grows faster.");
        } else {
            snprintf(formula, formula_size, "F = F0 + k*t");
            snprintf(meaning, meaning_size, "k is rate of force change with time.");
            snprintf(conclusion, conclusion_size, "k tells how quickly force grows.");
        }
    } else if (strcmp(symbol, "dp") == 0) {
        snprintf(formula, formula_size, "dp = p2 - p1");
        snprintf(meaning, meaning_size, "dp is change in momentum.");
        snprintf(conclusion, conclusion_size, "Impulse equals dp.");
    } else if (strcmp(symbol, "dt") == 0) {
        snprintf(formula, formula_size, "d/dt or integral ... dt");
        snprintf(meaning, meaning_size, "dt is a small time interval.");
        snprintf(conclusion, conclusion_size, "dt is used in derivatives and integrals.");
    } else if (strcmp(symbol, "W") == 0) {
        snprintf(formula, formula_size, "W = F*s*cos(phi)");
        snprintf(meaning, meaning_size, "Work is energy transferred by force during displacement.");
        snprintf(conclusion, conclusion_size, "If force is perpendicular to displacement, work is zero.");
    } else if (strcmp(symbol, "P") == 0) {
        snprintf(formula, formula_size, "P = dW/dt or F*v");
        snprintf(meaning, meaning_size, "Power is the rate of doing work or transferring energy.");
        snprintf(conclusion, conclusion_size, "Power shows how fast work is done.");
    } else if (strcmp(symbol, "Pavg") == 0) {
        snprintf(formula, formula_size, "Pavg = W/t");
        snprintf(meaning, meaning_size, "Pavg is average power.");
        snprintf(conclusion, conclusion_size, "Average power is work per time.");
    } else if (strcmp(symbol, "Ek") == 0) {
        snprintf(formula, formula_size, "Ek = 0.5*m*v^2");
        snprintf(meaning, meaning_size, "Kinetic energy is the energy of motion.");
        snprintf(conclusion, conclusion_size, "If speed doubles, kinetic energy becomes four times larger.");
    } else if (strcmp(symbol, "Ep") == 0) {
        snprintf(formula, formula_size, "Ep = m*g*h or 0.5*k*x^2");
        snprintf(meaning, meaning_size, "Potential energy is energy due to position or deformation.");
        snprintf(conclusion, conclusion_size, "Gravitational Ep depends on height, spring Ep on x^2.");
    } else if (strcmp(symbol, "E") == 0) {
        snprintf(formula, formula_size, "E = Ek + Ep");
        snprintf(meaning, meaning_size, "Mechanical energy is the sum of kinetic and potential energy.");
        snprintf(conclusion, conclusion_size, "If only conservative forces act, total E is constant.");
    } else if (strcmp(symbol, "dEp") == 0) {
        snprintf(formula, formula_size, "dEp = m*g*h");
        snprintf(meaning, meaning_size, "dEp is change in potential energy.");
        snprintf(conclusion, conclusion_size, "dEp tracks energy change with height.");
    } else if (strcmp(symbol, "dEk") == 0) {
        snprintf(formula, formula_size, "W = dEk");
        snprintf(meaning, meaning_size, "dEk is change in kinetic energy.");
        snprintf(conclusion, conclusion_size, "Net work equals dEk.");
    } else if (strcmp(symbol, "h") == 0 || strcmp(symbol, "h0,h1,h2") == 0 || strcmp(symbol, "hmax") == 0) {
        snprintf(formula, formula_size, "Ep = m*g*h");
        snprintf(meaning, meaning_size, "h is height in vertical motion.");
        snprintf(conclusion, conclusion_size, "Greater h gives greater gravitational Ep.");
    } else if (strcmp(symbol, "M") == 0) {
        snprintf(formula, formula_size, "M = dL/dt or r*F*sin(phi)");
        snprintf(meaning, meaning_size, "Torque is the rotational effect of force.");
        snprintf(conclusion, conclusion_size, "Torque causes angular acceleration of a body.");
    } else if (strcmp(symbol, "L") == 0) {
        snprintf(formula, formula_size, "L = I*omega");
        snprintf(meaning, meaning_size, "Angular momentum characterizes rotational motion.");
        snprintf(conclusion, conclusion_size, "If external torque is zero, angular momentum is conserved.");
    } else if (strcmp(symbol, "I0") == 0) {
        snprintf(formula, formula_size, "I = I0 + m*a^2");
        snprintf(meaning, meaning_size, "I0 is base moment of inertia about the main axis.");
        snprintf(conclusion, conclusion_size, "I0 is used in parallel-axis problems.");
    } else if (strcmp(symbol, "l") == 0) {
        snprintf(formula, formula_size, "Irod = (1/12)*m*l^2");
        snprintf(meaning, meaning_size, "l is rod length.");
        snprintf(conclusion, conclusion_size, "Length affects rotational inertia.");
    } else if (strcmp(symbol, "A") == 0) {
        snprintf(formula, formula_size, "vmax = A*omega");
        snprintf(meaning, meaning_size, "Amplitude is the maximum displacement from equilibrium.");
        snprintf(conclusion, conclusion_size, "If amplitude changes, SHM energy changes as A^2.");
    } else if (strcmp(symbol, "x") == 0) {
        snprintf(formula, formula_size, "x = A*cos(wt+phi)");
        snprintf(meaning, meaning_size, "x is displacement from equilibrium position.");
        snprintf(conclusion, conclusion_size, "In SHM, x determines force and acceleration.");
    } else if (strcmp(symbol, "vmax") == 0) {
        snprintf(formula, formula_size, "vmax = A*omega");
        snprintf(meaning, meaning_size, "vmax is maximum SHM speed.");
        snprintf(conclusion, conclusion_size, "vmax occurs at equilibrium.");
    } else if (strcmp(symbol, "amax") == 0) {
        snprintf(formula, formula_size, "amax = A*omega^2");
        snprintf(meaning, meaning_size, "amax is maximum SHM acceleration.");
        snprintf(conclusion, conclusion_size, "amax occurs at extreme positions.");
    } else if (strcmp(symbol, "Fmax") == 0) {
        snprintf(formula, formula_size, "Fmax = k*A = m*omega^2*A");
        snprintf(meaning, meaning_size, "Fmax is maximum restoring force in SHM.");
        snprintf(conclusion, conclusion_size, "Fmax occurs at max displacement.");
    } else if (strcmp(symbol, "dx/dt") == 0) {
        snprintf(formula, formula_size, "v = dx/dt");
        snprintf(meaning, meaning_size, "Derivative of position with respect to time gives instantaneous velocity.");
        snprintf(conclusion, conclusion_size, "Derivative gives the exact rate of change at one moment.");
    } else if (strcmp(symbol, "dr/dt") == 0) {
        snprintf(formula, formula_size, "v = dr/dt");
        snprintf(meaning, meaning_size, "Derivative of position vector gives velocity.");
        snprintf(conclusion, conclusion_size, "Use dr/dt in vector kinematics.");
    } else if (strcmp(symbol, "dv/dt") == 0) {
        snprintf(formula, formula_size, "a = dv/dt");
        snprintf(meaning, meaning_size, "Derivative of velocity with respect to time gives acceleration.");
        snprintf(conclusion, conclusion_size, "Use dv/dt when velocity changes with time.");
    } else if (strcmp(symbol, "int") == 0) {
        snprintf(formula, formula_size, "integral accumulates small parts");
        snprintf(meaning, meaning_size, "Integral sums changes over an interval.");
        snprintf(conclusion, conclusion_size, "Use integrals for variable quantities.");
    } else if (strcmp(symbol, "S a dt") == 0) {
        snprintf(formula, formula_size, "v = integral a dt");
        snprintf(meaning, meaning_size, "Integrating acceleration gives velocity.");
        snprintf(conclusion, conclusion_size, "Use when a depends on time.");
    } else if (strcmp(symbol, "S F dr") == 0) {
        snprintf(formula, formula_size, "W = integral F dr");
        snprintf(meaning, meaning_size, "Integrating force along displacement gives work.");
        snprintf(conclusion, conclusion_size, "Use for variable force or curved motion.");
    }
}

static void build_symbol_formula_lines(const char *selected_label, char lines[40][96], uint8_t *line_count) {
    char symbol[24];
    char meaning_in[32];
    char unit[24];

    parse_symbol_label(selected_label, symbol, sizeof(symbol), meaning_in, sizeof(meaning_in), unit, sizeof(unit));
    *line_count = 0;

    append_formula_line(lines, line_count, "Formulas:");

    if (strcmp(symbol, "omega") == 0) {
        append_formula_line(lines, line_count, "omega = dphi/dt");
        append_formula_line(lines, line_count, "omega = 2*pi*f");
        append_formula_line(lines, line_count, "omega = 2*pi/T");
        append_formula_line(lines, line_count, "v = r*omega");
        append_formula_line(lines, line_count, "a_c = r*omega^2");
        append_formula_line(lines, line_count, "omega = omega0 + alpha*t");
        append_formula_line(lines, line_count, "omega^2 = omega0^2 + 2*alpha*phi");
        append_formula_line(lines, line_count, "L = I*omega");
        append_formula_line(lines, line_count, "Ek = 0.5*I*omega^2");
    } else if (strcmp(symbol, "alpha") == 0) {
        append_formula_line(lines, line_count, "alpha = domega/dt");
        append_formula_line(lines, line_count, "a_t = r*alpha");
        append_formula_line(lines, line_count, "omega = omega0 + alpha*t");
        append_formula_line(lines, line_count, "phi = omega0*t + 0.5*alpha*t^2");
        append_formula_line(lines, line_count, "omega^2 = omega0^2 + 2*alpha*phi");
        append_formula_line(lines, line_count, "M = I*alpha");
    } else if (strcmp(symbol, "phi") == 0) {
        append_formula_line(lines, line_count, "phi = 2*pi*N");
        append_formula_line(lines, line_count, "N = phi/(2*pi)");
        append_formula_line(lines, line_count, "omega = dphi/dt");
        append_formula_line(lines, line_count, "phi = omega0*t + 0.5*alpha*t^2");
        append_formula_line(lines, line_count, "omega^2 = omega0^2 + 2*alpha*phi");
        append_formula_line(lines, line_count, "W = M*phi");
    } else if (strcmp(symbol, "r") == 0) {
        append_formula_line(lines, line_count, "r = x*i + y*j + z*k");
        append_formula_line(lines, line_count, "v = dr/dt");
        append_formula_line(lines, line_count, "d = |r1|");
        append_formula_line(lines, line_count, "v = r*omega");
        append_formula_line(lines, line_count, "a_t = r*alpha");
        append_formula_line(lines, line_count, "a_c = v^2/r = r*omega^2");
    } else if (strcmp(symbol, "a_t") == 0) {
        append_formula_line(lines, line_count, "a_t = r*alpha");
        append_formula_line(lines, line_count, "a = sqrt(a_t^2+a_c^2)");
        append_formula_line(lines, line_count, "tan(beta) = a_c/a_t");
    } else if (strcmp(symbol, "a_c") == 0) {
        append_formula_line(lines, line_count, "a_c = v^2/r");
        append_formula_line(lines, line_count, "a_c = r*omega^2");
        append_formula_line(lines, line_count, "a = sqrt(a_t^2+a_c^2)");
        append_formula_line(lines, line_count, "tan(beta) = a_c/a_t");
    } else if (strcmp(symbol, "T") == 0) {
        append_formula_line(lines, line_count, "T = 1/f");
        append_formula_line(lines, line_count, "omega = 2*pi/T");
        append_formula_line(lines, line_count, "T = 2*pi/omega");
    } else if (strcmp(symbol, "f") == 0) {
        append_formula_line(lines, line_count, "f = 1/T");
        append_formula_line(lines, line_count, "omega = 2*pi*f");
        append_formula_line(lines, line_count, "f = omega/(2*pi)");
    } else if (strcmp(symbol, "v") == 0) {
        append_formula_line(lines, line_count, "v = ds/dt");
        append_formula_line(lines, line_count, "v = dr/dt");
        append_formula_line(lines, line_count, "v = v0 + a*t");
        append_formula_line(lines, line_count, "s = s0 + v*t if v const");
        append_formula_line(lines, line_count, "p = m*v");
        append_formula_line(lines, line_count, "Ek = 0.5*m*v^2");
        append_formula_line(lines, line_count, "P = F*v");
        append_formula_line(lines, line_count, "v = omega*sqrt(A^2-x^2)");
        append_formula_line(lines, line_count, "v = r*omega");
    } else if (strcmp(symbol, "v0") == 0) {
        append_formula_line(lines, line_count, "v = v0 + a*t");
        append_formula_line(lines, line_count, "s = s0 + v0*t + 0.5*a*t^2");
        append_formula_line(lines, line_count, "v^2 = v0^2 + 2*a*(s-s0)");
        append_formula_line(lines, line_count, "hmax = v0^2/(2*g)");
    } else if (strcmp(symbol, "a") == 0) {
        append_formula_line(lines, line_count, "a = dv/dt");
        append_formula_line(lines, line_count, "v = v0 + a*t");
        append_formula_line(lines, line_count, "s = s0 + v0*t + 0.5*a*t^2");
        append_formula_line(lines, line_count, "v^2 = v0^2 + 2*a*(s-s0)");
        append_formula_line(lines, line_count, "F = m*a");
        append_formula_line(lines, line_count, "a = F/m");
        append_formula_line(lines, line_count, "a = -omega^2*x");
    } else if (strcmp(symbol, "s") == 0) {
        append_formula_line(lines, line_count, "v_avg = delta s/delta t");
        append_formula_line(lines, line_count, "s = s0 + v*t");
        append_formula_line(lines, line_count, "s = s0 + v0*t + 0.5*a*t^2");
        append_formula_line(lines, line_count, "v^2 = v0^2 + 2*a*(s-s0)");
        append_formula_line(lines, line_count, "W = F*s");
    } else if (strcmp(symbol, "p") == 0) {
        append_formula_line(lines, line_count, "p = m*v");
        append_formula_line(lines, line_count, "F = dp/dt");
        append_formula_line(lines, line_count, "I = delta p");
    } else if (strcmp(symbol, "F") == 0) {
        append_formula_line(lines, line_count, "F = dp/dt");
        append_formula_line(lines, line_count, "F = m*a");
        append_formula_line(lines, line_count, "a = F/m");
        append_formula_line(lines, line_count, "I = F*t if const");
        append_formula_line(lines, line_count, "F = k*t");
        append_formula_line(lines, line_count, "W = F*s");
        append_formula_line(lines, line_count, "P = F*v");
    } else if (strcmp(symbol, "I") == 0) {
        if (strstr(meaning_in, "impulse") != NULL) {
            append_formula_line(lines, line_count, "I = integral F dt");
            append_formula_line(lines, line_count, "I = delta p");
            append_formula_line(lines, line_count, "I = F*t if F const");
        } else {
            append_formula_line(lines, line_count, "L = I*omega");
            append_formula_line(lines, line_count, "M = I*alpha");
            append_formula_line(lines, line_count, "Ek = 0.5*I*omega^2");
            append_formula_line(lines, line_count, "I = I0 + m*a^2");
        }
    } else if (strcmp(symbol, "W") == 0) {
        append_formula_line(lines, line_count, "W = F*s");
        append_formula_line(lines, line_count, "W = F*s*cos(phi)");
        append_formula_line(lines, line_count, "W = delta Ek");
        append_formula_line(lines, line_count, "W = M*phi");
    } else if (strcmp(symbol, "P") == 0) {
        append_formula_line(lines, line_count, "P = W/t");
        append_formula_line(lines, line_count, "P = dW/dt");
        append_formula_line(lines, line_count, "P = F*v");
        append_formula_line(lines, line_count, "P = M*omega");
    } else if (strcmp(symbol, "Ek") == 0) {
        append_formula_line(lines, line_count, "Ek = 0.5*m*v^2");
        append_formula_line(lines, line_count, "W = delta Ek");
        append_formula_line(lines, line_count, "E = Ep + Ek");
        append_formula_line(lines, line_count, "Ek = 0.5*I*omega^2");
    } else if (strcmp(symbol, "Ep") == 0) {
        append_formula_line(lines, line_count, "Ep = m*g*h");
        append_formula_line(lines, line_count, "Ep = 0.5*k*x^2");
        append_formula_line(lines, line_count, "E = Ep + Ek");
    } else if (strcmp(symbol, "E") == 0) {
        append_formula_line(lines, line_count, "E = Ep + Ek");
        append_formula_line(lines, line_count, "E = 0.5*k*A^2");
        append_formula_line(lines, line_count, "E = 0.5*m*omega^2*A^2");
        append_formula_line(lines, line_count, "h = E/(m*g) if v=0");
    } else if (strcmp(symbol, "M") == 0) {
        append_formula_line(lines, line_count, "M = r*F*sin(phi)");
        append_formula_line(lines, line_count, "M = dL/dt");
        append_formula_line(lines, line_count, "M = I*alpha");
        append_formula_line(lines, line_count, "W = M*phi");
        append_formula_line(lines, line_count, "P = M*omega");
    } else if (strcmp(symbol, "L") == 0) {
        append_formula_line(lines, line_count, "L = I*omega");
        append_formula_line(lines, line_count, "M = dL/dt");
    } else if (strcmp(symbol, "A") == 0) {
        append_formula_line(lines, line_count, "x = A*cos(wt+phi)");
        append_formula_line(lines, line_count, "x = A*sin(wt+phi)");
        append_formula_line(lines, line_count, "vmax = A*omega");
        append_formula_line(lines, line_count, "amax = A*omega^2");
        append_formula_line(lines, line_count, "E = 0.5*k*A^2");
    } else if (strcmp(symbol, "x") == 0) {
        append_formula_line(lines, line_count, "x = A*cos(wt+phi)");
        append_formula_line(lines, line_count, "x = A*sin(wt+phi)");
        append_formula_line(lines, line_count, "F = -k*x");
        append_formula_line(lines, line_count, "Ep = 0.5*k*x^2");
        append_formula_line(lines, line_count, "a = -omega^2*x");
        append_formula_line(lines, line_count, "v = omega*sqrt(A^2-x^2)");
    } else {
        append_formula_line(lines, line_count, "Open Related theory");
        append_formula_line(lines, line_count, "for explanations,");
        append_formula_line(lines, line_count, "or open Browse formulas");
        append_formula_line(lines, line_count, "for the full formula set.");
    }
}

static menu_result_t symbols_show_formulas(const char *selected_label) {
    uint8_t offset = 0;
    char lines[40][96];
    uint8_t line_count = 0;

    build_symbol_formula_lines(selected_label, lines, &line_count);

    while (true) {
        uint8_t line = 2;
        uint8_t i;
        uint8_t action;

        io_clear_screen();
        io_draw_title("Related formulas");
        io_draw_wrapped_text(1, selected_label, 26);

        for (i = offset; i < line_count && line < 9; ++i) {
            if (lines[i][0] != '\0') {
                io_draw_wrapped_text(line, lines[i], 26);
                ++line;
            }
        }

        io_draw_footer("UP/DN SCR MD/CLR BK");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && offset > 0) {
            --offset;
        } else if (action == IO_MENU_DOWN && offset + 1 < line_count) {
            ++offset;
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

static void build_symbol_definition_lines(const char *selected_label, char lines[40][96], uint8_t *line_count) {
    char symbol[24];
    char meaning_in[32];
    char unit[24];
    char question[128];
    char formula[64];
    char exact_unit[32];
    char meaning[96];
    char conclusion[96];

    parse_symbol_label(selected_label, symbol, sizeof(symbol), meaning_in, sizeof(meaning_in), unit, sizeof(unit));
    exact_unit[0] = '\0';

    if (!get_exact_label_definition(selected_label, question, sizeof(question), formula, sizeof(formula),
                                    exact_unit, sizeof(exact_unit), meaning, sizeof(meaning),
                                    conclusion, sizeof(conclusion))) {
        get_symbol_question(symbol, meaning_in, question, sizeof(question));
        get_symbol_formula_and_meaning(symbol, meaning_in, formula, sizeof(formula), meaning, sizeof(meaning), conclusion, sizeof(conclusion));
    }

    *line_count = 0;
    append_wrapped_definition(lines, line_count, "Question:");
    append_wrapped_definition(lines, line_count, question);
    append_wrapped_definition(lines, line_count, "Answer:");
    append_wrapped_definition(lines, line_count, meaning);
    append_wrapped_definition(lines, line_count, "Formula:");
    append_wrapped_definition(lines, line_count, formula);
    snprintf(lines[*line_count], 96, "%s = %s", symbol[0] ? symbol : "sym", meaning_in[0] ? meaning_in : "physics quantity");
    append_wrapped_definition(lines, line_count, lines[*line_count]);
    if (exact_unit[0] != '\0') {
        append_wrapped_definition(lines, line_count, "Unit:");
        append_wrapped_definition(lines, line_count, exact_unit);
    } else if (unit[0] != '\0') {
        append_wrapped_definition(lines, line_count, "Unit:");
        append_wrapped_definition(lines, line_count, unit);
    }
    append_wrapped_definition(lines, line_count, "Meaning:");
    append_wrapped_definition(lines, line_count, meaning);
    append_wrapped_definition(lines, line_count, "Conclusion:");
    append_wrapped_definition(lines, line_count, conclusion);
}

static menu_result_t symbols_show_definitions(const char *selected_label) {
    uint8_t offset = 0;
    char lines[40][96];
    uint8_t line_count = 0;

    build_symbol_definition_lines(selected_label, lines, &line_count);

    while (true) {
        uint8_t line = 2;
        uint8_t i;
        uint8_t action;

        io_clear_screen();
        io_draw_title("Definitions");
        io_draw_wrapped_text(1, selected_label, 26);

        for (i = offset; i < line_count && line < 9; ++i) {
            if (lines[i][0] != '\0') {
                io_draw_wrapped_text(line, lines[i], 26);
                ++line;
            }
        }

        io_draw_footer("UP/DN SCR MD/CLR BK");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && offset > 0) {
            --offset;
        } else if (action == IO_MENU_DOWN && offset + 1 < line_count) {
            ++offset;
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

static menu_result_t symbols_choose_related(const symbol_entry_t *entry, const char *selected_label) {
    uint8_t selected = 0;

    while (true) {
        uint8_t action;

        io_clear_screen();
        io_draw_title("Open related");
        io_draw_wrapped_text(1, selected_label, 26);
        io_draw_wrapped_text(3, selected == 0 ? "> Definitions" : "  Definitions", 26);
        io_draw_wrapped_text(4, selected == 1 ? "> Related theory" : "  Related theory", 26);
        io_draw_wrapped_text(5, selected == 2 ? "> Related formulas" : "  Related formulas", 26);
        io_draw_footer("UP/DN OK MD/CLR BK");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && selected > 0) {
            --selected;
        } else if (action == IO_MENU_DOWN && selected < 2) {
            ++selected;
        } else if (action == IO_MENU_SELECT) {
            if (selected == 0) {
                return symbols_show_definitions(selected_label);
            }
            if (selected == 1) {
                return symbols_show_related(entry);
            }
            return symbols_show_formulas(selected_label);
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

menu_result_t symbols_run(void) {
    uint8_t index = 0;
    uint8_t selected_line = 0;
    const uint8_t count = (uint8_t)(sizeof(SYMBOL_ENTRIES) / sizeof(SYMBOL_ENTRIES[0]));

    while (true) {
        const symbol_entry_t *entry = &SYMBOL_ENTRIES[index];
        uint8_t action;
        uint8_t i;
        char header[28];
        const char *selected_label = NULL;

        io_clear_screen();
        snprintf(header, sizeof(header), "Symbols %u/%u", (unsigned)(index + 1), (unsigned)count);
        io_draw_title(header);
        io_draw_wrapped_text(1, entry->title, 26);

        for (i = 0; i < 6; ++i) {
            if (entry->lines[i] != NULL && entry->lines[i][0] != '\0') {
                char row[28];
                snprintf(row, sizeof(row), "%c %s", i == selected_line ? '>' : ' ', entry->lines[i]);
                io_draw_wrapped_text((uint8_t)(2 + i), row, 26);
            }
        }

        selected_label = entry->lines[selected_line] != NULL ? entry->lines[selected_line] : entry->title;

        io_draw_footer("LF/RT PG UP/DN SEL");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && selected_line > 0) {
            --selected_line;
        } else if (action == IO_MENU_DOWN && selected_line + 1 < 6 &&
                   entry->lines[selected_line + 1] != NULL &&
                   entry->lines[selected_line + 1][0] != '\0') {
            ++selected_line;
        } else if (action == IO_MENU_LEFT && index > 0) {
            --index;
            selected_line = 0;
        } else if (action == IO_MENU_RIGHT && index + 1 < count) {
            ++index;
            selected_line = 0;
        } else if (action == IO_MENU_SELECT) {
            menu_result_t result = symbols_choose_related(entry, selected_label);
            if (result == MENU_EXIT_APP) {
                return MENU_EXIT_APP;
            }
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}
