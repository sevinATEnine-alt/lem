#include <toml++/toml.hpp>
#include <openssl/sha.h>
#include <iostream>
#include <cstdlib>
#include <string>
#include <sstream>
#include <iomanip>
#include <sys/utsname.h>
#include "utils.hpp"
#include "unarchive.hpp"
#include <stdexcept>
#include <utility> 
#include <filesystem>

namespace fs = std::filesystem;

using namespace std;

void usage() {
    cout << "Usage:" << endl <<
    "   lem install <package name>" << endl <<
    "   lem uninstall <package name>" << endl <<
    "   lem list [installed | all]" << endl <<
    "   lem update <package name>" << endl <<
    "   lem find <package name>" << endl;
    return;
}

int main(int argc, char *argv[]) {
    createOptDir();
    if (argc == 3) {
        if (!strcmp(argv[1],"install")) {
        
            string packageName(argv[2]);
            
            cout << "Finding package." << endl;

            auto [statusCodeRecv, packageToml] = get_package(packageName);

            if (statusCodeRecv < 200 && statusCodeRecv >= 300) {
                cerr << "The package " << packageName << " does not exist." << endl;
                exit(4);
            };


            try {
                auto tbl = toml::parse(packageToml);

                auto name = tbl["package"]["name"].value_or("");
                auto version = tbl["package"]["version"].value_or("");

                if (name == "") {
                    cerr << "Error: package.name not found." << endl;
                    exit(1);
                }
                if (version == "") {
                    cerr << "Error: package.version not found." << endl;
                    exit(2);
                }

                cout << "Installing " << name << "@" << version << endl;

                // cout << sha256_string("test") << endl;

                // cout << http_request("https://httpbin.org/get") << endl;

                string os = detect_os();
                string arch = detect_arch();

                std::cout << "Running " << os << " on " << arch << endl;

                auto preBuiltExecutable = tbl["targets"][os][arch].value<string>();
                if (preBuiltExecutable) {
                    cout << "Pre-built executable found. Installing.";

                    // TODO: IMPLIMENT PRE-BUILT EXECUTABLE

                } else {
                    cout << "No pre-built executable found. Compiling from source." << endl;
                    
                    auto url = tbl["targets"]["source"]["url"].value<string>();

                    auto expectedHash = tbl["targets"]["source"]["sha256"].value<string>();

                    if (!url) {
                        cerr << "Error: targets.source.url not found." << endl;
                        exit(5);
                    }

                    if (!expectedHash) {
                        cerr << "Error: targets.source.sha256 not found." << endl;
                        exit(10);
                    }
                    cout << "Downloading package" << endl;

                    auto [statusCode, data] = http_request(url.value_or(""));
                    if (!fs::exists("/opt/lem/temp")) {
                        fs::create_directories("/opt/lem/temp");
                    }

                    cout << "Finished downloading. Calculating hashes." << endl;

                    if (expectedHash == sha256_string(data)) {
                        cout << "Hashes match. Installing." << endl;
                    } else {
                        cerr << "Hashes do not match!" << endl;
                        cerr << "Expected hash: '" << expectedHash.value_or("") << "' Real hash: '" << sha256_string(data) << "'.";
                        exit(11);
                    }

                    cout << "Unarchiving package" << endl;

                    clear_folder("/opt/lem/temp");
                    
                    extract_tar_gz_from_string(data, "/opt/lem/temp");

                    cout << "Building package" << endl;
                }
            } catch (const toml::parse_error& err) {
                cerr << "TOML parse error: " << err.description()
                        << " (at " << err.source().begin << ")\n";
                exit(1);
            }

        } else {
            usage();
            exit(3);
        }
    } else {
        usage();
        exit(3);
    }
    return 0;
}