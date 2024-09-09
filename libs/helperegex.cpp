#include <iostream>
#include <string>
#include <regex>
#include <tuple>
#include <vector>
#include <unordered_set>
#include <algorithm>
// Menghapus duplikat dari sisi kanan
std::vector<std::tuple<std::string, int, int>> remove_duplicates_from_right(const std::vector<std::tuple<std::string, int, int>>& lst) {
    std::unordered_set<std::tuple<std::string, int, int>> seen;
    std::vector<std::tuple<std::string, int, int>> result;

    // Iterasi dari akhir ke awal
    for (auto it = lst.rbegin(); it != lst.rend(); ++it) {
        if (seen.find(*it) == seen.end()) {
            seen.insert(*it);
            result.push_back(*it);
        }
    }

    // Membalikkan hasil untuk mempertahankan urutan awal
    std::reverse(result.begin(), result.end());

    // Buat list akhir dengan mempertahankan posisi index
    std::vector<std::tuple<std::string, int, int>> final_result;
    seen.clear();

    for (const auto& item : lst) {
        if (std::find(result.begin(), result.end(), item) != result.end() && seen.find(item) == seen.end()) {
            final_result.push_back(item);
            seen.insert(item);
        }
    }

    return final_result;
}

// Menghapus duplikat dari sisi kiri dengan batas tertentu
std::vector<std::tuple<std::string, int, int>> lremove_duplicates_from_left(const std::vector<std::tuple<std::string, int, int>>& lst, int limit = 1, int total = 0) {
    int removal_count = 0;
    std::vector<std::tuple<std::string, int, int>> result;

    if (total == 0) {
        total = lst.size() / 2;
        if (total % 2 == 0) {
            total /= 2;
        } else if (total % 3) {
            total = (total + 1) / 2;
        }
    }

    if (lst.size() > limit + total) {
        for (const auto& item : lst) {
            if (removal_count < limit) {
                ++removal_count;
            } else {
                result.push_back(item);
            }
        }
    } else {
        result = lst;
    }

    return result;
}

std::string clean_string(const std::string& str, const std::string& regex_pattern, const std::string& newstring) {
    std::regex pattern(regex_pattern);
    std::string result = std::regex_replace(str, pattern, newstring);
    // Hapus spasi putih di awal dan akhir string
    result.erase(result.find_last_not_of(" \t\n\r\f\v") + 1);
    result.erase(0, result.find_first_not_of(" \t\n\r\f\v"));
    return result;
}


std::string remove_laststring(const std::string& data, const std::string& spec, const std::string& new_string = "") {
    std::string result = data;
    size_t pos = result.rfind(spec);
    if (pos != std::string::npos) {
        result.replace(pos, spec.length(), new_string);
    }
    return result;
}


std::string rreplace(const std::string& s, const std::string& old_str, const std::string& new_str, int count) {
    std::string reversed_s(s.rbegin(), s.rend());
    std::string reversed_old(old_str.rbegin(), old_str.rend());
    std::string reversed_new(new_str.rbegin(), new_str.rend());

    size_t pos = 0;
    int replaced_count = 0;
    while ((pos = reversed_s.find(reversed_old, pos)) != std::string::npos && replaced_count < count) {
        reversed_s.replace(pos, reversed_old.length(), reversed_new);
        pos += reversed_new.length();
        replaced_count++;
    }

    return std::string(reversed_s.rbegin(), reversed_s.rend());
}


std::string replaceusinglength(const std::string& data, int length) {
    if (length >= data.length()) {
        return data; // Jika panjang lebih besar dari atau sama dengan data, kembalikan data asli
    }
    return data.substr(0, data.length() - length);
}

std::string replace_char_at_position(const std::string& s, size_t position, char new_char) {
    if (position >= s.length()) {
        return s; // Posisi di luar batas string
    }

    std::string result = s;
    result[position] = new_char;
    return result;
}

std::string replacebypost(const std::string& data, const std::string& new_character, const std::string& regex_pattern = "\\((.*)\\)") {
    std::regex pattern(regex_pattern);
    std::string result = data;
    auto words_begin = std::sregex_iterator(data.begin(), data.end(), pattern);
    auto words_end = std::sregex_iterator();

    for (std::sregex_iterator i = words_begin; i != words_end; ++i) {
        std::smatch match = *i;
        size_t start_pos = match.position();
        size_t end_pos = start_pos + match.length();
        result.replace(start_pos, end_pos - start_pos, new_character);
    }

    return result;
}

bool fullmatch(const std::string& pattern, const std::string& data) {
    std::regex regex(pattern);
    std::smatch match;
    return std::regex_match(data, match, regex);
}

std::string searching(const std::string& patternRegex, const std::string& data) {
    std::regex pattern(patternRegex);
    std::smatch match;

    // Cari pola dalam string
    if (std::regex_search(data, match, pattern)) {
        // Jika cocok, kembalikan hasil pencocokan pertama
        return match[1].str();
    } else {
        return ""; // Kembalikan string kosong jika tidak ada pencocokan
    }
}

std::vector<int> find_regex_in_list(const std::string& pattern, const std::vector<std::string>& lst, int limit = -1) {
    std::regex regex(pattern, std::regex::icase); // Compile the regex pattern with case insensitive flag
    std::vector<int> positions;

    // Iterate through the list and search for the pattern
    for (size_t i = 0; i < lst.size(); ++i) {
        if (limit == 0) {
            break;
        }
        if (std::regex_search(lst[i], regex)) {
            positions.push_back(static_cast<int>(i));
        }
        if (limit > 0) {
            --limit;
        }
    }
    return positions;
}

std::tuple<std::string, std::string> find_and_split(const std::string& text, const std::string& pattern) {
    std::regex regex(pattern);
    std::smatch match;

    if (std::regex_search(text, match, regex)) {
        // Menemukan posisi akhir dari pola yang cocok
        size_t split_pos = match.position() + match.length();
        // Memisahkan string satu kali berdasarkan posisi akhir dari pola yang cocok
        std::string part1 = text.substr(0, split_pos);
        std::string part2 = text.substr(split_pos);
        return std::make_tuple(part1, part2);
    } else {
        return std::make_tuple(text, "");
    }
}

std::vector<int> reverse_vector(const std::vector<int>& datalist) {
    std::vector<int> process = datalist; // Buat salinan dari datalist
    std::reverse(process.begin(), process.end()); // Balikkan elemen
    return process;
}

std::vector<std::string> split_from_right_with_regex(const std::string& text, const std::string& pattern, int maxsplit = -1) {
    std::vector<std::string> parts;
    std::regex regex(pattern);

    // Membalik string untuk memproses dari kanan ke kiri
    std::string reversed_text(text.rbegin(), text.rend());

    // Membagi string yang dibalik
    std::sregex_token_iterator iter(reversed_text.begin(), reversed_text.end(), regex, -1);
    std::sregex_token_iterator end;
    
    // Menyimpan hasil split dalam vector
    std::vector<std::string> reversed_parts(iter, end);

    // Jika maxsplit diatur, batasi jumlah split dari kanan
    if (maxsplit > 0 && reversed_parts.size() > static_cast<size_t>(maxsplit + 1)) {
        reversed_parts.resize(maxsplit + 1);
    }

    // Membalik kembali hasil split untuk urutan asli
    parts.assign(reversed_parts.rbegin(), reversed_parts.rend());

    return parts;
}
