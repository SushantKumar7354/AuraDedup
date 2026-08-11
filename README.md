<hr>

<h2 style="font-family: 'Comic Sans MS', cursive;">
⚠️ Advanced Edge Cases / Known Limitations
</h2>

<h3 style="font-family: 'Comic Sans MS', cursive;">
1. The Aliasing Vulnerability (High-Frequency Noise)
</h3>

<p style="font-family: 'Comic Sans MS', cursive;">
The current downsampling algorithm uses extremely fast
<b>nearest-neighbor sampling</b> using calculations such as
<code>srcX = x * w / TW</code>.
While this approach is computationally efficient, it is susceptible
to <b>aliasing</b> when processing images containing sharp,
high-frequency patterns.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
For example, an image containing a fine checkerboard pattern or
similar high-frequency details may produce significantly different
brightness gradients if the image is shifted by even a single pixel.
This can cause several bits of the dHash to flip and reduce the
stability of similarity detection.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
💡 <b>Future improvement:</b> Apply a small box-blur or averaging
operation before downsampling and hashing. This could reduce
high-frequency noise and make the generated hash more stable.
</p>

<h3 style="font-family: 'Comic Sans MS', cursive;">
2. Unicode File Paths on Windows
</h3>

<p style="font-family: 'Comic Sans MS', cursive;">
File paths containing non-ASCII Unicode characters may cause problems
on Windows. The current image loading process relies on the standard
C <code>open()</code> mechanism internally through <code>stb_image</code>,
which may struggle with paths containing characters such as
Japanese Kanji, emojis, or certain accented characters.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
As a result, files whose paths contain non-standard Unicode characters
may be discovered by the scanner but fail during image decoding on
Windows.
</p>

<p style="font-family: 'Comic Sans MS', cursive;">
💡 <b>Future improvement:</b> Add proper Windows Unicode path handling
using wide-character file APIs or a Unicode-compatible image-loading
path.
</p>