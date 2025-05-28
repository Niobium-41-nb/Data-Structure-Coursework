#ifndef TEACHER_H
#define TEACHER_H
#include <algorithm>
#include <iostream>
#include <string>
#include "HashTable.h"
#include "course_vector.h"
#include "Student.h"
using namespace std;

struct Teacher
{
    unsigned long long num;
    string name;
    int age;
    string teach;
    int classid;//新加
};

// 节点
struct Node
{
    Teacher tea;
    Node* next;
};

// 链表
struct List
{
    Node* front; // 头指针
    int size; // 当前节点数量
};
void SaveTeachersToCSV(List *list);
void LoadTeachersFromCSV(List *list,Tree root);
// 添加教师信息
void entryTeacher(List* list);

// 修改教师信息
void ChangeTeacher(List* list);
void ChangeTeachernum(List* list);
// 查找教师信息
Node* SearchTeacher(List* list);
Node* SearchTeachernum(List* list);
// 删除教师信息
void DeleteTeacher(List* list);
void DeleteTeachernum(List* list);
// 查看教师信息
void ViewTeachers(List* list);

extern HashTable<unsigned long long, Teacher*> teacherHashByNum;  // 教师编号作为键
extern HashTable<std::string, Teacher*> teacherHashByName;        // 姓名作为键

void ViewTeachers(List* list); 
void CourseSearch(Course* root, int index, StuList& allStudents);
//新加
//查询教授信息
void TeachInformation(List* teacherList, Course* courseRoot, StuList& studentList,unsigned long long teacherId);
#endif