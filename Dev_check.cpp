#include "scanner.hpp"
#include "phash.hpp"
#include "matcher.hpp"
#include "vptree.hpp"

#include <bits/stdc++.h>
#include <chrono>

using namespace std;

bool groups_match(vector<vector<int>> a, vector<vector<int>> b)
{
    if(a.size() != b.size())
    {
        return false;
    }

    for(auto &g : a)
    {
        sort(g.begin(), g.end());
    }

    for(auto &g : b)
    {
        sort(g.begin(), g.end());
    }

    sort(a.begin(), a.end());
    sort(b.begin(), b.end());

    return a == b;
}

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <folder> [threshold]\n";
        return 1;
    }

    int threshold = (argc >= 3) ? stoi(argv[2]) : 5;

    vector<string> extensions = {
        ".jpg",
        ".jpeg",
        ".png",
        ".bmp",
        ".gif"
    };

    auto files = scan_directory(argv[1], extensions);

    cout << "Found " << files.size()
         << " image file(s) in " << argv[1] << ":\n";

    vector<FileHash> items;

    for(const auto &f : files)
    {
        auto hash_opt = compute_dhash(f);

        if(!hash_opt.has_value())
        {
            cout << "  [skip] " << f << "  (couldn't decode)\n";
            continue;
        }

        items.push_back({f, hash_opt.value()});
    }

    cout << "\nBuilding VP-Tree index over " << items.size() << " image(s)...\n";

    VPTree tree;
    tree.build(items);

    cout << "  " << tree.node_count() << " node(s), depth " << tree.depth();

    if(!tree.verify_invariant())
    {
        cout << "  [WARNING: invariant check failed -- do not trust search on this tree]";
    }

    cout << "\n";

    cout << "\nComparing " << items.size()
         << " image(s) both ways (threshold=" << threshold << ")...\n";

    auto t0 = chrono::steady_clock::now();
    auto naive_groups = find_duplicates_naive(items, threshold);
    auto t1 = chrono::steady_clock::now();
    auto tree_groups = find_duplicates_vptree(tree, items, threshold);
    auto t2 = chrono::steady_clock::now();

    double naive_ms = chrono::duration<double, milli>(t1 - t0).count();
    double tree_ms = chrono::duration<double, milli>(t2 - t1).count();

    bool match = groups_match(naive_groups, tree_groups);

    cout << "  naive: " << naive_ms << " ms   VP-Tree: " << tree_ms << " ms   ";

    if(match)
    {
        cout << "(results match)\n";
    }
    else
    {
        cout << "(MISMATCH -- falling back to naive for safety)\n";
    }

    // Trust the tree only when it's both verified correct on this run AND
    // actually faster -- at small thresholds it wins by a lot, but at the
    // loose thresholds this project actually uses it can lose to a plain
    // scan (see README, Day 5). Don't assume; measure every time.
    const auto &groups = (match && tree_ms < naive_ms) ? tree_groups : naive_groups;

    if(groups.empty())
    {
        cout << "No duplicates found.\n";
        return 0;
    }

    cout << "\nFound " << groups.size() << " duplicate group(s):\n";

    int g = 1;

    for(const auto &group : groups)
    {
        cout << "Group " << g++ << ":\n";

        for(int idx : group)
        {
            cout << "  " << items[idx].path << "\n";
        }
    }

    return 0;
}