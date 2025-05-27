#include <string>
#include <filesystem>
#include <curl/curl.h>


namespace fs = std::filesystem;
using namespace std;

string sha256_string(const string& input);

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);

pair<long, string> http_request(const string& url,
    const string& method = "GET",
    const string& user_agent = "libcurl-agent/1.0",
    const string& post_fields = "");

string detect_os();

string detect_arch();

bool package_exists(string packageName);

pair<long, string> get_package(string packageName);

void createOptDir();

void clear_folder(const fs::path& folder_path);

int spinner_callback(void* clientp,
    curl_off_t /*dltotal*/, curl_off_t /*dlnow*/,
    curl_off_t /*ultotal*/, curl_off_t /*ulnow*/);