#include <string>

using namespace std;

string sha256_string(const string& input);

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp);
pair<long, string>  http_request(const string& url,
    const string& method = "GET",
    const string& user_agent = "libcurl-agent/1.0",
    const string& post_fields = "");

string detect_os();
string detect_arch();

bool package_exists(string packageName);
string get_package(string packageName);