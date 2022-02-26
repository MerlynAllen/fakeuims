#include "curriculum.h"

int initData()
{
    TEACHERS = makeDLinkedList();
    STUDENTS = makeDLinkedList();
    COURSES = makeDLinkedList();
    ROOMS = makeDLinkedList();
    ENROLLMENTS = makeDLinkedList();
    REGISTRYS = makeDLinkedList();
}

int loadData()
{
    // load courses
    FILE *fp = fopen("courses", "r");
    Course course;
    for (; fscanf(fp, "%s:%d:%s:%d:%d:%d:%d:%d:%d:%d\n", course.name, &course.courseid, course.description, &course.credits, &course.capacity, &course.time.weekstart, &course.time.weekend, &course.time.weekday, &course.time.timestart, &course.time.timeend) != EOF;)
    {
        Course *newcourse = calloc(1, sizeof(Course));
        memcpy(newcourse, &course, sizeof(Course));
        appendNode(COURSES, newcourse);
    }
    fclose(fp);
    // load rooms
    fp = fopen("rooms", "r");
    Room room;
    for (; fscanf(fp, "");)
}

bool checkRoomid(void *data, void *roomid)
{
    Room *room = (Room *)data;
    uint32_t id = *(uint32_t *)roomid;
    return room->roomid == id;
}

bool checkTimeConflict(void *data, void *time)
{
    CTime *exist = &((Course *)data)->time;
    CTime *t = (CTime *)time;
    if ((t->weekend >= exist->weekstart) && (exist->weekend >= t->weekstart)) // week collision
    {
        if ((t->weekday == exist->weekday) && (t->timestart <= exist->timeend) && (exist->timestart <= t->timeend)) // day collision
        {
            return true;
        }
    }
    return false;
}

int userAddCourse()
{
    Course *course = calloc(1, sizeof(Course));
    char *coursename[MAX_LEN] = {0};
    char *description[MAX_LEN] = {0};
    printf("Course name: ");
    scanf("%s", coursename);
    while (getchar() != '\n')
        ;
    printf("Description: ");
    scanf("%s", description);
    while (getchar() != '\n')
        ;
    printf("Credits: ");
    scanf("%d", &course->credits);
    while (getchar() != '\n')
        ;
inputroomid:
    printf("Roomid: ");
    uint32_t roomid;
    scanf("%d", &roomid);
    seekNode(ROOMS, checkRoomid, &roomid);
    if (ROOMS->ptr == ROOMS->tail)
    {
        printf("Error: roomid not found. Retry.\n");
        goto inputroomid;
    }
inputcapacity:
    printf("Capacity: ");
    scanf("%d", &course->capacity);
    if (course->capacity > ((Room *)ROOMS->ptr->data)->capacity)
    {
        printf("Error: capacity is larger than room capacity. Retry.\n");
        goto inputcapacity;
    }
timeschedule:
    printf("Week start: ");
    scanf("%d", &course->time.weekstart);
    printf("Week end: ");
    scanf("%d", &course->time.weekend);
    printf("Week: ");
    scanf("%d", &course->time.weekday);
    printf("Class start at: ");
    scanf("%d", &course->time.timestart);
    printf("Class end at: ");
    scanf("%d", &course->time.timeend);
    // check conflict
    if (seekNode(COURSES, checkTimeConflict, &course->time))
    {
        printf("Error: time conflict. Retry.\n");
        goto timeschedule;
    }
    course->room = ROOMS->ptr->data;
    course->courseid = COURSES->count;
    appendNode(COURSES, course);
    printf("Course added.\n");
}

int addCourse(char *name, char *description, uint32_t credits, uint32_t capacity, CTime time, uint32_t roomid)
{
    Course *course = calloc(1, sizeof(Course));
    course->courseid = COURSES->count;
    strcpy(course->name, name);
    strcpy(course->description, description);
    course->credits = credits;
    course->capacity = capacity;
    course->time = time;
    seekNode(ROOMS, &checkRoomid, &roomid);
    course->room = (Room *)ROOMS->ptr->data;
    appendNode(COURSES, course);
}
int userAddRoom()
{
    Room *room = calloc(1, sizeof(Room));
    char *name[MAX_LEN] = {0};
    printf("Room name: ");
    scanf("%s", name);
    while (getchar() != '\n')
        ;
    printf("Capacity: ");
    scanf("%d", &room->capacity);
    while (getchar() != '\n')
        ;
    room->roomid = ROOMS->count;
    appendNode(ROOMS, room);
    printf("Room added.\n");
}

int addRoom(char *name, uint32_t capacity)
{
    Room *room = calloc(1, sizeof(Room));
    room->roomid = ROOMS->count;
    strcpy(room->name, name);
    room->capacity = capacity;
    initDLinkedList(&room->courses);
    appendNode(ROOMS, room);
}

int addStudent(char *name, uint32_t studentid)
{
    Student *student = calloc(1, sizeof(Student));
    student->studentid = STUDENTS->count;
    strcpy(student->name, name);
    initDLinkedList(&student->enrollment);
    appendNode(STUDENTS, student);
}

int addTeacher(char *name, uint32_t teacherid)
{
    Teacher *teacher = calloc(1, sizeof(Teacher));
    teacher->teacherid = TEACHERS->count;
    strcpy(teacher->name, name);
    appendNode(TEACHERS, teacher);
}
bool checkUserid(void *data, void *userid)
{
    UserInfo *user = (UserInfo *)data;
    uint32_t id = *(uint32_t *)userid;
    return user->userid == id;
}

bool checkCourseid(void *data, void *courseid)
{
    Course *course = (Course *)data;
    uint32_t id = *(uint32_t *)courseid;
    return course->courseid == id;
}

int userEnroll() // student
{
    uint32_t studentid = uid2subid(USERID);
    uint32_t courseid;
    // if students
    if (isStudent)
    {
    inputcourseid:
        printf("Courseid: ");
        scanf("%d", &courseid);
        while (getchar() != '\n')
            ;
        if (seekNode(COURSES, checkCourseid, &courseid) == 0)
        {
            printf("Error: course not found. Retry.\n");
            goto inputcourseid;
        }
        if (((Course *)COURSES->ptr->data)->capacity <= ((Course *)COURSES->ptr->data)->enrollment.count)
        {
            printf("Sorry, the course is full. \n");
            return -1;
        }
        enroll(studentid, courseid);
        printf("Enrolled.\n");
        return 0;
    }
    else
    {
        printf("Sorry, but you are not a student.\n");
        return -1;
    }
}

int enroll(uint32_t userid, uint32_t courseid) // student
{
    Enrollment *enrollment = calloc(1, sizeof(Enrollment));
    enrollment->studentid = userid;
    enrollment->courseid = courseid;
    seekNode(STUDENTS, checkUserid, &userid);
    seekNode(COURSES, checkCourseid, &courseid);
    Student *student = (Student *)STUDENTS->ptr->data;
    Course *course = (Course *)COURSES->ptr->data;
    appendNode(&student->enrollment, enrollment);
    appendNode(&course->enrollment, enrollment);
    appendNode(ENROLLMENTS, enrollment);
}

int userCourseRegister()
{
    uint32_t teacherid = uid2subid(USERID);
    uint32_t courseid;
    // if teachers
    if (isTeacher)
    {
    inputcourseid:
        printf("Courseid: ");
        scanf("%d", &courseid);
        while (getchar() != '\n')
            ;
        if (seekNode(COURSES, checkCourseid, &courseid) == 0)
        {
            printf("Error: course not found. Retry.\n");
            goto inputcourseid;
        }
        if (((Course *)COURSES->ptr->data)->capacity <= ((Course *)COURSES->ptr->data)->enrollment.count)
        {
            printf("Sorry, the course is full. \n");
            return -1;
        }
        courseRegister(teacherid, courseid);
        printf("Course registered.\n");
        return 0;
    }
    else
    {
        printf("Sorry, but you are not a teacher.\n");
        return -1;
    }
}
int courseRegister(uint32_t userid, uint32_t courseid) // teacher
{
    Registry *registry = calloc(1, sizeof(Registry));
    registry->courseid = courseid;
    registry->teacherid = userid;
    seekNode(TEACHERS, checkUserid, &userid);
    seekNode(COURSES, checkCourseid, &courseid);
    Teacher *teacher = (Teacher *)TEACHERS->ptr->data;
    Course *course = (Course *)COURSES->ptr->data;
    appendNode(&teacher->registry, registry);
    appendNode(&course->teachers, registry);
    appendNode(REGISTRYS, registry);
}

int saveData()
{
    // save Courses
    FILE *fpCourse = fopen("courses", "w");
    if (fpCourse == NULL)
    {
        printf("Error opening courses file\n");
        return -1;
    }

    for (int i = 0; i < COURSES->count; i++)
    {
        Course *course = (Course *)COURSES->ptr->data;
        fprintf(fpCourse, "%s:%d:%s:%d:%d:%d:%d:%d:%d:%d\n", course->name, course->courseid, course->description, course->credits, course->capacity, course->time.weekstart, course->time.weekend, course->time.weekday, course->time.timestart, course->time.timeend);
        moveNext(COURSES);
    }

    fclose(fpCourse);
    // save Rooms
    FILE *fpRoom = fopen("rooms", "w");
    if (fpRoom == NULL)
    {
        printf("Error opening rooms file\n");
        return -1;
    }

    for (int i = 0; i < ROOMS->count; i++)
    {
        Room *room = (Room *)ROOMS->ptr->data;
        fprintf(fpRoom, "%s:%d:%d\n", room->name, room->roomid, room->capacity);
        moveNext(ROOMS);
    }
    // save Enrollments
    FILE *fpEnrollment = fopen("enrollments", "w");
    if (fpEnrollment == NULL)
    {
        printf("Error opening enrollments file\n");
        return -1;
    }

    for (int i = 0; i < ENROLLMENTS->count; i++)
    {
        Enrollment *enrollment = (Enrollment *)ENROLLMENTS->ptr->data;
        fprintf(fpEnrollment, "%d:%d\n", enrollment->studentid, enrollment->courseid);
        moveNext(ENROLLMENTS);
    }
    // save Registries
    FILE *fpRegistry = fopen("registries", "w");
    if (fpRegistry == NULL)
    {
        printf("Error opening registries file\n");
        return -1;
    }

    for (int i = 0; i < REGISTRYS->count; i++)
    {
        Registry *registry = (Registry *)REGISTRYS->ptr->data;
        fprintf(fpRegistry, "%d:%d\n", registry->courseid, registry->teacherid);
        moveNext(REGISTRYS);
    }
    return 0;
}