#include <iostream>

#include "opencv2/opencv.hpp"

int main()
{
    std::cout << "OpenCV config sample." << std::endl;
    std::cout<< cv::getBuildInformation() <<std::endl;
    return 0;
}

