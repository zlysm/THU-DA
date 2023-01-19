//
// Created by zlysm on 1/20/23.
//

/*
 * 问题描述
  瓦力是机械城邦的一位机器人，作为一名快递员，瓦力每天的任务是：到城里的某个地方提取客人的物品，并把物品运到城里的某位客人家。
 随着电气时代的到来，机械城邦的地理环境变得非常复杂，以至于机械居民们出行的时候常常无法找到正确的道路。
 机械城邦的地图可以被视作一个大小为Lx×Ly的网格，有些格子为不可越过的居民楼，好在瓦力早就将机械城邦的地图牢记于心。
 同时，凭借优秀的定位装置，瓦力也能够得知自己当前的位置和朝向（初始位姿）、为了提取货物所需要到达的位置和朝向（中间位姿）、
 以及为了把货物送到客人手中所需的位置和朝向（终止位姿）。
 为了节省工作时间，瓦力希望找到一种能够最短时间策略，先提取货物，然后尽快送到客人手中。

如图3所示，机械迷城的地图是一个Lx × Ly个格点划分的二维平面，黑色部分代表墙壁，白色部分代表可通行区域（地图的总面积不超过250000，
 即：Lx × Ly<=250000）。

对于所示的地图，有Lx=10，Ly=7。瓦力的起始位置为（0，0），中间位置为（9，0），终止位置为（9，6）。
 而瓦力的头朝向共有4种状态（上、左、下、右），分别用数字可以表示为：

瓦力头朝向	上	左	下	右
数字表示	1	2	3	4

从而，瓦力的位姿可以表示为结构体（X轴坐标，Y轴坐标，头朝向）。
 例如，对于图1所示的场景下，瓦力的起始位姿可以表示为（0，0，1），中间位姿可以表示为（9，0，1），终止位姿可以表示为（9，6，1）。
 作为履带式机器人的模范代表，瓦力能够执行4种动作：向前移动 1 格；向前移动 2 格；向左转；向右转。如下表所示：

瓦力动作	前进一格	前进两格	向左转	向右转
字符表示	F	F2	L	R
应当注意的是，瓦力的头朝向决定了瓦力在迷宫里的前进方向。例如，头朝下的时候向前移动一格，如果没有撞到墙壁，则将使得当前Y轴坐标+1。

请你给出瓦力从初始位姿出发，经历中间位姿，最终到达终止位姿所需要的最小行动次数，并输出一种行动序列。

 * 输入格式
输入的第1行为地图的长度Lx （0< Lx <= 1000）
输入的第2行为地图的宽度Ly （0< Ly <= 1000）（另外请注意，地图的总面积不超过250000，即：Lx × Ly<=250000）
输入的第3行至第2+Ly行为bool型二维地图，每行有Lx个元素，其中1代表有障碍物，0代表可通行
输入的倒数第三行为起始位姿，例如对于图1则为（0，0，1）
输入的倒数第二行为中间位姿，例如对于图1则为（9，0，1）
输入的最后一行为终止位姿，例如对于图1则为（9，6，1）

 * 输出格式
输出的第一行为瓦力最少所需行动次数N
第二行至最后一行（N+1行）为每次具体采取的行动

 * 输入样例
10
7
0 1 1 1 1 1 1 1 0 0
0 0 0 1 0 0 0 0 0 0
1 1 0 1 0 1 1 0 1 1
1 1 0 1 0 1 1 0 1 1
1 0 0 0 0 1 1 0 1 1
0 0 1 1 1 1 1 0 1 0
0 0 1 1 1 1 1 0 0 0
0 0 1
9 0 1
9 6 1

 * 输出样例
31
L
L
F
L
F2
R
F
F2
L
F2
L
F
F2
R
F
F2
F2
L
F
L
L
F
R
F2
L
F
F2
F2
L
F2
L

 * 提示
1. 本题能够确保从起始位姿到中间位姿再到终止位姿至少存在一条可行路径。
2. 如果存在多个最短行动策略，输出其中一个即可。
3. 瓦力不喜欢撞墙。
4. 地图的边界处也是墙壁。例如对于图3，在（0，0）点上方和左方都默认有墙壁。
 */

#include <cstdio>
#include <cstring>
#include <utility>
#include <vector>
#include <stack>
#include <queue>

#define isdigit(c) ((c) >= '0' && (c) <= '9')

static inline bool blank(char ch) { return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t'; }

template<typename T>
inline T read() {  //no negative number in this pa
    T res = 0;
    char ch = getchar();

    while (blank(ch)) ch = getchar();
    for (; isdigit(ch); ch = getchar()) res = (res << 1) + (res << 3) + (ch ^ 48);
    return res;
}

struct Point {
    short x, y, dir;

    explicit Point(short _x = -1, short _y = -1, short _dir = 0) : x(_x), y(_y), dir(_dir) {}

    bool operator==(const Point &cmp) const {
        return cmp.x == this->x && cmp.y == this->y && cmp.dir == this->dir;
    }

    bool operator!=(const Point &cmp) const { return !(*this == cmp); }
};

typedef std::pair<Point, int> SI;

class Map {
    short Lx, Ly;
    int SIZE;
    bool *graph, *visited;
    char *action;
    std::vector<char> path;
    const short dx[4] = {0, -1, 0, 1};  //left, right
    const short dy[4] = {-1, 0, 1, 0};  //up, down

    inline int Point2Id(const Point &p) const {  //encode
        return p.y + p.x * Ly + p.dir * Ly * Lx;
    }

    bool collision(const Point &p) const {
        if (p.x < 0 || p.x >= Lx || p.y < 0 || p.y >= Ly) return true;
        return graph[Point2Id(p)];
    }

    void moveNext(Point &p, short dir) {
        p.x += dx[dir];
        p.y += dy[dir];
    }

public:
    Map(short x, short y);

    int BFS(const Point &start, const Point &end);

    void print();
};

int main() {
    auto x = read<short>();
    auto y = read<short>();
    Map map(x, y);
    std::vector<char> path;
    Point targets[3];
    for (auto &target: targets) {
        target.x = read<short>();
        target.y = read<short>();
        target.dir = read<short>();
        --target.dir;
    }

    int count = 0;
    count += map.BFS(targets[0], targets[1]);
    count += map.BFS(targets[1], targets[2]);
    printf("%d\n", count);
    map.print();

    return 0;
}

Map::Map(short x, short y) {
    Lx = x;
    Ly = y;
    SIZE = 4 * Lx * Ly;
    graph = new bool[SIZE];
    visited = new bool[SIZE];
    action = new char[SIZE];

    for (short i = 0; i < Ly; ++i)
        for (short j = 0; j < Lx; ++j) {
            int temp = read<int>();
            for (short k = 0; k < 4; ++k)
                graph[Point2Id(Point(j, i, k))] = temp;
        }
}

int Map::BFS(const Point &start, const Point &end) {
    int res = 0;
    memset(visited, false, SIZE * sizeof(bool));
    std::queue <SI> queue;
    queue.emplace(start, 0);
    visited[Point2Id(start)] = true;
    while (!queue.empty()) {
        SI temp = queue.front();
        queue.pop();
        Point cur = temp.first;
        if (cur == end) {
            res = temp.second;
            break;
        }

        Point next = cur;
        moveNext(next, cur.dir);  //move forward: F
        if (!collision(next)) {
            if (!visited[Point2Id(next)]) {
                visited[Point2Id(next)] = true;
                queue.emplace(next, temp.second + 1);
                action[Point2Id(next)] = 'F';
            }

            moveNext(next, cur.dir);  //move two steps: F2
            if (!collision(next) && !visited[Point2Id(next)]) {
                visited[Point2Id(next)] = true;
                queue.emplace(next, temp.second + 1);
                action[Point2Id(next)] = '2';
            }
        }

        next = cur;
        next.dir = (cur.dir + 1) % 4;  //relative left
        if (!visited[Point2Id(next)] && !collision(next)) {
            visited[Point2Id(next)] = true;
            queue.emplace(next, temp.second + 1);
            action[Point2Id(next)] = 'L';
        }

        next = cur;
        next.dir = (cur.dir - 1 + 4) % 4;  //relative right
        if (!visited[Point2Id(next)] && !collision(next)) {
            visited[Point2Id(next)] = true;
            queue.emplace(next, temp.second + 1);
            action[Point2Id(next)] = 'R';
        }
    }

    std::stack<char> stack;
    Point cur = end;
    while (cur != start) {  //backtrack
        char op = action[Point2Id(cur)];
        switch (op) {
            case 'F':
                cur.x -= dx[cur.dir];  //move backward
                cur.y -= dy[cur.dir];
                stack.push('F');
                break;
            case '2':
                cur.x -= 2 * dx[cur.dir];
                cur.y -= 2 * dy[cur.dir];
                stack.push('2');
                break;
            case 'L':
                cur.dir = (cur.dir + 3) % 4;
                stack.push('L');
                break;
            case 'R':
                cur.dir = (cur.dir + 1) % 4;
                stack.push('R');
                break;
            default:;
        }
    }

    while (!stack.empty()) {
        path.push_back(stack.top());
        stack.pop();
    }

    return res;
}

void Map::print() {
    for (auto &op: path) {
        if (op == '2') putchar('F');
        putchar(op);
        putchar('\n');
    }
}
