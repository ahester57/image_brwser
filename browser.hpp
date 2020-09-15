// assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Austin Hester CS542o sept 2020
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <sys/stat.h>
#include <iostream>
#include <dirent.h>

void display_images(std::vector<std::string> file_paths, uint rows, uint cols);
int parse_arguments(const int argc, const char** argv, std::string* input_dir_path, uint* rows, uint* cols);
std::vector<std::string> open_dir(const char* dir_string);
void open_dir(const std::string dir_string, const std::string line_prefix, std::vector<std::string>* file_paths);
int is_directory(const char* path);