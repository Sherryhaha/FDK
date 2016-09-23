//
// Created by sunguoyan on 16/9/18.
//
#include "FDK.h"


int main() {
    //从.3dr数据文件中读取100张图片存储到vector<vector<double>> pic数组中
    vector<vector<usi> > pic(MAX, vector<usi>(N * N));
    vector<vector<double>> fpic(MAX, vector<double>(N * N));
    string sourceName = "/Users/sunguoyan/Downloads/picture/FDK/sourceData/23.dat";
    string centreName = "/Users/sunguoyan/Downloads/picture/FDK/parameter/23/circlecentre.txt";
    string curveName = "/Users/sunguoyan/Downloads/picture/FDK/parameter/23/Curve.txt";
    string matrixName = "Users/sunguoyan/Downloads/picture/FDK/parameter/23/matrix123.txt";
    FDK p;
    p.col = 1024;
    p.row = 1024;
    p.X_image = p.row;
    p.Y_image = p.col;
//    double k = 360/100;//投影角度间隔,貌似没有关系。
    double R;
    double w1, w2;
    double a, b, d0 = 1, da = 1, db = 1;
    //滤波
    vector<double> h(2 * N + 1);
    //装半径R与sinp,cosp;
    double sinp, cosp;
    vector<double> D;
    vector<double> sin;
    vector<double> cos;
    //打开Curve.txt文件
    double x, y, z;
    ifstream f;
    f.open(curveName);
    if (!f.is_open()) {
        cout << "can't open the curve.txt!" << endl;
        exit(1);
    }
    p.readImage(pic, fpic, sourceName);
    p.S_LFilter(h);

    //对不同角度获得的图像分别进行加权(加权值的计算：R/(R*R+a*a+b*b)^(1/2) ，a,b是探测器坐标中的横坐标和纵坐标。
    // 其中a=R*(-x*sinp+y*cosp)/(R+xcosp+ysinp)  b=z*R/(R+xcosp+ysinp))
    //其中角度p为射线源中心线与旋转坐标系x轴的夹角。
    //从Circlecentre.txt和Curve.txt文件中分别读取旋转中心坐标和射线源坐标，计算焦距R，并且为了得到sinp与cosp，需要对两个文件中的数据进行计算
    // sinp=((z1-z2)^2+(y1-y2)^2)^0.5/((x1-x2)^2+(y1-y2)^2+(z1-z2)^2)^0.5  cosp = x/((x1-x2)^2+(y1-y2)^2+(z1-z2)^2)^0.5。
    // 滤波.
    //第二个加权值：R^2/(R+xcosp+ysinp)^2
    p.readCircle(centreName);
    for (int k = 0; k < MAX; k++) {
        f >> x >> y >> z;
        R = sqrt((x - p.X) * (x - p.X) + (y - p.Y) * (y - p.Y) + (z - p.Z) * (z - p.Z));
        D.push_back(R);
        sinp = sqrt((z - p.Z) * (z - p.Z) + (y - p.Y) * (y - p.Y)) /
               sqrt((x - p.X) * (x - p.X) + (y - p.Y) * (y - p.Y) + (z - p.Z) * (z - p.Z));
        sin.push_back(sinp);
        cosp = fabs(x - p.X) / sqrt((x - p.X) * (x - p.X) + (y - p.Y) * (y - p.Y) + (z - p.Z) * (z - p.Z));
        cos.push_back(cosp);
        for (int j = 0; j < p.X_image; j++) {
            for (int i = 0; i < p.Y_image; i++) {
                a = (j - p.Y_image / 2 + 0.5) * d0;
                b = (i - p.Y_image / 2 + 0.5) * d0;
                w1 = R / sqrt(R * R + a * a + b * b);
                fpic[k][j * p.Y_image + i] = w1 * fpic[k][j * p.Y_image + i]; //加权
            }
        }
        p.conv(fpic[k], h);   //卷积
    }

    double u;
//对图像进行反投影
    ifstream mf;
    mf.open(matrixName);
    if (!mf.is_open()) {
        cout << "can't open the matrix file!" << endl;
    }
    vector<double> matrix;
    vector<double> re(N * N, 0);
    double aa, bb, a0, an0, bn0, b0, s, v;
    double tmp;
    double xx, yy, zz;
    double d = 1;
    for (int z = 0; z < N; z++) {

        for (int beta = 0; beta < 100; beta++) {
            for (int m = 0; m < 12; m++) {
                mf >> matrix[m];
            }
            for (int j = 0; j < p.X_image; j++) {
                for (int i = 0; i < p.Y_image; i++) {
                    s = (matrix[0] * j + matrix[1] * i + matrix[2] * z + matrix[3]) /
                        (matrix[8] * j + matrix[9] * i + matrix[10] * z + matrix[11]);
                    v = (matrix[4] * j + matrix[5] * i + matrix[6] * z + matrix[7]) /
                        (matrix[8] * j + matrix[9] * i + matrix[10] * z + matrix[11]);
                    xx = (j - s - 0.5) * d;
                    yy = (i - v - 0.5) * d;
                    zz = (z - N / 2 - 0.5) * d;
                    u = D[beta] + xx * cos[beta] + yy * sin[beta];
                    aa = D[beta] * (-xx * sin[beta] + yy * cos[beta]) / u;
                    bb = zz * D[beta] / u;
                    if (aa > 0) {
                        a0 = floor(aa);
                        b0 = floor(bb);
                    } else {
                        a0 = ceil(aa);
                        b0 = ceil(bb);
                    }
                    an0 = aa - a0;
                    bn0 = bb - b0;
                    re[j * p.Y_image + i] += ((1 - an0) * ((1 - bn0) * fpic[beta][a0 * p.Y_image + b0] +
                                                           bn0 * fpic[beta][a0 * p.Y_image + b0 + 1])
                                              + an0 * ((1 - bn0) * fpic[beta][(a0 + 1) * p.Y_image + b0]
                                                       + bn0 * fpic[beta][(a0 + 1) * p.Y_image + b0 + 1])) * (R * R) /
                                             (u * u);


                }

            }
        }
    }

    //从Circlecentre.txt和Curve.txt文件中分别读取旋转中心坐标和射线源坐标，计算焦距，并保存到一个double数组中。
    //对图像进行反投影以及插值操作。

    f.close();
    mf.close();
    return 0;
}

