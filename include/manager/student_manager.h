#ifndef STUDENT_MANAGER_H
#define STUDENT_MANAGER_H

#include "../modules/student.h"
#include <stdbool.h>

#define MAX_STUDENTS 1000

// Student manager structure
typedef struct {
    Student* students[MAX_STUDENTS];
    int student_count;
    char data_file[256];
} StudentManager;

// Function declarations
StudentManager* student_manager_create(const char* data_file);
void student_manager_destroy(StudentManager* manager);
bool student_manager_add(StudentManager* manager, Student* student);
bool student_manager_remove(StudentManager* manager, int student_id);
Student* student_manager_find_by_id(StudentManager* manager, int student_id);
Student* student_manager_find_by_code(StudentManager* manager, const char* student_code);
Student** student_manager_find_by_name(StudentManager* manager, const char* name, int* count);
Student** student_manager_find_by_major(StudentManager* manager, const char* major, int* count);
bool student_manager_save_to_file(StudentManager* manager);
bool student_manager_load_from_file(StudentManager* manager);
int student_manager_get_count(StudentManager* manager);
Student** student_manager_get_all(StudentManager* manager, int* count);
bool student_manager_update_gpa(StudentManager* manager, int student_id, float new_gpa);
Student** student_manager_get_honor_students(StudentManager* manager, float min_gpa, int* count);
int student_manager_count_by_status(StudentManager* manager, StudentStatus status);
bool student_manager_generate_report(StudentManager* manager, const char* report_file);
bool student_manager_backup_data(StudentManager* manager, const char* backup_file);

#endif // STUDENT_MANAGER_H
