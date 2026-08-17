#pragma once
#include <string>
#include <vector>

using namespace std;

// Full pipeline: scan folder -> hash images -> build VP-Tree index ->
// find duplicates, cross-checked against the naive baseline, using
// whichever approach actually measured faster this run (see Day 5 --
// that's not always the tree, at this project's loose default threshold).
// Returns groups of duplicate/near-duplicate image paths.
vector<vector<string>> find_duplicates_in_folder(const string &folder, int threshold);