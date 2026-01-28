#ifndef STUDENT_H
#define STUDENT_H

#include <stdbool.h>

// Student status
typedef enum {
    STUDENT_ACTIVE,
    STUDENT_SUSPENDED,
    STUDENT_GRADUATED,
    STUDENT_WITHDRAWN
} StudentStatus;

// Student structure
typedef struct {
    int id;
    char student_code[20];
    char full_name[100];
    char date_of_birth[11];  // YYYY-MM-DD
    char gender[10];
    char address[200];
    char email[100];
    char phone[20];
    char enrollment_date[11]; // YYYY-MM-DD
    char major[50];
    int current_semester;
    float gpa;
    int total_credits;
    StudentStatus status;
    char advisor[100];
    char emergency_contact[100];
    char emergency_phone[20];
} Student;

// Function declarations
Student* student_create(int id, const char* student_code, const char* full_name,
                       const char* date_of_birth, const char* gender);
void student_destroy(Student* student);
void student_update_profile(Student* student, const char* address, 
                          const char* email, const char* phone);
bool student_enroll_course(Student* student, int course_id);
bool student_drop_course(Student* student, int course_id);
float student_calculate_gpa(Student* student);
bool student_check_graduation_eligibility(Student* student);
void student_update_academic_status(Student* student, float new_gpa, int new_credits);
const char* student_status_to_string(StudentStatus status);
StudentStatus string_to_student_status(const char* status_str);
bool student_validate_code(const char* student_code);

#endif // STUDENT_H
