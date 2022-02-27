#include "curriculum.h"

bool checkRoomid(void *data, void *roomid)
{
    Room *room = (Room *)data;
    uint32_t id = *(uint32_t *)roomid;
    return room->roomid == id;
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

bool checkTeacherid(void *data, void *teacherid)
{
    Teacher *teacher = (Teacher *)data;
    uint32_t id = *(uint32_t *)teacherid;
    return teacher->teacherid == id;
}

bool checkStudentid(void *data, void *studentid)
{
    Student *student = (Student *)data;
    uint32_t id = *(uint32_t *)studentid;
    return student->studentid == id;
}

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
    // load rooms
    FILE *fpRooms = fopen("rooms", "r");
    if (fpRooms == NULL)
    {
        printf("Cannot open rooms file.\n");
        return -1;
    }
    Room room;
    for (; fscanf(fpRooms, "%s %d %d", room.name, &room.roomid, &room.capacity) != EOF;)
    {
        Room *newroom = calloc(1, sizeof(Room));
        memcpy(newroom, &room, sizeof(Room));
        appendNode(ROOMS, newroom);
    }
    fclose(fpRooms);

    // load courses
    FILE *fpCourses = fopen("courses", "r");
    if (fpCourses == NULL)
    {
        printf("Cannot open courses file.\n");
        return -1;
    }

    Course course;
    uint32_t roomid;
    for (; fscanf(fpCourses, "%s %d %s %d %d %d %d %d %d %d %d\n", course.name, &course.courseid, course.description, &course.credits, &course.capacity, &course.time.weekstart, &course.time.weekend, &course.time.weekday, &course.time.timestart, &course.time.timeend, &roomid) != EOF;)
    {
        Course *newcourse = calloc(1, sizeof(Course));
        seekNode(ROOMS, checkRoomid, &roomid);
        newcourse->room = (Room *)ROOMS->ptr->data;
        Room *room = ROOMS->ptr->data;
        appendNode(&room->courses, newcourse);
        memcpy(newcourse, &course, sizeof(Course));
        appendNode(COURSES, newcourse);
    }
    fclose(fpCourses);

    // load enrollments
    FILE *fpEnrollment = fopen("enrollments", "r");
    if (fpEnrollment == NULL)
    {
        printf("Cannot open enrollments file.\n");
        return -1;
    }
    Enrollment enrollment;
    for (; fscanf(fpEnrollment, "%d %d", &enrollment.studentid, &enrollment.courseid) != EOF;)
    {
        Enrollment *newenrollment = calloc(1, sizeof(Enrollment));
        memcpy(newenrollment, &enrollment, sizeof(Enrollment));
        appendNode(ENROLLMENTS, newenrollment);

        seekNode(COURSES, checkCourseid, &enrollment.courseid);
        Course *course = COURSES->ptr->data;
        appendNode(&course->enrollment, newenrollment);
        seekNode(STUDENTS, checkStudentid, &enrollment.studentid);
        Student *student = STUDENTS->ptr->data;
        appendNode(&student->enrollment, newenrollment);
    }

    // load registries
    FILE *fpRegistry = fopen("registries", "r");
    if (fpRegistry == NULL)
    {
        printf("Cannot open registries file.\n");
        return -1;
    }
    Registry registry;
    for (; fscanf(fpRegistry, "%d %d", &registry.courseid, &registry.teacherid) != EOF;)
    {
        Registry *newregistry = calloc(1, sizeof(Registry));
        memcpy(newregistry, &registry, sizeof(Registry));
        appendNode(REGISTRYS, newregistry);

        seekNode(COURSES, checkCourseid, &registry.courseid);
        Course *course = COURSES->ptr->data;
        appendNode(&course->teachers, newregistry);
        seekNode(TEACHERS, checkTeacherid, &registry.teacherid);
        Teacher *teacher = TEACHERS->ptr->data;
        appendNode(&teacher->registry, newregistry);
    }
    fclose(fpRegistry);
}

int userAddCourse()
{
    Course *course = calloc(1, sizeof(Course));
    char coursename[MAX_LEN] = {0};
    char description[MAX_LEN] = {0};
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

    if (seekNode(ROOMS, checkRoomid, &roomid) == -1)
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
    switch (seekNode(COURSES, checkTimeConflict, &course->time) == 0)
    {
    case 1:
        printf("Error: empty list of room.\n");
        return -1;
        break;
    case 0:
        printf("Error: time conflict. Retry.\n");
        goto timeschedule;
        break;
    default:
        break;
    }
    course->room = ROOMS->ptr->data;
    course->courseid = COURSES->count;
    appendNode(COURSES, course);
    printf("Course added.\n");
    return 0;
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
    if (seekNode(ROOMS, &checkRoomid, &roomid) != 0)
    {
        printf("Error: roomid not found.\n");
        return -1;
    }
    course->room = (Room *)ROOMS->ptr->data;
    appendNode(COURSES, course);
}

int userAddRoom()
{
    Room *room = calloc(1, sizeof(Room));
    // char name[MAX_LEN] = {0};
    printf("Room name: ");
    scanf("%s", room->name);
    while (getchar() != '\n')
        ;
    printf("Capacity: ");
    scanf("%d", &room->capacity);
    while (getchar() != '\n')
        ;
    room->roomid = ROOMS->count;
    initDLinkedList(&room->courses);
    appendNode(ROOMS, room);
    printf("Room %s uid %d with capacity %d added.\n", room->name, room->roomid, room->capacity);
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
        printf("Enrolled course id %d successfully.\n", courseid);
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
        printf("Course %d registered.\n", courseid);
        return 0;
    }
    else
    {
        printf("Sorry, but you are not a teacher.\n");
        return -1;
    }
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
        fprintf(fpCourse, "%s %d %s %d %d %d %d %d %d %d %d\n", course->name, course->courseid, course->description, course->credits, course->capacity, course->time.weekstart, course->time.weekend, course->time.weekday, course->time.timestart, course->time.timeend, course->room->roomid);
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
        fprintf(fpRoom, "%s %d %d\n", room->name, room->roomid, room->capacity);
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
        fprintf(fpEnrollment, "%d %d\n", enrollment->studentid, enrollment->courseid);
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
        fprintf(fpRegistry, "%d %d\n", registry->courseid, registry->teacherid);
        moveNext(REGISTRYS);
    }
    return 0;
}

int listCourses()
{
    if (COURSES->count == 0)
    {
        printf("No courses found.\n");
        return -1;
    }
    printf("Courses:\n");
    for (int i = 0; i < COURSES->count; i++)
    {
        Course *course = (Course *)COURSES->ptr->data;
        printf("%d: %s\n", course->courseid, course->name);
        moveNext(COURSES);
    }
    return 0;
}

int listRooms()
{
    if (ROOMS->count == 0)
    {
        printf("No rooms found.\n");
        return -1;
    }
    printf("Rooms:\n");
    for (int i = 0; i < ROOMS->count; i++)
    {
        Room *room = (Room *)ROOMS->ptr->data;
        printf("%d: %s\n", room->roomid, room->name);
        moveNext(ROOMS);
    }
    return 0;
}

int listTeachers()
{
    if (TEACHERS->count == 0)
    {
        printf("No teachers found.\n");
        return -1;
    }
    printf("Teachers:\n");
    for (int i = 0; i < TEACHERS->count; i++)
    {
        Teacher *teacher = (Teacher *)TEACHERS->ptr->data;
        printf("%d: %s\n", teacher->teacherid, teacher->name);
        moveNext(TEACHERS);
    }
    return 0;
}

int listStudents()
{
    if (STUDENTS->count == 0)
    {
        printf("No students found.\n");
        return -1;
    }
    printf("Students:\n");
    for (int i = 0; i < STUDENTS->count; i++)
    {
        Student *student = (Student *)STUDENTS->ptr->data;
        printf("%d: %s\n", student->studentid, student->name);
        moveNext(STUDENTS);
    }
    return 0;
}

int showStudentInfo(uint32_t studentid)
{
    if (seekNode(STUDENTS, checkStudentid, &studentid) == -1)
    {
        printf("Error: student not found.\n");
        return -1;
    }
    if (seekNode(USERLIST, checkUserid, &studentid) == -1)
    {
        printf("Error: user not found.\n");
        return -1;
    }
    printf("UID: %d", ((UserInfo *)USERLIST->ptr->data)->userid);

    Student *student = (Student *)STUDENTS->ptr->data;
    printf("Student: %s\n", student->name);
    printf("Studentid: %d\n", student->studentid);
    printf("Enrollments:\n");
    for (int i = 0; i < student->enrollment.count; i++)
    {
        Enrollment *enrollment = (Enrollment *)student->enrollment.ptr->data;
        printf("%d: %s\n", enrollment->courseid, ((Course *)COURSES->ptr->data)->name);
        moveNext(&student->enrollment);
    }
    return 0;
}

int showTeacherInfo(uint32_t teacherid)
{
    if (seekNode(TEACHERS, checkTeacherid, &teacherid) == -1)
    {
        printf("Error: teacher not found.\n");
        return -1;
    }
    if (seekNode(USERLIST, checkUserid, &teacherid) == -1)
    {
        printf("Error: user not found.\n");
        return -1;
    }
    printf("UID: %d", ((UserInfo *)USERLIST->ptr->data)->userid);

    Teacher *teacher = (Teacher *)TEACHERS->ptr->data;
    printf("Teacher: %s\n", teacher->name);
    printf("Teacherid: %d\n", teacher->teacherid);
    printf("Courses:\n");
    for (int i = 0; i < teacher->registry.count; i++)
    {
        Registry *registry = (Registry *)teacher->registry.ptr->data;
        printf("%d: %s\n", registry->courseid, ((Course *)COURSES->ptr->data)->name);
        moveNext(&teacher->registry);
    }
    return 0;
}

int showCourseInfo(uint32_t courseid)
{
    if (seekNode(COURSES, checkCourseid, &courseid) == -1)
    {
        printf("Error: course not found.\n");
        return -1;
    }
    Course *course = (Course *)COURSES->ptr->data;

    printf("Course: %s\n", course->name);
    printf("Courseid: %d\n", course->courseid);
    printf("Teacher: %s\n", ((Teacher *)TEACHERS->ptr->data)->name);
    printf("Teacherid: %d\n", ((Teacher *)TEACHERS->ptr->data)->teacherid);
    printf("Registry:\n");
    for (int i = 0; i < course->teachers.count; i++)
    {
        Registry *registry = (Registry *)course->teachers.ptr->data;
        printf("%d: %s\n", registry->teacherid, ((Teacher *)TEACHERS->ptr->data)->name);
        moveNext(&course->teachers);
    }
    printf("Enrollments:\n");
    for (int i = 0; i < course->enrollment.count; i++)
    {
        Enrollment *enrollment = (Enrollment *)course->enrollment.ptr->data;
        printf("%d: %s\n", enrollment->studentid, ((Student *)STUDENTS->ptr->data)->name);
        moveNext(&course->enrollment);
    }
    return 0;
}
