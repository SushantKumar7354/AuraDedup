Author Sushant Kumar
<h1 align="center" style="font-family: 'Times New Roman',Times,serif;">AuraDedup</h1>
<p align="left">A local tool to find duplicate and near-duplicate photos — so you don't have to eyeball thousands of files to spot the copies eating your storage.</p>
<h3>Why</h3>
<p>Most "find duplicate files" tools only catch exact byte-for-byte matches. They miss the same photo saved twice at different sizes, re-compressed, or renamed — which is most of what actually wastes space. Plan: a small C++ engine (for speed, using a perceptual hash + VP-Tree index) driven by a simple Python CLI.</p>