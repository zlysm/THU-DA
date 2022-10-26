//
// Created by zlysm on 10/26/22.
//

/*
 * 问题描述
某数据库为提升搜索效率，对某一整型字段构建二叉搜索树(BST)。每个结点包含两个数据信息：
 1）结点的数据；
 2）该结点子树的元素数目。
 为了压缩搜索树规模，该数据库为每个结点增加一个字段，该字段用于存储中序遍历时在访问该结点之前访问的结点数据。
 在该改进下，被存储的结点如果为叶子结点，该结点在新树中将被删除，以提高存储效率。如果一个叶子结点无中序后继，则无需删除。

给定该BST的先序遍历（第二个字段未给出），请编写程序，输出经过压缩后新BST的先序遍历结果。

输入格式
 * 输入共两行：
第一行为一个int数据n，表示该BST的总结点个数。1<=n<=100000；
第二行为n个int数据，为该BST的先序遍历结果（保证顺序是正确的，并保证数据两两不同）。 每个数据的范围：0<=x<=1*107。
输出格式
输出共一行：
第一行为新BST的先序遍历结果，依序输出结点数据以及保存的其他结点数据（若没有则输出字符-）
输入样例

7
6 4 2 5 9 7 8

输出样例

6 5 4 2 9 8 7 -

提示
1. 需要从先序遍历构建二叉树，并对二叉树进行压缩。

2. 【具体存储规则】题目描述等价于如下表述：对任意结点v：
 (1) 如果其有左子树，则存储其左子树的最大值，保留该结点；
 （2）如果没有左子树但有右子树，则不存储任何值，保留该结点；
 （3）否则（为叶子结点时），如果有中序后继，则可以删除，反之则不存储任何值，保留该结点。
【举例】一些结点可能并未被删除，但你仍需将其记录在中序遍历中下一个结点的第二个字段上（对应规则1）。
 下面的例子中，尽管左图中结点(3,1)没有被删除，但是为了充分利用已经开辟的存储空间，其父结点仍需存储该节点的值。

3. 注意不要输出多余空格。

4. 个别输入样例需要在压缩二叉树时具有O(1)的空间复杂度，请搜索相关算法实现
 */

#include <cstdio>

#define HasLeft(x) (x->left)
#define HasRightAndNoLeft(x) (x->right && !HasLeft(x))

struct NODE {
    int data;
    int inorderVal;
    NODE *left;
    NODE *right;

    explicit NODE(int value) : data(value), inorderVal(-1), left(nullptr), right(nullptr) {}

    NODE() : data(0), inorderVal(-1), left(nullptr), right(nullptr) {}
};

class BST {
private:
    NODE *inSucc(NODE *node) const;

public:
    NODE *root;

    BST() { root = nullptr; }

    void insert(int value);

    void compress(NODE *node);

    void print(NODE *node);
};

void BST::insert(int value) {
    NODE *node = new NODE(value);
    if (!root) {
        root = node;
        return;
    }
    for (NODE *p = root;;)
        if (value < p->data) {
            if (p->left) {
                p = p->left;
            } else {
                p->left = node;
                break;
            }
        } else {
            if (p->right) {
                p = p->right;
            } else {
                p->right = node;
                break;
            }
        }
}

NODE *BST::inSucc(NODE *node) const {
    NODE *cur = root;
    NODE *res = nullptr;

    while (cur)
        if (cur->data <= node->data) {
            cur = cur->right;
        } else {
            if (!res || res->data > cur->data) res = cur;
            cur = cur->left;
        }
    return res;
}

void BST::compress(NODE *node) {
    if (!node) return;
    if (HasLeft(node)) {
        NODE *p = node->left;
        while (p->right) p = p->right;  //左子树最大值
        node->inorderVal = p->data;
    } else if (HasRightAndNoLeft(node)) {
        node->inorderVal = -1;
    } else {
        if (!inSucc(node)) node->inorderVal = -1;
        else node->inorderVal = -2;
    }
    compress(node->left);
    compress(node->right);
}

void BST::print(NODE *node) {
    if (node) {
        if (node->inorderVal != -2) {  //不打印除最后一个外的叶子节点（已删除）
            printf("%d ", node->data);
            if (node->inorderVal == -1) printf("- ");  //未存储值
            else printf("%d ", node->inorderVal);
        }
        print(node->left);
        print(node->right);
    }
}

int main() {
    int n;
    BST bst;
    scanf("%d", &n);
    for (int i = 0; i < n; ++i) {
        int value;
        scanf("%d", &value);
        bst.insert(value);
    }
    bst.compress(bst.root);
    bst.print(bst.root);

    return 0;
}