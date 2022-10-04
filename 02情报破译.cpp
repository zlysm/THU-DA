//
// Created by zlysm on 10/4/22.
//

/*
 * 问题描述
现截获一段情报，该类型情报由两个单向链表（长度分别为M，N）组成，需要找到两个链表尾部重合的部分（长度为S）才能提取到情报中的信息，
请你写一段程序，破译这份情报（找到两个链表重合的部分，输出重合部分以及重合部分在两个链表中的位置）。
 * 输入格式
输入共M+N+1行；
第0行为两个整数，代表两个链表的长度M，N；
1～M行为第一个链表每个结点的信息；
M+1～M+N行为第二个链表每个结点的信息；
每个结点的信息包含：
1. 该结点的值（为长度小于等于4的字符串，字符只包含26个字母），
2. 该结点指向的下一个结点出现在输入的第几行，对于尾结点，这个值为-1（这是一个相对值/逻辑值，比如对于1～M行中，
3代表所有M+N+1行中的第4行，而对于M+1～M+N行中，3代表所有M+N+1行中的第M+4行），1，2中间用空格分割；
注：每个链表的长度不超过100000。
 * 输出格式
输出可能包含1或2行：
当两个链表有重合部分时，第1行输出重合起始位置在较短的链表中的位置索引（从0开始），第2行输出重合部分各结点的值，用空格分割；
当两个链表无重合部分时，直接输出-1，代表这条情报为干扰情报。
 * 输入样例
5 6
a 3
zqp -1
te 4
fj 1
oz 0
zqp -1
rvi 5
fj 0
v 1
b 2
o 4
 * 输出样例
3
fj zqp
 */

#include <cstdio>
#include <cstring>

struct ListNode {
    char _val[5];
    ListNode *succ;
};

class List {
private:
    static void insertAsSucc(ListNode *p, ListNode *s) {
        s->succ = p->succ;
        p->succ = s;
    }

    void insertAsFirst(ListNode *s) const {
        insertAsSucc(header, s);
    }

public:
    ListNode *header;

    List() {
        header = new ListNode;
        header->succ = nullptr;
    }

    void generate(ListNode *node, int len, bool *visited, int *nextLoc);

    static void check(List &list_m, List &list_n, int len_m, int len_n);
};

void List::generate(ListNode *node, int len, bool *visited, int *nextLoc) {
    int count = 0;
    int flag;
    for (int i = 0; i < len; ++i)  //是否访问完全，顺便定位到最后一个未被访问节点
        if (!visited[i]) {
            ++count;
            flag = i;
        }
    if (count == 0)
        return;

    insertAsFirst(node + flag);
    visited[flag] = true;

    ListNode *p = header->succ;
    for (int i = 0; i < len; ++i) {  //循环插入，直到尾部或上一次的头部
        if (nextLoc[flag] != -1 && !visited[nextLoc[flag]]) {
            insertAsSucc(p, node + nextLoc[flag]);
            flag = nextLoc[flag];
            visited[flag] = true;
            p = p->succ;
        } else break;
    }
    generate(node, len, visited, nextLoc);
}

void List::check(List &list_m, List &list_n, int len_m, int len_n) {  //m更长
    ListNode *p = list_m.header;
    ListNode *q = list_n.header;
    for (int i = 0; i < len_m; ++i) {  //移动到末尾
        p = p->succ;
        if (i < len_n) q = q->succ;
    }
    if (strcmp(p->_val, q->_val) != 0) {
        printf("-1");
        return;
    }
    int flag = len_m - len_n;
    int count;
    for (p = list_m.header->succ; 0 < flag--; p = p->succ);  //使两个指针对齐
    for (q = list_n.header->succ, flag = 0; flag < len_n; q = q->succ, p = p->succ, ++flag) {
        count = 0;
        for (ListNode *m = p, *n = q; m && n; m = m->succ, n = n->succ) {
            if (strcmp(m->_val, n->_val) != 0) break;
            else ++count;
        }
        if (count == len_n - flag) {
            printf("%d\n", len_n - count);
            break;
        }
    }
    flag = len_n - count;
    for (q = list_n.header->succ; 0 < flag--; q = q->succ);
    for (; q; q = q->succ)
        printf("%s ", q->_val);
}

int main() {
    List list_m;
    List list_n;
    int len_m;
    int len_n;
    scanf("%d %d", &len_m, &len_n);
    int len = len_m > len_n ? len_m : len_n;

    bool visited[len];
    int nextLoc[len];

    //输入m
    memset(visited, false, sizeof(visited));
    auto *node_m = new ListNode[len_m];

    for (int i = 0; i < len_m; ++i) {
        scanf("%s %d", node_m[i]._val, &nextLoc[i]);
        node_m[i].succ = nullptr;
    }
    list_m.generate(node_m, len_m, visited, nextLoc);

    //输入n
    memset(visited, false, sizeof(visited));
    auto *node_n = new ListNode[len_n];

    for (int i = 0; i < len_n; ++i) {
        scanf("%s %d", node_n[i]._val, &nextLoc[i]);
        node_n[i].succ = nullptr;
    }
    list_n.generate(node_n, len_n, visited, nextLoc);

    if (len_m > len_n)
        list_m.check(list_m, list_n, len_m, len_n);
    else
        list_n.check(list_n, list_m, len_n, len_m);

    delete[] node_m;
    delete[] node_n;

    return 0;
}