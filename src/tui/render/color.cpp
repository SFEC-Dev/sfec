#include "color.h"

const std::wstring start_seq{L"\033["};

std::wstring tui::render::color::get_style(Color text_col, Color bg_col, text_flags flags) {
    std::wstring result;
    
    if (flags != FLAG_NONE)
        result = start_seq;
    
    if (flags & FLAG_BOLD) {
        result.append(L"1;");
    }
    if (flags & FLAG_ITALIC) {
        result.append(L"3;");
    }
    if (flags & FLAG_DIM) {
        result.append(L"2;");
    }
    if (flags & FLAG_UNDERLINED) {
        result.append(L"4;");
    }
    if (flags & FLAG_BLINK) {
        result.append(L"5;");
    }
    if (flags & FLAG_REVERSE) {
        result.append(L"7;");
    }
    if (flags & FLAG_HIDDEN) {
        result.append(L"8;");
    }

    if (flags != FLAG_NONE) {
        result.pop_back();
        result.push_back(L'm');
    }

    if (!text_col.is_empty) {
        result.append(L"\033[38;2;" + std::to_wstring(text_col.r) + L";" + 
                                        std::to_wstring(text_col.g) + L";" + 
                                        std::to_wstring(text_col.b) + L"m");
    }
    if (!bg_col.is_empty) {
        result.append(L"\033[48;2;" + std::to_wstring(bg_col.r) + L";" + 
                                        std::to_wstring(bg_col.g) + L";" + 
                                        std::to_wstring(bg_col.b) + L"m");
    }

    return result;
}

void tui::render::write_styled(TerminalMatrix& matrix, vec2d where, char letter, 
                    Color text_col, Color bg_col, text_flags flags) {
                        
    write(matrix, where, letter);

    matrix[where].second = color::get_style(text_col, bg_col, flags);
}
                    
void tui::render::write_styled(TerminalMatrix& matrix, vec2d start, std::wstring text, 
                    Color text_col, Color bg_col, text_flags flags) {

    write(matrix, start, text);                   


    for (std::size_t col = 0; col < text.size(); col++) { 
        matrix[vec2d(start.x + col, start.y)].second = color::get_style(text_col, bg_col, flags);
    }
}