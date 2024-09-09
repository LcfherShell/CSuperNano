#include <iostream>
#include <windows.h> // Untuk fungsi SetConsoleTitle dan GetConsoleTitle

void getConsoleTitle() {
    // Menentukan buffer untuk menyimpan judul
    const int bufferSize = 256;
    char buffer[bufferSize];

    // Mendapatkan judul jendela CMD
    if (GetConsoleTitleA(buffer, bufferSize)) {
        std::cout << "Judul jendela CMD saat ini: " << buffer << std::endl;
    } else {
        std::cerr << "Gagal mendapatkan judul jendela CMD." << std::endl;
    }
}

void setConsoleTitle(const std::string& title) {
    // Mengatur judul jendela CMD
    if (SetConsoleTitleA(title.c_str())) {
        std::cout << "Judul jendela CMD berhasil diperbarui menjadi: " << title << std::endl;
    } else {
        std::cerr << "Gagal mengatur judul jendela CMD." << std::endl;
    }
}
