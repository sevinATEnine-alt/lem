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
    if (argc == 3) {
        if (!strcmp(argv[1],"install")) {
        
            string packageName(argv[2]);

            if (!package_exists(packageName)) {
                cout << "The package " << packageName << " does not exist." << endl;
                exit(4);
            };

            string packageToml = get_package(packageName);

            try {
                auto tbl = toml::parse(packageToml);

                auto name = tbl["package"]["name"].value_or("");
                auto version = tbl["package"]["version"].value_or("");

                if (name == "") {
                    cout << "Error: package.name not found." << endl;
                    exit(1);
                }
                if (version == "") {
                    cout << "Error: package.version not found." << endl;
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
                    cout << "No pre-built executable found. Compiling from source.";
                    
                    auto url = tbl["targets"]["source"]["url"].value<string>();
                    if (!url) {
                        cout << "Error: targets.source.url not found." << endl;
                        exit(5);
                    }
                    cout << url.value_or("") << endl;
                    auto [statusCode, data] = http_request(url.value_or(""));
                    cout << statusCode << " " << data << endl;
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
}