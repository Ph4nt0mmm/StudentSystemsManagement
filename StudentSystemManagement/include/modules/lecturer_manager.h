#ifndef LECTURER_MANAGER_H
#define LECTURER_MANAGER_H

#include "../modules/lecturer.h"
#include <stdbool.h>

#define MAX_LECTURERS 200

// Lecturer manager structure
typedef struct {
    Lecturer* lecturers[MAX_LECTURERS];
    int lecturer_count;
    char data_file[256];
} LecturerManager;

// Function declarations
LecturerManager* lecturer_manager_create(const char* data_file);
void lecturer_manager_destroy(LecturerManager* manager);
bool lecturer_manager_add(LecturerManager* manager, Lecturer* lecturer);
bool lecturer_manager_remove(LecturerManager* manager, int lecturer_id);
Lecturer* lecturer_manager_find_by_id(LecturerManager* manager, int lecturer_id);
Lecturer* lecturer_manager_find_by_code(LecturerManager* manager, const char* lecturer_code);
Lecturer** lecturer_manager_find_by_name(LecturerManager* manager, const char* name, int* count);
Lecturer** lecturer_manager_find_by_department(LecturerManager* manager, Department department, int* count);
bool lecturer_manager_save_to_file(LecturerManager* manager);
bool lecturer_manager_load_from_file(LecturerManager* manager);
int lecturer_manager_get_count(LecturerManager* manager);
bool lecturer_manager_assign_course(LecturerManager* manager, int lecturer_id, int course_id);
bool lecturer_manager_remove_course(LecturerManager* manager, int lecturer_id, int course_id);
int* lecturer_manager_get_courses(LecturerManager* manager, int lecturer_id, int* count);
bool lecturer_manager_check_workload(LecturerManager* manager, int lecturer_id, float additional_hours);
Lecturer** lecturer_manager_get_available_lecturers(LecturerManager* manager, Department department, float max_hours, int* count);
bool lecturer_manager_update_teaching_hours(LecturerManager* manager, int lecturer_id, float hours);
bool lecturer_manager_generate_schedule(LecturerManager* manager, int lecturer_id, const char* schedule_file);

#endif // LECTURER_MANAGER_H