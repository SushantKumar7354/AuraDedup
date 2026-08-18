#!/usr/bin/env python3
"""
AuraDedup CLI.

Thin Python front-end over the compiled `aura_engine` C++ module (Day 6).
The engine does scanning, hashing, indexing, and matching -- this script
just handles arguments, fetches file sizes (cheap and easy in Python, no
need for C++ to touch that), and prints the report.
"""
import argparse
import os
import sys


def import_engine():
    """Import the compiled C++ module, failing with clear next-steps
    instead of a bare traceback if it hasn't been built (or isn't sitting
    next to this script) yet."""
    try:
        import aura_engine
        return aura_engine
    except ImportError as e:
        print("ERROR: could not import 'aura_engine'.")
        print(f"  ({e})")
        print()
        print("Either the C++ module (Day 6) hasn't been built yet, or the")
        print("built .pyd/.so isn't in this folder. To build it:")
        print()
        print("  pip install pybind11")
        print("  mkdir build && cd build")
        print('  cmake .. -DCMAKE_PREFIX_PATH="$(python -c "import pybind11; print(pybind11.get_cmake_dir())")"')
        print("  cmake --build . --config Release")
        print("  cd ..")
        print()
        print("That drops aura_engine*.pyd (Windows) or .so (Linux/Mac) right")
        print("here automatically. Re-run this script once that exists.")
        print()
        print("If the cmake/build step itself errors, that's the thing to fix")
        print("first -- this script can't do anything until that module exists.")
        sys.exit(1)


def format_size(num_bytes):
    size = float(num_bytes)
    for unit in ("B", "KB", "MB", "GB", "TB"):
        if size < 1024:
            return f"{size:.1f} {unit}"
        size /= 1024
    return f"{size:.1f} PB"


def parse_args(argv=None):
    parser = argparse.ArgumentParser(
        prog="deduplicate.py",
        description="Find duplicate and near-duplicate images in a folder.",
    )
    parser.add_argument(
        "--folder", required=True,
        help="Folder to scan (recurses into subfolders)",
    )
    parser.add_argument(
        "--threshold", type=int, default=5,
        help="Max Hamming distance (0-64) to count as a duplicate. "
             "0 = exact match only, higher = looser matching. Default: 5",
    )
    return parser.parse_args(argv)


def build_report(groups):
    """Attach real file sizes to each group (fetched here, not by the C++
    engine) and sort each group largest-first, so the first entry is the
    copy worth keeping. Returns (report, total_reclaimable_bytes)."""
    report = []
    total_wasted = 0

    for group in groups:
        sized = []

        for path in group:
            try:
                size = os.path.getsize(path)
            except OSError:
                size = 0  # file vanished/permission changed since scanning
            sized.append((path, size))

        sized.sort(key=lambda item: item[1], reverse=True)

        wasted = sum(size for _, size in sized[1:])
        total_wasted += wasted

        report.append((sized, wasted))

    return report, total_wasted


def print_report(report, total_wasted):
    if not report:
        print("No duplicates found.")
        return

    print(f"\nFound {len(report)} duplicate group(s):\n")

    for i, (sized, wasted) in enumerate(report, 1):
        print(f"Group {i} ({len(sized)} files, {format_size(wasted)} reclaimable):")
        for j, (path, size) in enumerate(sized):
            marker = "KEEP" if j == 0 else "dup "
            print(f"  [{marker}] {path}  ({format_size(size)})")
        print()

    print(f"Total space that could be freed: {format_size(total_wasted)}")


def main(argv=None):
    args = parse_args(argv)

    if not os.path.isdir(args.folder):
        print(f"ERROR: '{args.folder}' is not a valid folder.")
        sys.exit(1)

    if not 0 <= args.threshold <= 64:
        print("ERROR: --threshold must be between 0 and 64.")
        sys.exit(1)

    aura_engine = import_engine()

    print(f"Scanning '{args.folder}' (threshold={args.threshold})...")
    groups = aura_engine.find_duplicates(args.folder, args.threshold)

    report, total_wasted = build_report(groups)
    print_report(report, total_wasted)


if __name__ == "__main__":
    main()