#include <stdio.h>

#include "io_helpers.h"
#include "symbols.h"

typedef struct {
    const char *title;
    const char *lines[6];
} symbol_entry_t;

static const symbol_entry_t SYMBOL_ENTRIES[] = {
    {
        "Basic motion",
        {
            "s = displacement [m]",
            "s0 = initial position [m]",
            "t = time [s]",
            "v = velocity / speed [m/s]",
            "v0 = initial velocity [m/s]",
            "a = acceleration [m/s^2]"
        }
    },
    {
        "Circular motion",
        {
            "r = radius [m]",
            "phi = angle [rad]",
            "omega = angular speed [rad/s]",
            "omega0 = init ang speed [rad/s]",
            "alpha = ang accel [rad/s^2]",
            "N = revolutions [rev]"
        }
    },
    {
        "Circular accel",
        {
            "a_t = tangential accel [m/s^2]",
            "a_c = centripetal accel [m/s^2]",
            "T = period [s]",
            "f = frequency [Hz]",
            "pi = 3.14159...",
            ""
        }
    },
    {
        "Dynamics",
        {
            "F = force [N]",
            "m = mass [kg]",
            "p = momentum [kg*m/s]",
            "I = impulse [N*s]",
            "g = grav accel [m/s^2]",
            ""
        }
    },
    {
        "Energy / power",
        {
            "W = work [J]",
            "P = power [W]",
            "Pavg = avg power [W]",
            "Ek = kinetic energy [J]",
            "Ep = potential energy [J]",
            "E = mechanical energy [J]"
        }
    },
    {
        "More symbols",
        {
            "dEp = change in Ep [J]",
            "dEk = change in Ek [J]",
            "d = distance [m]",
            "k = spring const [N/m]",
            "dx/dt = derivative",
            "int = integral"
        }
    }
};

menu_result_t symbols_run(void) {
    uint8_t index = 0;
    const uint8_t count = (uint8_t)(sizeof(SYMBOL_ENTRIES) / sizeof(SYMBOL_ENTRIES[0]));

    while (true) {
        const symbol_entry_t *entry = &SYMBOL_ENTRIES[index];
        uint8_t action;
        uint8_t i;
        char header[28];

        io_clear_screen();
        snprintf(header, sizeof(header), "Symbols %u/%u", (unsigned)(index + 1), (unsigned)count);
        io_draw_title(header);
        io_draw_wrapped_text(1, entry->title, 26);

        for (i = 0; i < 6; ++i) {
            if (entry->lines[i] != NULL && entry->lines[i][0] != '\0') {
                io_draw_wrapped_text((uint8_t)(2 + i), entry->lines[i], 26);
            }
        }

        io_draw_footer("UP/DN scroll MODE back");
        action = io_read_menu_key();

        if (action == IO_MENU_UP && index > 0) {
            --index;
        } else if (action == IO_MENU_DOWN && index + 1 < count) {
            ++index;
        } else if (action == IO_MENU_BACK) {
            return MENU_BACK;
        } else if (action == IO_MENU_EXIT) {
            return MENU_EXIT_APP;
        }
    }
}
