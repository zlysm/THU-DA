//
// Created by zlysm on 12/22/2022.
//

//用贪心混了90分

/*
 * 问题描述
孙鹏同学想要控制自己的体重，为此他决定制定一个健康膳食计划，其目标是在保证身体必须的营养的前提下尽量少吃东西，
 由于每天能买到的食物种类众多，每种食物含有的营养物质又各不相同，所以他想要利用计算机帮助自己规划每日健康食谱，
 请你用所学《数据与算法》的知识帮助孙鹏同学制定他的健康食谱。

 * 输入格式
输入由三部分组成：
第1行前两个整数分别表示当日需要的营养物质种类数 N 和当日可以买到的食物种类数 M，其中 1<=N<=16，
 16种营养物质分别用编号0~15表示；1<=M<=216=65536，后 M 个整数分别表示当日能买到的 M 种食物各自对应的营养物质种类的数量；
第2行为 N 个整数，表示当日需要的营养物质列表，按数字大小升序排列；
第3~M+2行为每种食物自己含有的营养物质列表，按数字大小升序排列，且食物自身的序号按行数从上到下依次编为0~M - 1号，
 需要注意，食物中可能含有当日不需要的营养物质，对于此类营养物质可以摄入也可以不摄入。

 * 输出格式
输出为1行，为当日计划吃的食物，用空格分隔，且按数字大小升序排列，
 如果没有合适的食谱，则输出-1，题目数据保证如果有解，则解是唯一的。

 * 输入样例
3 3 1 1 2
1 4 6
1
4
4 6

 * 输出样例
0 2

 * 提示
采用动态规划方法求解，可以考虑采用位运算提升程序效率。
*/

#include <cstdio>
//NUTRIENT: 16bit, stand for 16 kinds of nutrient
using NUTRIENT = unsigned short;

struct Food {
    int kind;
    unsigned short id;
    NUTRIENT nutrients;
};

void sort(Food *foods, int n) {
    for (int i = 0; i < n; i++)
        for (int j = i + 1; j < n; j++)
            if (foods[i].kind < foods[j].kind) {
                Food tmp = foods[i];
                foods[i] = foods[j];
                foods[j] = tmp;
            }
}

int main() {
    int N, M;
    NUTRIENT check = 0;
    scanf("%d %d", &N, &M);
    Food *foods = new Food[M];
    for (int i = 0; i < M; ++i) {
        scanf("%d", &foods[i].kind);
        foods[i].id = i;
    }

    NUTRIENT need = 0;
    bool need_hash[16] = {false};
    for (int i = 0; i < N; ++i) {
        NUTRIENT tmp;
        scanf("%hu", &tmp);
        need |= 1 << tmp;
        need_hash[tmp] = true;
    }

    for (int i = 0; i < M; ++i) {
        int count = 0;
        for (int j = 0; j < foods[i].kind; ++j) {
            NUTRIENT tmp;
            scanf("%hu", &tmp);
            if (need_hash[tmp]) {
                foods[i].nutrients |= 1 << tmp;
                ++count;
            }
        }
        check |= foods[i].nutrients;
        foods[i].kind = count;
    }

    if (need ^ check) {
        printf("-1");
        return 0;
    }

    sort(foods, M);  //greedy
    bool *output = new bool[M]{false};
    for (int i = 0; i < M; ++i) {
        for (int j = i; j < M; ++j) {
            if (!foods[j].kind) continue;
            if (foods[j].nutrients & need) {
                output[foods[j].id] = true;
                need ^= foods[j].nutrients;
                foods[j].kind = 0;
                break;
            }
        }
        if (!need) break;
    }

    for (int i = 0; i < M; ++i)
        if (output[i])
            printf("%d ", i);

    delete[] foods;
    delete[] output;
    return 0;
}