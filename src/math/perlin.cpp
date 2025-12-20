#include "perlin.h"

#include <algorithm>
#include <cmath>
#include <random>
#include <vector>
#include <iostream>

static int permutation[512];

inline float fade(const float t) {
    return t* t* t* (t* (t* 6 - 15) + 10);
}

inline float lerp(const float t, const float a, const float b) {
    return a + t* (b - a);
}

inline float grad(const int hash, const float x, const float y) {
    // magic lol so hard to read
    const int h = hash & 15;                      // Convert low 4 bits of hash code into 12 gradient directions
    const float u = h < 8 ? x : y,                 // Into 12 gradient directions
    v = h < 4 ? y : h == 12 || h == 14 ? x : 0;
    return ((h & 1) == 0 ? u : -u) + ((h & 2) == 0 ? v : -v); // returns the dot product between the pseudo-random gradient and the pos vector
}

float perlin(float x, float y) {
    // Find unit square that contains the point
    const int X = static_cast<int>(std::floor(x)) & 255;
    const int Y = static_cast<int>(std::floor(y)) & 255;

    // Relative point coordinates within the square
    x -= std::floor(x);
    y -= std::floor(y);

    const float u = fade(x);
    const float v = fade(y);

    // Hash coordinates of the 4 corners of the square
    const int A = permutation[X] + Y;
    const int B = permutation[(X + 1) & 255] + Y;

        // Perlin noise contribution from each corner
    const float AA = grad(permutation[A], x, y);
    const float BA = grad(permutation[B], x - 1, y);
    const float AB = grad(permutation[A + 1], x, y - 1);
    const float BB = grad(permutation[B + 1], x - 1, y - 1);

    // Interpolate along x-axis
    const float x1 = lerp(u, AA, BA);
    const float x2 = lerp(u, AB, BB);

    // Interpolate along y-axis
    return lerp(v, x1, x2);
}


/**
 * @param scale the scale
 * @param octaves # of layers; more octaves, more detail and compute time ~(1 - 8)
 * @param persistence the amount each successive octave contributes ~(0.3 - 0.7)
 * @param lacunarity the scale multiple factor (make thing smooth)
 * @return
 */
float perlinNoise(float x, float y, const int seed, const float scale, const int octaves, const float persistence,
                  const float lacunarity) {
    generatePermutation(seed);
    float total = 0.0f;
    float frequency = 1.0f;
    float amplitude = 1.0f;
    float maxValue = 0.0f;  // Used for normalization

    x /= scale;
    y /= scale;

    for (int i = 0; i < octaves; i++) {
        total += perlin(x* frequency, y* frequency) * amplitude;

        maxValue += amplitude;

        amplitude *= persistence;
        frequency *= lacunarity;
    }

    // Normalize to range [0,1]
    return total / maxValue;
}

void generatePermutation(const int seed) {
    std::vector<int> perm(256);
    for (int i = 0; i < 256; i++)
        perm[i] = i;

    std::mt19937 rng(seed); // Mersenne Twister seeded
    std::ranges::shuffle(perm, rng);

    for (int i = 0; i < 256; ++i) {
        permutation[i] = perm[i];
        permutation[256 + i] = perm[i]; // Duplicate for overflow safety
    }
}
