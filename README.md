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
