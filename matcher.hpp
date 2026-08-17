#pragma once
#include <string>
#include <vector>
#include <cstdint>

using namespace std;

// One scanned image: its path and its 64-bit perceptual hash.
struct FileHash
{
    string path;
    uint64_t hash;
};

// Hamming distance = number of differing bits between two 64-bit hashes.
int hamming_distance(uint64_t a, uint64_t b);

// Brute-force duplicate grouping: checks every image against every other
// image (O(n^2)). Slow, but obviously correct -- this is the baseline
// we'll check the VP-Tree version against once that exists.
// Returns groups of indices into `items` whose hashes are within
// `threshold` of each other. Groups of size 1 (no match found) are dropped.
vector<vector<int>> find_duplicates_naive(const vector<FileHash> &items, int threshold);

// Compares two duplicate-group results for equality, ignoring group order
// and the order of indices within each group. Used to cross-check
// find_duplicates_vptree() against this naive baseline (see Day 5).
bool groups_match(vector<vector<int>> a, vector<vector<int>> b);