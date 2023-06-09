#pragma once

#include <Controller/Logger.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/noise.hpp>
#include <vector>


/**
 * @brief Perform linear interpolation between two values.
 *
 * @param a The starting value.
 * @param b The ending value.
 * @param t The interpolation parameter (between 0 and 1).
 * @return float The interpolated value.
 */
float Lerp(const float a, const float b, const float t);

/**
 * @brief Calculate the fraction 't' (between 0 and 1) based on the given value.
 *
 * @param a The starting value.
 * @param b The ending value.
 * @param v The value to calculate the fraction for.
 * @return float The calculated fraction.
 */
float InverseLerp(const float a, const float b, const float v);

/**
 * @brief Map a value from one input range to a corresponding value in an output range.
 *
 * @param iMin The minimum value of the input range.
 * @param iMax The maximum value of the input range.
 * @param oMin The minimum value of the output range.
 * @param oMax The maximum value of the output range.
 * @param v The value to remap.
 * @return float The remapped value.
 */
float Remap(const float iMin, const float iMax, const float oMin, const float oMax, const float v);

/**
 * @brief Generate noise using Perlin noise algorithm.
 *
 * @param size The size of the generated noise grid.
 * @param scale The scale of the noise.
 * @param octaves The number of octaves for generating noise.
 * @param persistence The persistence of the noise.
 * @param lacunarity The lacunarity of the noise.
 * @return unsigned char* The generated noise data.
 */
unsigned char* GenerateNoise(const int size, float scale, const int octaves, const float persistence,
                             const float lacunarity);

/**
 * @namespace glm
 * @brief Namespace for GLM library.
 */
namespace glm {

    /**
     * @brief Serialize glm::vec2 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::vec2& v) {
        archive(v.x, v.y);
    }

    /**
     * @brief Serialize glm::vec3 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::vec3& v) {
        archive(v.x, v.y, v.z);
    }

    /**
     * @brief Serialize glm::vec4 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::vec4& v) {
        archive(v.x, v.y, v.z, v.w);
    }

    /**
     * @brief Serialize glm::ivec2 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::ivec2& v) {
        archive(v.x, v.y);
    }

    /**
     * @brief Serialize glm::ivec3 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::ivec3& v) {
        archive(v.x, v.y, v.z);
    }

    /**
     * @brief Serialize glm::ivec4 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::ivec4& v) {
        archive(v.x, v.y, v.z, v.w);
    }

    /**
     * @brief Serialize glm::uvec2 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::uvec2& v) {
        archive(v.x, v.y);
    }

    /**
     * @brief Serialize glm::uvec3 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::uvec3& v) {
        archive(v.x, v.y, v.z);
    }

    /**
     * @brief Serialize glm::uvec4 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::uvec4& v) {
        archive(v.x, v.y, v.z, v.w);
    }

    /**
     * @brief Serialize glm::dvec2 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::dvec2& v) {
        archive(v.x, v.y);
    }

    /**
     * @brief Serialize glm::dvec3 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::dvec3& v) {
        archive(v.x, v.y, v.z);
    }

    /**
     * @brief Serialize glm::dvec4 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::dvec4& v) {
        archive(v.x, v.y, v.z, v.w);
    }

    /**
     * @brief Serialize glm::mat2 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::mat2& m) {
        archive(m[0], m[1]);
    }

    /**
     * @brief Serialize glm::dmat2 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::dmat2& m) {
        archive(m[0], m[1]);
    }

    /**
     * @brief Serialize glm::mat3 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::mat3& m) {
        archive(m[0], m[1], m[2]);
    }

    /**
     * @brief Serialize glm::mat4 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::mat4& m) {
        archive(m[0], m[1], m[2], m[3]);
    }

    /**
     * @brief Serialize glm::dmat4 for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::dmat4& m) {
        archive(m[0], m[1], m[2], m[3]);
    }

    /**
     * @brief Serialize glm::quat for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::quat& q) {
        archive(q.x, q.y, q.z, q.w);
    }

    /**
     * @brief Serialize glm::dquat for serialization.
     */
    template <class Archive>
    void serialize(Archive& archive, glm::dquat& q) {
        archive(q.x, q.y, q.z, q.w);
    }

}  // namespace glm
