#pragma once

#include <string>
#include <vector>

using namespace std;

// Recursively scans the folder and returns matching file paths

vector<string> scan_directory(
    const string &folder,
    const vector<string> &extensions);