#ifndef LECTURER_H
#define LECTURER_H

#include <stdbool.h>

// Lecturer rank
typedef enum {
    RANK_ASSISTANT_PROFESSOR,
    RANK_ASSOCIATE_PROFESSOR,
    RANK_PROFESSOR,
    RANK_SENIOR_LECTURER,
    RANK_LECTURER
} LecturerRank;

// Lecturer department
typedef enum {
    DEPT_COMPUTER_SCIENCE,
    DEPT_MATHEMATICS,
    DEPT_PHYSICS,
    DEPT_CHEMISTRY,
    DEPT_BIOLOGY,
    DEPT_ENGINEERING,
    DEPT_BUSINESS,
    DEPT_HUMANITIES
} Department;

// Lecturer structure
typedef struct {
    int id;
    char lecturer_code[20];
    char full_name[100];
    char date_of_birth[11];
    char gender[10];
    char email[100];
    char phone[20];
    char office[50];
    Department department;
    LecturerRank rank;
    char hire_date[11];
    char expertise[200];
    float teaching_hours;
    int max_courses;
    bool is_active;
    char office_hours[100];
} Lecturer;

// Function declarations
Lecturer* lecturer_create(int id, const char* lecturer_code, const char* full_name,
                         const char* date_of_birth, Department department);
void lecturer_destroy(Lecturer* lecturer);
void lecturer_update_profile(Lecturer* lecturer, const char* email, 
                           const char* phone, const char* office);
bool lecturer_assign_course(Lecturer* lecturer, int course_id);
bool lecturer_remove_course(Lecturer* lecturer, int course_id);
bool lecturer_check_availability(Lecturer* lecturer, float additional_hours);
void lecturer_update_teaching_hours(Lecturer* lecturer, float hours);
const char* lecturer_rank_to_string(LecturerRank rank);
const char* department_to_string(Department dept);
LecturerRank string_to_lecturer_rank(const char* rank_str);
Department string_to_department(const char* dept_str);

#endif // LECTURER_H
