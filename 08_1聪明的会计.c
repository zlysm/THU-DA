//
// Created by zlysm on 12/14/22.
//

/*
 * 问题描述

N位公民（个人收入分别为x[0],x[1],…,x[N-1]）前去税务局排队缴个人所得税。税务局大厅内摆有M（M<N）张桌子，
 每张桌子前分别坐着一名收税官（税率分别为h[0],h[1],…,h[M-1]），此外还有一位会计负责统计赋税。
 从t=0时刻起，公民们依次排队进入大厅，并经过这M个收税官。在t=0时刻，第0名公民站在第0张桌子前，其余N-1名公民在大厅外等候，
 此时第0名公民按照税率h[0]，交h[0]*x[0]的所得税。因此t=0时，会计记录赋税总额为y[0]=h[0]*x[0]。
 在t=1时刻，第0名公民移动到第1名收税官面前，交h[1]*x[0]的所得税的同时，第1名公民紧随其后，在第0名收税官面前交h[0]*x[1]的所得税。
 因此t=1时，会计记录赋税总额为y[1]=h[1]*x[0]+h[0]*x[1]。以此类推，当t=M+N-2时，最后一位公民在最后一位收税官面前缴税。

问：假设会计只能看到任意t时刻的赋税总额y[t],0<=t<=M+N-2和所有的税率h[0],h[1],…,h[M-1]。
 那么，会计是否能据此计算出每位公民的收入x[0],x[1],…,x[N-1]？如果问题的解存在，请输出此解；如果不存在，请输出数字N。
 * 输入格式
输入共三行。
第一行有两个正整数，分别为N和M，N的范围为[1000,100000]，M的范围为[100,50000]；
第二行有N+M-1个实数，代表序列y[t], 0<=t<=M+N-2，每个实数的范围为[1,2000000]；
第三行有M个实数，代表序列h[t]，0<=t<=M-1，每个实数的范围为[1,10]。

 * 输出格式
输出共一行。若问题的解存在，依次输出x[t]，0<=t<=N-1共N个实数（保留四位小数，实数间用空格分隔）；若问题解不存在，输出N。
注意：正确结果的每个实数都恰好有四位小数（×10000后为整数）。

 * 输入样例
7 3
3 1 16 10 27 14 28 9 20
3 1 4

 * 输出样例
1.0000 0.0000 4.0000 2.0000 3.0000 1.0000 5.0000

 * 提示
1. 解决该问题的一种简单的方式是直接求解线性方程组。需要注意：（1）需要确认解是否存在；（2）求解方程时会出现误差累积的现象，注意利用结果的特性避免。这种方法至少能通过8个样本点。
2. 解决该问题还能采用复杂度更低的方法。具体请参考https://cloud.tsinghua.edu.cn/d/c568ea33933d4ed7b57e/。
 */

/* A is a (N + M - 1) by N matrix:
 * [h_1, 0, 0, ..., ..., 0]              // 1st row
 * [h_2, h_1, 0, ..., ..., 0]
 * [h_3, h_2, h_1, ..., 0]
 * [..., ..., ..., ..., ...]
 * [h_M, ..., h_1, 0, ..., 0]
 * [0, ..., h_M, ..., ..., h_1]          //Nth row
 * [0, ..., ..., H_M, ..., h_k]
 * [..., ..., ..., ..., ...]
 * [0, ..., ..., ..., ..., h_M]     //(N + M - 1)th row
 *
 * x is the result vector, b is y
 * subject to Ax = b
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int main() {
    int people, table;  //N, M
    scanf("%d %d", &people, &table);
    double *total = (double *) malloc((table + people - 1) * sizeof(double));
    double *tax = (double *) malloc(table * sizeof(double));
    double *solve = (double *) malloc(people * sizeof(double));

    for (int i = 0; i < table + people - 1; ++i)
        scanf("%lf", &total[i]);
    for (int i = 0; i < table; ++i)
        scanf("%lf", &tax[i]);

    for (int i = 0; i < people; ++i) {  //lower triangular matrix
        double sum = total[i];
        if (i >= table - 1)
            for (int j = i - 1; j > i - table; --j)
                sum -= solve[j] * tax[i - j];
        else
            for (int j = i - 1; j >= 0; --j)
                sum -= solve[j] * tax[i - j];

        solve[i] = (int) (sum / tax[0] * 10000 + 0.5) / 10000.0;  //round to 4 decimal places
    }

    //check
    for (int i = 0; i < table - 1; ++i) {
        double sum = 0;
        int flag = table - 1 - i;  //number of left tax
        for (int j = people - 1; j >= people - flag; --j)
            sum += solve[j] * tax[people + i - j];
        if (fabs(sum - total[people + i]) > 1e-6) {
            printf("%d", people);
            return 0;
        }
    }

    for (int i = 0; i < people; ++i)
        printf("%.4lf ", solve[i]);

    free(total);
    free(tax);
    free(solve);
    return 0;
}