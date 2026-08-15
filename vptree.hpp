#pragma once
#include <vector>
#include <memory>
#include "matcher.hpp"

using namespace std;

// A Vantage-Point Tree over Hamming space. Organizes hashes so a query can
// ask "what's within distance D of this point?" in roughly O(log n) time
// instead of comparing against every other point.
class VPTree
{
public:
    void build(vector<FileHash> data);

    // Returns indices (into the data passed to build()) of every point
    // within max_distance of target, including target itself.
    vector<int> find_within(const FileHash &target, int max_distance) const;

    // Diagnostics -- useful for sanity-checking the build.
    int node_count() const;
    int depth() const;

    // Walks the whole tree and checks the core VP-Tree invariant at every
    // node: everything in `inside` must be within `threshold` of the
    // vantage point, everything in `outside` must be at or beyond it.
    // Deliberately O(n^2) -- it re-derives distances from scratch instead
    // of trusting the build, so it's only meant for testing/debugging,
    // never called in the hot path.
    bool verify_invariant() const;

private:
    struct Node
    {
        int point_idx = -1;
        int threshold = 0;
        unique_ptr<Node> inside;   // points closer than threshold
        unique_ptr<Node> outside;  // points farther than threshold
    };

    vector<FileHash> items;
    unique_ptr<Node> root;

    unique_ptr<Node> build_recursive(vector<int> &indices, int lo, int hi);
    void search_recursive(const Node *node, const FileHash &target, int max_distance,
                           vector<int> &results) const;

    int count_recursive(const Node *node) const;
    int depth_recursive(const Node *node) const;
    void collect_recursive(const Node *node, vector<int> &out) const;
    bool verify_recursive(const Node *node) const;
};

// Same grouping semantics as find_duplicates_naive() (see matcher.hpp) --
// every unvisited image becomes a group leader, everything the tree finds
// within threshold of it joins the group and gets marked visited, groups
// of size 1 are dropped. The only difference is *how* matches get found:
// a tree query instead of a full linear scan.
vector<vector<int>> find_duplicates_vptree(const VPTree &tree, const vector<FileHash> &items, int threshold);