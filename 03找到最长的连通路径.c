//
// Created by zlysm on 10/8/22.
//

/*
 * 问题描述
给定一个矩阵，元素为0或者1。如果两个“1”元素连通，则他们通过横向、纵向或者对角线方向连接。相互连接的“1”元素构成一个子区域，一个矩阵中可能存在多个子区域。
给定一个矩阵，请找出其中最大的连接子区域，包括元素的数目和这些元素的位置。
输入格式
首行包含一个正整数N，为输入矩阵的维度。
随后一行的输入为，矩阵元素(0或者1)按照逐行的方式排列，元素之间用空格分隔。
输出格式
第一行为最大连接子区域中1的个数。
随后的每一行为最大连接子区域中1的坐标(x,y)，x为行数，y为列数，矩阵左上角为(0,0)，坐标之间按照矩阵中位置从上到下、从左到右的顺序排列（两个坐标之间不加任何分隔字符）。例如样例1中的矩阵为
有几个最大连接子区域则有几行坐标，不同的子区域也根据每行的第一个元素按照从上到下、从左到右的顺序逐行输出。
若矩阵为全零，则直接输出-1。
 * 输入样例
// 样例1
5
1 1 0 0 0 0 1 1 0 1 0 0 1 0 1 1 0 0 0 1 0 1 0 1 1

// 样例2
3
0 0 0 0 0 0 0 0 0
 * 输出样例
// 样例1，由于区域一的第一个元素(0,0)比区域二的第一个元素(1,4)行数更小，因此优先输出区域一的坐标
5
(0,0)(0,1)(1,1)(1,2)(2,2)
(1,4)(2,4)(3,4)(4,3)(4,4)

// 样例2
-1
*/

#include <stdio.h>
#include <malloc.h>

int count = 0;
int coord[1000];

void findLongestPath(char **path, int dim, int xCoord, int yCoord);

void bubbleSort(int *arr, int len);

int main() {
    int dim;
    int max = 0;
    int max_count = 1;
    scanf("%d", &dim);

    char **path = (char **) malloc(sizeof(char *) * dim);
    char **path_cpy = (char **) malloc(sizeof(char *) * dim);
    for (int i = 0; i < dim; ++i) {
        path[i] = (char *) malloc(sizeof(char) * dim);
        path_cpy[i] = (char *) malloc(sizeof(char) * dim);
    }

    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j) {
            scanf("%d", &path[i][j]);
            path_cpy[i][j] = path[i][j];
        }

    int cnt = 0;  //判断全为0
    for (int i = 0; i < dim; ++i)
        for (int j = 0; j < dim; ++j)
            if (path[i][j] == 1)
                ++cnt;
    if (cnt == 0) {
        printf("-1");
        return 0;
    }

    for (int i = 0; i < dim; i++)  //找最大及次数
        for (int j = 0; j < dim; j++) {
            findLongestPath(path_cpy, dim, i, j);
            if (max == count && count != 0) ++max_count;
            max = max > count ? max : count;
            count = 0;
        }
    printf("%d\n", max);

    count = 0;
    int flag = 0;
    for (int i = 0; i < dim; i++) {  //再扫一边输出
        for (int j = 0; j < dim; j++) {
            findLongestPath(path, dim, i, j);
            if (count == max) {
                ++flag;
                bubbleSort(coord, max);
                for (int k = 0; k < max; ++k)
                    printf("(%d,%d)", coord[k] / 1000, coord[k] % 1000);
                printf("\n");
            }
            count = 0;
        }
        if (flag == max_count) break;
    }

    return 0;
}

void findLongestPath(char **path, int dim, int xCoord, int yCoord) {
    if (xCoord < 0 || xCoord >= dim || yCoord < 0 || yCoord >= dim) return;
    if (!path[xCoord][yCoord]) return;
    coord[count++] = xCoord * 1000 + yCoord;  //记录坐标
    path[xCoord][yCoord] = 0;  //已经访问
    findLongestPath(path, dim, xCoord - 1, yCoord - 1);
    findLongestPath(path, dim, xCoord - 1, yCoord);
    findLongestPath(path, dim, xCoord - 1, yCoord + 1);
    findLongestPath(path, dim, xCoord, yCoord - 1);
    findLongestPath(path, dim, xCoord, yCoord + 1);
    findLongestPath(path, dim, xCoord + 1, yCoord - 1);
    findLongestPath(path, dim, xCoord + 1, yCoord);
    findLongestPath(path, dim, xCoord + 1, yCoord + 1);
}

void bubbleSort(int *arr, int len) {
    int temp;
    for (int i = 0; i < len; ++i)
        for (int j = 0; j < len - 1; ++j)
            if (arr[j] > arr[j + 1]) {
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
}