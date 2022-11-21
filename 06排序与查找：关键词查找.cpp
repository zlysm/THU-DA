//
// Created by zlysm on 11/21/22.
//

/*
 * 问题描述
小明要整理自己记录在笔记上的论文。假设每个论文有若干个关键词，希望你能够帮忙设计一个算法统计出每个关键词对应的论文数目。
但是小明有个坏习惯，抄写关键词的时候，可能会随机多打或者少打一个字母“l”（不区分大小写），且每个关键词至多增添一次错误。
 由于错误可能已经多次叠加了，比如："cnn->cLnn->clnnl->cnnl"，所以小明希望信息录入查询系统后，容错地统计每个关键词出现的总次数。具体容错性的定义如下：
- 如果一个关键词x只是在某个位置上比另外一个关键词y多了一个“l”或者少了一个“l”，那么称它们是关联的：x~y；
- 如果x~y，y~z，则x,y,z三者视为同一个关键词。在等价关系~的意义下，相同的关键词构成关联集合；
- 同一个关联集合内的任意一个关键词x，总能找到另外一个关键词y和它是关联的；
- 分好关联集合之后，一个关联集合内的关键词和其它集合内的任意关键词都不关联。

 * 输入格式
第1行：正整数 n (0 < n < 2000)，代表需要录入的论文篇数；
第2行~第n+1行：每一行代表一个论文的关键词: "关键词数目k    关键词1    关键词2    ...    关键词k"  ，即该论文的关键词数目，及它的关键词字符串；

注：
- 输入数据的行间内容用空格分隔；
- 每一个关键词（字符串）的内部没有空格和特殊字符，要求统计时不区分字母大小写；
- 每个字符串长度不超过100。

 * 输出格式
只有一个整数：每个关联集合内关键词出现的总次数的最大值。

 * 输入样例
3
2 DataFree graphl
3 clnnl  Graph  DataFree
3 cnnl Graph tree

 * 输出样例
3

// 最大的关联集合是{Graph * 2, graphl}，因此关键词出现的总次数的最大值为3次。

提示
本题主要考察哈希表空间换时间，实现每个关键词查询次数时时间复杂度为O(1)。其中，最后三个样例的论文数目均超过1000.
 */

#include <iostream>

#define HASH_SIZE 1000000

struct keyWord {
    std::string word;
    int count;

    keyWord() : count(0) {}

    explicit keyWord(std::string w) : word(w), count(1) {}
};

int hash(const std::string &word) {
    int seed = 131;
    int hash = 0;
    for (auto &i: word)
        hash = hash * seed + i;
    return (hash & 0x7FFFFFFF) % HASH_SIZE;
}

void cutL(std::string &word) {
    for (int i = 0; i < word.length(); ++i) {
        if (word[i] >= 'A' && word[i] <= 'Z')
            word[i] += 'a' - 'A';
        if (word[i] == 'l') {
            word.erase(i, 1);
            i--;
        }
    }
}

int main() {
    std::ios::sync_with_stdio(false);

    int papers;
    int maxCount = 0;
    std::cin >> papers;
    auto *hashTable = new keyWord[HASH_SIZE];

    for (int i = 0; i < papers; ++i) {
        int words;
        std::cin >> words;
        for (int j = 0; j < words; ++j) {
            std::string word;
            std::cin >> word;
            cutL(word);
            int index = hash(word);
            if (hashTable[index].count == 0) {
                hashTable[index].word = word;
                ++hashTable[index].count;
            } else if (hashTable[index].word == word) {
                ++hashTable[index].count;
                maxCount = std::max(maxCount, hashTable[index].count);
            } else {
                while (hashTable[index].count != 0 && hashTable[index].word != word)
                    index = (index + 1) % HASH_SIZE;
                hashTable[index].word = word;
                ++hashTable[index].count;
                maxCount = std::max(maxCount, hashTable[index].count);
            }
        }
    }

    std::cout << maxCount;

    return 0;
}