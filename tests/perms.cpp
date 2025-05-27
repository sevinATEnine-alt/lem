#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <pwd.h>
#include <grp.h>
#include <errno.h>
#include <string.h>

using namespace std;

int main() {
    const char* dir = "/opt/lem";

    // Check if directory exists
    struct stat st;
    if (stat(dir, &st) == 0 && S_ISDIR(st.st_mode)) {
        cout << "Directory already exists. Nothing to do." << endl ;
        return 0;
    }

    // Directory does not exist — check if root
    if (geteuid() != 0) {
        cerr << "LEM needs permissons to create a directory." << endl << "Please run this program as root." << endl << "You will not need to again." << endl;
        return 1;
    }

    // Create directory with 0755 permissions
    if (mkdir(dir, 0755) != 0) {
        cerr << "Failed to create directory " << dir << ": " << strerror(errno) << endl;
        return 1;
    }

    // Get the real user's UID and GID (the user who ran the program)
    uid_t user_uid = getuid();
    gid_t user_gid = getgid();

    // Change ownership of directory to the user
    if (chown(dir, user_uid, user_gid) != 0) {
        cerr << "Failed to change ownership of " << dir << ": " << strerror(errno) << endl;
        return 1;
    }

    cout << "Directory created." << endl;
    return 0;
}