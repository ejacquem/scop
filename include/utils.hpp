#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <vector>
#include <filesystem>
#include <algorithm>
#include "glad/include/glad/glad.h" //load opengl function

// Define color codes
#define RESET   "\033[0m"
#define BLACK   "\033[30m"      // Black
#define RED     "\033[31m"      // Red
#define GREEN   "\033[32m"      // Green
#define YELLOW  "\033[33m"      // Yellow
#define BLUE    "\033[34m"      // Blue
#define MAGENTA "\033[35m"      // Magenta
#define CYAN    "\033[36m"      // Cyan
#define WHITE   "\033[37m"      // White

// Define bright color codes
#define BRIGHT_BLACK   "\033[90m"
#define BRIGHT_RED     "\033[91m"
#define BRIGHT_GREEN   "\033[92m"
#define BRIGHT_YELLOW  "\033[93m"
#define BRIGHT_BLUE    "\033[94m"
#define BRIGHT_MAGENTA "\033[95m"
#define BRIGHT_CYAN    "\033[96m"
#define BRIGHT_WHITE   "\033[97m"

// int load_image(const char *path, int srcDataFormat, int option1);
std::vector<std::string> get_sorted_file_list(const std::string& directory, const std::string& extension);
int get_file_index(const std::string& filename, const std::string& directory);
int load_image(const char *path, int option1);

#endif