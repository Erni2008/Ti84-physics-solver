#include <stdio.h>

#include "io_helpers.h"
#include "theory.h"

typedef struct {
    const char *title;
    const char *lines[80];
} theory_entry_t;

typedef struct {
    const char *name;
    uint8_t entry_count;
    const theory_entry_t *const *entries;
} theory_category_t;

static const theory_entry_t THEORY_ENTRIES[] = {
    {
        "1. Avg / inst vel",
        {
            "Possible questions:",
            "- define avg velocity",
            "- define inst velocity",
            "- difference?",
            "",
            "Answer:",
            "Average velocity:",
            "v_avg = delta s / delta t",
            "",
            "Meaning:",
            "displacement per time",
            "interval.",
            "",
            "Instantaneous velocity:",
            "v = ds/dt",
            "",
            "Meaning:",
            "velocity at one exact",
            "moment of time.",
            "",
            "Conclusion:",
            "Average = interval,",
            "instantaneous = moment.",
            NULL
        }
    },
    {
        "2. Acceleration",
        {
            "Possible questions:",
            "- define accel",
            "- what describe?",
            "- relation to vel?",
            "",
            "Answer:",
            "Acceleration is the",
            "rate of change of",
            "velocity with time.",
            "",
            "Formula:",
            "a = dv/dt",
            "",
            "a accel, v velocity,",
            "t time",
            "",
            "If velocity const:",
            "a = 0",
            "",
            "Unit: m/s^2",
            NULL
        }
    },
    {
        "3. UAM",
        {
            "Possible questions:",
            "- what is UAM?",
            "- formulas for const a",
            "- when use v=v0+at?",
            "",
            "Answer:",
            "Uniformly accelerated",
            "motion means accel is",
            "constant.",
            "",
            "Condition:",
            "a = const",
            "",
            "Formulas:",
            "v = v0 + a*t",
            "s = v0*t + 0.5*a*t^2",
            "v^2 = v0^2 + 2*a*s",
            "",
            "Use when accel does",
            "not change with time.",
            NULL
        }
    },
    {
        "4. Free fall",
        {
            "Possible questions:",
            "- accel in free fall?",
            "- accel near Earth?",
            "- upward motion?",
            "",
            "Answer:",
            "In free fall body moves",
            "under gravity only.",
            "",
            "Acceleration:",
            "g = 9.81 m/s^2",
            "or about 10 m/s^2",
            "",
            "When body moves up:",
            "gravity acts down,",
            "so velocity decreases.",
            "",
            "At max height:",
            "v = 0",
            "",
            "Conclusion:",
            "Gravity always down.",
            NULL
        }
    },
    {
        "5. Upward energy",
        {
            "Possible questions:",
            "- Ek and Ep upward?",
            "- Ek at max height?",
            "- hmax from energy?",
            "",
            "Answer:",
            "Mechanical energy:",
            "E = Ek + Ep",
            "Ek = 0.5*m*v^2",
            "Ep = m*g*h",
            "",
            "Going upward:",
            "Ep increases,",
            "Ek decreases.",
            "",
            "At max height:",
            "v = 0, so Ek = 0",
            "All energy is pot:",
            "E = m*g*hmax",
            NULL
        }
    },
    {
        "6. Conservative F",
        {
            "Possible questions:",
            "- what are cons forces?",
            "- when mech E cons?",
            "- examples?",
            "",
            "Answer:",
            "Mechanical energy is",
            "conserved if only",
            "conservative forces act.",
            "",
            "Conservative:",
            "gravity, spring force",
            "",
            "Non-conservative:",
            "friction, air resist",
            "",
            "If only cons act:",
            "E = Ek + Ep = const",
            NULL
        }
    },
    {
        "7. Kinetic energy",
        {
            "Possible questions:",
            "- define Ek",
            "- what depend on?",
            "- if speed doubles?",
            "",
            "Answer:",
            "Kinetic energy:",
            "Ek = 0.5*m*v^2",
            "",
            "Meaning:",
            "energy of motion.",
            "",
            "It depends on square",
            "of speed.",
            "",
            "If speed doubles:",
            "Ek -> 4Ek",
            NULL
        }
    },
    {
        "8. Potential energy",
        {
            "Possible questions:",
            "- define grav Ep",
            "- what depend on?",
            "- change with h?",
            "",
            "Answer:",
            "Grav potential energy:",
            "Ep = m*g*h",
            "",
            "Meaning:",
            "energy due to position",
            "in grav field.",
            "",
            "If height increases:",
            "Ep increases.",
            "",
            "Unit: J",
            NULL
        }
    },
    {
        "9. Net force",
        {
            "Possible questions:",
            "- what is net force?",
            "- if net force zero?",
            "- if non-zero?",
            "",
            "Answer:",
            "Net force is vector sum",
            "of all forces on body.",
            "",
            "If Fnet = 0:",
            "a = 0",
            "body at rest or",
            "moves with const vel.",
            "",
            "If Fnet not zero:",
            "body accelerates.",
            NULL
        }
    },
    {
        "10. Newton 1 law",
        {
            "Possible questions:",
            "- state Newton 1",
            "- if no net force?",
            "- what is inertia?",
            "",
            "Answer:",
            "Newton's first law",
            "states that if the",
            "net external force",
            "acting on a body is",
            "zero, the body",
            "remains at rest or",
            "moves with constant",
            "velocity.",
            "",
            "Condition:",
            "Fnet = 0",
            "Then v = const, a = 0",
            "",
            "Inertia is the",
            "property of a body",
            "to resist changes",
            "in its motion.",
            NULL
        }
    },
    {
        "11. Newton 3 law",
        {
            "Possible questions:",
            "- state Newton 3",
            "- action reaction?",
            "- equal forces?",
            "",
            "Answer:",
            "Newton's third law",
            "states that if body 1",
            "acts on body 2 with",
            "force F12, then",
            "body 2 acts on body 1",
            "with force F21.",
            "",
            "Formula:",
            "F21 = -F12",
            "",
            "Action and reaction",
            "forces are equal in",
            "magnitude, opposite",
            "in direction, and",
            "act on different",
            "bodies.",
            NULL
        }
    },
    {
        "12. Weight vs mass",
        {
            "Possible questions:",
            "- diff mass/weight?",
            "- formula for weight?",
            "- unit of weight?",
            "",
            "Answer:",
            "Mass: m",
            "amount of matter /",
            "measure of inertia.",
            "Unit: kg",
            "",
            "Weight:",
            "G = m*g",
            "grav force on body.",
            "Unit: N",
            "",
            "Conclusion:",
            "Mass in kg, weight in N.",
            NULL
        }
    },
    {
        "13. Friction",
        {
            "Possible questions:",
            "- what is friction?",
            "- which direction?",
            "- kinetic friction law?",
            "",
            "Answer:",
            "Friction opposes",
            "relative motion or",
            "tendency of motion.",
            "",
            "Kinetic friction:",
            "Ff = mu*N",
            "",
            "Direction:",
            "opposite to motion.",
            "",
            "Friction reduces",
            "mech energy.",
            NULL
        }
    },
    {
        "14. Spring force",
        {
            "Possible questions:",
            "- Hooke law?",
            "- minus sign in -kx?",
            "- what is k?",
            "",
            "Answer:",
            "Hooke law:",
            "F = -k*x",
            "",
            "F spring force",
            "k spring constant",
            "x displacement",
            "",
            "Minus sign means",
            "force opposite to",
            "displacement.",
            "",
            "Magnitude: F = k*x",
            NULL
        }
    },
    {
        "15. Spring energy",
        {
            "Possible questions:",
            "- spring Ep?",
            "- depend on x?",
            "- if x doubles?",
            "",
            "Answer:",
            "Spring potential:",
            "Ep = 0.5*k*x^2",
            "",
            "Meaning:",
            "energy stored in",
            "deformed spring.",
            "",
            "If x doubles:",
            "Ep -> 4Ep",
            "",
            "Conclusion:",
            "Spring Ep depends on x^2.",
            NULL
        }
    },
    {
        "16. Angular disp",
        {
            "Possible questions:",
            "- define ang disp?",
            "- unit of phi?",
            "- relate to revs?",
            "",
            "Answer:",
            "Angular displacement is",
            "angle through which",
            "body rotates.",
            "",
            "Symbol: phi",
            "Unit: rad",
            "",
            "phi = 2*pi*N",
            "N = rev count",
            NULL
        }
    },
    {
        "17. Revolutions",
        {
            "Possible questions:",
            "- find N from phi?",
            "- relation N and phi?",
            "",
            "Answer:",
            "Relation:",
            "N = phi/(2*pi)",
            "",
            "Also:",
            "phi = 2*pi*N",
            "",
            "Use when task gives",
            "turns or rot angle.",
            NULL
        }
    },
    {
        "18. Rot kinematics",
        {
            "Possible questions:",
            "- formulas for const a?",
            "- what is alpha?",
            "- when use rot eqs?",
            "",
            "Answer:",
            "Use when angular",
            "acceleration is const.",
            "Condition: alpha=const",
            "",
            "w = w0 + alpha*t",
            "phi = w0*t + 0.5*",
            " alpha*t^2",
            "w^2 = w0^2 + 2*",
            " alpha*phi",
            "",
            "Rot analogues of",
            "linear kinematics.",
            NULL
        }
    },
    {
        "19. v and w",
        {
            "Possible questions:",
            "- link v and w?",
            "- tangential speed?",
            "",
            "Answer:",
            "Linear velocity:",
            "v = r*w",
            "",
            "v tang velocity",
            "r radius",
            "w ang velocity",
            "",
            "Larger r at same w",
            "gives larger v.",
            NULL
        }
    },
    {
        "20. at and alpha",
        {
            "Possible questions:",
            "- at related alpha?",
            "- what changes speed?",
            "",
            "Answer:",
            "Tangential accel:",
            "at = r*alpha",
            "",
            "Meaning:",
            "Tangential accel",
            "changes speed.",
            "",
            "If alpha = 0:",
            "at = 0",
            NULL
        }
    },
    {
        "21. Rotational work",
        {
            "Possible questions:",
            "- work by torque?",
            "- analogue of W=Fs?",
            "- when W=Mphi valid?",
            "",
            "Answer:",
            "Work in rotation:",
            "W = M*phi",
            "",
            "Valid when torque",
            "is constant.",
            "",
            "Analogy:",
            "linear W=F*s",
            "rotation W=M*phi",
            NULL
        }
    },
    {
        "22. Rot power",
        {
            "Possible questions:",
            "- power in rotation?",
            "- via M and w?",
            "",
            "Answer:",
            "Rotational power:",
            "P = M*w",
            "",
            "Meaning:",
            "rate of rotational",
            "work.",
            "",
            "Analogy:",
            "linear P=F*v",
            "rotation P=M*w",
            NULL
        }
    },
    {
        "23. Inertia vs axis",
        {
            "Possible questions:",
            "- only on mass?",
            "- why axis matters?",
            "",
            "Answer:",
            "Moment of inertia",
            "depends on mass,",
            "shape, mass distrib,",
            "and rot axis.",
            "",
            "Mass farther from axis",
            "gives larger inertia.",
            NULL
        }
    },
    {
        "24. Centre of mass",
        {
            "Possible questions:",
            "- what is COM?",
            "- how system moves?",
            "- what used for?",
            "",
            "Answer:",
            "Centre of mass is",
            "average position of",
            "mass of system.",
            "",
            "System motion often",
            "described as motion",
            "of COM.",
            "",
            "For isolated system:",
            "COM moves with const v.",
            NULL
        }
    },
    {
        "25. Equilibrium",
        {
            "Possible questions:",
            "- conditions eqbm?",
            "- translational eqbm?",
            "- rotational eqbm?",
            "",
            "Answer:",
            "A body is in",
            "translational",
            "equilibrium when the",
            "sum of forces is zero.",
            "",
            "Translational eqbm:",
            "sum F = 0",
            "",
            "A rigid body is in",
            "rotational eqbm when",
            "the sum of torques",
            "is zero.",
            "",
            "Rotational eqbm:",
            "sum M = 0",
            "",
            "Complete eqbm when",
            "sum F = 0 and",
            "sum M = 0.",
            NULL
        }
    },
    {
        "26. SHM max speed",
        {
            "Possible questions:",
            "- vmax in SHM?",
            "- where max speed?",
            "",
            "Answer:",
            "Maximum speed:",
            "vmax = A*w",
            "",
            "Speed max at",
            "equilibrium position:",
            "x = 0",
            "",
            "At equilibrium:",
            "Ep min, Ek max.",
            NULL
        }
    },
    {
        "27. SHM max accel",
        {
            "Possible questions:",
            "- amax in SHM?",
            "- where accel max?",
            "",
            "Answer:",
            "Maximum accel:",
            "amax = A*w^2",
            "",
            "a = -w^2*x",
            "",
            "Accel magnitude max",
            "when displacement max:",
            "x = A",
            NULL
        }
    },
    {
        "28. SHM v(x)",
        {
            "Possible questions:",
            "- v via A and x?",
            "- speed if x known?",
            "",
            "Answer:",
            "Velocity in SHM:",
            "v = w*sqrt(A^2-x^2)",
            "",
            "At x = 0:",
            "v = vmax = A*w",
            "",
            "At x = A:",
            "v = 0",
            NULL
        }
    },
    {
        "29. SHM phase",
        {
            "Possible questions:",
            "- what is phase?",
            "- explain phi in x=..",
            "",
            "Answer:",
            "In SHM:",
            "x = A*cos(wt+phi)",
            "",
            "The quantity wt+phi",
            "is phase of osc.",
            "phi = initial phase /",
            "phase constant.",
            "",
            "Phase sets position",
            "at t = 0.",
            NULL
        }
    },
    {
        "30. Damped osc",
        {
            "Possible questions:",
            "- what are damped osc?",
            "- what happens to A?",
            "",
            "Answer:",
            "Damped oscillations",
            "occur when resistive",
            "forces act on osc.",
            "",
            "Mechanical energy",
            "decreases with time.",
            "Amplitude decreases",
            "with time.",
            "",
            "Cause: friction or air",
            "resistance.",
            NULL
        }
    },
    {
        "31. Units extra",
        {
            "Possible questions:",
            "- unit of phi?",
            "- spring Ep unit?",
            "- rot work unit?",
            "- ang momentum unit?",
            "",
            "Answer:",
            "phi: rad",
            "spring Ep: J",
            "rot work: J",
            "L ang momentum:",
            "kg*m^2/s",
            "M torque: N*m",
            "I inertia: kg*m^2",
            "w: rad/s or s^-1",
            "alpha: rad/s^2 or s^-2",
            NULL
        }
    },
    {
        "32. SHM transform",
        {
            "Possible questions:",
            "- how energy changes?",
            "- where Ek max?",
            "- where Ep max?",
            "",
            "Answer:",
            "In ideal SHM:",
            "E = Ek + Ep = const",
            "",
            "At equilibrium x=0:",
            "speed max, Ek max,",
            "Ep min.",
            "",
            "At extremes x=+/-A:",
            "speed = 0, Ek = 0,",
            "Ep max.",
            "",
            "Energy transforms",
            "between Ek and Ep.",
            NULL
        }
    }
};

static const theory_entry_t THEORY_CLASSIC_EXAM_ENTRIES[] = {
    {
        "1. Uniform circle",
        {
            "Possible questions:",
            "- speed const, a=0?",
            "- which accel non-zero?",
            "- why accel exists?",
            "",
            "Answer:",
            "Speed is constant,",
            "but velocity is not",
            "constant, direction",
            "changes.",
            "",
            "Tangential accel:",
            "at = 0",
            "",
            "Centripetal accel:",
            "ac = v^2/r = r*w^2",
            "ac is non-zero and",
            "toward centre.",
            "",
            "Conclusion:",
            "Acceleration is not",
            "zero. Only ac non-zero.",
            NULL
        }
    },
    {
        "2. Accel comps",
        {
            "Possible questions:",
            "- resolve total accel",
            "- explain at and ac",
            "- angle with velocity",
            "",
            "Answer:",
            "Total accel has two",
            "perpendicular comps.",
            "",
            "at = r*alpha",
            "It is along tangent",
            "to the circle.",
            "It changes magnitude",
            "of velocity, so speed",
            "changes.",
            "",
            "ac = v^2/r = r*w^2",
            "It is directed toward",
            "the centre.",
            "It changes direction",
            "of velocity.",
            "",
            "a = sqrt(at^2+ac^2)",
            "",
            "v is tangent to path.",
            "at is parallel to v.",
            "ac is perpendicular",
            "to v.",
            "",
            "tan(beta)=ac/at",
            "beta=arctan(ac/at)",
            NULL
        }
    },
    {
        "3. Formula s=vt",
        {
            "Possible questions:",
            "- when is s=vt valid?",
            "- for accel motion?",
            "",
            "Answer:",
            "s = v*t is valid only",
            "for uniform motion.",
            "",
            "Condition:",
            "v = const",
            "",
            "Then:",
            "a = 0",
            "",
            "Conclusion:",
            "Use s=v*t only when",
            "speed is constant.",
            NULL
        }
    },
    {
        "4. Newton 2nd",
        {
            "Possible questions:",
            "- general form?",
            "- explain quantities",
            "- derive F=ma",
            "",
            "Answer:",
            "Newton's second law",
            "states that the net",
            "force acting on a",
            "body is equal to the",
            "time rate of change",
            "of its momentum.",
            "",
            "General form:",
            "F = dp/dt",
            "p = m*v",
            "",
            "F net force",
            "p linear momentum",
            "m mass, v velocity",
            "t time",
            "",
            "If m const:",
            "F = d(mv)/dt",
            "F = m*dv/dt = m*a",
            "",
            "Conclusion:",
            "For constant mass,",
            "Newton 2 becomes",
            "F = m*a.",
            NULL
        }
    },
    {
        "5. Momentum",
        {
            "Possible questions:",
            "- define momentum",
            "- formula and unit",
            "- scalar or vector?",
            "",
            "Answer:",
            "Linear momentum:",
            "p = m*v",
            "",
            "Meaning:",
            "quantity of motion.",
            "",
            "Unit:",
            "kg*m/s",
            "",
            "Momentum is a vector,",
            "same dir as velocity.",
            NULL
        }
    },
    {
        "6. Impulse",
        {
            "Possible questions:",
            "- define impulse",
            "- relation to p?",
            "- when I=Ft valid?",
            "",
            "Answer:",
            "Impulse is time",
            "effect of force.",
            "",
            "I = integral F dt",
            "I = delta p = p2-p1",
            "",
            "Unit:",
            "N*s = kg*m/s",
            "",
            "If force const:",
            "I = F*t",
            NULL
        }
    },
    {
        "7. Mom conserve",
        {
            "Possible questions:",
            "- when conserved?",
            "- when P const?",
            "",
            "Answer:",
            "Total linear momentum",
            "is conserved if",
            "net external force",
            "is zero.",
            "",
            "Condition:",
            "Fext = 0",
            "",
            "Then:",
            "P = const",
            NULL
        }
    },
    {
        "8. Work",
        {
            "Possible questions:",
            "- define work",
            "- formula for work",
            "- if F perp s?",
            "",
            "Answer:",
            "Work done by a force",
            "is the energy",
            "transferred by the",
            "force through a",
            "displacement.",
            "",
            "W = F*s = F*s*cos(phi)",
            "",
            "W work",
            "F force",
            "s displacement",
            "phi angle F,s",
            "",
            "If F parallel s:",
            "W = F*s",
            "If F perp s:",
            "W = 0",
            "Unit: J",
            NULL
        }
    },
    {
        "9. Work-energy",
        {
            "Possible questions:",
            "- state theorem",
            "- relation to Ek?",
            "",
            "Answer:",
            "The work-energy",
            "theorem states that",
            "the work done by the",
            "net force on a body",
            "is equal to the",
            "change in its",
            "kinetic energy.",
            "",
            "W = delta Ek",
            "W = Ek2 - Ek1",
            "",
            "Ek = 0.5*m*v^2",
            "",
            "Meaning:",
            "Work by net force",
            "changes kinetic",
            "energy.",
            "",
            "Work is change in Ek,",
            "not final Ek.",
            NULL
        }
    },
    {
        "10. Mech energy",
        {
            "Possible questions:",
            "- define mech energy",
            "- when conserved?",
            "- at max height?",
            "",
            "Answer:",
            "Mechanical energy is",
            "the sum of kinetic",
            "and potential energy",
            "of a body or system.",
            "",
            "E = Ek + Ep",
            "Ek = 0.5*m*v^2",
            "Ep = m*g*h",
            "",
            "If only conservative",
            "forces act:",
            "E = const",
            "",
            "At max height:",
            "v = 0, Ek = 0",
            "Energy is potential.",
            NULL
        }
    },
    {
        "11. Power",
        {
            "Possible questions:",
            "- define power",
            "- avg and inst?",
            "- use F and v?",
            "",
            "Answer:",
            "Power is the rate",
            "of doing work or",
            "transferring energy.",
            "",
            "P = W/t",
            "P = dW/dt",
            "P = F*v",
            "",
            "If same dir:",
            "P = Fv",
            "",
            "Unit: W",
            NULL
        }
    },
    {
        "12. Normal force",
        {
            "Possible questions:",
            "- force by table?",
            "- find normal force",
            "",
            "Answer:",
            "For body at rest on",
            "horizontal table:",
            "N = m*g",
            "",
            "N normal force",
            "m mass",
            "g grav accel",
            "",
            "Table pushes upward",
            "with force equal to",
            "weight.",
            NULL
        }
    },
    {
        "13. Torque",
        {
            "Possible questions:",
            "- define torque",
            "- formula and vars",
            "- what sets size?",
            "",
            "Answer:",
            "Torque is rotational",
            "effect of force.",
            "",
            "M = r x F",
            "Magnitude:",
            "M = r*F*sin(phi)",
            "",
            "Unit: N*m",
            "",
            "Larger force, arm,",
            "or angle to 90 deg",
            "gives larger torque.",
            NULL
        }
    },
    {
        "14. Rigid law",
        {
            "Possible questions:",
            "- 2nd law rotation",
            "- analogue of F=ma",
            "- explain M=Ialpha",
            "",
            "Answer:",
            "General:",
            "M = dL/dt",
            "L = I*w",
            "",
            "If I const:",
            "M = I*alpha",
            "",
            "Torque changes ang",
            "momentum.",
            NULL
        }
    },
    {
        "15. Inertia",
        {
            "Possible questions:",
            "- define inertia I",
            "- analogue of mass?",
            "- unit of I?",
            "",
            "Answer:",
            "Moment of inertia",
            "characterizes resistance",
            "to rotational motion.",
            "",
            "It is rotational",
            "analogue of mass.",
            "",
            "Unit:",
            "kg*m^2",
            "",
            "disk: 0.5*m*r^2",
            "rod c: (1/12)m*l^2",
            NULL
        }
    },
    {
        "16. Ang mom",
        {
            "Possible questions:",
            "- define ang momentum",
            "- when conserved?",
            "",
            "Answer:",
            "L = I*w",
            "",
            "If Mext = 0:",
            "L = const",
            "",
            "For two states:",
            "I1*w1 = I2*w2",
            "",
            "If no external torque,",
            "angular momentum",
            "is conserved.",
            NULL
        }
    },
    {
        "17. Rot Ek",
        {
            "Possible questions:",
            "- rotational Ek?",
            "- compare with linear?",
            "",
            "Answer:",
            "Ek = 0.5*I*w^2",
            "",
            "Unit: J",
            "",
            "Analogy:",
            "linear: 0.5*m*v^2",
            "rotation: 0.5*I*w^2",
            NULL
        }
    },
    {
        "18. SHM def",
        {
            "Possible questions:",
            "- define SHM",
            "- restoring force?",
            "- minus sign in -kx?",
            "",
            "Answer:",
            "SHM has restoring",
            "force proportional to",
            "displacement and",
            "opposite to it.",
            "",
            "F = -k*x",
            "a = -(k/m)*x",
            "w^2 = k/m",
            "a = -w^2*x",
            NULL
        }
    },
    {
        "19. SHM eqs",
        {
            "Possible questions:",
            "- write x,v,a",
            "- find v,a from x",
            "- what is a=-w^2x?",
            "",
            "Answer:",
            "If x=A*cos(wt+phi):",
            "v=-A*w*sin(wt+phi)",
            "a=-A*w^2*cos(wt+phi)",
            "",
            "If x=A*sin(wt+phi):",
            "v=A*w*cos(wt+phi)",
            "a=-A*w^2*sin(wt+phi)",
            "",
            "Always:",
            "a = -w^2*x",
            NULL
        }
    },
    {
        "20. T f w",
        {
            "Possible questions:",
            "- define T and f",
            "- relations T,f,w",
            "",
            "Answer:",
            "T = one oscillation",
            "f = oscill/s",
            "w = ang frequency",
            "",
            "f = 1/T",
            "T = 1/f",
            "w = 2*pi*f",
            "w = 2*pi/T",
            "T = 2*pi/w",
            NULL
        }
    },
    {
        "21. SHM energy",
        {
            "Possible questions:",
            "- Ek and Ep in SHM",
            "- total energy?",
            "- depend on time?",
            "",
            "Answer:",
            "Ek = 0.5*m*v^2",
            "Ep = 0.5*k*x^2",
            "E = Ek + Ep",
            "",
            "For SHM:",
            "E = 0.5*k*A^2",
            "E = 0.5*m*w^2*A^2",
            "",
            "Time dependence:",
            "E = const",
            NULL
        }
    },
    {
        "22. SHM derive",
        {
            "Possible questions:",
            "- derive SHM energy",
            "- show E=0.5kA^2",
            "- time dependence?",
            "",
            "Answer:",
            "x = A*cos(wt+phi)",
            "v = -A*w*sin(wt+phi)",
            "Ek=0.5*m*A^2*w^2*",
            "sin^2(wt+phi)",
            "Ep=0.5*k*A^2*",
            "cos^2(wt+phi)",
            "k = m*w^2",
            "E=0.5*m*w^2*A^2",
            "E=0.5*k*A^2",
            "Conclusion: E=const.",
            NULL
        }
    },
    {
        "23. A and energy",
        {
            "Possible questions:",
            "- if A doubles?",
            "- if A triples?",
            "- freq depend on A?",
            "",
            "Answer:",
            "E = 0.5*k*A^2",
            "E proportional A^2",
            "",
            "A -> 2A => E -> 4E",
            "A -> 3A => E -> 9E",
            "",
            "For ideal SHM:",
            "frequency does not",
            "depend on amplitude.",
            NULL
        }
    },
    {
        "24. Units",
        {
            "Possible questions:",
            "- unit of impulse?",
            "- unit of torque?",
            "- unit of power?",
            "- unit of ang vel?",
            "",
            "Answer:",
            "p: kg*m/s",
            "I impulse: N*s",
            "F: N",
            "W,E: J",
            "P: W",
            "M torque: N*m",
            "w: s^-1 or rad/s",
            "alpha: s^-2 or rad/s^2",
            "I inertia: kg*m^2",
            "k: N/m, f: Hz, T: s",
            NULL
        }
    }
};

static const theory_entry_t THEORY_EXTRA_HIGH_PROB_ENTRIES[] = {
    {
        "1. Speed vs vel",
        {
            "Possible questions:",
            "- speed vs velocity?",
            "- speed const, vel chg?",
            "- scalar or vector?",
            "",
            "Answer:",
            "Speed is scalar.",
            "It shows how fast",
            "a body moves.",
            "",
            "Velocity is vector.",
            "It has magnitude",
            "and direction.",
            "",
            "In circle motion,",
            "speed can be const",
            "while velocity changes.",
            NULL
        }
    },
    {
        "2. Dist vs disp",
        {
            "Possible questions:",
            "- distance vs disp?",
            "- disp scalar/vector?",
            "- can dist be larger?",
            "",
            "Answer:",
            "Distance is total",
            "path length.",
            "It is scalar.",
            "",
            "Displacement is",
            "change of position.",
            "delta r = r2 - r1",
            "It is vector.",
            "",
            "Distance depends on",
            "path, displacement not.",
            NULL
        }
    },
    {
        "3. Avg spd/vel",
        {
            "Possible questions:",
            "- define avg speed",
            "- define avg velocity",
            "- difference?",
            "",
            "Answer:",
            "Avg speed = total",
            "distance / total time",
            "",
            "Avg velocity =",
            "displacement / time",
            "v_avg = delta r/delta t",
            "",
            "Avg speed scalar,",
            "avg velocity vector.",
            NULL
        }
    },
    {
        "4. Cons force",
        {
            "Possible questions:",
            "- define cons force",
            "- examples?",
            "- special about work?",
            "",
            "Answer:",
            "Conservative force has",
            "work independent of",
            "path.",
            "",
            "Examples:",
            "gravity, spring force",
            "",
            "Then mechanical energy",
            "is conserved:",
            "E = Ek + Ep = const",
            NULL
        }
    },
    {
        "5. Noncons force",
        {
            "Possible questions:",
            "- define non-cons",
            "- examples?",
            "- mech energy?",
            "",
            "Answer:",
            "Non-conservative force",
            "has work depending",
            "on path.",
            "",
            "Examples:",
            "friction, air resist",
            "",
            "Mechanical energy is",
            "not conserved.",
            NULL
        }
    },
    {
        "6. Weight",
        {
            "Possible questions:",
            "- define weight",
            "- same as mass?",
            "- formula and unit?",
            "",
            "Answer:",
            "Weight is the",
            "gravitational force",
            "acting on a body.",
            "",
            "G = m*g",
            "",
            "Unit: N",
            "Mass is in kg.",
            "",
            "Mass and weight are",
            "not the same.",
            NULL
        }
    },
    {
        "7. Inertia",
        {
            "Possible questions:",
            "- what is inertia?",
            "- what measures it?",
            "- link to Newton 1?",
            "",
            "Answer:",
            "Inertia is property",
            "of body to resist",
            "changes in motion.",
            "",
            "Mass measures inertia.",
            "Larger mass =>",
            "larger inertia.",
            "",
            "Newton 1 describes it.",
            NULL
        }
    },
    {
        "8. Net force",
        {
            "Possible questions:",
            "- what is net force?",
            "- if zero?",
            "- if non-zero?",
            "",
            "Answer:",
            "Net force is the",
            "vector sum of all",
            "forces acting on",
            "a body.",
            "",
            "If Fnet = 0:",
            "a = 0, v = const",
            "",
            "If Fnet != 0:",
            "body accelerates.",
            NULL
        }
    },
    {
        "9. Resultant F",
        {
            "Possible questions:",
            "- what is resultant F",
            "- transl eqbm?",
            "- move if zero?",
            "",
            "Answer:",
            "Resultant force is",
            "the same as net",
            "force. It is the",
            "vector sum of all",
            "forces on a body.",
            "",
            "Transl equilibrium:",
            "sum F = 0",
            "",
            "Body may be at rest",
            "or move with const v.",
            NULL
        }
    },
    {
        "10. Scalar/vector",
        {
            "Possible questions:",
            "- scalar examples?",
            "- vector examples?",
            "- is energy vector?",
            "",
            "Answer:",
            "Scalars: mass, time,",
            "speed, distance,",
            "energy, work, power.",
            "",
            "Vectors: disp, vel,",
            "accel, force,",
            "momentum, impulse.",
            "",
            "Energy and work",
            "are scalar.",
            NULL
        }
    },
    {
        "11. Periodic mot",
        {
            "Possible questions:",
            "- what is periodic?",
            "- is SHM periodic?",
            "- what is period?",
            "",
            "Answer:",
            "Periodic motion repeats",
            "after equal time",
            "intervals.",
            "",
            "T is time of one",
            "complete repetition.",
            "",
            "SHM is periodic.",
            NULL
        }
    },
    {
        "12. Freq meaning",
        {
            "Possible questions:",
            "- meaning of freq?",
            "- unit?",
            "- relation to T?",
            "",
            "Answer:",
            "Frequency is number",
            "of oscillations or",
            "revolutions per second.",
            "",
            "f = 1/T",
            "Unit: Hz or s^-1",
            "",
            "Higher f means more",
            "cycles per second.",
            NULL
        }
    },
    {
        "13. Ang freq",
        {
            "Possible questions:",
            "- what is ang freq?",
            "- link to f?",
            "- what unit?",
            "",
            "Answer:",
            "Angular frequency is",
            "oscillation rate in",
            "radians per second.",
            "",
            "w = 2*pi*f",
            "w = 2*pi/T",
            "",
            "Unit: rad/s or s^-1",
            NULL
        }
    },
    {
        "14. SHM phase",
        {
            "Possible questions:",
            "- what is phase?",
            "- explain phi in x=..",
            "- initial phase?",
            "",
            "Answer:",
            "In x=A*cos(wt+phi),",
            "the quantity wt+phi",
            "is phase.",
            "",
            "phi is initial phase",
            "or phase constant.",
            "",
            "Phase gives state of",
            "oscillator at time t.",
            NULL
        }
    },
    {
        "15. Amplitude",
        {
            "Possible questions:",
            "- define amplitude",
            "- amplitude in SHM?",
            "- affect energy?",
            "",
            "Answer:",
            "Amplitude A is max",
            "displacement from",
            "equilibrium.",
            "",
            "x ranges from -A to A",
            "E = 0.5*k*A^2",
            "",
            "Energy proportional",
            "to A^2.",
            NULL
        }
    },
    {
        "16. Eqbm in SHM",
        {
            "Possible questions:",
            "- what is eqbm pos?",
            "- v and a there?",
            "- energies there?",
            "",
            "Answer:",
            "Equilibrium position:",
            "x = 0",
            "F = 0",
            "a = 0",
            "",
            "Speed and Ek max,",
            "Ep min.",
            NULL
        }
    },
    {
        "17. Extreme SHM",
        {
            "Possible questions:",
            "- what at x=A?",
            "- v and a there?",
            "- energies there?",
            "",
            "Answer:",
            "Extreme position:",
            "x = +/-A",
            "v = 0",
            "Ek = 0",
            "Ep max",
            "",
            "|a| max from",
            "a = -w^2*x",
            NULL
        }
    },
    {
        "18. SHM vmax",
        {
            "Possible questions:",
            "- where vmax in SHM?",
            "- formula?",
            "- where Ek max?",
            "",
            "Answer:",
            "vmax occurs at",
            "equilibrium x = 0",
            "",
            "vmax = A*w",
            "",
            "There Ek max,",
            "Ep min.",
            NULL
        }
    },
    {
        "19. SHM amax",
        {
            "Possible questions:",
            "- where amax in SHM?",
            "- formula?",
            "",
            "Answer:",
            "a = -w^2*x",
            "",
            "|a| is max when",
            "|x| is max,",
            "so at x = +/-A",
            "",
            "amax = A*w^2",
            NULL
        }
    },
    {
        "20. Ek vs speed",
        {
            "Possible questions:",
            "- if speed doubles?",
            "- if speed triples?",
            "",
            "Answer:",
            "Ek = 0.5*m*v^2",
            "Ek proportional v^2",
            "",
            "v -> 2v => Ek -> 4Ek",
            "v -> 3v => Ek -> 9Ek",
            "",
            "Kinetic energy depends",
            "on square of speed.",
            NULL
        }
    },
    {
        "21. Work sign",
        {
            "Possible questions:",
            "- when W positive?",
            "- when W negative?",
            "- when W zero?",
            "",
            "Answer:",
            "W = F*s*cos(phi)",
            "",
            "Positive: force has",
            "component along motion.",
            "Negative: opposite.",
            "Zero: perpendicular.",
            "",
            "Sign depends on angle.",
            NULL
        }
    },
    {
        "22. Watt meaning",
        {
            "Possible questions:",
            "- what does watt mean?",
            "- unit of power?",
            "- J and s relation?",
            "",
            "Answer:",
            "Power: P = W/t",
            "",
            "Unit: watt",
            "1 W = 1 J/s",
            "",
            "Power shows how much",
            "work is done per sec.",
            NULL
        }
    },
    {
        "23. Torque zero",
        {
            "Possible questions:",
            "- when torque zero?",
            "- if F parallel arm?",
            "- if at axis?",
            "",
            "Answer:",
            "M = r*F*sin(phi)",
            "",
            "M = 0 if r=0,",
            "or F=0,",
            "or phi=0 or 180 deg",
            "",
            "Parallel to arm =>",
            "sin(phi)=0 => M=0",
            NULL
        }
    },
    {
        "24. Torque max",
        {
            "Possible questions:",
            "- when torque max?",
            "- what angle?",
            "",
            "Answer:",
            "M = r*F*sin(phi)",
            "",
            "Torque max when",
            "sin(phi)=1",
            "so phi=90 deg",
            "",
            "Then M = r*F",
            NULL
        }
    },
    {
        "25. I depends on",
        {
            "Possible questions:",
            "- only on mass?",
            "- why distrib matter?",
            "",
            "Answer:",
            "Moment of inertia",
            "depends on mass,",
            "shape, axis,",
            "mass distribution.",
            "",
            "Mass farther from axis",
            "gives larger I.",
            NULL
        }
    },
    {
        "26. Rot analogies",
        {
            "Possible questions:",
            "- compare lin/rot",
            "- analogue of force?",
            "- analogue of mass?",
            "",
            "Answer:",
            "s->phi, v->w, a->a",
            "m->I, F->M, p->L",
            "",
            "F = m*a",
            "M = I*alpha",
            "",
            "Ek=0.5*m*v^2",
            "Ek_rot=0.5*I*w^2",
            NULL
        }
    },
    {
        "27. Angle and N",
        {
            "Possible questions:",
            "- radians in one rev?",
            "- phi and N relation?",
            "",
            "Answer:",
            "One revolution =",
            "2*pi radians",
            "",
            "phi = 2*pi*N",
            "N = phi/(2*pi)",
            "",
            "Use 2*pi for one turn.",
            NULL
        }
    },
    {
        "28. Const alpha",
        {
            "Possible questions:",
            "- formulas for alpha c",
            "- uniform rot accel?",
            "",
            "Answer:",
            "alpha = const",
            "",
            "w = w0 + alpha*t",
            "phi = w0*t +",
            "0.5*alpha*t^2",
            "w^2 = w0^2 +",
            "2*alpha*phi",
            NULL
        }
    },
    {
        "29. v and w",
        {
            "Possible questions:",
            "- linear vs angular?",
            "- tangential speed?",
            "",
            "Answer:",
            "v = r*w",
            "",
            "v tangential speed",
            "r radius",
            "w angular speed",
            "",
            "Larger r gives",
            "larger v for same w.",
            NULL
        }
    },
    {
        "30. p vs Ek",
        {
            "Possible questions:",
            "- same thing?",
            "- depend on velocity?",
            "",
            "Answer:",
            "p = m*v",
            "Ek = 0.5*m*v^2",
            "",
            "Momentum is vector.",
            "Kinetic energy scalar.",
            "",
            "p depends on v,",
            "Ek depends on v^2",
            NULL
        }
    },
    {
        "31. I unit trap",
        {
            "Possible questions:",
            "- why N*s=kg*m/s?",
            "- unit of impulse?",
            "",
            "Answer:",
            "I = F*t",
            "Unit: N*s",
            "",
            "1 N = 1 kg*m/s^2",
            "so N*s = kg*m/s",
            "",
            "Impulse and momentum",
            "have same units.",
            NULL
        }
    },
    {
        "32. F vs M",
        {
            "Possible questions:",
            "- force vs torque?",
            "- what do they cause?",
            "",
            "Answer:",
            "Force causes linear",
            "acceleration:",
            "F = m*a",
            "",
            "Torque causes angular",
            "acceleration:",
            "M = I*alpha",
            "",
            "Torque is rotational",
            "analogue of force.",
            NULL
        }
    }
};

static const theory_entry_t *const THEORY_CLASSIC_EXAM_PTRS[] = {
    &THEORY_CLASSIC_EXAM_ENTRIES[0],  &THEORY_CLASSIC_EXAM_ENTRIES[1],
    &THEORY_CLASSIC_EXAM_ENTRIES[2],  &THEORY_CLASSIC_EXAM_ENTRIES[3],
    &THEORY_CLASSIC_EXAM_ENTRIES[4],  &THEORY_CLASSIC_EXAM_ENTRIES[5],
    &THEORY_CLASSIC_EXAM_ENTRIES[6],  &THEORY_CLASSIC_EXAM_ENTRIES[7],
    &THEORY_CLASSIC_EXAM_ENTRIES[8],  &THEORY_CLASSIC_EXAM_ENTRIES[9],
    &THEORY_CLASSIC_EXAM_ENTRIES[10], &THEORY_CLASSIC_EXAM_ENTRIES[11],
    &THEORY_CLASSIC_EXAM_ENTRIES[12], &THEORY_CLASSIC_EXAM_ENTRIES[13],
    &THEORY_CLASSIC_EXAM_ENTRIES[14], &THEORY_CLASSIC_EXAM_ENTRIES[15],
    &THEORY_CLASSIC_EXAM_ENTRIES[16], &THEORY_CLASSIC_EXAM_ENTRIES[17],
    &THEORY_CLASSIC_EXAM_ENTRIES[18], &THEORY_CLASSIC_EXAM_ENTRIES[19],
    &THEORY_CLASSIC_EXAM_ENTRIES[20], &THEORY_CLASSIC_EXAM_ENTRIES[21],
    &THEORY_CLASSIC_EXAM_ENTRIES[22], &THEORY_CLASSIC_EXAM_ENTRIES[23]
};

static const theory_entry_t *const THEORY_QUICK_ENTRIES[] = {
    &THEORY_ENTRIES[0],
    &THEORY_ENTRIES[1],
    &THEORY_ENTRIES[2],
    &THEORY_ENTRIES[3],
    &THEORY_ENTRIES[6],
    &THEORY_ENTRIES[8],
    &THEORY_ENTRIES[9],
    &THEORY_ENTRIES[10],
    &THEORY_ENTRIES[11],
    &THEORY_ENTRIES[12],
    &THEORY_ENTRIES[13],
    &THEORY_ENTRIES[17],
    &THEORY_ENTRIES[18],
    &THEORY_ENTRIES[20],
    &THEORY_ENTRIES[25],
    &THEORY_ENTRIES[26],
    &THEORY_ENTRIES[31]
};

static const theory_entry_t *const THEORY_EXAM_FAST_ENTRIES[] = {
    &THEORY_CLASSIC_EXAM_ENTRIES[3],
    &THEORY_CLASSIC_EXAM_ENTRIES[5],
    &THEORY_CLASSIC_EXAM_ENTRIES[6],
    &THEORY_CLASSIC_EXAM_ENTRIES[8],
    &THEORY_CLASSIC_EXAM_ENTRIES[9],
    &THEORY_CLASSIC_EXAM_ENTRIES[10],
    &THEORY_CLASSIC_EXAM_ENTRIES[12],
    &THEORY_CLASSIC_EXAM_ENTRIES[13],
    &THEORY_CLASSIC_EXAM_ENTRIES[15],
    &THEORY_CLASSIC_EXAM_ENTRIES[17],
    &THEORY_CLASSIC_EXAM_ENTRIES[18],
    &THEORY_CLASSIC_EXAM_ENTRIES[19],
    &THEORY_CLASSIC_EXAM_ENTRIES[20],
    &THEORY_CLASSIC_EXAM_ENTRIES[21],
    &THEORY_CLASSIC_EXAM_ENTRIES[23]
};

static const theory_entry_t *const THEORY_MOTION_ENTRIES[] = {
    &THEORY_ENTRIES[0],
    &THEORY_ENTRIES[1],
    &THEORY_ENTRIES[2],
    &THEORY_ENTRIES[3],
    &THEORY_ENTRIES[4],
    &THEORY_ENTRIES[8],
    &THEORY_ENTRIES[9],
    &THEORY_ENTRIES[23],
    &THEORY_ENTRIES[24]
};

static const theory_entry_t *const THEORY_FORCES_CORE_ENTRIES[] = {
    &THEORY_ENTRIES[5],
    &THEORY_ENTRIES[6],
    &THEORY_ENTRIES[7],
    &THEORY_ENTRIES[10],
    &THEORY_ENTRIES[11],
    &THEORY_ENTRIES[12],
    &THEORY_ENTRIES[13],
    &THEORY_ENTRIES[14],
    &THEORY_ENTRIES[24]
};

static const theory_entry_t *const THEORY_ROTATION_CORE_ENTRIES[] = {
    &THEORY_ENTRIES[15],
    &THEORY_ENTRIES[16],
    &THEORY_ENTRIES[17],
    &THEORY_ENTRIES[18],
    &THEORY_ENTRIES[19],
    &THEORY_ENTRIES[20],
    &THEORY_ENTRIES[21],
    &THEORY_ENTRIES[22],
    &THEORY_ENTRIES[24]
};

static const theory_entry_t *const THEORY_SHM_FAST_ENTRIES[] = {
    &THEORY_ENTRIES[25],
    &THEORY_ENTRIES[26],
    &THEORY_ENTRIES[27],
    &THEORY_ENTRIES[28],
    &THEORY_ENTRIES[29],
    &THEORY_ENTRIES[30],
    &THEORY_ENTRIES[31]
};

static const theory_entry_t *const THEORY_FORCES_ENERGY_ENTRIES[] = {
    &THEORY_ENTRIES[2],
    &THEORY_ENTRIES[3],
    &THEORY_ENTRIES[4],
    &THEORY_ENTRIES[5],
    &THEORY_ENTRIES[6],
    &THEORY_ENTRIES[7],
    &THEORY_ENTRIES[8],
    &THEORY_ENTRIES[9],
    &THEORY_ENTRIES[10],
    &THEORY_ENTRIES[11],
    &THEORY_ENTRIES[12],
    &THEORY_ENTRIES[13],
    &THEORY_ENTRIES[14],
    &THEORY_ENTRIES[24]
};

static const theory_entry_t *const THEORY_ROTATION_ENTRIES[] = {
    &THEORY_ENTRIES[15],
    &THEORY_ENTRIES[16],
    &THEORY_ENTRIES[17],
    &THEORY_ENTRIES[18],
    &THEORY_ENTRIES[19],
    &THEORY_ENTRIES[20],
    &THEORY_ENTRIES[21],
    &THEORY_ENTRIES[22],
    &THEORY_ENTRIES[23],
    &THEORY_ENTRIES[24]
};

static const theory_entry_t *const THEORY_SHM_ENTRIES[] = {
    &THEORY_ENTRIES[18],
    &THEORY_ENTRIES[19],
    &THEORY_ENTRIES[20],
    &THEORY_ENTRIES[21],
    &THEORY_ENTRIES[22],
    &THEORY_ENTRIES[25],
    &THEORY_ENTRIES[26],
    &THEORY_ENTRIES[27],
    &THEORY_ENTRIES[28],
    &THEORY_ENTRIES[29],
    &THEORY_ENTRIES[30],
    &THEORY_ENTRIES[31]
};

static const theory_entry_t *const THEORY_EXAMPLE_ENTRIES[] = {
    &THEORY_ENTRIES[0],
    &THEORY_ENTRIES[2],
    &THEORY_ENTRIES[4],
    &THEORY_ENTRIES[10],
    &THEORY_ENTRIES[17],
    &THEORY_ENTRIES[21],
    &THEORY_ENTRIES[31]
};

static const theory_entry_t *const THEORY_ALL_ENTRIES[] = {
    &THEORY_ENTRIES[0],  &THEORY_ENTRIES[1],  &THEORY_ENTRIES[2],
    &THEORY_ENTRIES[3],  &THEORY_ENTRIES[4],  &THEORY_ENTRIES[5],
    &THEORY_ENTRIES[6],  &THEORY_ENTRIES[7],  &THEORY_ENTRIES[8],
    &THEORY_ENTRIES[9],  &THEORY_ENTRIES[10], &THEORY_ENTRIES[11],
    &THEORY_ENTRIES[12], &THEORY_ENTRIES[13], &THEORY_ENTRIES[14],
    &THEORY_ENTRIES[15], &THEORY_ENTRIES[16], &THEORY_ENTRIES[17],
    &THEORY_ENTRIES[18], &THEORY_ENTRIES[19], &THEORY_ENTRIES[20],
    &THEORY_ENTRIES[21], &THEORY_ENTRIES[22], &THEORY_ENTRIES[23],
    &THEORY_ENTRIES[24], &THEORY_ENTRIES[25], &THEORY_ENTRIES[26],
    &THEORY_ENTRIES[27], &THEORY_ENTRIES[28], &THEORY_ENTRIES[29],
    &THEORY_ENTRIES[30], &THEORY_ENTRIES[31]
};

static const theory_entry_t *const THEORY_FULL_ENTRIES[] = {
    &THEORY_CLASSIC_EXAM_ENTRIES[0],  &THEORY_CLASSIC_EXAM_ENTRIES[1],
    &THEORY_CLASSIC_EXAM_ENTRIES[2],  &THEORY_CLASSIC_EXAM_ENTRIES[3],
    &THEORY_CLASSIC_EXAM_ENTRIES[4],  &THEORY_CLASSIC_EXAM_ENTRIES[5],
    &THEORY_CLASSIC_EXAM_ENTRIES[6],  &THEORY_CLASSIC_EXAM_ENTRIES[7],
    &THEORY_CLASSIC_EXAM_ENTRIES[8],  &THEORY_CLASSIC_EXAM_ENTRIES[9],
    &THEORY_CLASSIC_EXAM_ENTRIES[10], &THEORY_CLASSIC_EXAM_ENTRIES[11],
    &THEORY_CLASSIC_EXAM_ENTRIES[12], &THEORY_CLASSIC_EXAM_ENTRIES[13],
    &THEORY_CLASSIC_EXAM_ENTRIES[14], &THEORY_CLASSIC_EXAM_ENTRIES[15],
    &THEORY_CLASSIC_EXAM_ENTRIES[16], &THEORY_CLASSIC_EXAM_ENTRIES[17],
    &THEORY_CLASSIC_EXAM_ENTRIES[18], &THEORY_CLASSIC_EXAM_ENTRIES[19],
    &THEORY_CLASSIC_EXAM_ENTRIES[20], &THEORY_CLASSIC_EXAM_ENTRIES[21],
    &THEORY_CLASSIC_EXAM_ENTRIES[22], &THEORY_CLASSIC_EXAM_ENTRIES[23],
    &THEORY_ENTRIES[0],  &THEORY_ENTRIES[1],  &THEORY_ENTRIES[2],
    &THEORY_ENTRIES[3],  &THEORY_ENTRIES[4],  &THEORY_ENTRIES[5],
    &THEORY_ENTRIES[6],  &THEORY_ENTRIES[7],  &THEORY_ENTRIES[8],
    &THEORY_ENTRIES[9],  &THEORY_ENTRIES[10], &THEORY_ENTRIES[11],
    &THEORY_ENTRIES[12], &THEORY_ENTRIES[13], &THEORY_ENTRIES[14],
    &THEORY_ENTRIES[15], &THEORY_ENTRIES[16], &THEORY_ENTRIES[17],
    &THEORY_ENTRIES[18], &THEORY_ENTRIES[19], &THEORY_ENTRIES[20],
    &THEORY_ENTRIES[21], &THEORY_ENTRIES[22], &THEORY_ENTRIES[23],
    &THEORY_ENTRIES[24], &THEORY_ENTRIES[25], &THEORY_ENTRIES[26],
    &THEORY_ENTRIES[27], &THEORY_ENTRIES[28], &THEORY_ENTRIES[29],
    &THEORY_ENTRIES[30], &THEORY_ENTRIES[31],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[0],  &THEORY_EXTRA_HIGH_PROB_ENTRIES[1],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[2],  &THEORY_EXTRA_HIGH_PROB_ENTRIES[3],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[4],  &THEORY_EXTRA_HIGH_PROB_ENTRIES[5],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[6],  &THEORY_EXTRA_HIGH_PROB_ENTRIES[7],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[8],  &THEORY_EXTRA_HIGH_PROB_ENTRIES[9],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[10], &THEORY_EXTRA_HIGH_PROB_ENTRIES[11],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[12], &THEORY_EXTRA_HIGH_PROB_ENTRIES[13],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[14], &THEORY_EXTRA_HIGH_PROB_ENTRIES[15],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[16], &THEORY_EXTRA_HIGH_PROB_ENTRIES[17],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[18], &THEORY_EXTRA_HIGH_PROB_ENTRIES[19],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[20], &THEORY_EXTRA_HIGH_PROB_ENTRIES[21],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[22], &THEORY_EXTRA_HIGH_PROB_ENTRIES[23],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[24], &THEORY_EXTRA_HIGH_PROB_ENTRIES[25],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[26], &THEORY_EXTRA_HIGH_PROB_ENTRIES[27],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[28], &THEORY_EXTRA_HIGH_PROB_ENTRIES[29],
    &THEORY_EXTRA_HIGH_PROB_ENTRIES[30], &THEORY_EXTRA_HIGH_PROB_ENTRIES[31]
};

static const theory_category_t THEORY_CATEGORIES[] = {
    { "All Theory", (uint8_t)(sizeof(THEORY_FULL_ENTRIES) / sizeof(THEORY_FULL_ENTRIES[0])), THEORY_FULL_ENTRIES }
};
static menu_result_t theory_select_category(uint8_t *selected_index, const theory_category_t **category_out) {
    const uint8_t count = (uint8_t)(sizeof(THEORY_CATEGORIES) / sizeof(THEORY_CATEGORIES[0]));

    while (true) {
        uint8_t start = 0;
        uint8_t i = 0;
        uint8_t line = 2;
        uint8_t action;

        if (*selected_index >= 6) {
            start = *selected_index - 5;
        }

        io_clear_screen();
        io_draw_title("Physics Theory");

        for (i = start; i < count && line < 9; ++i, ++line) {
            char row[28];
            snprintf(row, sizeof(row), "%c %s", i == *selected_index ? '>' : ' ', THEORY_CATEGORIES[i].name);
            io_draw_wrapped_text(line, row, 26);
        }

        io_draw_footer("UP/DN OK MD/CLR BK");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && *selected_index > 0) {
            --(*selected_index);
        } else if (action == IO_MENU_DOWN && *selected_index + 1 < count) {
            ++(*selected_index);
        } else if (action == IO_MENU_SELECT) {
            *category_out = &THEORY_CATEGORIES[*selected_index];
            return MENU_STAY;
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

static menu_result_t theory_select_entry(const theory_category_t *category, uint8_t *selected_index, const theory_entry_t **entry_out) {
    while (true) {
        uint8_t start = 0;
        uint8_t i = 0;
        uint8_t line = 2;
        uint8_t action;

        if (*selected_index >= 6) {
            start = *selected_index - 5;
        }

        io_clear_screen();
        io_draw_title(category->name);

        for (i = start; i < category->entry_count && line < 9; ++i, ++line) {
            char row[28];
            snprintf(row, sizeof(row), "%c %s", i == *selected_index ? '>' : ' ', category->entries[i]->title);
            io_draw_wrapped_text(line, row, 26);
        }

        io_draw_footer("UP/DN OK MD/CLR BK");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && *selected_index > 0) {
            --(*selected_index);
        } else if (action == IO_MENU_DOWN && *selected_index + 1 < category->entry_count) {
            ++(*selected_index);
        } else if (action == IO_MENU_SELECT) {
            *entry_out = category->entries[*selected_index];
            return MENU_STAY;
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

static menu_result_t theory_show_entry(const theory_entry_t *entry) {
    uint8_t start = 0;

    while (true) {
        uint8_t total = 0;
        uint8_t shown = 0;
        uint8_t i = 0;
        uint8_t action;

        while (total < 80 && entry->lines[total] != NULL) {
            ++total;
        }

        io_clear_screen();
        io_draw_title(entry->title);

        for (i = start; i < total && shown < 7; ++i, ++shown) {
            io_draw_wrapped_text((uint8_t)(1 + shown), entry->lines[i], 26);
        }

        io_draw_footer("UP/DN scr MD/CLR BK");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && start > 0) {
            --start;
        } else if (action == IO_MENU_DOWN && start + 7 < total) {
            ++start;
        } else if (action == IO_MENU_BACK || action == IO_MENU_SELECT) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}

menu_result_t theory_run(void) {
    uint8_t selected_category = 0;
    uint8_t selected_entry = 0;

    while (true) {
        const theory_category_t *category = NULL;
        menu_result_t result = theory_select_category(&selected_category, &category);

        if (result != MENU_STAY) {
            return result;
        }

        selected_entry = 0;
        while (true) {
            const theory_entry_t *entry = NULL;
            result = theory_select_entry(category, &selected_entry, &entry);
            if (result == MENU_BACK) {
                break;
            }
            if (result == MENU_EXIT_APP) {
                return MENU_EXIT_APP;
            }

            result = theory_show_entry(entry);
            if (result == MENU_EXIT_APP) {
                return MENU_EXIT_APP;
            }
        }
    }
}

menu_result_t theory_show_topic_by_index(uint8_t index) {
    if (index >= (uint8_t)(sizeof(THEORY_ENTRIES) / sizeof(THEORY_ENTRIES[0]))) {
        return MENU_BACK;
    }
    return theory_show_entry(&THEORY_ENTRIES[index]);
}
