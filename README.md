# 课程管理系统 - 数据结构课程项目

## 项目概述

本项目分为三大功能模块：**学生系统**、**老师系统**和**管理员系统**，实现了完整的课程管理功能。

## 一、学生系统

### (一) 课程管理功能
1. **选课** - `void AddCourseForStudent()`
2. **退课** - `DropCourseForStudent()`
3. **根据编号查课程并输出选择该课程的学生名称** - `CourseSearch()`

### (二) 学生查询功能
1. **查看个人信息** - 直接输出学生节点信息
2. **查看老师信息** - `ViewTeachers()`
3. **查看所有课程信息** - `courseList.print_all_courses()`
4. **查看所有课程先修课** - `graph.printGraph()`
5. **查询某个课程的先修课** - `graph.getPres()`
6. **查询某两门课程是否构成先修关系** - `graph.hasPre()`

### 数据结构设计

#### 1. 链表（StuList）
- **节点结构**：
  - `key`结构体：包含`Student`类型的学生信息(`stu`)和指向下一个节点的指针(`next`)
  - `StuList`结构体：包含指向链表头部的指针(`front`)和表示节点数量的整数(`size`)
- **优点**：
  - 动态存储
  - 插入和删除操作方便
  - 能顺序存储学生信息

#### 2. 哈希表（HashTable）
- **studentHashByNum**：以学生学号(`unsigned long long`)为键，学生信息指针(`Student*`)为值
- **studentHashByName**：以学生姓名(`std::string`)为键，学生信息指针(`Student*`)为值
- **优点**：
  - 快速查询
  - 支持多种查询方式
  - 提高系统效率

## 二、老师系统

### 主要功能
1. **查看个人信息**
2. **查询教学信息** - `TeachInformation(&teacherList, courseList.get_root(), studentList, teacherId)`
3. **查看所有课程** - `print_all_courses()`

## 三、管理员系统

### 1. 课程管理系统

#### 功能列表
1. **增加课程** - `void push(Course_date c)`
2. **通过编号查找课程** - `bool find_by_index(int goal)`
3. **通过名称查找课程** - `bool find_by_name(std::string goal)`
4. **通过编号删除课程** - `void delete_by_index(int goal)`
5. **通过名称删除课程** - `void delete_by_name(std::string name)`
6. **修改课程** - `bool update_course(int old_index, int new_index, const std::string& new_name)`
7. **查看所有课程信息** - `int get_index_by_name(std::string name)`

#### 数据持久化
- **构造函数**：从`course_vector.csv`读取数据（格式：`index,name`），自动构建BST
- **析构函数**：将BST数据按中序遍历顺序保存回CSV文件，确保数据有序性

#### 数据结构设计
- 使用**二叉搜索树**存储课程数据（`course_head`为根节点）
- 每个节点包含课程信息（名称`name`和编号`index`）及左右子节点指针
- 通过编号(`index`)实现BST的排序规则

#### 先修图的实现

##### 数据结构
- `AdjListNode`：邻接表节点
  - `dest`：目标课程在数组中的索引
  - `next`：指向下一个邻接表节点的指针
- `CourseNode`：课程节点
  - `name`：课程名称
  - `credit`：课程学分
  - `head`：指向邻接表头节点的指针（表示该课程的所有直接后续课程）

##### 核心功能实现
1. **添加课程关系（addPre）**
   - 逻辑：建立课程间的有向边，表示先修关系
   - 步骤：
     - 通过课程名查找源课程和目标课程的索引
     - 检查边是否已存在，避免重复添加
     - 使用头插法将目标课程索引加入源课程的邻接表

2. **判断先修关系（hasPre）**
   - 逻辑：通过深度优先搜索（DFS）判断是否存在从源课程到目标课程的路径
   - 步骤：
     - 初始化访问标记数组`visited`
     - 递归遍历邻接表，若找到目标课程则返回`true`，否则回溯

3. **获取直接先修课程（getPres）**
   - 逻辑：遍历所有课程，收集直接指向目标课程的节点
   - 步骤：
     - 检查每个课程的邻接表是否包含目标课程索引
     - 若存在，则记录该课程名称（即目标课程的直接先修课程）

##### 先修关系示例
- **直接先修**：如`计算机科学导论 → Python程序设计`，通过邻接表直接关联
- **间接先修**：如`计算机科学导论 → Python程序设计 → 数据结构`，通过路径存在性判断（DFS）

### 2. 教师管理系统

#### 功能列表
1. **数据加载与保存**：
   - `LoadTeacherFromCSV()`：从CSV文件加载教师数据到内存链表
   - `SaveTeachersToCSV()`：将链表中的教师数据保存到CSV文件
2. **添加新教师信息** - `entryTeacher()`
3. **修改教师信息**（通过姓名或工号）- `ChangeTeacher()`/`ChangeTeachernum()`
4. **查找教师信息**（通过姓名或工号）- `SearchTeacher()`/`SearchTeachernum()`
5. **删除教师信息**（通过姓名或工号）- `DeleteTeacher()`/`DeleteTeachernum()`
6. **查看所有教师信息** - `ViewTeacher()`
7. **计算数字的位数**（用于验证工号长度）- `ViewTeacher()`

#### 数据结构
- 使用**链表(List)**存储教师信息，每个节点(`key`)包含一个教师(`Teacher`)结构体

#### 数据持久化
- 使用CSV文件(`"Teacher.csv"`)作为存储格式，每次修改后自动保存数据

### 3. 学生管理系统

#### 功能列表
1. **数据加载与保存**：
   - `LoadStudentsFromCSV()`：从CSV文件加载学生数据到内存链表
   - `SaveStudentsToCSV()`：将链表中的学生数据保存到CSV文件
2. **添加新学生信息** - `entryStudent()`
3. **修改学生信息**（通过姓名或学号）- `ChangeStudent()`/`ChangeStudentnum()`
4. **查找学生信息**（通过姓名或学号）- `SearchStudent()`/`SearchStudentnum()`
5. **删除学生信息**（通过姓名或学号）- `DeleteStudent()`/`DeleteStudentnum()`
6. **查看所有学生信息** - `ViewStudent()`
7. **计算数字的位数**（用于验证学号长度）- `ViewStudent()`

#### 数据结构
- 使用**链表(StuList)**存储学生信息，每个节点(`key`)包含一个学生(`Student`)结构体

#### 数据持久化
- 使用CSV文件(`"Student.csv"`)作为存储格式，每次修改后自动保存数据

## 哈希表应用

### 快速数据检索
- **键值映射**：通过学号、姓名、课程编号等唯一标识（键）直接定位到对应的学生、教师或课程对象（值），时间复杂度接近O(1)
- **典型场景**：
  - 学生登录时通过学号(`studentHashByNum`)快速获取个人信息
  - 教师通过课程名称(`courseHashByName`)查找课程详情

### 数据一致性维护
- **双向同步**：哈希表与链表、二叉搜索树（BST）协同工作，确保内存数据与持久化数据（CSV）一致
- **示例**：
  - 添加学生时，同时插入链表（`StuList`）和哈希表（`studentHashByNum/Name`）
  - 删除课程时，同步更新BST和哈希表（`courseHashByIndex/Name`）

### 功能实现优化
- **复杂操作加速**：
  - 选课/退课：通过哈希表快速定位学生和课程，更新关联关系
  - 教学信息查询：教师通过哈希表找到课程编号，再遍历学生链表统计选修人数
- **冲突处理**：采用链地址法（`std::list`存储冲突节点），保证数据完整性

### 多维度查询支持
- **复合键设计**：
  - 学生和教师支持按编号（主键）或姓名（辅助键）查询
  - 课程支持按编号或名称查询

### 性能与扩展性平衡
- **高效与灵活兼顾**：
  - 哈希表提供快速访问，适用于高频查询场景
  - 链表和BST支持顺序遍历或范围查询
- **动态扩容**：哈希表默认大小为100，可随数据量增长自动调整

## 总结

哈希表在系统中充当"高速缓存"角色，通过键值映射大幅减少数据访问时间，同时与链表、树结构协同，兼顾功能实现与性能优化。其核心价值在于：

1. **提速**：将线性查找（O(n)）优化为常数级（O(1)）
2. **简化逻辑**：通过直接定位对象，避免复杂遍历
3. **支撑复杂业务**：如选课退课、教学统计等依赖快速数据关联的功能
