//
// Created by sunguoyan on 16/9/18.
//

#ifndef FDK_FDK_H
#define FDK_FDK_H

#include <cv.h>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <math.h>
#include <time.h>
#include <fstream>
#include <string>
#include <sstream>
#include <iomanip>
#include <vector>

using namespace cv;
using namespace std;

typedef double ty;
typedef uchar tc;
typedef unsigned short int usi;
const double pi = 3.14159;
const int MAX = 100;      //总共处理100张不同角度的图片
const int N = 1024;

class FDK {
public:
    int X_image, Y_image;
    int row, col, maxPixel;
    double X, Y, Z;

    //设置旋转中心的坐标
    void S_LFilter(vector<double> &h);

    void conv(vector<double> &pic, vector<double> h);

    void readImage(vector<vector<usi> > &pic, vector<vector<double> > &fpic, string filename);

    void readCircle(string filename);

    void readCurve(double &x, double &y, double &z, string filename);

    void readMatrix(vector<double> &v, string filename);

    void writeToFile(vector<double> v, string name);
};

#endif //FDK_FDK_H
