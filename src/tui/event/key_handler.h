#include "../tui.h"
#include <termios.h>

namespace tui {
    class key_handler {
    public:
        key_handler();
        ~key_handler();

        void handle();
    private:
        struct termios orig_termios;
    };

    bool is_key_pressed(const keys key);
    bool is_any_pressed();

}