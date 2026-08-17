#include "pipeline.hpp"
#include "scanner.hpp"
#include "phash.hpp"
#include "matcher.hpp"
#include "vptree.hpp"

#include <chrono>

vector<vector<string>> find_duplicates_in_folder(const string &folder, int threshold)
{
    auto files = scan_directory(folder, DEFAULT_IMAGE_EXTENSIONS);

    vector<FileHash> items;

    for(const auto &f : files)
    {
        auto hash_opt = compute_dhash(f);

        if(hash_opt.has_value())
        {
            items.push_back({f, hash_opt.value()});
        }
    }

    VPTree tree;
    tree.build(items);

    auto t0 = chrono::steady_clock::now();
    auto naive_groups = find_duplicates_naive(items, threshold);
    auto t1 = chrono::steady_clock::now();
    auto tree_groups = find_duplicates_vptree(tree, items, threshold);
    auto t2 = chrono::steady_clock::now();

    double naive_ms = chrono::duration<double, milli>(t1 - t0).count();
    double tree_ms = chrono::duration<double, milli>(t2 - t1).count();

    bool match = groups_match(naive_groups, tree_groups);

    // Same "measure, don't assume" rule as dev_check.cpp: trust the tree
    // only when it's verified correct AND actually faster this run.
    const auto &groups = (match && tree_ms < naive_ms) ? tree_groups : naive_groups;

    vector<vector<string>> result;

    for(const auto &group : groups)
    {
        vector<string> paths;

        for(int idx : group)
        {
            paths.push_back(items[idx].path);
        }

        result.push_back(paths);
    }

    return result;
}