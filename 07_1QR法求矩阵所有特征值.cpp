//
// Created by zlysm on 12/1/22.
//

/*
 * 问题描述
QR法是计算一般矩阵全部特征值以及特征向量的一种重要方法。现给定一N阶方阵A，要求用QR法求出A的所有特征值并输出

 * 输入格式
输入总共N+1行：
第1行为正整数N，代表矩阵阶数
第2行到第N+1行每行分别有N个浮点数，代表A的元素

 * 输出格式
假设A的矩阵特征值均为实数，要求从小到大输出A的N个特征值（保留两位小数）

 * 输入样例
4
-2  10   10   0
0      3      5   0
0      0      4    0
9      7      5    1

 * 输出样例
-2.00
1.00
3.00
4.00

 * 提示
1.矩阵A给定，求其所有特征值的基本QR算法如下：
    初始化：令A0 = A
    对于k = 0, 1, 2, 3, ...，重复以下步骤：
        a.对Ak做QR分解，得到：Ak = Qk*Rk
        b.计算新的矩阵Ak+1= Rk*Qk
   可以证明，若A满足一定的条件，当k足够大以后，Ak基本收敛到上三角矩阵，其对角线元素为A的特征值。

2. 数据范围：前9个测试点矩阵维度不超过30，直接对原矩阵用上述QR法迭代就可以在规定时间内收敛到足够的精度；
 最后一个测试点矩阵维度为60，直接用QR法迭代复杂度较高（每次迭代复杂度为O(N3)），推荐先将原矩阵正交相似变换为Hessenberg矩阵：
 即找到正交矩阵Q，使得矩阵B = QTAQ为一个Hessenberg矩阵，则矩阵A与B具有完全相同的特征值。 之后再对矩阵B使用QR迭代的方法求特征值，
 可以证明，之后每次得到的矩阵Bk均为Hessenberg矩阵，此时可以用Givens变换的方法对矩阵进行QR分解，每次迭代的复杂度减小为O(N2)。

3.Hessenberg矩阵的靠下的次对角线下方的元素均为0，具有以下的形式：
将A相似变换为Hessenberg矩阵的具体步骤为：
 首先通过Householder变换或者Givens变换，找到正交变换阵Q1'，使得变换后矩阵Q1'A的第1列的第3~N个元素均为0，且第1行的元素保持不变；
 计算B1'=Q1'AQ1'T，可以证明，B1'的第1列的第3~N个元素也均为0；
 对于B1'，类似地，利用Householder变换或者Givens变换，找到正交变换阵Q2'，使得变换后矩阵Q2'B1'的第2列的第4~N个元素均为0，且第1-2行的元素保持不变；
 计算B2'=Q2'AQ2'T，可以证明，B2'的第二列的第4~N个元素也均为0；
 重复上述步骤，直到将B的第N-2列的第N个元素化为0，此时得到的BN-2’就为相似于A的Hessenberg矩阵。

4.Givens变换可以将向量的两个分量按照一定角度旋转，用于将矩阵或者向量的某一个或者某几个分量化为0，具体的做法请同学们自行查找相关资料。

5.Householder变换和Givens变换均可以看成对矩阵的行向量或者列向量进行操作，不需要写出具体的变换矩阵。

6.我们测试点给的矩阵的特征值均为非零实数，但是不一定是对称矩阵。此外，请同学们注意输出精度的要求。
 */

#include <cstdio>
#include <cmath>

#define TRY_TIMES 2000

void QR(double **matrix, int dim);

void bubbleSort(double *array, int dim);

int main() {
    int dim;
    scanf("%d", &dim);
    auto **matrix = new double *[dim];
    auto *eigenValues = new double[dim];
    for (int i = 0; i < dim; ++i) {
        matrix[i] = new double[dim];
        for (int j = 0; j < dim; ++j)
            scanf("%lf", &matrix[i][j]);
    }
    int times = TRY_TIMES;
    while (times--)
        QR(matrix, dim);
    for (int i = 0; i < dim; ++i)
        eigenValues[i] = matrix[i][i];
    bubbleSort(eigenValues, dim);
    for (int i = 0; i < dim; ++i)
        printf("%.2lf\n", eigenValues[i]);

    return 0;
}

void QR(double **matrix, int dim) {
    auto **Q = new double *[dim];
    auto **R = new double *[dim];
    for (int i = 0; i < dim; ++i) {
        Q[i] = new double[dim];
        R[i] = new double[dim];
        for (int j = 0; j < dim; ++j) {
            Q[i][j] = 0;
            R[i][j] = 0;
        }
    }

    for (int i = 0; i < dim; ++i) {  //i-th column
        double norm = 0;
        for (int j = 0; j < dim; ++j)
            norm += matrix[j][i] * matrix[j][i];
        norm = sqrt(norm);  //norm: i-th column of matrix

        for (int j = 0; j < dim; ++j)
            Q[j][i] = matrix[j][i] / norm;

        for (int j = i; j < dim; ++j) {
            double sum = 0;
            for (int k = 0; k < dim; ++k)
                sum += Q[k][i] * matrix[k][j];
            R[i][j] = sum;  //R = Q^T * matrix
        }
        //subtract the projection of matrix on Q
        for (int j = i; j < dim; ++j)
            for (int k = 0; k < dim; ++k)
                matrix[k][j] -= R[i][j] * Q[k][i];  //column j of matrix
    }

    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j) {  //A_k+1 = R_k * Q_k
            matrix[i][j] = 0;
            for (int k = 0; k < dim; ++k)
                matrix[i][j] += R[i][k] * Q[k][j];
        }

    for (int i = 0; i < dim; ++i) {
        delete[] Q[i];
        delete[] R[i];
    }
    delete[] Q;
    delete[] R;
}

void bubbleSort(double *array, int dim) {
    for (int i = 0; i < dim; ++i)
        for (int j = dim - 1; j > i; --j)
            if (array[j] < array[j - 1]) {
                double temp = array[j];
                array[j] = array[j - 1];
                array[j - 1] = temp;
            }
}