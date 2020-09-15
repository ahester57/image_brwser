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


int parse_arguments(const int argc, const char** argv, std::string* input_dir_path, uint* rows, uint* cols);
std::vector<std::string> open_dir(const char* dir_string);
void open_dir(const char* dir_string, const std::string line_prefix, std::vector<std::string>* file_paths);
int is_directory(const char* path);


int
main(int argc, const char** argv)
{
    uint rows;
    uint cols;
    std::string input_dir_path;

    int parse_result = parse_arguments(argc, argv, &input_dir_path, &rows, &cols);

    if (parse_result == 1) return 0;
    if (parse_result == -1) return -1;

	std::cout << input_dir_path << std::endl;

    std::vector<std::string> file_paths = open_dir(input_dir_path.c_str());
    std::cout << std::endl << "Filename list:" << std::endl;
    for (std::vector<std::string>::iterator it = file_paths.begin(); it != file_paths.end(); ++it) {
        std::cout << ' ' << *it << std::endl;
    }
	return 0;
}


int
parse_arguments(int argc, const char** argv, std::string* input_dir_path, uint* rows, uint* cols)
{
    cv::String keys =
        "{@directory  |<none>| input directory}"             // input directory is the first argument (positional)
        "{rows r      |1080  | --rows=1080}"                 // optional, default value 1080
        "{cols c      |1920  | --cols=1920}"                 // optional, default value 1920
        "{help h      |      | show help message}";          // optional, show help optional

    cv::CommandLineParser parser(argc, argv, keys);

    if (parser.has("h")) {
        parser.printMessage();
        return 1;
    }

    try {
        *input_dir_path = (std::string) parser.get<std::string>(0).c_str();
    } catch(...) {
        std::cerr << "Failed to parse directory argument!:" << std::endl;
        return -1;
    }

    try {
        *rows = (uint) parser.get<uint>("r") ? parser.get<uint>("r") : 1080;
        *cols = (uint) parser.get<uint>("c") ? parser.get<uint>("c") : 1920;
    } catch(...) {
        std::cerr << "Failed to parse size argument!:" << std::endl;
        return -1;
    }

    if (!parser.check()) {
        parser.printErrors();
        return -1;
    }

    if (*rows > 1080 || *cols > 1920) {
        std::cerr << "Dimensions too large. Max 1920x1080" << std::endl;
        return -1;
    }
    return 0;
}


std::vector<std::string>
open_dir(const char* dir_string)
{
    std::vector<std::string> file_paths;
    open_dir(dir_string, " \n", &file_paths);
    return file_paths;
}


void
open_dir(const char* dir_string, const std::string line_prefix, std::vector<std::string>* file_paths)
{
    DIR *dr;
    try {
        dr = opendir(dir_string); //open all directories
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
        if (strcmp(drnt->d_name, ".") == 0 || strcmp(drnt->d_name, "..") == 0) {
            continue;
        }
        // check if is a directory
        const std::string rel_path = (dir_string + std::string("/") + std::string(drnt->d_name));
        // std::cout << std::endl << rel_path <<std::endl << rel_path.c_str();
        is_dir = is_directory(rel_path.c_str());
        std::cout << line_prefix << drnt->d_name << is_dir; //print all directory names
        if (is_dir) {
            // open next directory, sorry for this line
            open_dir(rel_path.c_str(), line_prefix + "\t", file_paths);
        } else {
            (*file_paths).push_back(rel_path);
        }
    }
    closedir(dr); //close the directory
}


int
is_directory(const char* path)
{
    // cred: https://stackoverflow.com/a/146938
    struct stat s;
    if( stat(path, &s) == 0 )
    {
        if( s.st_mode & S_IFDIR )
        {
            return true;
        }
    }
    return false;
}
