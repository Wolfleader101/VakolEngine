#pragma once

#include <string>

struct md2_model_t;

int LoadMD2File(const char* path, md2_model_t& mdl);
std::string LoadFile(const std::string& path);

unsigned char* LoadImage(int size, int& width, int& height, int& nrChannels, bool flip, const void* data);
unsigned char* LoadImage(std::string&& path, int& width, int& height);
unsigned char* LoadImage(const char* path, int& width, int& height, int& nrChannels, bool flip);
unsigned char* LoadImage(std::string&& path, int& width, int& height, int& nrChannels, bool flip);

/* Helper Functions */
bool FileExists(const std::string& file);

std::string GetFileSuffix(const std::string& path, const char ch);
std::string GetFileExtension(const std::string& path);