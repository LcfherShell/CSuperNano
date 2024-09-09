#include <iostream>
#include <string>
#include <curl/curl.h>
#include <stdexcept>
#include <vector>

class Fetch {
public:
    Fetch(const std::vector<std::string>& headers = {}) {
        curl_global_init(CURL_GLOBAL_DEFAULT);
        curl = curl_easy_init();
        if (!curl) {
            throw std::runtime_error("Failed to initialize CURL.");
        }

        if (!headers.empty()) {
            struct curl_slist* header_list = nullptr;
            for (const auto& header : headers) {
                header_list = curl_slist_append(header_list, header.c_str());
            }
            curl_easy_setopt(curl, CURLOPT_HTTPHEADER, header_list);
        }
    }

    ~Fetch() {
        curl_easy_cleanup(curl);
        curl_global_cleanup();
    }

    std::string get(const std::string& url, int max_retries = 5, long timeout = 5) {
        CURLcode res;
        long retries = 0;
        std::string response_data;

        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_data);
        curl_easy_setopt(curl, CURLOPT_TIMEOUT, timeout);

        while (retries < max_retries) {
            res = curl_easy_perform(curl);

            if (res == CURLE_OK) {
                return response_data;
            } else if (res == CURLE_OPERATION_TIMEOUTED) {
                retries++;
                std::cerr << "Timeout, retrying " << retries << "/" << max_retries << "...\n";
            } else {
                throw std::runtime_error(curl_easy_strerror(res));
            }
        }
        throw std::runtime_error("Failed to connect after retries.");
    }

private:
    CURL* curl;

    static size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
        size_t total_size = size * nmemb;
        std::string* response = (std::string*)userp;
        response->append((char*)contents, total_size);
        return total_size;
    }
};
