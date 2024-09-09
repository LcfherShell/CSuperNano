#include <iostream>
#include <windows.h> // Untuk fungsi SetConsoleTitle dan GetConsoleTitle
#include <string>    // Untuk std::string


std::string getConsoleTitle() {
    // Menentukan buffer untuk menyimpan judul
    const int bufferSize = 256;
    char buffer[bufferSize];

    // Mendapatkan judul jendela CMD
    if (GetConsoleTitleA(buffer, bufferSize)) {
        return std::string(buffer);
    } else {
        // Jika gagal, kembalikan pesan kesalahan
        return "Gagal mendapatkan judul jendela CMD.";
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
