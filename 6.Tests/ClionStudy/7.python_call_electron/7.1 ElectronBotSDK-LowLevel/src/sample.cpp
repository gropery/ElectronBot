#include "electron_low_level.h"
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc/types_c.h>
using namespace cv;

int main()
{
    ElectronLowLevel robot;

    if (robot.Connect())
        printf("Robot connected!\n");
    else
    {
        printf("Connect failed!\n");
        getchar();
        return 0;
    }

    VideoCapture video("happy.mp4");
    Mat frame;
    float jointAngles[6];

    while (true)
    {
        video >> frame;
        if (frame.empty())
            break;

        cv::Mat temp;
        resize(frame, temp, cv::Size(240, 240));
        cvtColor(temp, temp, CV_BGRA2RGB);
        robot.SetImageSrc(temp.data);
        robot.SetJointAngles(0, 0, 0, 0, 0, 0, true);

        robot.Sync();
        robot.GetJointAngles(jointAngles);

        printf("%f,%f,%f,%f,%f,%f\n",
               jointAngles[0], jointAngles[1], jointAngles[2],
               jointAngles[3], jointAngles[4], jointAngles[5]);
    }

    robot.Disconnect();
    printf("File play finished, robot Disconnected!\n");
    printf("Press [Enter] to exit.\n");

    getchar();
    return 0;
}

