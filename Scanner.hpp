#pragma once

#include <string>
#include <vector>

// Recursively scans the folder and returns matching file paths
std::vector<std::string> scan_directory(
    const std::string &folder,
    const std::vector<std::string> &extensions);