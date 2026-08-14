#include "matcher.hpp"
#include <bitset>

int hamming_distance(uint64_t a, uint64_t b)
{
    return (int)bitset<64>(a ^ b).count();
}

vector<vector<int>> find_duplicates_naive(const vector<FileHash> &items, int threshold)
{
    int n = (int)items.size();

    vector<bool> visited(n, false);
    vector<vector<int>> groups;

    for(int i = 0; i < n; i++)
    {
        if(visited[i])
        {
            continue;
        }

        vector<int> group;

        for(int j = 0; j < n; j++)
        {
            if(visited[j])
            {
                continue;
            }

            int dist = hamming_distance(items[i].hash, items[j].hash);

            if(dist <= threshold)
            {
                group.push_back(j);
                visited[j] = true;
            }
        }

        if((int)group.size() > 1)
        {
            groups.push_back(group);
        }
    }

    return groups;
}