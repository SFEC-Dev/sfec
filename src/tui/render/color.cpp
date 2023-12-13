#include "color.h"

#include <locale>
#include <codecvt>

const std::string start_seq{"\033["};

std::string tui::render::color::get_style(Color text_col, Color bg_col, text_flags flags) {
    std::string result;
    
    if (flags != FLAG_NONE)
        result = start_seq;
    
    if (flags & FLAG_BOLD) {
        result.append("1;");
    }
    if (flags & FLAG_ITALIC) {
        result.append("3;");
    }
    if (flags & FLAG_DIM) {
        result.append("2;");
    }
    if (flags & FLAG_UNDERLINED) {
        result.append("4;");
    }
    if (flags & FLAG_BLINK) {
        result.append("5;");
    }
    if (flags & FLAG_REVERSE) {
        result.append("7;");
    }
    if (flags & FLAG_HIDDEN) {
        result.append("8;");
    }

    if (flags != FLAG_NONE) {
        result.pop_back();
        result.push_back('m');
    }

    if (!text_col.is_empty) {
        result.append("\033[38;2;" + std::to_string(text_col.r) + ";" + 
                                        std::to_string(text_col.g) + ";" + 
                                        std::to_string(text_col.b) + "m");
    }
    if (!bg_col.is_empty) {
        result.append("\033[48;2;" + std::to_string(bg_col.r) + ";" + 
                                        std::to_string(bg_col.g) + ";" + 
                                        std::to_string(bg_col.b) + "m");
    }

    return result;
}

void tui::render::write_styled(TerminalMatrix& matrix, vec2d where, char letter, 
                    Color text_col, Color bg_col, text_flags flags) {
                        
    write(matrix, where, letter);

    matrix[where].second = color::get_style(text_col, bg_col, flags);
}
                    
void tui::render::write_styled(TerminalMatrix& matrix, vec2d start, std::string text, 
                    Color text_col, Color bg_col, text_flags flags) {

    write(matrix, start, text);                   


    for (std::size_t col = 0; col < text.size(); col++) { 
        matrix[vec2d(start.x + col, start.y)].second = color::get_style(text_col, bg_col, flags);
    }
}

void tui::render::write_styled(TerminalMatrix& matrix, vec2d where, uchar letter, 
                    Color text_col, Color bg_col, text_flags flags) {
                        
    write(matrix, where, letter);

    matrix[where].second = color::get_style(text_col, bg_col, flags);
}
                    
void tui::render::write_styled(TerminalMatrix& matrix, vec2d start, ustring text, 
                    Color text_col, Color bg_col, text_flags flags) {

    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    std::wstring wstr = converter.from_bytes(text());

    int shift = 0;
    for (auto it = wstr.begin(); it != wstr.end(); ++it) {
        std::wstring singleChar(1, *it);
        std::string utf8Char = converter.to_bytes(singleChar);
        if (utf8Char.size() >= 4) {
            write(matrix, vec2d(start.x + std::distance(wstr.begin(), it) + shift, start.y), uchar{utf8Char});
            matrix[vec2d(start.x + std::distance(wstr.begin(), it) + shift + 1, start.y)].first = "";
            matrix[vec2d(start.x + std::distance(wstr.begin(), it) + shift, start.y)].second = color::get_style(text_col, bg_col, flags);
            matrix[vec2d(start.x + std::distance(wstr.begin(), it) + shift + 1, start.y)].second = color::get_style(text_col, bg_col, flags);
            //std::cout << matrix[{100, start.y}].first << std::endl;
            //std::cout << start.x + std::distance(wstr.begin(), it) + shift << " " << start.y <<  " " << (matrix.begin() + start.y)->size() << std::endl;
            shift++;
        } else {
            write(matrix, vec2d(start.x + std::distance(wstr.begin(), it) + shift, start.y), uchar{utf8Char});
            matrix[vec2d(start.x + std::distance(wstr.begin(), it) + shift, start.y)].second = color::get_style(text_col, bg_col, flags);
        }
    }
}