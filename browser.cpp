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


void display_images(std::vector<std::string> file_paths, uint rows, uint cols);
int parse_arguments(const int argc, const char** argv, std::string* input_dir_path, uint* rows, uint* cols);
std::vector<std::string> open_dir(const char* dir_string);
void open_dir(const std::string dir_string, const std::string line_prefix, std::vector<std::string>* file_paths);
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

	std::cout << "Selected input directory:\t" << input_dir_path << std::endl;

    std::vector<std::string> file_paths = open_dir(input_dir_path.c_str());

    display_images(file_paths, rows, cols);
	return 0;
}

// display the images
void
display_images(std::vector<std::string> file_paths, uint rows, uint cols)
{
    std::vector<std::string>::iterator it = file_paths.begin();
    while (true) {
        std::cout << std::endl << "File info:" << std::endl;
        std::cout << ' ' << *it << std::endl;
        try {
            cv::Mat src = cv::imread(*it);
            if (src.empty()) {
                std::cerr << "Cannot open input image: " + *it << std::endl;
                it = file_paths.erase(it);
                continue;
            }
            std::cout << "Image size is:\t\t\t" << src.cols << "x" << src.rows << std::endl;

            // resize the image. this still needs some work...
            uint d_rows = src.rows, d_cols = src.cols;
            if (src.rows > src.cols) {
                float scale = (float) cols / src.cols;
                d_cols = cols;
                d_rows = (uint) (src.rows * scale);
            } else {
                float scale = (float) rows / src.rows;
                d_rows = rows;
                d_cols = (uint) (src.cols * scale);
            }
            std::cout << "Displayed image size is:\t" << d_cols << "x" << d_rows << std::endl;
            cv::namedWindow(*it, cv::WINDOW_NORMAL);
            cv::resizeWindow(*it, d_cols, d_rows);
            cv::imshow(*it, src);

            while (char key_pressed = cv::waitKey(0) & 255) {
                // pressing escape quits out
                if (key_pressed == 27 || key_pressed == 'q') {
                    cv::destroyAllWindows();
                    return;
                }
                if (key_pressed == 'n' || key_pressed == ' ') {
                    cv::destroyWindow(*it);
                    if (++it == file_paths.end()) --it;
                    break;
                }
                if (key_pressed == 'p') {
                    cv::destroyWindow(*it);
                    if (it != file_paths.begin()) --it;
                    break;
                }
            }
        } catch (std::string& str) {
            std::cerr << "Error: " << *it << ": " << str << std::endl;
            return;
        } catch (cv::Exception& e) {
            std::cerr << "Error: " << *it << ": " << e.msg << std::endl;
            return;
        }
    }
}

// parse command line arguments
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

    if (*rows == 0 || *cols == 0) {
        std::cerr << "Dimensions cannot be zero." << std::endl;
        return -1;
    }
    if (*rows > 1080 || *cols > 1920) {
        std::cerr << "Dimensions too large. Max 1920x1080" << std::endl;
        return -1;
    }
    return 0;
}

// Wrapper for open_dir, returns vector of strings (filenames)
std::vector<std::string>
open_dir(const char* dir_string)
{
    std::vector<std::string> file_paths;
    open_dir(dir_string, " \n", &file_paths);
    return file_paths;
}

// recursively open all directories and assemble a list of filenames
void
open_dir(const std::string dir_string, const std::string line_prefix, std::vector<std::string>* file_paths)
{
    DIR *dr;
    try {
        dr = opendir(dir_string.c_str()); //open all directories
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
        // get the relative path
        const std::string rel_path = dir_string + (dir_string.back() == '/' ? "" : std::string("/")) + std::string(drnt->d_name);
        // check if is a directory
        is_dir = is_directory(rel_path.c_str());
        // print files in an organized manner
        std::cout << line_prefix << drnt->d_name;
        if (is_dir) {
            // open next directory
            open_dir(rel_path.c_str(), line_prefix + "\t", file_paths);
        } else {
            (*file_paths).push_back(rel_path);
        }
    }
    closedir(dr); //close the directory
}

// is the given path a directory?
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
