#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include "Lafin.h"

using namespace cv;
using namespace std;

int main()
{
    // 读取3张图
    Mat img = imread("E:\\code_git\\QTproject\\FaceInpaingApp\\FaceInpaintingProcess\\build\\Release\\195579.jpg");
    Mat landmark = imread("E:\\code_git\\QTproject\\FaceInpaingApp\\FaceInpaintingProcess\\build\\Release\\195579.png", IMREAD_GRAYSCALE);
    Mat mask = imread("E:\\code_git\\QTproject\\FaceInpaingApp\\FaceInpaintingProcess\\build\\Release\\11257.png", IMREAD_GRAYSCALE);

    // 打包进 vector
    vector<Mat> inputs;
    inputs.push_back(img);
    inputs.push_back(landmark);
    inputs.push_back(mask);

    // 输出图像
    Mat result;

    // 调用动态库
    uint32_t ret = LaFin_Inpaint(inputs, result);

    // 判断结果
    if (ret == LAFIN_OK) {
        cout << "success" << endl;
        imshow("Result", result);
        waitKey(0);
    }
    else {
        cout << "faild, code" << ret << endl;
    }

    return 0;
}