#pragma once

#include <string>
#include <optional>
#include <cstdint>

using namespace std;

// Computes a 64-bit difference-hash (dHash) for the image at filepath.
// Returns std::nullopt if the file can't be loaded/decoded.
optional<uint64_t> compute_dhash(const string &filepath);