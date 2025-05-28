#include "Teacher.h"
#include <fstream>
#include <sstream>
#include <cstring>  // 新增
#include "course_vector.h" // 新增
#include "Student.h" // 新增
using namespace  std;

HashTable<unsigned long long, Teacher*> teacherHashByNum;
HashTable<std::string, Teacher*> teacherHashByName;

int count(unsigned long long n)
{
	int sum=0;
	while(n>0)
	{
		sum++;
		n/=10;
	}
	return sum;
}
// 加载教师数据
void LoadTeachersFromCSV(List *list,Tree root) {
    ifstream file("Teacher.csv");
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string token;
        Teacher tea;

        try{
            getline(ss, token, ',');
            tea.num = stoull(token);
            getline(ss, token, ',');
            tea.name = token.c_str();
            // strncpy(tea.name, token.c_str(), sizeof(tea.name));
        }catch (const std::invalid_argument& e) {
            std::cerr << "警告: 无效的数值格式: " << e.what() << std::endl;
            // continue;  // 跳过当前行，继续处理下一行
        } catch (const std::out_of_range& e) {
            std::cerr << "警告: 数值超出范围: " << e.what() << std::endl;
            // continue;  // 跳过当前行，继续处理下一行
        }
        try{
            getline(ss, token, ',');
            tea.age = stoi(token);
        }catch (const std::invalid_argument& e) {
            std::cerr << "警告: 无效的数值格式: " << e.what() << std::endl;
            // continue;  // 跳过当前行，继续处理下一行
        } catch (const std::out_of_range& e) {
            std::cerr << "警告: 数值超出范围: " << e.what() << std::endl;
            // continue;  // 跳过当前行，继续处理下一行
        }
        try{
            getline(ss, token,',');
            tea.teach = token;
        }catch (const std::invalid_argument& e) {
            std::cerr << "警告: 无效的数值格式: " << e.what() << std::endl;
            // continue;  // 跳过当前行，继续处理下一行
        } catch (const std::out_of_range& e) {
            std::cerr << "警告: 数值超出范围: " << e.what() << std::endl;
            // continue;  // 跳过当前行，继续处理下一行
        }
        tea.classid = inorder_return_index(root,tea.teach);
        // std::cout << tea.classid << std::endl;

        Node *node = new Node();
        node->tea = tea;
        node->next = nullptr;

        // 填充哈希表
        teacherHashByNum.insert(node->tea.num, &node->tea);
        teacherHashByName.insert(node->tea.name, &node->tea);

        if (!list->front) {
            list->front = node;
        } else {
            Node *cur = list->front;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        list->size++;
    }
    file.close();
}

// 保存教师数据
void SaveTeachersToCSV(List *list) {
    ofstream file("Teacher.csv");
    if (!file.is_open()) {
        cout << "保存失败：无法打开文件。" << endl;
        return;
    }
    Node *cur = list->front;
    while (cur) {
        file << cur->tea.num << ","
             << cur->tea.name << ","
             << cur->tea.age << ","
             << cur->tea.teach << ","
             << cur->tea.classid << "\n";//新加
        cur = cur->next;
    }
    cout<<"已成功保存该教师信息"<<endl;
    file.close();
}
void entryTeacher(List* list)
{
    // 创建节点
    Node* node = new Node();
    // 检查节点是否分配成功
    if (!node)
    {
        cout << "节点内存分配失败" << endl;
        return;
    }
    node->next = NULL;
    cout << "请输入教师编号" << endl;
    while(1)
    {
    	cin>>node->tea.num;
    	if(count(node->tea.num)==7)
    	break;
    	else if(count(node->tea.num)>7)
    	cout<<" 你输入的编号大于七位，请重新输入："<<endl;
    	else
    	cout<<" 你输入的编号小于七位，请重新输入："<<endl;
	}
    cout << "请输入教师姓名" << endl;
    cin >> node->tea.name;
    cout << "请输入教师年龄" << endl;
    cin >> node->tea.age;
    cout << "请输入教师所授课程" << endl;
    cin >> node->tea.teach;
    cout << "请输入教师所授课程编号" << endl;//新加
    cin >> node->tea.classid;
    // 插入哈希表
    teacherHashByNum.insert(node->tea.num, &node->tea);
    teacherHashByName.insert(node->tea.name, &node->tea);

    // 插入到链表头部
    node->next = list->front;
    list->front = node;
    list->size++;
    SaveTeachersToCSV(list);
}

Node* SearchTeacher(List* list)
{
    string name;
    cout << "请输入要查找的教师姓名" << endl;
    cin >> name;
    Node* cur = list->front;
    while (cur != NULL)
    {
        if (cur->tea.name == name || to_string(cur->tea.num) == name)
        {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}
Node* SearchTeachernum(List* list)
{
    string name;
    cout << "请输入要查找的教师编号" << endl;
    cin >> name;
    Node* cur = list->front;
    while (cur != NULL)
    {
        if ( to_string(cur->tea.num) == name)
        {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}
void DeleteTeacher(List* list)
{
    if (list->size == 0)
    {
        cout << "暂无教师信息" << endl;
        return;
    }
    string name;
    cout << "请输入要删除的教师姓名" << endl;
    cin >> name;
    Node* cur = list->front;
    Node* pre = NULL;
    while (cur != NULL)
    {
        if (cur->tea.name == name || to_string(cur->tea.num) == name)
        {
            break;
        }
        pre = cur;
        cur = cur->next;
    }
    if (cur)
    {
        if (pre == NULL)
        {
            list->front = cur->next;
        }
        else
        {
            pre->next = cur->next;
        }
        delete(cur);
        list->size--;
        cout << "删除成功" << endl;
    }
    else
    {
        cout << "没有找到该教师，删除失败" << endl;
    }
    SaveTeachersToCSV(list);
}
void DeleteTeachernum(List* list)
{
    if (list->size == 0)
    {
        cout << "暂无教师信息" << endl;
        return;
    }
    string numStr;
    cout << "请输入要删除的教师编号: ";
    cin >> numStr;
    unsigned long long num = stoull(numStr);

    // 从哈希表中删除
    Teacher* tea;
    if (teacherHashByNum.find(num, tea)) {
        teacherHashByNum.remove(num);
        teacherHashByName.remove(tea->name);
    }

    Node* cur = list->front;
    Node* pre = NULL;
    while (cur != NULL)
    {
        if (to_string(cur->tea.num) == numStr)
        {
            break;
        }
        pre = cur;
        cur = cur->next;
    }
    if (cur)
    {
        if (pre == NULL)
        {
            list->front = cur->next;
        }
        else
        {
            pre->next = cur->next;
        }
        delete(cur);
        list->size--;
        cout << "删除成功" << endl;
    }
    else
    {
        cout << "没有找到该教师，删除失败" << endl;
    }
}
void ViewTeachers(List* list)
{
    Node* cur = list->front;
    while (cur != NULL)
    {
        cout << "编号: " << cur->tea.num << endl;
        cout << "姓名: " << cur->tea.name << endl;
        cout << "年龄: " << cur->tea.age << endl;
        cout << "所授课程: " << cur->tea.teach << endl;
        cout << "所授课程编号: " << cur->tea.classid << endl;//新加
        cout << "-------------------" << endl;
        cur = cur->next;
    }
}

void ChangeTeacher(List* list)
{
    string name;
    cout << "请输入要修改的教师姓名" << endl;
    cin >> name;
    Node* cur = list->front;
    while (cur != NULL)
    {
        if (cur->tea.name == name || to_string(cur->tea.num) == name)
        {
            cout << "请输入要修改的年龄,所授课程和课程编号" << endl;//改
            cin >> cur->tea.age >> cur->tea.teach>>cur->tea.classid;
            cout << "修改成功" << endl;
            break;
        }
        cur = cur->next;
    }
    if (cur == NULL)
    {
        cout << "没有找到该教师，修改失败" << endl;
    }
    SaveTeachersToCSV(list);
}
void ChangeTeachernum(List* list)
{
    string name;
    cout << "请输入要修改的教师编号" << endl;
    cin >> name;
    Node* cur = list->front;
    while (cur != NULL)
    {
        if (to_string(cur->tea.num) == name)
        {
            cout << "请输入要修改的年龄,所授课程和课程编号" << endl;//改
            cin >> cur->tea.age >> cur->tea.teach >> cur->tea.classid;
            cout << "修改成功" << endl;
            break;
        }
        cur = cur->next;
    }
    if (cur == NULL)
    {
        cout << "没有找到该教师，修改失败" << endl;
    }
}

//新加
//在课程树中查找特定课程编号的课程
// 查找课程，假设课程树是按index有序的二叉搜索树
Course* findCourseByIndex(Course* root, int index) {
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    Course* current = root;
    while (current != nullptr) {
        if (current->date.index == index) {
            break;
        } else if (index > current->date.index) {
            current = current->right;  // ✅ 修正为右子树
        } else {
            current = current->left;    // ✅ 修正为左子树
        }
    }
    return current;
}
// 在学生列表中查找特定学号的学生
Student* findStudentByNum(StuList& students, unsigned long long num) {
    key* cur = students.front;
    while (cur != nullptr) {
        if (cur->stu.num == num) {
            return &(cur->stu);
        }
        cur = cur->next;
    }
    return nullptr;
}

// 修改后的教师信息查询函数
void TeachInformation(List* teacherList, Course* courseRoot, StuList& studentList,unsigned long long teacherId) {
    unsigned long long index = teacherId;
    Node* cur = teacherList->front;
    Teacher* foundTeacher = nullptr;

    while (cur != nullptr) {
        if (cur->tea.num == index) {
            foundTeacher = &(cur->tea);
            break;
        }
        cur = cur->next;
    }

    if (foundTeacher == nullptr) {
        cout << "未找到该教师!" << endl;
        return;
    }

    int courseIndex = foundTeacher->classid;
    string courseIndexStr = to_string(courseIndex); // 新增转换语句

    // 修改后的学生统计逻辑
    int studentCount = 0;
    key* stuNode = studentList.front;
    while (stuNode) {
        std::stringstream ss(stuNode->stu.courses);
        std::string course;
        bool found = false;
        while (getline(ss, course, ',')) {
            if (course == courseIndexStr) {
                found = true;
                break;
            }
        }
        if (found) {
            studentCount++;
        }
        stuNode = stuNode->next;
    }

    Course* course = findCourseByIndex(courseRoot, courseIndex);
    if (course == nullptr) {
        cout << "未找到该教师教授的课程!" << endl;
        return;
    }

    cout << "教师 " << foundTeacher->name << " 教授的课程：" << course->date.name << endl;
    
    // 重新初始化指针进行第二次遍历
    stuNode = studentList.front;
    while (stuNode) {
        // 使用strstr进行字符串匹配
        if (strstr(stuNode->stu.courses, courseIndexStr.c_str()) != nullptr) {
            studentCount++;
        }
        stuNode = stuNode->next;
    }
    
    cout << " 学生共" << studentCount << "人" << endl;
    cout << "以下是学生名单：" << endl;

    // 修改后的学生列表显示
    key* currentStudent = studentList.front;
    while (currentStudent != nullptr) {
        std::stringstream ss(currentStudent->stu.courses);
        std::string course;
        bool found = false;
        while (getline(ss, course, ',')) {
            if (course == courseIndexStr) {
                found = true;
                break;
            }
        }
        if (found) {
            cout << "学号: " << currentStudent->stu.num << " 姓名: " << currentStudent->stu.name << endl;
        }
        currentStudent = currentStudent->next;
    }
}
void CourseSearch(Course* root, int index, StuList& allStudents) {
    Course* course = findCourseByIndex(root, index);
    if (!course) {
        cout << "未找到课程 "  << endl;
        return;
    }

    cout << "课程 " << course->date.name << " 的学生：" << endl;

    for (unsigned long long num : course->date.students) {
        Student* student = findStudentByNum(allStudents, num);
        if (student) {
            cout << "姓名: " << student->name << ", 学号: " << student->num << endl;
        }
    }
}