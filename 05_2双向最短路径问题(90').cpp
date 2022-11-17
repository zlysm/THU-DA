//
// Created by zlysm on 11/18/22.
//

//注意！！！OJ支持STL容器
//造轮子版，关于优先级队列的堆实现值得参考
//但不知道问什么OJ最后一组样例是WA

#include <cstdio>

#define INF 0x3f3f3f3f

struct ListNode {
    int vertex;
    int weight;  //weight <= 10000
    ListNode *next;

    bool operator<(const ListNode &target) const { return weight < target.weight; }

    ListNode() : vertex(INF), weight(INF), next(nullptr) {}

    ListNode(int ver, int w) : vertex(ver), weight(w), next(nullptr) {}
};

class PriorityQueue {
private:
    ListNode *heap;  //最小堆
    int heapSize;
    int count;  //堆元素个数

    static void swap(ListNode &a, ListNode &b) {
        ListNode temp = a;
        a = b;
        b = temp;
    }

public:
    explicit PriorityQueue(int size) : heapSize(size), count(0) { heap = new ListNode[size + 1]; }

    ~PriorityQueue() { delete[] heap; }

    void push(int vertex, int weight);

    int pop();

    bool empty() const { return !count; }
};

void PriorityQueue::push(int vertex, int weight) {
    if (count == heapSize) return;  //full
    heap[++count] = ListNode(vertex, weight);
    int cur = count;
    while (cur > 1 && heap[cur] < heap[cur >> 1]) {  //自底而上维护堆
        swap(heap[cur], heap[cur >> 1]);
        cur >>= 1;
    }
}

int PriorityQueue::pop() {
    if (empty()) return INF;
    int min = heap[1].vertex;
    heap[1] = heap[count--];
    int cur = 1;
    while (cur << 1 <= count) {  //自顶而下维护堆
        int flag = cur << 1;
        if (flag < count && heap[flag + 1] < heap[flag])  //左右孩子中较小的
            flag++;
        if (heap[cur] < heap[flag])  //父节点较小,已经是最小堆
            break;
        swap(heap[cur], heap[flag]);
        cur = flag;  //到原来较小孩子的位置继续向下
    }
    return min;
}

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

int *GraphList::dijkstra() const {  //从0开始跑
    int *dist = new int[n];
    bool *visited = new bool[n];
    for (int i = 0; i < n; i++) {
        dist[i] = INF;
        visited[i] = false;
    }
    dist[0] = 0;
    PriorityQueue queue(n);
    queue.push(0, 0);

    while (!queue.empty()) {
        int flag = queue.pop();
        if (visited[flag]) continue;
        visited[flag] = true;

        for (ListNode *p = V[flag].next; p; p = p->next)
            if (!visited[p->vertex] && dist[p->vertex] > dist[flag] + p->weight) {
                dist[p->vertex] = dist[flag] + p->weight;
                queue.push(p->vertex, dist[p->vertex]);
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