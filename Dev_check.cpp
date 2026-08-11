#include "scanner.hpp"
#include "phash.hpp"

#include <bits/stdc++.h>

using namespace std;

int main(int argc, char **argv)
{
    if(argc < 2)
    {
        cerr << "Usage: " << argv[0] << " <folder>\n";
        return 1;
    }

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

    for(const auto &f : files)
    {
        uint64_t hash = compute_dhash(f);

        cout << "  "
             << hex
             << setw(16)
             << setfill('0')
             << hash
             << dec
             << "  "
             << f;

        if(hash == 0)
        {
            cout << "   (couldn't decode -- unsupported or corrupt file)";
        }

        cout << "\n";
    }

    return 0;
}