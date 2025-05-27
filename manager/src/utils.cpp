
#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <curl/curl.h>
#include <sys/utsname.h>
#include <stdexcept>
#include <utility> 

using namespace std;

string sha256_string(const string& input) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX ctx;

    SHA256_Init(&ctx);
    SHA256_Update(&ctx, input.c_str(), input.size());
    SHA256_Final(hash, &ctx);

    ostringstream result;
    for (int i = 0; i < SHA256_DIGEST_LENGTH; ++i)
        result << hex << setw(2) << setfill('0') << (int)hash[i];

    return result.str();
}

size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    string* str = (string*)userp;
    str->append((char*)contents, totalSize);
    return totalSize;
}

pair<long, string> http_request(const string& url,
    const string& method = "GET",
    const string& user_agent = "libcurl-agent/1.0",
    const string& post_fields = "") {
    CURL* curl = curl_easy_init();
    if (!curl) {
        throw runtime_error("Failed to initialize curl");
    }

    string response;
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    if (method == "POST") {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
    } else if (method != "GET") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
    if (!post_fields.empty()) {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
    }
    }

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        curl_easy_cleanup(curl);
        throw runtime_error(string("curl_easy_perform() failed: ") + curl_easy_strerror(res));
    }

    long status_code = 0;
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status_code);

    curl_easy_cleanup(curl);

    return {status_code, response};
}

string detect_arch() {
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        return "unknown";
    }

    string machine = buffer.machine;
    transform(machine.begin(), machine.end(), machine.begin(), ::tolower);

    // Normalize common names and aliases
    if (machine == "x86_64" || machine == "amd64")
        return "x86_64";
    if (machine == "i386" || machine == "i686")
        return "x86";
    if (machine == "arm64" || machine == "aarch64")
        return "arm64";
    if (machine.find("armv7") != string::npos)
        return "armv7";
    if (machine.find("armv6") != string::npos)
        return "armv6";
    if (machine.find("arm") != string::npos)
        return "arm";
    if (machine.find("ppc64le") != string::npos)
        return "ppc64le";
    if (machine.find("ppc64") != string::npos)
        return "ppc64";
    if (machine.find("ppc") != string::npos)
        return "ppc";
    if (machine.find("mips64") != string::npos)
        return "mips64";
    if (machine.find("mips") != string::npos)
        return "mips";
    if (machine.find("riscv64") != string::npos)
        return "riscv64";
    if (machine.find("riscv") != string::npos)
        return "riscv";

    return machine; // fallback for unknown strings
}

bool package_exists(string packageName) {
    stringstream url;
    url << "https://sevinatenine-alt.github.io/lem-packages/" << packageName[0] << "/" << packageName << ".toml";
    auto [status, response] = http_request(url.str());

    if (status >= 200 && status <= 300) {
        return true;
    }
    return false;
};

string get_package(string packageName) {
    stringstream url;
    url << "https://sevinatenine-alt.github.io/lem-packages/" << packageName[0] << "/" << packageName << ".toml";
    auto [status, response] = http_request(url.str());
    return response;
};

string detect_os() {
    struct utsname buffer;
    if (uname(&buffer) != 0) {
        return "unknown";
    }

    string sysname = buffer.sysname;
    transform(sysname.begin(), sysname.end(), sysname.begin(), ::tolower);

    if (sysname.find("linux") != string::npos)
        return "linux";
    if (sysname.find("darwin") != string::npos)
        return "darwin";
    if (sysname.find("bsd") != string::npos)
        return "bsd";
    if (sysname.find("windows") != string::npos || sysname.find("mingw") != string::npos)
        return "windows";
    if (sysname.find("sunos") != string::npos || sysname.find("solaris") != string::npos)
        return "solaris";
    
    return sysname; // fallback
}
