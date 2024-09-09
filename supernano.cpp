#include <curses.h>
#include <string>
#include <vector>
#include <filesystem>  // Untuk membaca isi folder
#include <iostream>    // Untuk debugging atau error message

namespace fs = std::filesystem;  // Namespace untuk filesystem

// Fungsi untuk mendapatkan daftar file dalam direktori
std::vector<std::string> get_files_in_directory(const std::string &directory) {
    std::vector<std::string> files;
    try {
        for (const auto &entry : fs::directory_iterator(directory)) {
            files.push_back(entry.path().filename().string());  // Ambil nama file
        }
    } catch (const fs::filesystem_error &e) {
        std::cerr << "Error accessing directory: " << e.what() << std::endl;
    }
    return files;
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

int main(int argc, char *argv[]) {
    // Memeriksa argumen dari command line
    std::string directory = ".";  // Default directory is current directory

    // Jika ada argumen kedua (path folder), periksa apakah valid
    if (argc > 1) {
        std::string arg_path = argv[1];
        if (fs::exists(arg_path) && fs::is_directory(arg_path)) {
            directory = arg_path;  // Set directory to user-specified path
        } else {
            std::cerr << "Invalid directory path: " << arg_path << ". Using current directory instead.\n";
        }
    }

    // Mengambil daftar file dari direktori yang dipilih
    std::vector<std::string> files = get_files_in_directory(directory);

    // Jika folder kosong
    if (files.empty()) {
        files.push_back("No files in directory");
    }

    // Inisialisasi layar curses
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    curs_set(0);  // Hide cursor

    // Menginisialisasi window layout
    int height, width;
    getmaxyx(stdscr, height, width);
    WINDOW *menu_win = newwin(height - 3, width / 4, 0, 0);  // File menu window
    WINDOW *editor_win = newwin(height - 3, 3 * (width / 4), 0, width / 4);  // Text editor window
    WINDOW *footer_win = newwin(3, width, height - 3, 0);  // Footer window

    int highlight = 1;
    int choice;
    int file_count = files.size();

    // Tampilkan elemen-elemen UI
    display_file_menu(menu_win, files, highlight);

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
            case 'q':
                goto end_loop;  // Quit the loop
        }
        display_file_menu(menu_win, files, highlight);
    }
  end_loop:
    // Bersihkan dan tutup
    delwin(menu_win);
    delwin(editor_win);
    delwin(footer_win);
    endwin();

    return 0;
}
