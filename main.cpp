#include <iostream>
#include <windows.h>
#include <string>
#include "Student.cpp"
#include "Teacher.cpp"
#include "course_vector.cpp"
#include "Student.h"
#include "Teacher.h"
#include "course_vector.h"
#include "course_graph.cpp"
using namespace std;

// 全局数据声明
StuList studentList = {nullptr, 0};
List teacherList = {nullptr, 0};
course_vector courseList;
CourseGraph graph =construct();
// 函数声明
void loadAllData();
int menu();
void studentMenu(unsigned long long studentId);
void teacherMenu(unsigned long long teacherId);
void displayAdminMenu();
void displayStudentMenu();
void displayTeacherMenu();
void displayGStudentMenu();
void displayGTeacherMenu();
void displayGCourseMenu();
void drawcourse();
void drawStudent();
void drawTeacher();
int glymenu();
int main() {
    
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);
    loadAllData(); // 加载所有数据
    while (1)
    {
        int t1 = menu();
        switch (t1)
        {
        case 0:
            cout << "感谢使用" << endl;
            Sleep(1000);
            return 0;
            break;
        case 1:
            int id;
            cout << "请输入学号：";
            cin >> id;
            Student* stu;
            if (studentHashByNum.find(id, stu)) {
                 studentMenu(id);
            } else {
                cout << "学号不存在！" << endl;
            }
            break;
        case 2:
            cout << "请输入工号：";
            cin >> id;
            Teacher* tea;
            if (teacherHashByNum.find(id, tea)) {
                teacherMenu(id); 
            }
            else {
                cout << "工号不存在！" << endl; 
            }
            break;
        case 3:
            cout << "请输入管理员工号：";
            cin >> id;
            if (teacherHashByNum.find(id, tea)) 
            {
            int t = glymenu();
            switch (t)
            {
                case 0:
                    cout << "感谢使用" << endl;
                    Sleep(1000);
                    // return 0;
                    break;
                case 1:
                    drawTeacher();
                    break;
                case 2:
                    drawStudent();
                    break;
                case 3:
                    drawcourse();
                    break;
            }
            } 
            else 
            {
                cout << "工号不存在！" << endl;
            }
            break;
        default:
            cout << "无效选项！" << endl;
        }
    }
    // 退出时自动保存数据（依赖析构函数）
    return 0;
}

void loadAllData() {
    LoadStudentsFromCSV(&studentList);
    LoadTeachersFromCSV(&teacherList,courseList.get_root());
    // courseList的构造器已自动加载课程数据
}
int menu()
{
    cout << "*****************************************************************" << endl;
    cout << "*                 	 欢迎来到使用教务系统V1.0                 	*" << endl;
    cout << "*****************************************************************" << endl;
    cout << "*                   请选择要操作的模块                   	*" << endl;
    cout << "*****************************************************************" << endl;
    cout << "*                     1. 学生系统                     		*" << endl;
    cout << "*****************************************************************" << endl;
    cout << "*                     2. 教师系统                     		*" << endl;
    cout << "*****************************************************************" << endl;
    cout << "*                     3.管理员系统                    		*" << endl;
    cout << "*****************************************************************" << endl;
    cout << "*                     0. 退出系统                     		*" << endl;
    cout << "*****************************************************************" << endl;
    cout << "请选择你要进入的系统：";
    int select = -1;
    cin >> select;
    return select;
}
int glymenu()
{
    cout << "*****************************************************************" << endl;
    cout << "*                 	 欢迎来到管理员系统                 	*" << endl;
    cout << "*****************************************************************" << endl;
    cout << "*                   请选择要操作的模块                   	*" << endl;
    cout << "*****************************************************************" << endl;
    cout << "*                     1. 教师管理系统                     	*" << endl;
    cout << "*****************************************************************" << endl;
    cout << "*                     2. 学生管理系统                     	*" << endl;
    cout << "*****************************************************************" << endl;
    cout << "*                     3. 课程管理系统                     	*" << endl;
    cout << "*****************************************************************" << endl;
    cout << "*                     0. 退出系统                     		*" << endl;
    cout << "*****************************************************************" << endl;
    cout << "请选择你要执行的功能：";
    int select = -1;
    cin >> select;
    return select;
}
void studentMenu(unsigned long long studentId) {
    Student* stu;
    studentHashByNum.find(studentId, stu);
    
    while (true) {
        displayStudentMenu();
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                // 查看个人信息
                cout << "\n学号: " << stu->num << endl;
                cout << "姓名: " << stu->name << endl;
                cout << "年龄: " << stu->age << endl;
                cout << "学分: " << stu->credits << endl;
                cout << "已选课程: " << stu->courses << endl;
                break;
            }
            case 2: {
                // 查看老师
                ViewTeachers(&teacherList);
                break;
            }
            case 3: {
                // 查看所有课程信息
                cout << "\n--- 所有课程 ---" << endl;
                courseList.print_all_courses();
                break; 
            }
            case 4: {
                // 选课
                cout << "输入课程编号：";
                int courseIndex;
                cin >> courseIndex;
                courseList.add_student(courseIndex, studentId);
                AddCourseForStudent(&studentList, studentId, courseIndex);
                break;
            }
            case 5: {
                // 退课
                cout << "输入课程名称：";
                string courseName;
                cin.ignore();
                getline(cin, courseName);
                DropCourseForStudent(&studentList, studentId, courseName);
                break;
            }
            case 6: {
                // 查看所有课程先修课
                graph=construct();
                graph.printGraph();
                break; 
            }
            case 7: {
                // 查询某个课程的先修课
                string courseName;
                cout << "输入课程名称：";
                cin.ignore();
                getline(cin, courseName);

                graph.getPres(courseName); 
                break;
            }
            case 8: {
                // 查询某两门课程是否构成先修关系
                string p1, p2;
                cout << "输入第一门课程名称：";
                cin.ignore();
                getline(cin, p1);
                cout << "输入第二门课程名称：";
                getline(cin, p2);
                cout << "\n=== 先修关系验证 ===\n";
                graph.hasPre(p1, p2);
                cout << endl; 
                break;
            }
            case 9: {
                    string c1, c2;
                    cout << "请输入起始课程名称：";
                    cin.ignore(numeric_limits<streamsize>::max(), '\n'); // 清除之前残留的换行符
                    getline(cin, c1);
                    cout << "请输入毕业课程名称：";
                    getline(cin, c2);
                    graph.findShortestPathByCredit(c1, c2);
                    break;
                    }
            case 0:
                return;
            default:
                cout << "无效选项！" << endl;
        }
    }
}
void teacherMenu(unsigned long long teacherId) {
    Teacher* tea;
    teacherHashByNum.find(teacherId, tea);
    while (true) {
        displayTeacherMenu();
        int choice;
        cin >> choice;

        switch (choice) {
            case 1: {
                // 查看个人信息
                cout << "\n工号: " << tea->num << endl;
                cout << "姓名: " << tea->name << endl;
                cout << "年龄: " << tea->age << endl;
                cout << "所教课程: " << tea->classid<<" "<<tea->teach << endl;
                break;
            }
            case 2: {
               //查询教学信息
                TeachInformation(&teacherList,courseList.get_root(),studentList,teacherId);
               break; 
            }
            case 3: {
                // 查看所有课程信息
                cout << "\n--- 所有课程 ---" << endl;
                courseList.print_all_courses();
                break; 
            }
            case 0:
                return;
            default:
                cout << "无效选项！" << endl;
        }
    }

}
void displayAdminMenu() {
    cout << "\n======= 管理员菜单 =======" << endl;
    cout << "1. 查看所有学生" << endl;
    cout << "2. 查看所有教师" << endl;
    cout << "3. 查看所有课程" << endl;
    cout << "4. 添加新课程" << endl;
    cout << "5. 删除课程" << endl;
    cout<<  "6. 添加学生信息"<<endl;
    cout<<  "7. 删除学生信息"<<endl;
    cout<<  "8. 修改学生信息"<<endl;
    cout<<  "9. 查看学生信息"<<endl;
    cout<<  "10. 添加教师信息"<<endl;
    cout<<  "11. 删除教师信息"<<endl;
    cout<<  "12. 修改教师信息"<<endl;
    cout<<  "13. 查看教师信息"<<endl;
    cout<<  "6. 查看所有课程先修课"<<endl;
    cout<<  "7. 查询某个课程的先修课"<<endl;
    cout << "8. 查询某两门课程是否构成先修关系" << endl;
    cout << "0. 返回上级菜单" << endl;
    cout << "请选择操作：";
}

void displayStudentMenu() {
    cout << "\n======= 学生菜单 =======" << endl;
    cout << "1. 查看个人信息" << endl;
    cout << "2. 查看所有教师" << endl;
    cout << "3. 查看所有课程" << endl;
    cout << "4. 选课" << endl;
    cout << "5. 退课" << endl;
    cout<<  "6. 查看所有课程先修课"<<endl;
    cout<<  "7. 查询某个课程的先修课"<<endl;
    cout << "8. 查询某两门课程是否构成先修关系" << endl;
    cout << "9. 最优学习路径规划" << endl;
    cout << "0. 返回上级菜单" << endl;
    cout << "请选择操作：";
}
void displayTeacherMenu() {
    cout<<"\n======= 教师菜单 ======="<<endl;
    cout<<"1. 查看个人信息"<<endl;
    cout<<"2. 查询教学信息"<<endl;
    cout<<"3. 查看所有课程"<<endl;
    cout<<"0. 返回上级菜单"<<endl;
}
void displayCourseMenu() {
    cout << "\n欢迎进入课程管理系统" << endl;
    cout << "1. 增加课程" << endl;
    cout << "2. 通过编号查找课程" << endl;
    cout << "3. 通过名称查找课程" << endl;
    cout << "4. 通过编号删除课程" << endl;
    cout << "5. 通过名称删除课程" << endl;
    cout << "6. 修改课程" << endl;
    cout << "7. 查看所有的课程信息" << endl;
    cout << "8. 退出系统" << endl;
    cout << "请输入你的操作：";
}

void displayGTeacherMenu()
{
    cout << "\n欢迎进入教师管理系统" << endl;
    cout << "1. 查看教师信息" << endl;
    cout << "2. 录入教师信息" << endl;
    cout << "3. 通过编号删除教师信息" << endl;
    cout << "4. 通过姓名删除教师信息" << endl;
    cout << "5. 通过编号修改教师信息" << endl;
    cout << "6. 通过姓名修改教师信息" << endl;
    cout << "7. 通过编号查找教师信息" << endl;
    cout << "8. 通过姓名查找教师信息" << endl;
    cout << "9. 退出系统" << endl;
    cout << "请输入你的操作：";
}

void displayGStuentMenu()
{
    cout << "\n欢迎进入学生管理系统" << endl;
    cout << "1. 查看学生信息" << endl;
    cout << "2. 录入学生信息" << endl;
    cout << "3. 通过编号删除学生信息" << endl;
    cout << "4. 通过姓名删除学生信息" << endl;
    cout << "5. 通过编号修改学生信息" << endl;
    cout << "6. 通过姓名修改学生信息" << endl;
    cout << "7. 通过编号查找学生信息" << endl;
    cout << "8. 通过姓名查找学生信息" << endl;
    cout << "9. 退出系统" << endl;
    cout << "请输入你的操作：";
}
void drawcourse() {
    int choice;

    while (true) {
        displayCourseMenu();
        cin >> choice;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        switch (choice) {
        case 1: {
            Course_date c;
            cout << "请输入课程名称: ";
            getline(cin, c.name);
            cout << "请输入课程编号: ";
            cin >> c.index;
            cin.ignore();
            courseList.push(c);
            cout << "添加成功" << endl;
            break;
        }
        case 2: {
            int index;
            cout << "请输入你要查找的课程编号: ";
            cin >> index;
            cin.ignore();
            if (courseList.find_by_index(index)) {
                cout << "课程名称: " << courseList.get_name_by_index(index) << endl;
                CourseSearch(courseList.get_root(), index, studentList);
            }
            else {
                cout << "未找到该课程" << endl;
            }
            break;
        }
        case 3: {
            string name;
            cout << "请输入你要查找的课程名称: ";
            getline(cin, name);
            if (courseList.find_by_name(name)) {
                cout << "课程编号: " << courseList.get_index_by_name(name) << endl;
            }
            break;
        }
        case 4: {
            int index;
            cout << "请输入你要删除的课程编号: ";
            cin >> index;
            cin.ignore();
            courseList.delete_by_index(index);
            break;
        }
        case 5: {
            string name;
            cout << "请输入你要删除的课程名称: ";
            getline(cin, name);
            courseList.delete_by_name(name);
            break;
        }
        case 6: {
            int old_idx, new_idx;
            unsigned long long new_teacher_id;
            string new_name;
            cout << "请输入旧编号: ";
            cin >> old_idx;
            cout << "请输入新编号: ";
            cin >> new_idx;
            cin.ignore();
            cout << "请输入新名称: ";
            getline(cin, new_name);
            cout<<"请输入新教师工号: ";
            cin>>new_teacher_id;
            cin.ignore();
            if (courseList.update_course(old_idx, new_idx, new_name,new_teacher_id)) {
                cout << "修改成功" << endl;
            }
            break;
        }
        case 7:
            courseList.print_all_courses();
            break;
        case 8:
            cout << "正在退出系统" << endl;
            return;
        default:
            cout << "输入无效" << endl;
        }
    }
    return;
}

void drawTeacher()
{
    LoadTeachersFromCSV(&teacherList,courseList.get_root());
    int n;
    while (true)
    {
        displayGTeacherMenu();
        cin >> n;
        switch (n)
        {
        case 1:
            ViewTeachers(&teacherList);
            break;
        case 2:
            entryTeacher(&teacherList);
            break;
        case 3:
            DeleteTeachernum(&teacherList);
            break;
        case 4:
            DeleteTeacher(&teacherList);                               
            break;
        case 5:
            ChangeTeachernum(&teacherList);
            break;
        case 6:
            ChangeTeacher(&teacherList);
            break;
        case 7:
        {
            Node* node = SearchTeachernum(&teacherList);
            if (!node)
            {
                cout << "没有找到相关信息" << endl;
            }
            else
            {
                cout << "编号: " << node->tea.num << endl;
                cout << "姓名: " << node->tea.name << endl;
                cout << "年龄: " << node->tea.age << endl;
                cout << "所授课程: " << node->tea.teach << endl;
            }
            break;
        }
        case 8:
        {
            Node* node = SearchTeacher(&teacherList);
            if (!node)
            {
                cout << "没有找到相关信息" << endl;
            }
            else
            {
                cout << "编号: " << node->tea.num << endl;
                cout << "姓名: " << node->tea.name << endl;
                cout << "年龄: " << node->tea.age << endl;
                cout << "所授课程: " << node->tea.teach << endl;
            }
            break;
        }
        case 9:
            cout << "正在退出系统" << endl;
            return;
        default:
            cout << "输入错误，请重新输入" << endl;
            break;
        }
    }
    return;
}

void drawStudent()
{
    int n;
    while (true)
    {
        displayGStuentMenu();
        cin >> n;
        switch (n)
        {
        case 1:
            ViewStudent(&studentList);
            break;
        case 2:
            entryStudent(&studentList);
            break;
        case 3:
            DeleteStudentnum(&studentList);
            break;
        case 4:
            DeleteStudent(&studentList);
            break;
        case 5:
            ChangeStudentnum(&studentList);
            break;
        case 6:
            ChangeStudent(&studentList);
            break;
        case 7:
        {
            key *node = SearchStudentnum(&studentList);
            if (!node)
            {
                cout << "没有找到相关信息" << endl;
            }
            else
            {
                cout << "学号: " << node->stu.num << endl;
                cout << "姓名: " << node->stu.name << endl;
                cout << "年龄: " << node->stu.age << endl;
                cout << "所获得的学分: " << node->stu.credits << endl;
                cout << "所选课程：" << node->stu.courses << endl;
            }
            break;
        }
        case 8:
        {
            key *node = SearchStudent(&studentList);
            if (!node)
            {
                cout << "没有找到相关信息" << endl;
            } 
            else
            {
                cout << "学号: " << node->stu.num << endl;
                cout << "姓名: " << node->stu.name << endl;
                cout << "年龄: " << node->stu.age << endl;
                cout << "所获得的学分: " << node->stu.credits << endl;
                cout << "所选课程：" << node->stu.courses << endl;
            }
            break;
        }
        case 9:
            cout << "正在退出系统" << endl;
            return;
        default:
            cout << "输入错误，请重新输入" << endl;
            break;
        }
    }
    return;
}

// terminate called after throwing an instance of 'std::invalid_argument'
//   what():  stoi