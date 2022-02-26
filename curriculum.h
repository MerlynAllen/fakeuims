#ifndef CURRICULUM
#define CURRICULUM
#include "limits.h"
#include "lnklist.h"
#include "login.h"
#include "globals.h"
#include <stdint.h>

typedef struct CTime
{
    uint8_t weekstart;
    uint8_t weekend;
    uint8_t weekday;
    uint8_t timestart;
    uint8_t timeend;
} CTime;

typedef struct Room
{
    uint32_t roomid;
    char name[MAX_LEN];
    uint32_t capacity;
    DLnklist courses;
} Room;

typedef struct Enrollment
{
    uint32_t studentid;
    uint32_t courseid;
} Enrollment;

typedef struct Registry
{
    uint32_t courseid;
    uint32_t teacherid;
} Registry;

typedef struct Course
{
    uint32_t courseid;
    char name[MAX_LEN];
    char description[MAX_LEN];
    uint32_t credits;
    DLnklist teachers; // teachers
    uint32_t capacity;
    DLnklist enrollment; // students
    CTime time;
    Room *room;
} Course;

typedef struct Teacher
{
    uint32_t teacherid;
    char name[MAX_LEN];
    DLnklist registry; // courses
} Teacher;

typedef struct Student
{
    uint32_t studentid;
    char name[MAX_LEN];
    DLnklist enrollment; // courses
} Student;

int initData();
int loadData();

int addCourse(char *name, char *description, uint32_t credits, uint32_t capacity, CTime time, uint32_t roomid);
int addRoom(char *name, uint32_t capacity);
int addStudent(char *name, uint32_t studentid);
int addTeacher(char *name, uint32_t teacherid);
int enroll(uint32_t userid, uint32_t courseid);

#endif