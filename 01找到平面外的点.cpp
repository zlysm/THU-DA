//
// Created by zlysm on 10/4/22.
//

/*
 * 问题描述
在3维空间中给定N（5≤N<10000000)个点，有N-1个点在一个平面上，请找出不在平面上的那个点。
 * 输入格式
输入共N+1行；
第0行为1个整数N，表示后续输入数据的行数；
第1~N行为输入数据，每行数据由3个浮点数构成，表示N个点的(x,y,z)笛卡尔坐标。N个点中，有且仅有1个点不在平面上。
 * 输出格式
不在平面上的那个点的序号，范围为[0, N-1]。
 * 输入样例
6
2.769437 -0.204966 1.111109
0.318765 -1.349887 -1.401742
3.578397 0.714743 1.050345
-0.433592 0.725404 0.104038
-1.307688 3.034923 1.771004
0.342624 -0.063055 -0.351326
 * 输出样例
0
 */

#include <iostream>

bool isInSamePlane(const float *num1, const float *num2, const float *num3, const float *num4) {
    float det = (num2[0] - num1[0]) * (num3[1] - num1[1]) * (num4[2] - num1[2]) +
                (num2[1] - num1[1]) * (num3[2] - num1[2]) * (num4[0] - num1[0]) +
                (num2[2] - num1[2]) * (num3[0] - num1[0]) * (num4[1] - num1[1]) -
                (num2[2] - num1[2]) * (num3[1] - num1[1]) * (num4[0] - num1[0]) -
                (num2[1] - num1[1]) * (num3[0] - num1[0]) * (num4[2] - num1[2]) -
                (num2[0] - num1[0]) * (num3[2] - num1[2]) * (num4[1] - num1[1]);
    return (det < 1e-1 && det > -1e-1);
}

int main() {
    std::ios::sync_with_stdio(false);
    int n;
    std::cin >> n;

    float coord[4][3];

    for (int i = 0; i < n; ++i)
        if (i < 4)
            std::cin >> coord[i][0] >> coord[i][1] >> coord[i][2];
        else {
            if (isInSamePlane(coord[0], coord[1], coord[2], coord[3])) {
                std::cin >> coord[3][0] >> coord[3][1] >> coord[3][2];
                if (!isInSamePlane(coord[0], coord[1], coord[2], coord[3])) {
                    std::cout << i << std::endl;
                    break;
                }
            } else {
                float temp[3];
                std::cin >> temp[0] >> temp[1] >> temp[2];
                for (int j = 0; j < 4; ++j)
                    if (isInSamePlane(coord[j % 4], coord[(j + 1) % 4], coord[(j + 2) % 4], temp)) {
                        std::cout << (j + 3) % 4 << std::endl;
                        break;
                    }
                break;
            }
        }

    return 0;
}