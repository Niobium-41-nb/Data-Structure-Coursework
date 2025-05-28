// 课程向量类实现文件
// 包含必要的头文件
#include "Teacher.h"
#include "Student.h"
#include "course_vector.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <cstring>
#include <algorithm>
using namespace std;

// 构造函数
// 初始化课程向量，从course_vector.csv文件中读取数据
course_vector::course_vector() : course_head(nullptr), len(0) {
    std::ifstream file("course_vector.csv", std::ios::binary); // 添加二进制模式
    // 读取文件第一行（标题行）
    std::string line;
    getline(file, line); // 读取标题行
    // 添加BOM头检测
    if (line.size() >= 3 && 
        static_cast<unsigned char>(line[0]) == 0xEF &&
        static_cast<unsigned char>(line[1]) == 0xBB &&
        static_cast<unsigned char>(line[2]) == 0xBF) {
        line = line.substr(3); // 跳过BOM头
    }
    // 逐行读取课程数据
    while (getline(file, line)) {
        // 创建临时向量存储当前行数据
        std::vector<std::string> row;
        // 使用字符串流处理当前行
        std::stringstream ss(line);
        // 存储单个字段值
        std::string field;

        // 读取index、name、teacher_id
        getline(ss, field, ',');
        field.erase(0, field.find_first_not_of(" \t\n\r")); // 去除前导空白字符
        field.erase(field.find_last_not_of(" \t\n\r") + 1); // 去除尾随空白字符
        if (field.empty() || !std::all_of(field.begin(), field.end(), ::isdigit)) {
            std::cout << "警告: 无效的课程编号: " << field << std::endl;
            continue;
        }
        // 继续处理下一行数据

        int index;
        try{
            index = std::stoi(field);
        }catch (const std::invalid_argument& e) {
            std::cerr << "警告: 无效的数值格式: " << e.what() << std::endl;
            continue;  // 跳过当前行，继续处理下一行
        } catch (const std::out_of_range& e) {
            std::cerr << "警告: 数值超出范围: " << e.what() << std::endl;
            continue;  // 跳过当前行，继续处理下一行
        }
        
        getline(ss, field, ',');
        std::string name = field;

        // 读取学分
        getline(ss, field, ',');
        field.erase(0, field.find_first_not_of(" \t\n\r"));
        field.erase(field.find_last_not_of(" \t\n\r") + 1);
        if (field.empty() || !std::all_of(field.begin(), field.end(), ::isdigit)) {
            std::cout << "警告: 无效的学分: " << field << std::endl;
            continue;
        }
        int credit = std::stoi(field);

        // 读取教师ID
        getline(ss, field, ',');
        field.erase(0, field.find_first_not_of(" \t\n\r"));
        field.erase(field.find_last_not_of(" \t\n\r") + 1);
        if (field.empty() || !std::all_of(field.begin(), field.end(), ::isdigit)) {
            std::cout << "警告: 无效的教师编号: " << field << std::endl;
            continue;
        }
        unsigned long long teacher_id = std::stoull(field);

        // 读取学生数组
        std::vector<int> students;
        while (getline(ss, field, ',')) {
            if (!field.empty() && std::all_of(field.begin(), field.end(), ::isdigit)) {
                try{
                    students.push_back(std::stoi(field));
                }catch (const std::invalid_argument& e) {
                    std::cerr << "警告: 无效的数值格式: " << e.what() << std::endl;
                    continue;  // 跳过当前行，继续处理下一行
                } catch (const std::out_of_range& e) {
                    std::cerr << "警告: 数值超出范围: " << e.what() << std::endl;
                    continue;  // 跳过当前行，继续处理下一行
                }
            }
        // 继续处理下一行数据
        }
        // 继续处理下一行数据

        // 创建课程数据结构
        Course_date c{name, index,credit, teacher_id, students,"8:00-9:40","Monday"};
        // 检查课程是否重复
        if (!is_duplicate(c)) {
            // 将课程添加到二叉树中
            push(c);
        }else{
        // 继续处理下一行数据 else {
            // 输出重复课程警告
            std::cout << "错误: 课程重复（编号、名称、教师） " << index << std::endl;
        }
        // 继续处理下一行数据
    }
    file.close();
}

// 修改析构函数中的文件头
course_vector::~course_vector() {
    std::ofstream file("course_vector.csv", std::ios::binary); // 添加二进制模式
    if (file.is_open()) {
        file << "\xEF\xBB\xBF"; // 添加UTF-8 BOM头
        file << "index,name,credit,teacher_id,students\n"; // 修正标题行
        saveInOrder(course_head, file);
        file.close();
        std::cout << "课程数据已保存到 course_vector.csv" << std::endl;
    }
    else {
        std::cerr << "错误: 无法打开文件 course_vector.csv 进行写入" << std::endl;
    }
    freeCourseTree(course_head);
    course_head = nullptr;
    len = 0;
}

void course_vector::push(Course_date c) {
    if (is_duplicate(c)) {
        std::cout << "插入失败: 课程（编号、名称、教师）已存在 " << c.index << std::endl;
        return;
    }
    Tree newnode = new Course{ {c.name, c.index,c.credit, c.teacher_id, c.students,"8:00-9:40","Monday"}, nullptr, nullptr};
    courseHashByIndex.insert(newnode->date.index, &newnode->date);
    courseHashByName.insert(newnode->date.name, &newnode->date);
    if (course_head == nullptr) {
        course_head = newnode;
    }
    else {
        Tree nownode = course_head;
        while (true) {
            if (c.index < nownode->date.index) {
                if (nownode->left == nullptr) {
                    nownode->left = newnode;
                    break;
                }
        // 继续处理下一行数据
                else {
                    nownode = nownode->left;
                }
        // 继续处理下一行数据
            }
        // 继续处理下一行数据
            else {
                if (nownode->right == nullptr) {
                    nownode->right = newnode;
                    break;
                }
        // 继续处理下一行数据
                else {
                    nownode = nownode->right;
                }
        // 继续处理下一行数据
            }
        // 继续处理下一行数据
        }
        // 继续处理下一行数据
    }
    len++;
}
Tree course_vector::get_root(){
    return this->course_head;
}
bool course_vector::is_duplicate(const Course_date& c) {
    Tree current = course_head;
    while (current != nullptr) {
        if (current->date.index == c.index && 
            current->date.name == c.name && 
            current->date.teacher_id == c.teacher_id) {
            return true;
        }
        // 继续处理下一行数据
        if (c.index < current->date.index) {
            current = current->left;
        }else {
        // 继续处理下一行数据 
            // 输出重复课程警告
            current = current->right;
        }
        // 继续处理下一行数据
    }
    return false;
}
bool course_vector::find_by_index(int goal) {
    Tree nownode = course_head;
    while (nownode != nullptr) {
        if (nownode->date.index == goal) {
            return true;
        }
        // 继续处理下一行数据
        else if (goal < nownode->date.index) {
            nownode = nownode->left;
        }
        // 继续处理下一行数据
        else {
            nownode = nownode->right;
        }
        // 继续处理下一行数据
    }
    return false;
}

bool course_vector::find_by_name(std::string goal) {
    if (inorder_find(course_head, goal)) {
        return true;
    }
    else {
        std::cout << "未找到课程: " << goal << std::endl;
        return false;
    }
}

bool course_vector::update_course(int old_index, int new_index, const std::string& new_name,unsigned long long new_teacher_id) {
    if (new_index != old_index && find_by_index(new_index)) {
        std::cout << "更新失败: 编号 " << new_index << " 已存在" << std::endl;
        return false;
    }
    Tree current = course_head;
    while (current && current->date.index != old_index) {
        current = (old_index < current->date.index) ? current->left : current->right;
    }
    if (!current) {
        std::cout << "未找到编号: " << old_index << std::endl;
        return false;
    }
    // 先删除旧条目
    courseHashByIndex.remove(old_index);
    courseHashByName.remove(current->date.name);
        
    // 更新数据
    current->date.index = new_index;
    current->date.name = new_name;
    current->date.teacher_id = new_teacher_id;
        
    // 插入新条目
    courseHashByIndex.insert(new_index, &current->date);
    courseHashByName.insert(new_name, &current->date);
    delete_by_index(old_index);
    Course_date now_course = {new_name,new_index,0,new_teacher_id,{},"8:00-9:40","Monday"};
    push(now_course);
    return true;
}

void course_vector::print_all_courses() {
    inorder_print(course_head);
}

void course_vector::delete_by_index(int goal) {
    Tree parent = nullptr;
    Tree current = course_head;
    while (current != nullptr && current->date.index != goal) {
        parent = current;
        if (goal < current->date.index) {
            current = current->left;
        }
        // 继续处理下一行数据
        else {
            current = current->right;
        }
        // 继续处理下一行数据
    }
    if (current == nullptr) {
        std::cout << "未找到编号: " << goal << std::endl;
        return;
    }
    courseHashByIndex.remove(current->date.index);
    courseHashByName.remove(current->date.name);
    if (current->left == nullptr) {
        if (parent == nullptr) {
            course_head = current->right;
        }
        // 继续处理下一行数据
        else if (parent->left == current) {
            parent->left = current->right;
        }
        // 继续处理下一行数据
        else {
            parent->right = current->right;
        }
        // 继续处理下一行数据
        delete current;
        len--;
    }
    else if (current->right == nullptr) {
        if (parent == nullptr) {
            course_head = current->left;
        }
        // 继续处理下一行数据
        else if (parent->left == current) {
            parent->left = current->left;
        }
        // 继续处理下一行数据
        else {
            parent->right = current->left;
        }
        // 继续处理下一行数据
        delete current;
        len--;
    }else {
        Tree minParent = current;
        Tree minNode = current->right;
        while (minNode->left != nullptr) {
            minParent = minNode;
            minNode = minNode->left;
        }
        // 继续处理下一行数据
        current->date = minNode->date;
        if (minParent->left == minNode) {
            minParent->left = minNode->right;
        }
        // 继续处理下一行数据
        else {
            minParent->right = minNode->right;
        }
        // 继续处理下一行数据
        delete minNode;
        len--;
    }
    std::cout << "删除成功" << std::endl;
}

void course_vector::delete_by_name(std::string name) {
    int index = get_index_by_name(name);
    if (index != -1) {
        delete_by_index(index);
    }
    else {
        std::cout << "未找到课程: " << name << std::endl;
    }
}

std::string course_vector::get_name_by_index(int goal) {
    Tree current = course_head;
    while (current != nullptr) {
        if (goal == current->date.index) {
            // cout << "课程名称：" << current->date.name << endl;
            // cout << "授课老师：" << current->date.teacher_id << endl;
            return current->date.name;
        }
        // 继续处理下一行数据
        else if (goal < current->date.index) {
            current = current->left;
        }
        // 继续处理下一行数据
        else {
            current = current->right;
        }
        // 继续处理下一行数据
    }
    return "";
}

int course_vector::get_index_by_name(std::string name) {
    return inorder_return_index(course_head, name);
}

Course* course_vector::get_Course_by_index(int courseIndex){
    Tree current = course_head;
    while (current != nullptr) {
        if (courseIndex == current->date.index) {
            return current; // 找到对应课程节点时返回指针
        }
        current = (courseIndex < current->date.index) ? current->left : current->right;
    }
    return nullptr; // 未找到时返回空指针
}

void freeCourseTree(Tree root) {
    if (root == nullptr) return;
    freeCourseTree(root->left);
    freeCourseTree(root->right);
    delete root;
}

bool inorder_find(Tree node, const std::string& goal) {
    if (node == nullptr) return false;
    if (inorder_find(node->left, goal)) return true;
    if (node->date.name == goal) return true;
    return inorder_find(node->right, goal);
}

void inorder_print(Tree node) {
    if (node == nullptr) return;
    inorder_print(node->left);
    // 查找教师姓名
    Teacher* teacher;
    std::string teacher_name = "未知教师";
    if (teacherHashByNum.find(node->date.teacher_id, teacher)) {
        teacher_name = teacher->name;
    }
    // 输出课程基本信息
    std::cout << "编号: " << node->date.index 
              << ", 名称: " << node->date.name 
              << " - 教师: " << teacher_name 
              << " (ID: " << node->date.teacher_id
              << ")\n"
              << " 上课时间：" << node->date.week << " "<< node->date.time
              << "\n";
              
    // 输出学生名单
    std::cout << "  学生名单: \n";
    for (int student_id : node->date.students) {
        std::cout << "  " << std::endl;
        Student* student;
        std::string student_name = "未知学生";
        if (studentHashByNum.find(student_id, student)) {
            student_name = student->name;
        }
        std::cout << student_name << " (ID: " << student_id << "),\n";
    }
    std::cout << "-------------------\n";
    // Sleep(1000);
    inorder_print(node->right);
}

int inorder_return_index(Tree node, const std::string& name) {
    if (node == nullptr) return -1;
    int left = inorder_return_index(node->left, name);
    if (left != -1) return left;
    if (node->date.name == name) return node->date.index;
    return inorder_return_index(node->right, name);
}

// 修改保存函数确保编码正确
void course_vector::saveInOrder(Tree node, std::ofstream& file) {
    if (node == nullptr) return;
    saveInOrder(node->left, file);
    // 确保字符串字段使用双引号包裹
    file << node->date.index << "," << node->date.name << "," << node->date.credit << ","  << node->date.teacher_id;
    // cout << node->date.index << ",\"" << node->date.name << "\"," << node->date.teacher_id;
    for (int student : node->date.students) {
        file << "," << student;
        // cout << "," << student;
    }
    file << "\n";
    saveInOrder(node->right, file);
}
void course_vector::add_student(int course_index, int student_id) {
    Tree current = course_head;
    while (current != nullptr) {
        if (current->date.index == course_index) {
            auto& students = current->date.students;
            if (std::find(students.begin(), students.end(), student_id) == students.end()) {
                students.push_back(student_id);
                std::cout << "成功添加学生 " << student_id << " 到课程 " << course_index << std::endl;

            }else {
        // 继续处理下一行数据 
            // 输出重复课程警告
                std::cout << "学生 " << student_id << " 已存在于课程 " << course_index << std::endl;
            }
        // 继续处理下一行数据
            return;
        }else if (course_index < current->date.index) {
        // 继续处理下一行数据 
            current = current->left;
        }else {
        // 继续处理下一行数据 
            // 输出重复课程警告
            current = current->right;
        }
        // 继续处理下一行数据
    }
    std::cout << "未找到课程编号: " << course_index << std::endl;
}

void course_vector::remove_student(int course_index, int student_id) {
    Tree current = course_head;
    while (current != nullptr) {
        if (current->date.index == course_index) {
            auto& students = current->date.students;
            auto it = std::find(students.begin(), students.end(), student_id);
            if (it != students.end()) {
                students.erase(it);
                std::cout << "成功从课程 " << course_index << " 中删除学生 " << student_id << std::endl;
            }else {
        // 继续处理下一行数据 
            // 输出重复课程警告
                std::cout << "学生 " << student_id << " 不在课程 " << course_index << " 中" << std::endl;
            }
        // 继续处理下一行数据
            return;
        }else if (course_index < current->date.index) {
        // 继续处理下一行数据 
            current = current->left;
        }else {
        // 继续处理下一行数据 
            // 输出重复课程警告
            current = current->right;
        }
        // 继续处理下一行数据
    }
    std::cout << "未找到课程编号: " << course_index << std::endl;
}


