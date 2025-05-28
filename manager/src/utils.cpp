
#include <openssl/sha.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <curl/curl.h>
#include <sys/utsname.h>
#include <stdexcept>
#include <utility> 
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <string.h>
#include <filesystem>
#include <algorithm>
#include <cctype>


namespace fs = std::filesystem;


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

struct SpinnerData {
    int pos = 0;
    chrono::steady_clock::time_point last_update = chrono::steady_clock::now();
};

int spinner_callback(void* clientp,
                     curl_off_t /*dltotal*/, curl_off_t /*dlnow*/,
                     curl_off_t /*ultotal*/, curl_off_t /*ulnow*/) {
    SpinnerData* data = static_cast<SpinnerData*>(clientp);
    auto now = chrono::steady_clock::now();

    if (chrono::duration_cast<chrono::milliseconds>(now - data->last_update).count() < 100) {
        return 0; // update every 100ms
    }

    static const char spinner_chars[] = "|/-\\";
    cout << "\r" << spinner_chars[data->pos % 4] << " Downloading... " << flush;
    data->pos++;
    data->last_update = now;

    return 0;
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
    SpinnerData spinner_data;

    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_USERAGENT, user_agent.c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response);
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Enable progress callback
    curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0L);
    curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION, spinner_callback);
    curl_easy_setopt(curl, CURLOPT_XFERINFODATA, &spinner_data);

    if (method == "POST") {
        curl_easy_setopt(curl, CURLOPT_POST, 1L);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
    }
    else if (method != "GET") {
        curl_easy_setopt(curl, CURLOPT_CUSTOMREQUEST, method.c_str());
        if (!post_fields.empty()) {
            curl_easy_setopt(curl, CURLOPT_POSTFIELDS, post_fields.c_str());
        }
    }

    CURLcode res = curl_easy_perform(curl);
    cout << "\rDownloaded.     \n"; // Clear spinner line and print done

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

pair<long, string> get_package(string packageName) {
    stringstream url;
    url << "https://sevinatenine-alt.github.io/lem-packages/" << packageName[0] << "/" << packageName << ".toml";
    return http_request(url.str());
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
};

void createOptDir() {
    const char* dir = "/opt/lem";

    // Check if directory exists
    struct stat st;
    if (stat(dir, &st) == 0 && S_ISDIR(st.st_mode)) {
        return;
    }

    // Directory does not exist — check if root
    if (geteuid() != 0) {
        cerr << "LEM needs permissons to create a directory." << endl << "Please run this program as root." << endl << "You will not need to again." << endl;
        exit(6);
    }

    // Create directory with 0755 permissions
    if (mkdir(dir, 0755) != 0) {
        cerr << "Failed to create directory " << dir << ": " << strerror(errno) << endl;
        exit(7);
    }

    // Get the real user's UID and GID (the user who ran the program)
    const char* sudo_uid_str = std::getenv("SUDO_UID");
    const char* sudo_gid_str = std::getenv("SUDO_GID");

    uid_t user_uid = (sudo_uid_str != nullptr) ? static_cast<uid_t>(std::stoi(sudo_uid_str)) : getuid();
    gid_t user_gid = (sudo_gid_str != nullptr) ? static_cast<gid_t>(std::stoi(sudo_gid_str)) : getgid();

    // Change ownership of directory to the user
    if (chown(dir, user_uid, user_gid) != 0) {
        cerr << "Failed to change ownership of " << dir << ": " << strerror(errno) << endl;
        exit(8);
    }

    if (chmod(dir, 0775) != 0) {
        cerr << "Failed to change permissions: " << strerror(errno) << "\n";
        exit(9);
    }

    cout << "Directory created." << endl << endl;
}

void clear_folder(const fs::path& folder_path) {
    for (const auto& entry : fs::directory_iterator(folder_path)) {
        error_code ec;  // to avoid exceptions
        fs::remove_all(entry.path(), ec);
        if (ec) {
            cerr << "Failed to remove " << entry.path() << ": " << ec.message() << "\n";
        }
    }
}