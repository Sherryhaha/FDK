//
// Created by sunguoyan on 16/9/18.
//

#include "FDK.h"


//从文件中读入max幅图像，并将这些图像存入二维数组中。
void FDK::readImage(vector<vector<usi>> &pic, vector<vector<double>> &fpic, string filename) {
    ifstream image;
    usi tmp;
    int i, j, cycle;
    image.open(filename, ios::binary);
    if (!image.is_open()) {
        cout << "unable to open image!" << endl;
        exit(1);
    }
    //读入数据
    for (cycle = 0; cycle < MAX; cycle++) {
        maxPixel = 0;
        for (j = 0; j < X_image; j++) {
            for (i = 0; i < Y_image; i++) {
                image.read((char *) &tmp, sizeof(unsigned short int));
                if (tmp > maxPixel) {
                    maxPixel = tmp;
                }
                if (tmp < 0) {
                    tmp = 0;
                }
                pic[cycle][j * Y_image + i] = tmp;
                fpic[cycle][j * Y_image + i] = (double) tmp;
            }
        }
    }
    image.close();
}

//从circlecentre.txt文件夹中获得旋转中心的坐标
void FDK::readCircle(string filename) {
    ifstream f;
    f.open(filename, ios::binary);
    if (!f.is_open()) {
        cout << "can't open the circlecentre.txt!" << endl;
        exit(1);
    }
//    f>>X;
//    f>>Y;
//    f>>Z;
    f >> X >> Y >> Z;

    f.close();
}

void FDK::readCurve(double &x, double &y, double &z, string filename) {
    ifstream f;
    f.open(filename);
    if (!f.is_open()) {
        cout << "can't open the curve.txt!" << endl;
        exit(1);
    }
    f >> x >> y >> z;
    f.close();
}

void FDK::readMatrix(vector<double> &v, string filename) {

}

void FDK::writeToFile(vector<double> v, string name) {
    Mat src;
    src.create(X_image, Y_image, CV_8UC1);
    int tem;
    double tmpf;
    for (int j = 0; j < X_image; j++) {
        for (int i = 0; i < Y_image; i++) {
            tmpf = v[j * Y_image + i];
            tmpf = (tmpf / (double) maxPixel) * 255.0;
            tem = (int) (tmpf + 0.5);
            if (tem > 255) {
                tem = 255;
            }
            if (tem < 0) {
                tem = 0;
            }
            src.at<uchar>(j, i) = tem;

        }
    }
    vector<int> param = vector<int>(2);
    param[0] = CV_IMWRITE_JPEG_QUALITY;
    param[1] = 100;
    imwrite(name, src, param);
}


//
//int main() {
//    ifstream image1;
//    usi tmp;
//    double tmpf;
//    int i, j, maxPixel, cycle, X_image, Y_image, Row, Col;
//    char str[10];
//    string str1 = "/Users/sunguoyan/Downloads/yixue/fdk23/test";
//    string str2 = ".jpg";
//    string name;
//
//    Row = 1024;
//    Col = 1024;
//    Y_image = Col;
//    X_image = Row;
//
//
//    vector<int> param = vector<int>(2);
//    param[0] = CV_IMWRITE_JPEG_QUALITY;
//    param[1] = 100;
//
//    image1.open("/Users/sunguoyan/Downloads/picture/FDK/sourceData/23.dat", ios::binary);
//
//    if (!image1.is_open()) {
//        cout << "unable to open image1!" << endl;
//        exit(1);
//    }
//    usi *pic = new usi[Row * Col];
//    usi *pic1 = new usi[Row * Col];
//    double *fpic = new double[Row * Col];
//    double *fpic1 = new double[Row * Col];
//
////    开始读入数据
//    for (cycle = 0; cycle < 100; cycle++) {
//        maxPixel = 0;
//        for (j = 0; j < X_image; j++) {
//            for (i = 0; i < Y_image; i++) {
//                image1.read((char *) &tmp, sizeof(unsigned short int));
//                if (tmp > maxPixel) {
//                    maxPixel = tmp;
//                }
//                if (tmp < 0) {
//                    tmp = 0;
//                }
//
//                pic[j * Y_image + i] = tmp;
//                fpic[j * Y_image + i] = (double) tmp;
//            }
//        }
//        Mat src;
//        src.create(X_image, Y_image, CV_8UC1);
//        int tem;
//        for (j = 0; j < X_image; j++) {
//            for (i = 0; i < Y_image; i++) {
//                tmpf = (double) pic[j * Y_image + i];
//
//                tmpf = (tmpf / (double) maxPixel) * 255.0;
//                tem = (int) (tmpf + 0.5);
//                if (tem > 255) {
//                    tem = 255;
//                }
//                if (tem < 0) {
//                    tem = 0;
//                }
//                src.at<uchar>(j, i) = tem;
//
//
//            }
//        }
//
//        sprintf(str,"%d",cycle);
//
//        name = str1+str+str2;
//        imwrite(name, src, param);
//    }
//
//
//    image1.close();
//    delete[] pic;
//    delete[] pic1;
//
//    delete[] fpic;
//    delete[] fpic1;
//    return 0;
//}
