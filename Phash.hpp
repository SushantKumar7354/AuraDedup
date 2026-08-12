#pragma once

#include <string>
#include <optional>
#include <cstdint>

// Computes a 64-bit difference-hash (dHash) for the image at filepath.
// Returns std::nullopt if the file can't be loaded/decoded.
std::optional<uint64_t> compute_dhash(const std::string &filepath);