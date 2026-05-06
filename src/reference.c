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

static const reference_entry_t QUICK_ACCESS_ENTRIES[] = {
    {
        "Core kinematics",
        {
            "v=dr/dt, a=dv/dt",
            "v_avg=(x2-x1)/(t2-t1)",
            "v = v0 + a*t",
            "s = s0 + v0*t + 0.5*a*t^2",
            "v^2 = v0^2 + 2*a*(s-s0)",
            "d = |r1| = sqrt(x^2+y^2+z^2)"
        }
    },
    {
        "Accel grows with t",
        {
            "a = a0 + k*t",
            "decel: a = a0 - k*t",
            "k = (a-a0)/t",
            "v = S a*dt = S(k*t)*dt + C",
            "s = S v*dt + C",
            "ex: S(t^2+1)dt=t^3/3+t+C"
        }
    },
    {
        "Core circular motion",
        {
            "omega = 2*pi*f",
            "omega = 2*pi/T",
            "T = 1/f, f = 1/T",
            "v = r*omega",
            "a_t = r*alpha",
            "a_c = r*omega^2 = v^2/r"
        }
    },
    {
        "Core dynamics",
        {
            "p = m*v",
            "F = dp / dt",
            "F = m*a if m const",
            "I = S F*dt = dp",
            "F21 = -F12",
            ""
        }
    },
    {
        "Core work/energy",
        {
            "dW = F.dr",
            "W = S F*dr",
            "W = dEk",
            "P = dW/dt = F.v",
            "dEp = m*g*h",
            "E = Ep + Ek = const"
        }
    },
    {
        "Core calculus",
        {
            "v = dr / dt, a = dv / dt",
            "v = S a*dt, r = S v*dt",
            "F = dp / dt, I = S F*dt",
            "omega = dphi / dt",
            "phi = S omega*dt",
            ""
        }
    }
};

static const reference_entry_t KINEMATICS_ENTRIES[] = {
    {
        "When use kinematics",
        {
            "s = v*t -> if v const",
            "v = v0+a*t -> if a const",
            "s = v0*t+0.5*a*t^2",
            " -> if a const",
            "v^2=v0^2+2*a*ds ->",
            " when time not given"
        }
    },
    {
        "Kin cond 1/3",
        {
            "s = v*t: use if v const",
            "Cond: v const, a = 0",
            "uniform motion only",
            "Do not use if v changes",
            "v = v0+a*t: if a const",
            "v changes uniformly"
        }
    },
    {
        "Kin cond 2/3",
        {
            "s=s0+v0*t+0.5*a*t^2",
            "Use if a = const",
            "If s0=0: s=v0*t+0.5*a*t^2",
            "v^2=v0^2+2*a*(s-s0)",
            "Use if a const, no time",
            "links v and displacement"
        }
    },
    {
        "Kin cond 3/3",
        {
            "vavg=(s2-s1)/(t2-t1)",
            "whole interval velocity",
            "v=ds/dt, a=dv/dt",
            "instant quantities",
            "v=S a dt, s=S v dt",
            "if a(t) or v(t) given"
        }
    },
    {
        "Most used kinematics",
        {
            "v = v0 + a*t",
            "s = s0 + v0*t + 0.5*a*t^2",
            "v^2 = v0^2 + 2*a*(s-s0)",
            "s = s0 + v*t if v const",
            "v = ds / dt",
            "a = dv / dt"
        }
    },
    {
        "Vector definitions",
        {
            "r = x*i + y*j + z*k",
            "v = dr / dt",
            "a = dv / dt",
            "dr = r2 - r1",
            "v_avg = dr / dt (vector)",
            "a_avg = dv / dt (vector)"
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
        "Free fall cond",
        {
            "Use: vertical motion",
            "gravity acts downward",
            "v = v0 - g*t",
            "h = v0*t - 0.5*g*t^2",
            "v^2 = v0^2 - 2*g*h",
            "At max height: v = 0"
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
        "When use circular",
        {
            "v=r*omega -> link lin/ang",
            "a_t=r*alpha -> speed change",
            "a_c=v^2/r or r*omega^2",
            " -> change direction",
            "omega=2*pi*f=2*pi/T ->",
            " if freq/period given"
        }
    },
    {
        "Most used circular",
        {
            "v = r*omega",
            "at = r*alpha",
            "ac = r*omega^2 = v^2/r",
            "a = sqrt(at^2 + ac^2)",
            "omega = 2*pi*f = 2*pi/T",
            "N = phi / (2*pi)"
        }
    },
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
            "phi = phi0 + omega0*t + 0.5*alpha*t^2",
            "omega^2 = omega0^2 + 2*alpha*phi",
            "If start from rest: phi = alpha*t^2/2",
            "Then N = phi / (2*pi)"
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
        "When use dynamics",
        {
            "p=m*v -> find momentum",
            "F=dp/dt -> general Newton 2",
            "F=m*a -> if m const",
            "I=S F*dt -> changing force",
            "I=F*t -> if F const",
            "F=k*t -> force grows with t"
        }
    },
    {
        "Most used dynamics",
        {
            "p = m*v",
            "F = dp / dt",
            "If m const: F = m*a",
            "F = k*t",
            "I = integral(F*dt) = dp",
            "F21 = -F12"
        }
    },
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
            "I = integral(F*dt)",
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
    },
    {
        "Rigid body basics",
        {
            "L = I*omega",
            "M = dL/dt",
            "If I const => M = I*alpha",
            "W = dEk",
            "W = M*phi if M const",
            "Ek_rot = 0.5*I*omega^2"
        }
    }
};

static const reference_entry_t ENERGY_ENTRIES[] = {
    {
        "When use energy",
        {
            "W=F*s if F || s",
            "W=dEk -> speed/energy tasks",
            "E=Ek+Ep=const -> no losses",
            "P=W/t or F*v -> rate of work",
            "Ek=0.5*m*v^2, Ep=m*g*h",
            " -> use in height tasks"
        }
    },
    {
        "Most used energy/power",
        {
            "W = dEk",
            "P = dW/dt = F.v",
            "Ek = 0.5*m*v^2",
            "dEp = m*g*h",
            "E = Ep + Ek = const",
            "W = F*d if F || d"
        }
    },
    {
        "Work",
        {
            "W = F*d if F || d",
            "W = F*d*cos(phi)",
            "W = integral(F*dr)",
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
    },
    {
        "Rigid moments",
        {
            "I_disk = 0.5*m*r^2",
            "I_cyl = 0.5*m*r^2",
            "I_rod_c = (1/12)*m*l^2",
            "I_rod_end = (1/3)*m*l^2",
            "omega = 2*pi*f",
            "f = omega/(2*pi)"
        }
    },
    {
        "SHM core",
        {
            "F = -k*x",
            "k = m*omega^2",
            "x = A*sin(omega*t+phi)",
            "x = A*cos(omega*t+phi)",
            "a = -omega^2*x",
            "vmax = A*omega"
        }
    },
    {
        "SHM energy/forms",
        {
            "v = A*omega*cos(...)",
            "v = -A*omega*sin(...)",
            "a = -A*omega^2*sin(...)",
            "a = -A*omega^2*cos(...)",
            "amax = A*omega^2",
            "Fmax = k*A = m*omega^2*A"
        }
    },
    {
        "SHM energy/freq",
        {
            "Ek = 0.5*m*v^2",
            "Ep = 0.5*k*x^2",
            "E = 0.5*k*A^2",
            "E = 0.5*m*omega^2*A^2",
            "T = 2*pi/omega",
            "v = omega*sqrt(A^2-x^2)"
        }
    }
};

static const reference_entry_t CALCULUS_ENTRIES[] = {
    {
        "When use calculus",
        {
            "v=dr/dt -> if r(t) given",
            "a=dv/dt -> if v(t) given",
            "v=S a*dt -> if a(t) given",
            "r=S v*dt -> if v(t) given",
            "F=dp/dt, I=S F*dt",
            " use for variable force"
        }
    },
    {
        "Most used calc forms",
        {
            "v = dr / dt, a = dv / dt",
            "v = S a*dt, r = S v*dt",
            "F = dp / dt, I = S F*dt",
            "P = dW / dt, W = S F*dr",
            "omega = dphi / dt",
            "phi = S omega*dt"
        }
    },
    {
        "Physics derivatives",
        {
            "Velocity: v = dx / dt",
            "Acceleration: a = dv / dt",
            "Angular speed: omega = dphi / dt",
            "Angular accel: alpha = domega / dt",
            "Power: P = dW / dt",
            "Force: F = dp / dt"
        }
    },
    {
        "Physics integrals",
        {
            "Velocity: v = S a*dt",
            "Position: r = S v*dt",
            "Angular speed: omega = S alpha*dt",
            "Impulse: I = S F*dt",
            "Work: W = S F*dr",
            "Angle: phi = S omega*dt"
        }
    },
    {
        "Linear force motion",
        {
            "F(t) = F0 + k*t",
            "a(t) = F(t)/m",
            "v(t) = v0 + S a(t)dt",
            "s(t) = s0 + S v(t)dt",
            "For 20t: use F0=0, k=20",
            "good for test-style tasks"
        }
    },
    {
        "Test formulas",
        {
            "v = dr / dt, a = dv / dt",
            "v = S a*dt, r = S v*dt",
            "F = dp / dt, I = S F*dt",
            "P = dW / dt, W = S F*dr",
            "omega = dphi / dt",
            "phi = S omega*dt"
        }
    }
};

static const reference_entry_t EXAM_EXTRA_ENTRIES[] = {
    {
        "Units core",
        {
            "p: kg*m/s, Iimp: N*s",
            "F: N, W/E: J, P: W",
            "M: N*m, Iin: kg*m^2",
            "omega: rad/s, alpha: rad/s^2",
            "k: N/m, f: Hz, T: s",
            ""
        }
    },
    {
        "Newton + eqbm",
        {
            "Newton1: Fnet=0 => v const",
            "Newton2: Fnet = dp/dt",
            "if m const => Fnet = m*a",
            "Newton3: F12 = -F21",
            "eqbm: sumF=0, sumM=0",
            ""
        }
    },
    {
        "Friction",
        {
            "Ffr <= mu_s*N",
            "Ffr_max = mu_s*N",
            "Ffr = mu_k*N",
            "dir: opposite motion",
            "Wfr = -Ffr*s",
            "non-cons, lowers mech E"
        }
    },
    {
        "Inclined plane",
        {
            "G = m*g",
            "Fpar = m*g*sin(theta)",
            "N = m*g*cos(theta)",
            "no friction: a=g*sin(th)",
            "with friction: Ffr=mu*N",
            ""
        }
    },
    {
        "Friction energy",
        {
            "Wnet = delta Ek",
            "Ei + Wnoncons = Ef",
            "Ek1+Ep1+Wfr = Ek2+Ep2",
            "Wfr = -Ffr*s",
            "friction lowers mech E",
            ""
        }
    },
    {
        "Momentum collide",
        {
            "Fext = 0 => Pbefore=Pafter",
            "m1*v1+m2*v2 = m1*u1+m2*u2",
            "inelastic:",
            "m1*v1+m2*v2=(m1+m2)*u",
            "isolated => momentum const",
            ""
        }
    },
    {
        "Center of mass",
        {
            "rcm = (m1*r1+...)/Mtot",
            "xcm = (m1*x1+m2*x2)/(m1+m2)",
            "vcm = Ptotal/Mtotal",
            "avg position of mass",
            "system often moves as COM",
            ""
        }
    },
    {
        "Torque + ang mom",
        {
            "M = r*F*sin(theta)",
            "if perp: M = r*F",
            "if parallel: M = 0",
            "L = r x p, |L|=r*p*sin(th)",
            "rigid body: L = I*omega",
            "Mext=0 => L const"
        }
    },
    {
        "Rot analogies",
        {
            "s->phi, v->omega, a->alpha",
            "m->I, F->M, p->L",
            "omega=omega0+alpha*t",
            "phi=omega0*t+0.5*alpha*t^2",
            "omega^2=omega0^2+2*alpha*phi",
            "W=M*phi, P=M*omega"
        }
    },
    {
        "SHM time/phase",
        {
            "x=A*sin(omega*t)",
            "t = arcsin(x/A)/omega",
            "x=A*cos(omega*t)",
            "t = arccos(x/A)/omega",
            "sin(pi/6)=1/2, cos(pi/3)=1/2",
            ""
        }
    },
    {
        "SHM energy extra",
        {
            "eqbm: x=0, v=vmax, Ek=E",
            "extreme: x=A, v=0, Ep=E",
            "E = Ek+Ep = const",
            "A doubles => E to 4E",
            "A triples => E to 9E",
            "ideal SHM: f indep of A"
        }
    },
    {
        "SHM oscillator",
        {
            "F=-k*x => a=-(k/m)*x",
            "omega = sqrt(k/m)",
            "T = 2*pi*sqrt(m/k)",
            "f = 1/T",
            "pendulum: omega=sqrt(g/l)",
            "small angles only"
        }
    },
    {
        "Vertical energy",
        {
            "E = Ek + m*g*h",
            "at hmax: v=0, E=m*g*hmax",
            "hmax = E/(m*g)",
            "if Ek=qEp =>",
            "h=v0^2/(2*g*(q+1))",
            "Ek=Ep=>v0^2/(4g)"
        }
    },
    {
        "Spring + stop",
        {
            "Ep = 0.5*k*x^2",
            "W(0->x)=0.5*k*x^2",
            "W(x1->x2)=0.5*k*(x2^2-x1^2)",
            "F = -k*x, |F|=k*x",
            "F*s = 0.5*m*v0^2 if stop",
            "use if vfinal=0, F const"
        }
    },
    {
        "Important traps",
        {
            "s=v*t only if v const",
            "I=F*t only if F const",
            "W=delta Ek, not final Ek",
            "max height => v=0, Ek=0",
            "const speed circle: a!=0",
            "M=0 if F parallel arm"
        }
    },
    {
        "Cond/theory 1/5",
        {
            "F=m*a only if m const",
            "general: F = dp/dt",
            "I=F*t only if F const",
            "general: I = S F*dt",
            "W=F*s if F || s",
            "else use F*s*cos(phi)"
        }
    },
    {
        "Cond/theory 2/5",
        {
            "P=F*v if same dir",
            "general: P=F*v*cos(phi)",
            "E const only with",
            "conservative forces",
            "W=dEk means change",
            "in Ek, not final Ek"
        }
    },
    {
        "Cond/theory 3/5",
        {
            "F=-k*x restoring force",
            "F=k*x magnitude only",
            "a=-w^2*x => SHM cond",
            "T=2*pi*sqrt(m/k): spring",
            "T=2*pi*sqrt(l/g): pend",
            "small angles only"
        }
    },
    {
        "Cond/theory 4/5",
        {
            "ac=v^2/r=r*w^2",
            "const speed => ac != 0",
            "at=r*alpha changes speed",
            "at=0 if alpha=0",
            "M=I*alpha if I const",
            "M=dL/dt is general"
        }
    },
    {
        "Cond/theory 5/5",
        {
            "L const if Mext = 0",
            "P const if Fext = 0",
            "N=m*g not always true",
            "incline: N=m*g*cos(th)",
            "v=r*w, phi=2*pi*N",
            "Eshm=0.5*k*A^2"
        }
    }
};

static const reference_category_t REFERENCE_CATEGORIES[] = {
    { "Quick Access", (uint8_t)(sizeof(QUICK_ACCESS_ENTRIES) / sizeof(QUICK_ACCESS_ENTRIES[0])), QUICK_ACCESS_ENTRIES },
    { "Kinematics", (uint8_t)(sizeof(KINEMATICS_ENTRIES) / sizeof(KINEMATICS_ENTRIES[0])), KINEMATICS_ENTRIES },
    { "Circular Motion", (uint8_t)(sizeof(CIRCULAR_ENTRIES) / sizeof(CIRCULAR_ENTRIES[0])), CIRCULAR_ENTRIES },
    { "Dynamics", (uint8_t)(sizeof(DYNAMICS_ENTRIES) / sizeof(DYNAMICS_ENTRIES[0])), DYNAMICS_ENTRIES },
    { "Work / Energy / Power", (uint8_t)(sizeof(ENERGY_ENTRIES) / sizeof(ENERGY_ENTRIES[0])), ENERGY_ENTRIES },
    { "Physics Calculus", (uint8_t)(sizeof(CALCULUS_ENTRIES) / sizeof(CALCULUS_ENTRIES[0])), CALCULUS_ENTRIES },
    { "Exam Extras", (uint8_t)(sizeof(EXAM_EXTRA_ENTRIES) / sizeof(EXAM_EXTRA_ENTRIES[0])), EXAM_EXTRA_ENTRIES }
};

static menu_result_t reference_select_category(uint8_t *selected_index, const reference_category_t **category_out) {
    const uint8_t category_count = (uint8_t)(sizeof(REFERENCE_CATEGORIES) / sizeof(REFERENCE_CATEGORIES[0]));
    uint8_t i;
    uint8_t action;

    while (true) {
        io_clear_screen();
        io_draw_title("All Formulas");

        for (i = 0; i < category_count; ++i) {
            char line[28];
            snprintf(line, sizeof(line), "%c %s", i == *selected_index ? '>' : ' ', REFERENCE_CATEGORIES[i].name);
            io_draw_wrapped_text((uint8_t)(2 + i), line, 26);
        }

        io_draw_footer("UP/DN OK MD/CLR BK");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && *selected_index > 0) {
            --(*selected_index);
        } else if (action == IO_MENU_DOWN && *selected_index + 1 < category_count) {
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

menu_result_t reference_show_category_by_index(uint8_t index) {
    const uint8_t category_count = (uint8_t)(sizeof(REFERENCE_CATEGORIES) / sizeof(REFERENCE_CATEGORIES[0]));

    if (index >= category_count) {
        return MENU_BACK;
    }

    return reference_show_entries(&REFERENCE_CATEGORIES[index]);
}
