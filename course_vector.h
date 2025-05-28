
#ifndef COURSE_VECTOR_H
#define COURSE_VECTOR_H
#include <string>
#include <vector>
#include "HashTable.h"

typedef struct Course* Tree;

struct Course_date {
    std::string name;
    int index;
    int credit;  // 新增学分字段
    unsigned long long teacher_id;
    std::vector<int> students;
    std::string time;
    std::string week;
};
struct Course {
    Course_date date;
    Course* left;
    Course* right;
};

void freeCourseTree(Tree root);
bool inorder_find(Tree node, const std::string& goal);
void inorder_print(Tree node);
int inorder_return_index(Tree node, const std::string& name);

class course_vector {
private:
    Tree course_head;
    int len;
    void saveInOrder(Tree node, std::ofstream& file);
    bool is_duplicate(const Course_date& c);
    HashTable<int, Course_date*> courseHashByIndex;
    HashTable<std::string, Course_date*> courseHashByName;

public:
    course_vector();
    ~course_vector();

    Tree get_root();
    void push(Course_date c);
    bool find_by_index(int goal);
    bool find_by_name(std::string goal);
    void print_all_courses();
    void delete_by_index(int goal);
    void delete_by_name(std::string name);
    std::string get_name_by_index(int goal);
    int get_index_by_name(std::string name);
    bool update_course(int old_index, int new_index, const std::string& new_name,unsigned long long new_teacher_id);
    void add_student(int course_index, int student_id);
    void remove_student(int course_index, int student_id);
    HashTable<int, Course_date*>& getCourseHashByIndex();
    HashTable<std::string, Course_date*>& getCourseHashByName();
    Course* get_Course_by_index(int courseIndex);
};

#endif