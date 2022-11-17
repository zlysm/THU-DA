//
// Created by zlysm on 11/18/22.
//

/*
 * 问题描述
最短路径问题考虑的是带权有向图中从源点出发到达汇点时的最小的路径权。现在我们研究一种双向的最短路径问题，假设有两条分别以源点S1和源点S2为起点的路径，如果它们的终点是同一汇点，则称这两条路径“会合”了，此时他们分别有各自的路径权，将两个路径权的和称为总路径权。以下图为例，选取S1=1，S2=4，此时存在可以“会合”的两条路径，例如1-3-5和4-2-5，他们的汇点均为5，总路径权为18。在本题中，我们允许源点和汇点为同一个点，例如选取S1=1，S2=5时，1和5-1也是两条“会合”了的路径，他们的汇点均为1，总路径权为10。而如果选取S1=1，S2=6，则不存在可以“会合”的两条路径。
现给定一张顶点数为N的带权有向图，将顶点标记为1,...,N，假设源点S1=1，请判断源点S2=2,…,N时是否存在可以“会合”的两条路径，如果存在，请给出最小的总路径权。

 * 输入格式
第一行输入两个正整数N和M，分别为带权有向图的顶点数和边数，N不大于20000且不小于2，M不大于200000。
接下来的M行各输入三个正整数，分别为Ui、Vi和Wi，表示图中的第i条边从顶点Ui指向顶点Vi，权重为Wi，其中i=1,...,M，Ui、Vi均不大于N，Wi不大于10000。

 * 输出格式
输出共1行，为N-1个整数，分别表示S2=2,...,N时总路径权的情况。如果在题目描述的条件下不存在可以“会合”的两条路径，则输出-1，否则输出总路径权的值，不同值之间以空格隔开。

 * 输入样例
6 9
4 3 8
5 1 10
1 3 7
4 2 4
3 1 8
3 5 3
2 5 4
3 2 2
5 2 7

 * 输出样例
9 7 13 10 -1
 * 提示
1. 题目所给的有向图为稀疏图，可以据此优化图的存储以及最短路径的构造。

2. 题目所给的有向图为简单图，即没有自环和重边。

3. 尝试在所给图的基础上构造新图以寻求巧妙的解决方法。
 */

#include <cstdio>
#include <queue>

#define INF 0x3f3f3f3f

struct ListNode {
    int vertex;
    int weight;  //weight <= 10000
    ListNode *next;

    friend bool operator<(const ListNode &a, const ListNode &b) {
        return a.weight > b.weight;
    }

    ListNode() : vertex(INF), weight(INF), next(nullptr) {}

    ListNode(int ver, int w) : vertex(ver), weight(w), next(nullptr) {}
};

class GraphList {
public:
    int n;  //顶点 2 <= n <= 20000，此处存储原图和反向图总顶点
    ListNode *V = nullptr;

    explicit GraphList(int node) {
        this->n = 2 * node;
        this->V = new ListNode[2 * node];  //0~n-1为原图，n~2n-1为反向图
        for (int i = 0; i < 2 * node; i++)
            V[i].vertex = i;
    }

    ~GraphList() { delete[] V; }

    void insert(int parent, int child, int weight) const;

    int *dijkstra() const;
};

void GraphList::insert(int parent, int child, int weight) const {
    auto *p = new ListNode(child, weight);
    p->next = V[parent].next;
    V[parent].next = p;
}

int *GraphList::dijkstra() const {  //从0开始跑，路径初始化为INF
    int *dist = new int[n];
    bool *visited = new bool[n];
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = false;
    }
    dist[0] = 0;
    std::priority_queue<ListNode> queue;
    queue.push(ListNode(0, 0));

    while (!queue.empty()) {
        ListNode flag = queue.top();
        queue.pop();
        if (visited[flag.vertex]) continue;
        visited[flag.vertex] = true;
        for (ListNode *p = V[flag.vertex].next; p != nullptr; p = p->next)
            if (dist[p->vertex] > dist[flag.vertex] + p->weight) {
                dist[p->vertex] = dist[flag.vertex] + p->weight;
                queue.push(ListNode(p->vertex, dist[p->vertex]));
            }
    }
    delete[] visited;
    return dist;
}

int main() {
    int node;
    int edge;
    scanf("%d %d", &node, &edge);
    GraphList graph(node);

    while (edge--) {
        int parent, child, weight;
        scanf("%d %d %d", &parent, &child, &weight);
        graph.insert(--parent, --child, weight);  //同时插入原图和反向图
        graph.insert(child + node, parent + node, weight);
    }
    for (int i = 0; i < node; ++i)  //建立i到i_R的权值为0的边
        graph.insert(i, i + node, 0);

    int *dist = graph.dijkstra();
    for (int i = node + 1; i < 2 * node; ++i)
        printf("%d ", dist[i] == INF ? -1 : dist[i]);

    delete[] dist;
    return 0;
}