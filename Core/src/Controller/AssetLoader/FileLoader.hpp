#pragma once

#include <string>

std::string LoadFile(const std::string& path);

unsigned char* LoadImage(const std::string& path, int& width, int& height);
unsigned char* LoadImage(const std::string& path, int& width, int& height, int& nrChannels, const bool flip);

/* Helper Functions */
bool FileExists(const std::string& file);

std::string GetFileSuffix(const std::string& path, const char ch);
std::string GetFileExtension(const std::string& path);

class Image
{
public:
    Image(const int width, const int height) : m_width(width), m_height(height)
    {
        this->m_data = LoadNoiseMap(m_width | m_height, 1.0f, 4.0f);
    }

    Image(const std::string& path, const bool unsupported_format) : m_data(nullptr) 
    {
        this->m_width = 0;
        this->m_height = 0;

        if (unsupported_format)
            this->m_data = LoadImage(path, this->m_width, this->m_height);
        else
            this->m_data = LoadImage(path, this->m_width, this->m_height, this->m_channels, this->m_flipped);
    };

    unsigned char* LoadNoiseMap(const int size, float scale, float frequency);

    const int width() const { return this->m_width; }
    const int height() const { return this->m_height; }

    const int size() const { return this->m_width * this->m_height * this->m_channels; }

    const unsigned char* data() const { return this->m_data; }
    
    ~Image() { delete[] this->m_data; this->m_data = nullptr; } // Destructor

private:
    unsigned char* m_data = nullptr;

    int m_width = 0;
    int m_height = 0;

    int m_channels = 1;
    bool m_flipped = false;
};