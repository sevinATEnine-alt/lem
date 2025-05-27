#include <archive.h>
#include <archive_entry.h>
#include <iostream>
#include <string>
#include <filesystem>

int extract_tar_gz_from_string(const std::string& tarGzData, const std::string& outputDir) {
    namespace fs = std::filesystem;

    // Create the output directory if it doesn't exist
    if (!fs::exists(outputDir)) {
        fs::create_directories(outputDir);
    }

    struct archive* a = archive_read_new();
    archive_read_support_format_tar(a);
    archive_read_support_filter_gzip(a);

    if (archive_read_open_memory(a, tarGzData.data(), tarGzData.size()) != ARCHIVE_OK) {
        std::cerr << "Failed to open archive from memory: " << archive_error_string(a) << "\n";
        return 1;
    }

    struct archive* ext = archive_write_disk_new();
    archive_write_disk_set_options(ext,
        ARCHIVE_EXTRACT_TIME | ARCHIVE_EXTRACT_PERM | ARCHIVE_EXTRACT_ACL | ARCHIVE_EXTRACT_FFLAGS);

    struct archive_entry* entry;

    while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
        std::string fullPath = outputDir + "/" + archive_entry_pathname(entry);
        archive_entry_set_pathname(entry, fullPath.c_str());

        if (archive_write_header(ext, entry) != ARCHIVE_OK) {
            std::cerr << "Failed to write header: " << archive_error_string(ext) << "\n";
        } else {
            const void* buff;
            size_t size;
            la_int64_t offset;
            while (archive_read_data_block(a, &buff, &size, &offset) == ARCHIVE_OK) {
                archive_write_data_block(ext, buff, size, offset);
            }
        }
    }

    archive_read_close(a);
    archive_read_free(a);
    archive_write_close(ext);
    archive_write_free(ext);

    return 0;
}
