#ifndef STUDENT_H
#define STUDENT_H

#include <iostream>
#include <string>
#include "HashTable.h"
using namespace std;

//学生结构
struct Student
{
    unsigned long long num;
    char name[32];
    int age;
    int credits;
    char courses[50];
};

//节点
struct key
{
    Student stu;
    key *next;

};

//链表
struct StuList
{
    key *front;//头指针
    int size;//当前节点数量
};
void SaveStudentsToCSV(StuList *list);
void LoadStudentsFromCSV(StuList *list);
//增加学生信息
void entryStudent(StuList *list);

//更改学生信息
void ChangeStudent(StuList *list);
void ChangeStudentnum(StuList *list);
//查找学生信息
key* SearchStudent(StuList *list);
key* SearchStudentnum(StuList *list);
//删除学生信息
void DeleteStudent(StuList  *list);
void DeleteStudentnum(StuList  *list);
///查看学生信息
void ViewStudent(StuList *list);

void DropCourseForStudent(StuList *list, unsigned long long studentNum, const string& courseName);
void AddCourseForStudent(StuList *list, unsigned long long studentNum, const int courseIndex);

extern HashTable<unsigned long long, Student*> studentHashByNum;  // 学号作为键
extern HashTable<std::string, Student*> studentHashByName;        // 姓名作为键

#endif