#include "Student.h"
#include <iostream>
#include <string>
#include <cstring>
#include <fstream>
#include <sstream>
#include <vector>
#include "course_vector.h"
using namespace std;

extern course_vector courseList;

HashTable<unsigned long long, Student*> studentHashByNum;
HashTable<std::string, Student*> studentHashByName;

int stucount(int n)
{
	int sum=0;
	while(n>0)
	{
		sum++;
		n/=10;
	}
	return sum;
}

// 加载学生数据
void LoadStudentsFromCSV(StuList *list) {
    ifstream file("Student.csv");
    if (!file.is_open()) return;
    string line;
    while (getline(file, line)) {
        Student stu;
        stringstream ss(line);
        string token;

        try{
            getline(ss, token, ',');
            stu.num = stoull(token);
            getline(ss, token, ',');
            strncpy(stu.name, token.c_str(), sizeof(stu.name));
            getline(ss, token, ',');
            stu.age = stoi(token);
            getline(ss, token, ',');
            stu.credits = stoi(token);
            getline(ss, token);
            strncpy(stu.courses, token.c_str(), sizeof(stu.courses));
        } catch (const std::invalid_argument& e) {
            std::cerr << "警告: 无效的数值格式: " << e.what() << std::endl;
            continue;  // 跳过当前行，继续处理下一行
        } catch (const std::out_of_range& e) {
            std::cerr << "警告: 数值超出范围: " << e.what() << std::endl;
            continue;  // 跳过当前行，继续处理下一行
        }

        key *node = new key();
        node->stu = stu;
        node->next = nullptr;

        studentHashByNum.insert(node->stu.num, &node->stu);
        studentHashByName.insert(node->stu.name, &node->stu);

        if (!list->front) {
            list->front = node;
        } else {
            key *cur = list->front;
            while (cur->next) cur = cur->next;
            cur->next = node;
        }
        list->size++;
    }
    file.close();
}

// 保存学生数据
void SaveStudentsToCSV(StuList *list) {
    ofstream file("Student.csv");
    if (!file.is_open()) {
        cout << "保存失败：无法打开文件。" << endl;
        return;
    }
    key *cur = list->front;
    while (cur) {
        file << cur->stu.num << ","
             << cur->stu.name << ","
             << cur->stu.age << ","
             << cur->stu.credits << ","
             << cur->stu.courses << "\n";
        cur = cur->next;
    }
    cout<<"已成功保存该学生信息"<<endl;
    file.close();
}


//增加学生信息
void entryStudent(StuList *list)
{
    //创建节点
    key *node = new key();

    studentHashByNum.insert(node->stu.num, &node->stu);
    studentHashByName.insert(node->stu.name, &node->stu);

    //判断是否成功分配内存空间
    if(!node)
    {
        cout<<"分配内存空间失败"<<endl;
        return;
    }
    node->next = NULL;
    cout<<"请输入学生学号"<<endl;
    while(1)
    {
    	cin>>node->stu.num;
    	if(stucount(node->stu.num)==8)
    	break;
    	else if(stucount(node->stu.num)>8)
    	cout<<" 你输入的编号大于八位，请重新输入："<<endl;
    	else
    	cout<<" 你输入的编号小于八位，请重新输入："<<endl;
	}
    cout<<"请输入学生姓名"<<endl;
    cin>>node->stu.name;
    cout<<"请输入学生年龄"<<endl;
    cin>>node->stu.age;
    cout<<"请输入学生所获得的学分"<<endl;
    cin>>node->stu.credits;
    cout<<"请输入学生所选课程"<<endl;
    cin>>node->stu.courses;
    //插入到链表中
    node->next = list->front;
    list->front = node;
    list->size++;
    SaveStudentsToCSV(list);
}

//更改学生信息
void ChangeStudent(StuList *list)
{
    string name;
    cout<<"请输入要修改的学生姓名"<<endl;
    cin>>name;
    key *cur = list->front;
    while(cur != NULL)
    {
        if(cur->stu.name == name|| to_string(cur->stu.num) == name)
        {
            cout<<"请输入要修改的年龄，所获得的学分和所选课程"<<endl;
            cin>>cur->stu.age>>cur->stu.credits>>cur->stu.courses;
            cout<<"修改成功"<<endl;
            break;
        }
        cur = cur->next;
    }
    if(cur == NULL)
    {
        cout<<"没有找到该学生，修改失败"<<endl;
    }
    SaveStudentsToCSV(list);
}
void ChangeStudentnum(StuList *list)
{
    string name;
    cout<<"请输入要修改的学生学号"<<endl;
    cin>>name;
    key *cur = list->front;
    while(cur != NULL)
    {
        if(to_string(cur->stu.num) == name)
        {
            cout<<"请输入要修改的年龄，所获得的学分和所选课程"<<endl;
            cin>>cur->stu.age>>cur->stu.credits>>cur->stu.courses;
            cout<<"修改成功"<<endl;
            break;
        }
        cur = cur->next;
    }
    if(cur == NULL)
    {
        cout<<"没有找到该学生，修改失败"<<endl;
    }
    SaveStudentsToCSV(list);
}

//查找学生信息
key* SearchStudent(StuList *list)
{
    string name;
    cout<<"请输入要查找的学生姓名"<<endl;
    cin>>name;
    key *cur = list->front;
    while(cur != NULL)
    {
        if(cur->stu.name == name||to_string(cur->stu.num) == name)
        {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}
key* SearchStudentnum(StuList *list)
{
    string name;
    cout<<"请输入要查找的学生学号"<<endl;
    cin>>name;
    key *cur = list->front;
    while(cur != NULL)
    {
        if(to_string(cur->stu.num) == name)
        {
            return cur;
        }
        cur = cur->next;
    }
    return NULL;
}
//删除学生信息
void DeleteStudent(StuList  *list)
{
    if(list->size == 0)
    {
        cout<<"暂无信息"<<endl;
        return;
    }
    string name;
    cout<<"请输入要删除的学生姓名"<<endl;
    cin>>name;
    key *cur = list->front;
    key *pre = NULL;
    while(cur != NULL)
    {
        if(cur->stu.name == name||to_string(cur->stu.num) == name)
        {
            break;
        }
        pre = cur;
        cur = cur->next;
    }
    if(cur)
    {
        if(pre == NULL)
        {
            list->front = cur->next;
        }
        else
        {
            pre->next = cur->next;
        }
        delete(cur);
        list->size--;
        cout<<"删除成功"<<endl;
    }
    else
    {
        cout << "没有找到该学生，删除失败" << endl;
    }
    SaveStudentsToCSV(list);
}
void DeleteStudentnum(StuList  *list)
{
    if(list->size == 0)
    {
        cout<<"暂无信息"<<endl;
        return;
    }
    string name;
    cout<<"请输入要删除的学生学号"<<endl;
    cin>>name;
    unsigned long long num = stoull(name);
    
    // 从哈希表中删除
    studentHashByNum.remove(num);
    Student* stu;
    if (studentHashByNum.find(num, stu)) {
        studentHashByName.remove(stu->name);
    }

    key *cur = list->front;
    key *pre = NULL;
    while(cur != NULL)
    {
        if(to_string(cur->stu.num)==name)
        {
            break;
        }
        pre = cur;
        cur = cur->next;
    }
    if(cur)
    {
        if(pre == NULL)
        {
            list->front = cur->next;
        }
        else
        {
            pre->next = cur->next;
        }
        delete(cur);
        list->size--;
        cout<<"删除成功"<<endl;
    }
    else
    {
        cout << "没有找到该学生，删除失败" << endl;
    }
    SaveStudentsToCSV(list);
}

//查看学生信息
void ViewStudent(StuList *list)
{
    key *cur = list->front;
    while (cur != NULL)
    {
        cout << "学号: " << cur->stu.num << endl;
        cout << "姓名: " << cur->stu.name << endl;
        cout << "年龄: " << cur->stu.age << endl;
        cout << "所获得的学分：" << cur->stu.credits << endl;
        cout<<"所选课程："<<cur->stu.courses<<endl;
        cout << "-------------------" << endl;
        cur = cur->next;
    }
}

void DropCourseForStudent(StuList *list, unsigned long long studentNum, const string& courseName) {
    // 从课程中删除学生信息
    
    key *cur = list->front;
    course_vector courseList;
    while (cur != NULL) {
        if (cur->stu.num == studentNum) {
            // 将字符数组转为字符串处理
            string coursesStr = cur->stu.courses;
            vector<string> courses;
            stringstream ss(coursesStr);
            string course;
            
            // 按逗号分割课程
            while (getline(ss, course, ',')) {
                if (!course.empty()) {
                    courses.push_back(course);
                }
            }
            
            // 查找并删除课程
            bool found = false;
            for (auto it = courses.begin(); it != courses.end();) {
                if (*it == courseName) {
                    it = courses.erase(it);
                    found = true;
                    // 从课程中删除学生
                    int courseIndex = courseList.get_index_by_name(courseName);
                    if (courseIndex != -1) {
                        courseList.remove_student(courseIndex, studentNum);
                        // SaveCoursesToCSV(&courseList); // 保存课程信息
                    }
                } else {
                    ++it;
                }
            }
            
            // 重新组合课程字符串
            if (found) {
                string newCourses;
                for (size_t i = 0; i < courses.size(); ++i) {
                    if (i > 0) newCourses += ",";
                    newCourses += courses[i];
                }
                strncpy(cur->stu.courses, newCourses.c_str(), sizeof(cur->stu.courses));
                cur->stu.courses[sizeof(cur->stu.courses)-1] = '\0';
                SaveStudentsToCSV(list); // 保存学生信息
                return;
            }
            cout << "学生" << cur->stu.name << " 未选修课程 " << courseName << endl;
            return;
        }
        cur = cur->next;
    }
    cout << "未找到学号为 " << studentNum << " 的学生" << endl;
}

// stoi

void AddCourseForStudent(StuList *list, unsigned long long studentNum, const int courseIndex) {
    key *cur = list->front;
    while (cur != NULL) {
        if (cur->stu.num == studentNum) {
            // 通过课程序号验证课程存在性
            if (!courseList.find_by_index(courseIndex)) {
                cout << "错误：无效的课程序号" << endl;
                return;
            }

            // 将课程序号转为字符串用于比对
            string courseIndexStr = to_string(courseIndex);
            string coursesStr = cur->stu.courses;
            
            // 检查是否已选该课程（通过课程序号比对）
            if (coursesStr.find(courseIndexStr) != string::npos) {
                cout << "学生" << cur->stu.name << " 已选修该课程" << endl;
                return;
            }

            // 添加新课程（存储课程序号）
            if (strlen(cur->stu.courses) + courseIndexStr.length() + 1 < sizeof(cur->stu.courses)) {
                if (!coursesStr.empty()) {
                    strncat(cur->stu.courses, ",", sizeof(cur->stu.courses)-strlen(cur->stu.courses)-1);
                }
                strncat(cur->stu.courses, courseIndexStr.c_str(), sizeof(cur->stu.courses)-strlen(cur->stu.courses)-1);

                // 在课程对象中添加学生
                courseList.add_student(courseIndex, studentNum);
                
                SaveStudentsToCSV(list);
                cout << "选课成功" << endl;
                return;
            } else {
                cout << "错误：课程列表已满，无法添加新课程" << endl;
                return;
            }
        }
        cur = cur->next;
    }
    cout << "未找到学号为 " << studentNum << " 的学生" << endl;
}
