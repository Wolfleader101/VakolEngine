#pragma once

#include <string>

/**
 * @brief Forward declaration of md2_model_t struct.
 */
struct md2_model_t;

/**
 * @brief Load an MD2 file.
 *
 * @param path The file path of the MD2 file.
 * @param mdl Reference to the md2_model_t struct to store the loaded model.
 * @return int 0 if the file was successfully loaded, otherwise an error code.
 */
int LoadMD2File(const char* path, md2_model_t& mdl);

/**
 * @brief Load a file as a string.
 *
 * @param path The file path of the file.
 * @return std::string The content of the file as a string.
 */
std::string LoadFile(const std::string& path);

/**
 * @brief Load an image with specified parameters.
 *
 * @param size The size of the image.
 * @param width Reference to an integer to store the width of the loaded image.
 * @param height Reference to an integer to store the height of the loaded image.
 * @param nrChannels Reference to an integer to store the number of color channels in the loaded image.
 * @param flip Flag indicating whether the image should be flipped vertically.
 * @param data Pointer to additional data for the image.
 * @return unsigned char* A pointer to the loaded image data.
 */
unsigned char* LoadImage(int size, int& width, int& height, int& nrChannels, bool flip, const void* data);

/**
 * @brief Load an image from a file with specified parameters.
 *
 * @param path The file path of the image.
 * @param width Reference to an integer to store the width of the loaded image.
 * @param height Reference to an integer to store the height of the loaded image.
 * @return unsigned char* A pointer to the loaded image data.
 */
unsigned char* LoadImage(std::string&& path, int& width, int& height);

/**
 * @brief Load an image from a file with specified parameters.
 *
 * @param path The file path of the image.
 * @param width Reference to an integer to store the width of the loaded image.
 * @param height Reference to an integer to store the height of the loaded image.
 * @param nrChannels Reference to an integer to store the number of color channels in the loaded image.
 * @param flip Flag indicating whether the image should be flipped vertically.
 * @return unsigned char* A pointer to the loaded image data.
 */
unsigned char* LoadImage(const char* path, int& width, int& height, int& nrChannels, bool flip);

/**
 * @brief Load an image from a file with specified parameters.
 *
 * @param path The file path of the image.
 * @param width Reference to an integer to store the width of the loaded image.
 * @param height Reference to an integer to store the height of the loaded image.
 * @param nrChannels Reference to an integer to store the number of color channels in the loaded image.
 * @param flip Flag indicating whether the image should be flipped vertically.
 * @return unsigned char* A pointer to the loaded image data.
 */
unsigned char* LoadImage(std::string&& path, int& width, int& height, int& nrChannels, bool flip);

/**
 * @brief Check if a file exists.
 *
 * @param file The file path.
 * @return bool True if the file exists, false otherwise.
 */
bool FileExists(const std::string& file);

/**
 * @brief Get the suffix of a file path.
 *
 * @param path The file path.
 * @param ch The character delimiter for the suffix.
 * @return std::string The suffix of the file path.
 */
std::string GetFileSuffix(const std::string& path, const char ch);

/**
 * @brief Get the extension of a file path.
 *
 * @param path The file path.
 * @return std::string The extension of the file path.
 */
std::string GetFileExtension(const std::string& path);