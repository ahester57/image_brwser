// assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Austin Hester CS542o sept 2o2o
// g++.exe (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include <sys/stat.h>
#include <iostream>
#include <random>
#include <dirent.h>

void open_dir(const char* dir_string);
int is_directory(const struct dirent* drnt);


int
main(int argc, char** argv)
{
    cv::String keys =
        "{@directory  |<none>| input directory}"             // input directory is the first argument (positional)
        "{rows r      |1080  | --rows=1080}"                 // optional, default value 1080
        "{cols c      |1920  | --cols=1920}"                 // optional, default value 1920
        "{help h      |      | show help message}";          // optional, show help optional

    cv::CommandLineParser parser(argc, argv, keys);
    if (parser.has("h")) {
        parser.printMessage();
        return 0;
    }

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    const char* input_dir_path;
    try {
        input_dir_path = parser.get<std::string>(0).c_str();
    } catch(...) {
        std::cerr << "Failed to parse arguments!:" << std::endl;
        return -1;
    }

    uint rows = parser.get<int>("r") ? parser.get<int>("r") : 1080;
    uint cols = parser.get<int>("c") ? parser.get<int>("c") : 1920;

    if (rows > 1080 || cols > 1920) {
        std::cerr << "Dimensions too large. Max 1920x1080" << std::endl;
        return -1;
    }

	std::cout << input_dir_path << rows << cols << std::endl;

    open_dir(input_dir_path);
	return 0;
}

void
open_dir(const char* dir_string)
{
    DIR *dr;
    try {
        dr = opendir(dir_string); //open all directory
    } catch (...) {
        std::cerr << "Failed to open directory: [" << dir_string << "]" << std::endl;
        return;
    }
    if (!dr) {
        std::cerr << "Failed to open directory: [" << dir_string << "]" << std::endl;
        return;
    }
    bool is_dir = false;
    struct dirent *drnt;
    while ((drnt = readdir(dr)) != NULL) {
        // check if is a directory
        is_dir = is_directory(drnt);
        std::cout << " \n" << drnt->d_name << is_dir; //print all directory name
    }
    closedir(dr); //close the directory
}

int
is_directory(const struct dirent* drnt)
{
    // credit: https://stackoverflow.com/a/39430337
    struct stat stbuf;
    // stat follows symlinks, lstat doesn't.
    try {
        stat(drnt->d_name, &stbuf);
        return S_ISDIR(stbuf.st_mode);
    } catch (...) {
        return false;
    }
}
