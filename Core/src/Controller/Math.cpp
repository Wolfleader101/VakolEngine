#include "Math.hpp"

float Lerp(const float a, const float b, const float t)
{
    return (1.0f - t) * a + b * t;
}

/// return a fraction 't' (between 0 and 1) based on the given value
float InverseLerp(const float a, const float b, const float v)
{
    return (v - a) / (b - a);
}

/// takes a value within a given input range into a given output range
float Remap(const float iMin, const float iMax, const float oMin, const float oMax, const float v)
{
    float t = InverseLerp(iMin, iMax, v);

    return Lerp(oMin, oMax, t);
}

unsigned char* GenerateNoise(const int size, float scale, const int octaves, const float persistence,
                             const float lacunarity)
{
    const auto data = new unsigned char[size * size];

    srand(static_cast<unsigned int>(time(nullptr)));

    std::vector<glm::vec2> offsets;
    const auto offset = glm::vec2(0.0f);

    offsets.reserve(octaves);

    for (int i = 0; i < octaves; ++i)
    {
        const float off_x = rand() + offset.x;
        const float off_y = rand() + offset.y;

        offsets.emplace_back(off_x, off_y);
    }

    if (scale <= 0.0f)
        scale = 1e-5f;

    float max_noise_height = FLT_MIN;
    float min_noise_height = FLT_MAX;

    const float half_size = static_cast<float>(size) / 2.0f;

    for (int y = 0; y < size; ++y)
    {
        for (int x = 0; x < size; ++x)
        {
            float amplitude = 1.0f;
            float frequency = 1.0f;
            float height = 0.0f;

            for (int i = 0; i < octaves; ++i)
            {
                const float sample_x = (x - half_size) / scale * frequency + offsets.at(i).x;
                const float sample_y = (y - half_size) / scale * frequency + offsets.at(i).y;

                const float perlin = glm::perlin(glm::vec2(sample_x, sample_y)) * 2 - 1;
                height += perlin * amplitude;

                amplitude *= persistence;
                frequency *= lacunarity;
            }

            if (height > max_noise_height)
                max_noise_height = height;
            else if (height < min_noise_height)
                min_noise_height = height;

            data[y * size + x] =
                static_cast<unsigned char>(InverseLerp(min_noise_height, max_noise_height, height) * 255.0f);
        }
    }

    return data;
}