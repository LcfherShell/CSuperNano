#include <curses.h>
#include <string>
#include <vector>
#include <filesystem>
#include <fstream>
#include <iostream>

namespace fs = std::filesystem;

// Fungsi untuk mendapatkan daftar file dalam direktori
std::vector<std::string> get_files_in_directory(const std::string &directory) {
    std::vector<std::string> files;
    try {
        for (const auto &entry : fs::directory_iterator(directory)) {
            files.push_back(entry.path().filename().string());
        }
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    return files;
}

// Fungsi untuk membaca isi file dan mengisi vector editor_content
bool read_file_content(const std::string &file_path, std::vector<std::string> &content) {
    std::ifstream file(file_path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << file_path << std::endl;
        return false;
    }

    content.clear();
    std::string line;
    while (std::getline(file, line)) {
        content.push_back(line);
    }
    file.close();
    return true;
}

// Fungsi untuk menampilkan file menu di sisi kiri
void display_file_menu(WINDOW *menu_win, std::vector<std::string> &files, int highlight) {
    box(menu_win, 0, 0); // Buat border di sekitar window
    int x = 1, y = 1;
    for (int i = 0; i < files.size(); ++i) {
        if (highlight == i + 1) {
            wattron(menu_win, A_REVERSE); // Highlight file yang dipilih
            mvwprintw(menu_win, y, x, "%s", files[i].c_str());
            wattroff(menu_win, A_REVERSE);
        } else {
            mvwprintw(menu_win, y, x, "%s", files[i].c_str());
        }
        y++;
    }
    wrefresh(menu_win);
}

// Fungsi untuk menampilkan teks di jendela editor dengan nomor baris
void display_editor(WINDOW *editor_win, std::vector<std::string> &lines) {
    wclear(editor_win); // Membersihkan window sebelum menampilkan
    box(editor_win, 0, 0); // Border di sekitar window

    int y = 1; // Baris pertama untuk teks (dimulai dari 1)
    for (int i = 0; i < lines.size(); ++i) {
        // Cetak nomor baris di bagian kiri
        mvwprintw(editor_win, y, 1, "%d", i + 1); // Nomor baris (dengan padding di kolom 1)

        // Cetak teks pada kolom setelah nomor baris
        mvwprintw(editor_win, y, 6, "%s", lines[i].c_str()); // Teks dengan offset kolom 6

        y++;
    }
    wrefresh(editor_win); // Refresh window untuk menampilkan output
}

int main(int argc, char *argv[]) {
    std::string directory = ".";  // Default directory

    if (argc > 1) {
        std::string arg_path = argv[1];
        if (fs::exists(arg_path) && fs::is_directory(arg_path)) {
            directory = arg_path;
        } else {
            std::cerr << "Invalid directory path: " << arg_path << ". Using current directory instead.\n";
        }
    }

    std::vector<std::string> files = get_files_in_directory(directory);
    if (files.empty()) {
        files.push_back("No files in directory");
    }

    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);

    int height, width;
    getmaxyx(stdscr, height, width);
    WINDOW *menu_win = newwin(height - 3, width / 4, 0, 0);
    WINDOW *editor_win = newwin(height - 3, 3 * (width / 4), 0, width / 4);
    WINDOW *footer_win = newwin(3, width, height - 3, 0);

    int highlight = 1;
    int choice;
    int file_count = files.size();

    // Inisialisasi konten editor sebagai vector kosong
    std::vector<std::string> editor_content;

    display_file_menu(menu_win, files, highlight);
    display_editor(editor_win, editor_content);

    while (true) {
        choice = wgetch(menu_win);
        switch (choice) {
            case KEY_UP:
                if (highlight == 1)
                    highlight = file_count;
                else
                    --highlight;
                break;
            case KEY_DOWN:
                if (highlight == file_count)
                    highlight = 1;
                else
                    ++highlight;
                break;
            case 10: // Enter key
                {
                    std::string selected_file = files[highlight - 1];
                    std::string file_path = directory + "/" + selected_file;

                    if (fs::exists(file_path) && fs::is_regular_file(file_path)) {
                        if (read_file_content(file_path, editor_content)) {
                            display_editor(editor_win, editor_content);
                        }
                    } else {
                        std::cerr << "Selected path is not a file: " << file_path << std::endl;
                    }
                }
                break;
            case 'q':
                goto end_loop;
        }
        display_file_menu(menu_win, files, highlight);
        display_editor(editor_win, editor_content);
    }

end_loop:
    delwin(menu_win);
    delwin(editor_win);
    delwin(footer_win);
    endwin();

    return 0;
}
