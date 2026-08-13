#pragma once
#include <cstdint>
#include <string>
#include <optional>

using namespace std;

// Computes a 64-bit difference-hash (dHash) for the image at filepath.
// Visually similar images produce hashes that differ in only a few bits,
// even across different formats, resolutions, or light recompression --
// that's what makes this useful for near-duplicate detection later, not
// just exact-file matching.
//
// Returns nullopt if the file can't be loaded/decoded. This can't just be
// "return 0 on failure" -- 0 is also a real, reachable hash value for any
// image whose 9x8 downsample has non-decreasing brightness across every
// row, so a plain sentinel would misreport some real photos as corrupt.
optional<uint64_t> compute_dhash(const string &filepath);