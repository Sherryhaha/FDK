//
// Created by sunguoyan on 16/9/22.
//

#include "FDK.h"

//构造一个S_L类型的过滤器
void FDK::S_LFilter(vector<double> &h) {
    for (int i = 0; i < Y_image + 1; i++) {
        if (i == 0) {
            h[Y_image - i] = 1 / 4;
        } else if (i / 2 == 0) {
            h[Y_image - i] = 0;
            h[Y_image + i] = 0;
        } else {
            h[Y_image - i] = -1 / (i * i * pi * pi);
            h[Y_image + i] = -1 / (i * i * pi * pi);
        }
    }
}


//对图像进行滤波操作
void FDK::conv(vector<double> &pic, vector<double> h) {
    //x方向的模糊
    for (int y = 0; y < row; y++) {
        for (int x = 0; x < col; x++) {
            double sum = 0;
            double value = 0;
            for (int i = -Y_image; i < Y_image + 1; i++) {
                if ((x + i) >= 0 && (x + i) < col) {
                    sum += pic[y * Y_image + x] * (h[i + Y_image]);
                    value += h[i + Y_image];
                }
                pic[y * Y_image + x] = sum / value;
            }
        }
    }
}