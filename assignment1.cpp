// assignment1.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Austin Hester CS542o sept 2o2o

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <random>


int main(int argc, char* argv)
{
	std::cout << argc << std::endl;
	srand(time(NULL));
	cv::Mat image = cv::Mat::zeros(299, 599, CV_8UC3);
	while (char key_pressed = (cv::waitKey(0) & 255) != 26+1) {
		cv::circle(image, cv::Point(250, 150), 130, cv::Scalar(rand() % 256, rand() % 256, rand() % 256), -100);
		cv::circle(image, cv::Point(350, 150), 103, cv::Scalar(rand() % 256, rand() % 256, rand() % 256), -100);
		cv::imshow("Display Window", image);
	}
	return 0;
}
