#pragma once
#include <cstdint>
#include <string>
 
// Computes a 64-bit difference-hash (dHash) for the image at filepath.
// Visually similar images produce hashes that differ in only a few bits,
// even across different formats, resolutions, or light recompression --
// that's what makes this useful for near-duplicate detection later, not
// just exact-file matching.
// Returns 0 if the file can't be loaded/decoded.
uint64_t compute_dhash(const string & filepath);