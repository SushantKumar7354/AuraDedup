#include "scanner.hpp"

#include <filesystem>
#include <algorithm>
#include <cctype>

using namespace std;

namespace fs = filesystem;

// Added 'static' to prevent global linker errors
static string to_lower(string s)
{
    for(char &c : s)
    {
        c = tolower(c);
    }

    return s;
}

vector<string> scan_directory(
    const string &folder,
    const vector<string> &extensions)
{
    vector<string> ans;
    error_code ec;

    if(!fs::exists(folder, ec) || !fs::is_directory(folder, ec))
    {
        return ans;
    }

    auto opts = fs::directory_options::skip_permission_denied;

    for(auto it = fs::recursive_directory_iterator(
            folder,
            opts,
            ec);
        it != fs::recursive_directory_iterator();
        it.increment(ec))
    {
        if(ec)
        {
            continue;
        }

        auto entry = *it;
        error_code file_ec;

        if(!entry.is_regular_file(file_ec) || file_ec)
        {
            continue;
        }

        string ext = to_lower(entry.path().extension().string());

        if(find(extensions.begin(),
                extensions.end(),
                ext) != extensions.end())
        {
            ans.push_back(entry.path().string());
        }
    }

    return ans;
}