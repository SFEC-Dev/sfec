#pragma once

#include <string>

#include "helpers/style.h"

namespace tui {
    enum keys {
        KEY_NUL = 0, KEY_SOH, KEY_STX, KEY_ETX, KEY_EOT, KEY_ENQ, KEY_ACK, KEY_BEL, KEY_BS, KEY_HT, KEY_LF, KEY_VT, KEY_FF, KEY_CR, KEY_SO, KEY_SI,
        KEY_DLE, KEY_DC1, KEY_DC2, KEY_DC3, KEY_DC4, KEY_NAK, KEY_SYN, KEY_ETB, KEY_CAN, KEY_EM, KEY_SUB, KEY_ESC, KEY_FS, KEY_GS, KEY_RS, KEY_US,
        KEY_SPACE, KEY_EXCLAMATION_MARK, KEY_DOUBLE_QUOTES, KEY_NUMBER_SIGN, KEY_DOLLAR_SIGN, KEY_PERCENT, KEY_AMPERSAND, KEY_SINGLE_QUOTE,
        KEY_LEFT_PARENTHESIS, KEY_RIGHT_PARENTHESIS, KEY_ASTERISK, KEY_PLUS, KEY_COMMA, KEY_MINUS, KEY_PERIOD, KEY_FORWARD_SLASH,
        KEY_ZERO, KEY_ONE, KEY_TWO, KEY_THREE, KEY_FOUR, KEY_FIVE, KEY_SIX, KEY_SEVEN, KEY_EIGHT, KEY_NINE,
        KEY_COLON, KEY_SEMICOLON, KEY_LESS_THAN, KEY_EQUALS, KEY_GREATER_THAN, KEY_QUESTION_MARK, KEY_AT,
        KEY_UPPERCASE_A, KEY_UPPERCASE_B, KEY_UPPERCASE_C, KEY_UPPERCASE_D, KEY_UPPERCASE_E, KEY_UPPERCASE_F, KEY_UPPERCASE_G, KEY_UPPERCASE_H,
        KEY_UPPERCASE_I, KEY_UPPERCASE_J, KEY_UPPERCASE_K, KEY_UPPERCASE_L, KEY_UPPERCASE_M, KEY_UPPERCASE_N, KEY_UPPERCASE_O, KEY_UPPERCASE_P,
        KEY_UPPERCASE_Q, KEY_UPPERCASE_R, KEY_UPPERCASE_S, KEY_UPPERCASE_T, KEY_UPPERCASE_U, KEY_UPPERCASE_V, KEY_UPPERCASE_W, KEY_UPPERCASE_X,
        KEY_UPPERCASE_Y, KEY_UPPERCASE_Z,
        KEY_LEFT_BRACKET, KEY_BACKSLASH, KEY_RIGHT_BRACKET, KEY_CARET, KEY_UNDERSCORE, KEY_GRAVE_ACCENT,
        KEY_LOWERCASE_A, KEY_LOWERCASE_B, KEY_LOWERCASE_C, KEY_LOWERCASE_D, KEY_LOWERCASE_E, KEY_LOWERCASE_F, KEY_LOWERCASE_G, KEY_LOWERCASE_H,
        KEY_LOWERCASE_I, KEY_LOWERCASE_J, KEY_LOWERCASE_K, KEY_LOWERCASE_L, KEY_LOWERCASE_M, KEY_LOWERCASE_N, KEY_LOWERCASE_O, KEY_LOWERCASE_P,
        KEY_LOWERCASE_Q, KEY_LOWERCASE_R, KEY_LOWERCASE_S, KEY_LOWERCASE_T, KEY_LOWERCASE_U, KEY_LOWERCASE_V, KEY_LOWERCASE_W, KEY_LOWERCASE_X,
        KEY_LOWERCASE_Y, KEY_LOWERCASE_Z,
        KEY_LEFT_CURLY_BRACE, KEY_VERTICAL_BAR, KEY_RIGHT_CURLY_BRACE, KEY_TILDE, KEY_DELETE = 127
    };

    struct io {
        keys current_key;
    };

    io& get_io();

    class Window {

    };
}

