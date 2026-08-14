#include "vptree.hpp"
#include <algorithm>
#include <random>

void VPTree::build(vector<FileHash> data)
{
    items = move(data);

    vector<int> indices(items.size());

    for(size_t i = 0; i < indices.size(); i++)
    {
        indices[i] = (int)i;
    }

    root = build_recursive(indices, 0, (int)indices.size());
}

unique_ptr<VPTree::Node> VPTree::build_recursive(vector<int> &indices, int lo, int hi)
{
    if(lo >= hi)
    {
        return nullptr;
    }

    auto node = make_unique<Node>();

    // Fixed seed -- deterministic tree, easier to debug/reproduce.
    static mt19937 rng(42);
    uniform_int_distribution<int> dist(lo, hi - 1);
    int vp_pos = dist(rng);
    swap(indices[lo], indices[vp_pos]);

    int vp_idx = indices[lo];
    node->point_idx = vp_idx;

    if(hi - lo > 1)
    {
        int mid = lo + 1 + (hi - lo - 1) / 2;

        // Partition the remaining points so everything before `mid` is
        // closer to the vantage point than everything from `mid` onward.
        nth_element(
            indices.begin() + lo + 1, indices.begin() + mid, indices.begin() + hi,
            [&](int a, int b)
            {
                return hamming_distance(items[vp_idx].hash, items[a].hash) <
                       hamming_distance(items[vp_idx].hash, items[b].hash);
            });

        node->threshold = hamming_distance(items[vp_idx].hash, items[indices[mid]].hash);

        node->inside = build_recursive(indices, lo + 1, mid);
        node->outside = build_recursive(indices, mid, hi);
    }

    return node;
}

int VPTree::node_count() const
{
    return count_recursive(root.get());
}

int VPTree::count_recursive(const Node *node) const
{
    if(!node)
    {
        return 0;
    }

    return 1 + count_recursive(node->inside.get()) + count_recursive(node->outside.get());
}

int VPTree::depth() const
{
    return depth_recursive(root.get());
}

int VPTree::depth_recursive(const Node *node) const
{
    if(!node)
    {
        return 0;
    }

    int left = depth_recursive(node->inside.get());
    int right = depth_recursive(node->outside.get());

    return 1 + max(left, right);
}

void VPTree::collect_recursive(const Node *node, vector<int> &out) const
{
    if(!node)
    {
        return;
    }

    out.push_back(node->point_idx);
    collect_recursive(node->inside.get(), out);
    collect_recursive(node->outside.get(), out);
}

bool VPTree::verify_invariant() const
{
    return verify_recursive(root.get());
}

bool VPTree::verify_recursive(const Node *node) const
{
    if(!node)
    {
        return true;
    }

    vector<int> inside_points;
    collect_recursive(node->inside.get(), inside_points);

    for(int idx : inside_points)
    {
        if(hamming_distance(items[node->point_idx].hash, items[idx].hash) > node->threshold)
        {
            return false;
        }
    }

    vector<int> outside_points;
    collect_recursive(node->outside.get(), outside_points);

    for(int idx : outside_points)
    {
        if(hamming_distance(items[node->point_idx].hash, items[idx].hash) < node->threshold)
        {
            return false;
        }
    }

    return verify_recursive(node->inside.get()) && verify_recursive(node->outside.get());
}