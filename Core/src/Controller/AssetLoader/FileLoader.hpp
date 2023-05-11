#pragma once

#include <string>

struct md2_model_t;

int LoadMD2File(const char* path, md2_model_t& mdl);
std::string LoadFile(const std::string& path);

unsigned char* LoadImage(std::string&& path, int& width, int& height);
unsigned char* LoadImage(std::string&& path, int& width, int& height, int& nrChannels, const bool flip);
unsigned char* LoadNoiseImage(const int size, float scale, const int octaves, const float persistence, const float lacunarity);

/* Helper Functions */
bool FileExists(const std::string& file);

std::string GetFileSuffix(const std::string& path, const char ch);
std::string GetFileExtension(const std::string& path);