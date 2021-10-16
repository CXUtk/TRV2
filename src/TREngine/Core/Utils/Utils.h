#pragma once
#include <exception>
#include <cstdio>
#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

/**
 * @brief Returns the greatest integer less than or equal to Q/D
 * @param quotion 
 * @param divisor 
 * @return 
*/
inline int RoundDown(int quotion, int divisor)
{
    if (divisor < 0) divisor *= -1, quotion *= -1;
    if (quotion < 0) return (quotion - divisor + 1) / divisor;
    return quotion / divisor;
}

inline glm::ivec2 RoundDown(glm::ivec2 quotion, int divisor)
{
    return glm::ivec2(RoundDown(quotion.x, divisor), RoundDown(quotion.y, divisor));
}

/**
 * @brief Returns the smallest integer greater than or equal to Q/D
 * @param quotion
 * @param divisor
 * @return
*/
inline int RoundUp(int quotion, int divisor)
{
    if (divisor < 0) divisor *= -1, quotion *= -1;
    if (quotion < 0) return quotion / divisor;
    return (quotion + divisor - 1) / divisor;
}

inline glm::ivec2 RoundUp(glm::ivec2 quotion, int divisor)
{
    return glm::ivec2(RoundUp(quotion.x, divisor), RoundUp(quotion.y, divisor));
}


template<typename T>
inline T cross2(glm::vec<2, T> A, glm::vec<2, T> B)
{
    return A.x * B.y - A.y * B.x;
}

inline float sqrDistance(glm::vec2 A, glm::vec2 B)
{
    return glm::dot(A - B, A - B);
}

inline glm::vec2 rotateBy(glm::vec2 V, float r)
{
    glm::mat2 transform = glm::identity<glm::mat2>();
    if (r != 0.f)
    {
        auto cosr = std::cos(r);
        auto sinr = std::sin(r);
        transform[0][0] = cosr;
        transform[0][1] = sinr;
        transform[1][0] = -sinr;
        transform[1][1] = cosr;
    }
    return transform * V;
}

/**
 * @brief Get the formatted std::string
 * @tparam ...Args 
 * @param format The same format as printf
 * @param ...args The same arguments as arguments of printf
 * @return Formatted string
*/
template<typename ... Args>
inline std::string string_format(const std::string& format, Args&& ... args)
{
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throw std::exception("Error during formatting."); }
    auto size = static_cast<size_t>(size_s);
    auto buf = std::make_unique<char[]>(size);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
}

/**
 * @brief Read all string from a text file
 * @param fileName Name of the file
 * @return All string in file
*/
inline std::string ReadAllStringFromFile(const std::string& fileName) {
    std::ifstream iFile;
    iFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    std::string str;
    try {
        iFile.open(fileName);
        std::stringstream strStream;
        strStream << iFile.rdbuf();
        iFile.close();
        str = strStream.str();
    }
    catch (std::ifstream::failure e) {
        throw std::exception(string_format("Failed to read file %s: %s", fileName.c_str(), e.what()).c_str());
    }
    return str;
}
