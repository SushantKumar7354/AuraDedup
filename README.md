<p style="font-family: 'Caveat', cursive;">
Author Sushant Kumar
</p>

<h1 align="center" style="font-family: 'Times New Roman', Times, serif;">
AuraDedup
</h1>

<p align="left" style="font-family: 'Comic Sans MS', cursive;">
A local tool to find duplicate and near-duplicate photos — so you don't have to eyeball thousands of files to spot the copies eating your storage.
</p>

<h3>Why</h3>

<p style="font-family: 'Comic Sans MS', cursive;">
Everyday users constantly run out of phone and computer storage because of duplicate
or near-duplicate photos—think burst shots, slightly blurry accidental photos, or
compressed memes saved multiple times.
Standard software uses traditional matching (like checking file names or exact
cryptographic hashes), which completely fails if a picture is slightly resized, compressed,
or shifted by a single pixel. Conversely, modern AI photo scanners are often too slow,
require expensive cloud servers, and present a massive privacy risk by scanning
personal directories online.
</p>

<h3>What is this</h3>

<p style="font-family: 'Comic Sans MS', cursive;">
AuraDedup is a lightning-fast, highly optimized local desktop application. It scans a
user's computer, identifies visually similar images even if they have been altered, and
groups them together so the user can easily free up gigabytes of storage space.
Because it runs entirely locally using a highly efficient custom-built backend, it
guarantees 100% data privacy and operates at speeds standard applications cannot
match.
</p>

<h2 style="font-family: 'Comic Sans MS', cursive;">
🚀 Progress Log
</h2>

<h3 style="font-family: 'Comic Sans MS', cursive;">
📅 Day 1 — Repository Setup + Directory Scanner
</h3>

<p style="font-family: 'Comic Sans MS', cursive;">
Built the initial repository structure and implemented 
<b><code>scan_directory()</code></b>. It recursively walks through a folder 
and all its subfolders to find image files 
<b>(JPG, JPEG, PNG, BMP, GIF)</b>. File extensions are handled 
<b>case-insensitively</b>, while non-image files are skipped.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
🧪 Verified the scanner using <b><code>dev_check.cpp</code></b>, 
a temporary CLI program that lists all the image files found.
</p>

<hr>

<h3 style="font-family: 'Comic Sans MS', cursive;">
📅 Day 2 — dHash Implementation
</h3>

<p style="font-family: 'Comic Sans MS', cursive;">
Implemented <b><code>compute_dhash()</code></b>, which generates a 
<b>64-bit Difference Hash (dHash)</b> for each image using 
<a href="https://github.com/nothings/stb">stb_image</a> for image decoding.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
🖼️ Each image is downsampled to a <b>9×8 grayscale grid</b>. 
The algorithm then compares the brightness of neighboring pixels and 
stores these comparisons as individual bits, producing a compact 
<b>64-bit hash</b>.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
⚡ This makes the hash relatively stable against 
<b>recompression and resizing</b>, while remaining very cheap to 
calculate and compare.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
🔍 <b><code>dev_check.cpp</code></b> was updated to display each image's 
dHash alongside its file path.
</p>

<hr>

<h3 style="font-family: 'Comic Sans MS', cursive;">
📅 Day 3 — Naive Duplicate Matching
</h3>

<p style="font-family: 'Comic Sans MS', cursive;">
Implemented <b><code>find_duplicates_naive()</code></b>, a brute-force
O(n²) comparison that checks every image's hash against every other
image's hash and groups the ones within a Hamming-distance threshold.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
🐢 It's deliberately slow — this exists as a correctness baseline to
check the upcoming VP-Tree against later, not as the final approach.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
🔧 Also fixed two bugs found while testing today's code:
<b><code>compute_dhash()</code></b> used to return a plain <code>0</code>
to mean "couldn't decode" — but <code>0</code> is also a hash a real,
successfully-decoded photo can legitimately produce, so it now returns
<code>optional&lt;uint64_t&gt;</code> instead, which can't be confused
with a real value. Also fixed a filename/<code>#include</code> case
mismatch that broke the build on Linux/macOS.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
🔍 <b><code>dev_check.cpp</code></b> now prints actual duplicate groups
end-to-end, instead of just raw hashes.
</p>

<hr>

<h3 style="font-family: 'Comic Sans MS', cursive;">
📅 Day 4 — VP-Tree Construction
</h3>

<p style="font-family: 'Comic Sans MS', cursive;">
Implemented the build side of a <b>Vantage-Point Tree</b>: recursively
picks a vantage point, partitions the remaining hashes by Hamming distance
to it, and repeats on each half. This is the index the search will run
against tomorrow — today is just proving the tree's shape is actually
correct before search logic goes anywhere near it.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
🧪 Added <b><code>verify_invariant()</code></b>, which walks the whole
tree and re-derives every distance from scratch to confirm the core
VP-Tree property holds at every node: everything closer than the node's
threshold ends up in <code>inside</code>, everything farther in
<code>outside</code>. Deliberately slow (checks every pair) — it's a
correctness tool, not something meant to run in the hot path.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
📊 On a 200-item random test, the tree came out <b>8 levels deep</b>
(log₂(200) ≈ 7.6) instead of 200 — confirms the partitioning is actually
balancing the tree, not degenerating into a linked list.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
🔍 <b><code>dev_check.cpp</code></b> now builds the index and prints its
node count/depth alongside the (still naive, for now) duplicate groups.
</p>

<hr>

<h3 style="font-family: 'Comic Sans MS', cursive;">
📅 Day 5 — VP-Tree Search + a Real Finding About It
</h3>

<p style="font-family: 'Comic Sans MS', cursive;">
Implemented <b><code>find_within()</code></b> — the actual tree search,
using the standard VP-Tree pruning rule to skip branches that can't
possibly contain a match. Checked it against an independent brute-force
reference across 200 random queries on a 300-item tree: <b>zero
mismatches</b>. Then added <b><code>find_duplicates_vptree()</code></b>,
which groups duplicates using the tree instead of a linear scan, and
cross-checked it against Day 3's <code>find_duplicates_naive()</code>
across 49 different (size, threshold) combinations plus the earlier
clustered/all-identical test cases — <b>every result matched exactly</b>.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
📉 <b>Here's the part I didn't expect:</b> at this project's actual
default threshold (5), the VP-Tree came out <i>slower</i> than the naive
scan it was supposed to replace — 258ms vs 230ms on 10,000 items. At
threshold 0–2 it's dramatically faster (up to 48x at threshold 0), but
that advantage flips somewhere around threshold 3–4 and gets worse the
looser the matching gets.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
Why: in 64-bit Hamming space, a loose search radius means the tree's
pruning condition ends up true on <i>both</i> branches at nearly every
node, so it walks almost the whole tree anyway — just with recursion and
pointer-chasing overhead that a flat array scan doesn't pay. This is a
known limitation of VP-Trees (and metric trees generally) for exactly
this kind of loose/near-duplicate matching in high-bit-count spaces —
not a bug in the implementation, which is independently verified correct.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
🔧 <b><code>dev_check.cpp</code></b> now runs and times <i>both</i>
implementations every time, verifies they agree, and uses whichever was
actually faster on that run — rather than assuming the tree wins just
because it's the fancier data structure. Both stay in the codebase:
naive as the always-correct baseline, VP-Tree as validated, working
infrastructure that's a genuine win for tight-threshold matching even if
not (yet) for this project's default.
</p>

<hr>

<h3 style="font-family: 'Comic Sans MS', cursive;">
📅 Day 6 — pybind11 + CMake: Wiring C++ to Python
</h3>

<p style="font-family: 'Comic Sans MS', cursive;">
Added <b><code>pipeline.cpp</code></b>, which strings together everything
built Days 1–5 — scan, hash, build the index, cross-check naive against
the VP-Tree, keep whichever's actually faster — into one clean function
with no debug printing, since Python (not the terminal) is the caller
now. <b><code>bindings.cpp</code></b> wraps that one function with
<a href="https://github.com/pybind/pybind11">pybind11</a> as
<code>aura_engine.find_duplicates(folder, threshold)</code>. Added
<b><code>CMakeLists.txt</code></b> — the real build system, replacing
the ad-hoc <code>g++ file1.cpp file2.cpp...</code> commands used through
Day 5.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
🧹 Small refactor while wiring this up: <code>groups_match()</code> and
the default image-extensions list were both duplicated between
<code>dev_check.cpp</code> and the new pipeline code, so both got
promoted to shared homes (<code>matcher.hpp</code> and
<code>scanner.hpp</code> respectively). <code>dev_check.cpp</code>'s
actual behavior is unchanged — confirmed with the same test folder as
Day 5, identical output.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
⚠️ <b>Honest limitation on today's testing:</b> I couldn't install the
real pybind11 in this environment (no internet access) or compile the
actual CMake + pybind11 build, so <code>bindings.cpp</code> is verified
two different ways short of that: the exact function it calls
(<code>find_duplicates_in_folder</code>) is tested directly and returns
correct results, and the pybind11-specific syntax was checked against a
minimal stand-in header that mimics the real API surface, structurally.
What's <i>not</i> yet proven is the real build+import on a real machine
— that's the first thing to run once <code>stb_image.h</code>,
<code>pybind11</code>, and CMake are actually installed.
</p>