#pragma once

// C++ Standard Libraries
#include <filesystem>
#include <fstream>
#include <string>

// Imported Libraries

// Vakol Libraries
#include "Logger/Logger.hpp"

/**
 * @brief Load a file as a string.
 *
 * @param path The file path of the file.
 * @return std::string The content of the file as a string.
 */
std::string LoadFile(const std::string& path);

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