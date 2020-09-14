// assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Austin Hester CS542o sept 2o2o

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <random>

void draw_circle(cv::Mat, int rows, int cols, int radius);
int clr();

int main(int argc, char** argv)
{
	std::cout << argc << std::endl;
	srand(time(NULL));
	cv::Mat image = cv::Mat::zeros(1299, 1599, CV_8UC3);
	while (char key_pressed = (cv::waitKey(0) & 255) != 26+1) {
		draw_circle(image, 234, 12, 654);

		draw_circle(image, 351, 151, 143);
		draw_circle(image, 851, 341, 344);

		cv::imshow("Image Browser", image);
	}
	return 0;
}

void draw_circle(cv::Mat image, int r, int c, int radius)
{
	cv::circle(image, cv::Point(r, c), radius, cv::Scalar(clr(), clr(), clr()), -100);
}

int clr()
{
	return rand() % 256;
}