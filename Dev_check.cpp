#include "scanner.hpp"
#include "phash.hpp"
#include "matcher.hpp"

#include <bits/stdc++.h>

using namespace std;

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

    cout << "\nComparing " << items.size()
         << " image(s) pairwise (threshold=" << threshold << ")...\n";

    auto groups = find_duplicates_naive(items, threshold);

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