#include <iostream>
#include<windows.h>
#include <string>
using namespace std;
#define MAX_COURSES 200
#define INT_MAX 0x3f3f3f3f
struct AdjListNode {
    int dest;
    int weight;
    AdjListNode* next;
};

struct CourseNode {
    string name;
    int credit;
    AdjListNode* head;
};

class CourseGraph {
private:
    CourseNode courses[MAX_COURSES];
    int size;

    // 深度优先搜索辅助函数
    bool dfs(int current, int target, bool visited[]) const {
        if (current == target) return true;
        visited[current] = true;
        AdjListNode* node = courses[current].head;
        while (node) {
            if (!visited[node->dest] && dfs(node->dest, target, visited))
                return true;
            node = node->next;
        }
        return false;
    }

public:
    CourseGraph() : size(0) {}
    int findCourse(const string& name) const {
        for (int i = 0; i < size; ++i)
            if (courses[i].name == name) return i;
        return -1;
    }

    void addCourse(const string& name, int credit) {
        if (size >= MAX_COURSES) return;
        courses[size].name = name;
        courses[size].credit = credit;
        courses[size].head = nullptr;
        ++size;
    }
    void addPre(const string& src, const string& dest) {
    int p1 = findCourse(src);
    int p2 = findCourse(dest);
    if (p1 == -1 || p2 == -1) return;

    // 检查重复边
    AdjListNode* node = courses[p1].head;
    while (node) {
        if (node->dest == p2) return;
        node = node->next;
    }

    // 头插法添加带权边（例如权重=目标课程学分）
    AdjListNode* q = new AdjListNode{p2, courses[p2].credit, courses[p1].head};
    courses[p1].head = q;
}

    void printGraph() const {
        cout << "======== 课程先修关系 ========\n";
        for (int i = 0; i < size; ++i) {
            cout << "[" << courses[i].name << " (" << courses[i].credit << "学分)]\n";
            AdjListNode* p = courses[i].head;
            while (p) {
                cout << "  --> " << courses[p->dest].name << "\n";
                p = p->next;
            }
        }
    }
    void hasPre(const string& p1, const string& p2) const {
        int t1 = findCourse(p1);
        int t2 = findCourse(p2);

        // 任一课程不存在或相同课程
        if (t1 == -1 || t2 == -1 ) 
        {
            cout << "输入的课程不存在" << endl;
            return;
        }
        if (t1 == t2) {
            cout << p1 << " 和 " << p2 << " 是同一门课程" << endl;
            return; 
        }
        bool visited[MAX_COURSES] = {false};
        int f= dfs(t1, t2, visited);
        if (f) {
            cout << p1 << " 是 " << p2 << " 的先修课程" << endl; 
        }
        else {
            cout << p1 << " 不是 " << p2 << " 的先修课程" << endl; 
        }
    }
    void getPres(const string& p1) const {
        string s[20];
        int targetId = findCourse(p1);
        if (targetId == -1) {
            cout << "该门课程不存在" << endl;
            return;
        }

        int count = 0;
        // 遍历所有课程
        for (int i = 0; i < size; ++i) {
            if (i == targetId) continue; // 跳过自身
            AdjListNode* p = courses[i].head;
            while (p) {
                if (p->dest == targetId) { // 找到目标课程
                    s[count++] = courses[i].name; // 添加到结果数组
                    break;
                }
                p = p->next;
            }
        }
        if(count == 0)
        {
        	cout << p1<<"没有先修课程" << endl;
        	return;
		}
        cout << "\n" << p1 << "的先修课程 (" << count << "门):\n";
        for (int i = 0; i < count; ++i) {
            cout << " - " << s[i] << "\n";
        }
    }
    //最短路径算法
    void findShortestPathByCredit(const string& start, const string& end) const {
        int startIdx = findCourse(start);
        int endIdx = findCourse(end);

        // 校验课程合法性
        if (startIdx == -1 || endIdx == -1) {
            cout << "[错误] 课程不存在" << endl;
            return;
        }
        if (startIdx == endIdx) {
            cout << "[提示] 起始课程与目标课程相同" << endl;
            return;
        }

        // 初始化数据结构
        int dist[MAX_COURSES];    // 最短距离（总学分）
        bool visited[MAX_COURSES] = {false};
        int prev[MAX_COURSES];    // 前驱节点
        
        for (int i = 0; i < MAX_COURSES; ++i) {
            dist[i] = INT_MAX;
            prev[i] = -1;
        }
        dist[startIdx] = 0;  // 起点距离为0

        // 手动实现优先队列（遍历选择最小距离节点）
        for (int count = 0; count < size; ++count) {
            // 找到当前未访问的最小距离节点
            int u = -1;
            int minDist = INT_MAX;
            for (int i = 0; i < size; ++i) {
                if (!visited[i] && dist[i] < minDist) {
                    minDist = dist[i];
                    u = i;
                }
            }
            
            if (u == -1) break;  // 所有可达节点已处理
            visited[u] = true;

            // 遍历邻接节点更新距离
            AdjListNode* node = courses[u].head;
            while (node != nullptr) {
                int v = node->dest;
                int weight = node->weight;
                
                if (!visited[v] && dist[u] + weight < dist[v]) {
                    dist[v] = dist[u] + weight;
                    prev[v] = u;
                }
                node = node->next;
            }
        }

        // 处理路径输出
        if (dist[endIdx] == INT_MAX) {
            cout << "[提示] 无法从 " << start << " 到达 " << end << endl;
            return;
        }

        // 逆向回溯路径
        int path[MAX_COURSES];
        int count = 0;
        for (int at = endIdx; at != -1; at = prev[at]) {
            path[count++] = at;
        }

        // 正向打印路径
        cout << "==== 最小学分路径 (" << start << " → " << end << ") ====\n";
        for (int i = count-1; i >= 0; --i) {
            cout << "[" << courses[path[i]].name << " (" << courses[path[i]].credit << "学分)]";
            if (i > 0) cout << " → ";
        }
        cout << "\n总需学分: " << dist[endIdx] << endl;
    }
};
CourseGraph construct() {
        CourseGraph graph;
//    SetConsoleOutputCP(CP_UTF8);
//    SetConsoleCP(CP_UTF8);
    // 添加所有课程（名称，学分）
    // 大一课程
    graph.addCourse("计算机科学导论", 2);
    graph.addCourse("Python程序设计", 4);
    graph.addCourse("C语言程序设计", 4);
    graph.addCourse("高等数学（微积分）", 6);
    graph.addCourse("线性代数", 3);
    graph.addCourse("离散数学（逻辑、集合、图论）", 4);
    graph.addCourse("数字逻辑与数字电路", 4);

    // 大二课程
    graph.addCourse("数据结构", 4);
    graph.addCourse("面向对象程序设计（Java/C++）", 4);
    graph.addCourse("计算机组成原理", 4);
    graph.addCourse("概率论与数理统计", 3);
    graph.addCourse("算法设计与分析", 4);
    graph.addCourse("操作系统", 4);
    graph.addCourse("计算机网络", 4);

    // 大三课程
    graph.addCourse("数据库系统", 3);
    graph.addCourse("编译原理", 4);
    graph.addCourse("软件工程", 3);
    // 人工智能方向
    graph.addCourse("机器学习", 3);
    graph.addCourse("深度学习", 3);
    graph.addCourse("自然语言处理", 3);
    // 网络安全方向
    graph.addCourse("信息安全基础", 3);
    graph.addCourse("密码学", 3);
    graph.addCourse("渗透测试", 3);
    // 系统与云计算
    graph.addCourse("分布式系统", 3);
    graph.addCourse("云计算与虚拟化", 3);
    graph.addCourse("Linux内核分析", 3);
    // 前端开发方向
    graph.addCourse("Web前端开发", 3);
    graph.addCourse("后端开发技术", 3);
    graph.addCourse("移动开发", 3);

    // 大四课程
    graph.addCourse("毕业设计", 8);
    graph.addCourse("企业实习", 4);
    graph.addCourse("计算机图形学", 3);
    graph.addCourse("大数据技术（Hadoop/Spark）", 3);
    graph.addCourse("区块链原理", 2);
    graph.addCourse("嵌入式系统开发", 3);

    // 工具课程
    graph.addCourse("Git版本控制", 1);
    graph.addCourse("Linux系统与Shell编程", 2);
    graph.addCourse("Docker与Kubernetes", 2);
    graph.addCourse("技术文档写作", 1);
    graph.addCourse("项目管理（Agile/Scrum）", 2);

    // 完善先修关系
    // 大一基础课依赖
    graph.addPre("计算机科学导论", "Python程序设计");
    graph.addPre("计算机科学导论", "C语言程序设计");
    graph.addPre("高等数学（微积分）", "线性代数");
    graph.addPre("线性代数", "离散数学（逻辑、集合、图论）");
    graph.addPre("离散数学（逻辑、集合、图论）", "数字逻辑与数字电路");
    graph.addPre("数字逻辑与数字电路", "计算机组成原理");
    graph.addPre("高等数学（微积分）", "概率论与数理统计");

    // 大二核心课依赖
    graph.addPre("Python程序设计", "数据结构");
    graph.addPre("C语言程序设计", "数据结构");
    graph.addPre("数据结构", "算法设计与分析");
    graph.addPre("数据结构", "面向对象程序设计（Java/C++）");
    graph.addPre("面向对象程序设计（Java/C++）", "软件工程");
    graph.addPre("计算机组成原理", "操作系统");
    graph.addPre("数据结构", "操作系统");
    graph.addPre("操作系统", "计算机网络");
    graph.addPre("概率论与数理统计", "机器学习");

    // 大三必修课依赖
    graph.addPre("数据结构", "数据库系统");
    graph.addPre("算法设计与分析", "编译原理");
    graph.addPre("面向对象程序设计（Java/C++）", "软件工程");

    // 人工智能方向
    graph.addPre("机器学习", "深度学习");
    graph.addPre("深度学习", "自然语言处理");
    graph.addPre("线性代数", "深度学习");
    graph.addPre("概率论与数理统计", "自然语言处理");

    // 网络安全方向
    graph.addPre("计算机网络", "信息安全基础");
    graph.addPre("信息安全基础", "渗透测试");
    graph.addPre("离散数学（逻辑、集合、图论）", "密码学");
    graph.addPre("算法设计与分析", "渗透测试");

    // 系统与云计算
    graph.addPre("操作系统", "分布式系统");
    graph.addPre("分布式系统", "云计算与虚拟化");
    graph.addPre("计算机组成原理", "Linux内核分析");
    graph.addPre("计算机网络", "云计算与虚拟化");

    // 前端开发方向
    graph.addPre("数据结构", "Web前端开发");
    graph.addPre("Web前端开发", "后端开发技术");
    graph.addPre("面向对象程序设计（Java/C++）", "移动开发");

    // 大四课程依赖
    graph.addPre("数据库系统", "大数据技术（Hadoop/Spark）");
    graph.addPre("算法设计与分析", "计算机图形学");
    graph.addPre("计算机网络", "区块链原理");
    graph.addPre("计算机组成原理", "嵌入式系统开发");

    // 工具课程依赖
    graph.addPre("Python程序设计", "Git版本控制");
    graph.addPre("C语言程序设计", "Git版本控制");
    graph.addPre("Git版本控制", "Linux系统与Shell编程");
    graph.addPre("Linux系统与Shell编程", "Docker与Kubernetes");
    graph.addPre("软件工程", "技术文档写作");
    graph.addPre("软件工程", "项目管理（Agile/Scrum）");
    graph.addPre("算法设计与分析", "毕业设计");
    return graph;
}
