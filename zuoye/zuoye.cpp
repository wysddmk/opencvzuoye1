#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace std;
using namespace cv;

int main()
{
    //读取图像
    Mat image = imread("gh.jpg");

    //图像预处理
    //转为HSV空间
    Mat hsv;
    cvtColor(image, hsv, COLOR_BGR2HSV);

    //设置红色，蓝色上下限
    Scalar lower_red(140, 200, 200);
    Scalar upper_red(180, 255, 255);

    Scalar lower_blue(101, 208, 206);
    Scalar upper_blue(140, 255, 255);

    //转为二值图像
    Mat red_mask, blue_mask;
    inRange(hsv, lower_red, upper_red, red_mask);
    inRange(hsv, lower_blue, upper_blue, blue_mask);

    //输出合并图像
    Mat combined = red_mask + blue_mask;

    //图像腐蚀填补缝隙
    Mat m3 = getStructuringElement(0, Size(5, 5));
    dilate(combined, combined, m3);

    //边缘检测
    Mat edges;
    Canny(combined, edges, 50, 150);

    //高斯模糊
    GaussianBlur(edges, edges, Size(3, 3), 0);

    //轮廓检测
    vector<vector<Point>> contours;
    findContours(edges, contours, RETR_EXTERNAL, CHAIN_APPROX_NONE);

    for (unsigned long i = 0; i < contours.size(); i++)
    {
        //
        RotatedRect ellipse = fitEllipse(contours[i]);

        //
        Point center = ellipse.center;
        cout << "Ellipse center:(" << center.x << "," << center.y << ")" << endl;
        int r = 20;
        circle(image, center, r, Scalar(0, 0, 225), 2);
    }

    //
    namedWindow("result", WINDOW_AUTOSIZE);
    imshow("result", image);

    waitKey(0);
    return 0;
}