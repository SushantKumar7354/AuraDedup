#pragma once

#include <string>
#include <vector>

using namespace std;

// Recursively scans the folder and returns matching file paths

vector<string> scan_directory(
    const string &folder,
    const vector<string> &extensions);

// Image extensions AuraDedup understands -- matches what phash.cpp's
// stb_image decoder can actually load. Shared so dev_check.cpp and the
// pybind11-facing pipeline don't maintain two separate copies of this list.
extern const vector<string> DEFAULT_IMAGE_EXTENSIONS;