#pragma once
#include <vector>
#include <memory>
#include "matcher.hpp"

using namespace std;

// A Vantage-Point Tree over Hamming space. Organizes hashes so that a
// future search can ask "what's within distance D of this point?" without
// comparing against every other point. That's tomorrow's job though --
// today is just building the tree and proving its shape is actually
// correct before any search logic gets written on top of it.
class VPTree
{
public:
    void build(vector<FileHash> data);

    // Diagnostics -- useful today since there's no search yet to prove
    // the tree works end-to-end.
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

    int count_recursive(const Node *node) const;
    int depth_recursive(const Node *node) const;
    void collect_recursive(const Node *node, vector<int> &out) const;
    bool verify_recursive(const Node *node) const;
};