#define STB_IMAGE_IMPLEMENTATION

#include "stb_image.h"
#include "phash.hpp"

#include <bits/stdc++.h>

using namespace std;

vector<unsigned char> to_gray_9x8(
    const unsigned char *data,
    int w,
    int h,
    int channels)
{
    int TW = 9;
    int TH = 8;

    vector<unsigned char> gray(TW * TH);

    for(int y = 0; y < TH; y++)
    {
        for(int x = 0; x < TW; x++)
        {
            int srcX = x * w / TW;
            int srcY = y * h / TH;

            const unsigned char *px =
                data + (static_cast<size_t>(srcY) * w + srcX) * channels;

            int lum;

            if(channels >= 3)
            {
                lum = (px[0] * 299 +
                       px[1] * 587 +
                       px[2] * 114) / 1000;
            }
            else
            {
                lum = px[0];
            }

            gray[y * TW + x] = static_cast<unsigned char>(lum);
        }
    }

    return gray;
}

optional<uint64_t> compute_dhash(const string &filepath)
{
    int w, h, channels;

    unsigned char *data =
        stbi_load(filepath.c_str(), &w, &h, &channels, 0);

    if(!data || w <= 0 || h <= 0)
    {
        if(data)
        {
            stbi_image_free(data);
        }

        return nullopt;
    }

    vector<unsigned char> gray =
        to_gray_9x8(data, w, h, channels);

    stbi_image_free(data);

    uint64_t hash = 0;

    int bit = 0;

    for(int y = 0; y < 8; y++)
    {
        for(int x = 0; x < 8; x++)
        {
            unsigned char left =
                gray[y * 9 + x];

            unsigned char right =
                gray[y * 9 + x + 1];

            if(left > right)
            {
                hash |= (1ULL << bit);
            }

            bit++;
        }
    }

    return hash;
}