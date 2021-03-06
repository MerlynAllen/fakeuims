#include "curriculum.h"

bool isCompulsory(uint32_t courseid)
{
    return courseid >> 31;
}

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

bool cmpStudentName(void *a, void *b)
{
    Student *s1 = (Student *)a;
    Student *s2 = (Student *)b;
    return strcmp(s1->name, s2->name) > 0;
}

bool cmpStudentId(void *a, void *b)
{
    Student *s1 = (Student *)a;
    Student *s2 = (Student *)b;
    return s1->studentid > s2->studentid;
}

bool cmpCourseName(void *a, void *b)
{
    Course *c1 = (Course *)a;
    Course *c2 = (Course *)b;
    return strcmp(c1->name, c2->name) > 0;
}

bool cmpCourseId(void *a, void *b)
{
    Course *c1 = (Course *)a;
    Course *c2 = (Course *)b;
    return c1->courseid > c2->courseid;
}

bool cmpCourseCredits(void *a, void *b)
{
    Course *c1 = (Course *)a;
    Course *c2 = (Course *)b;
    return c1->credits > c2->credits;
}

bool cmpCourseCapacity(void *a, void *b)
{
    Course *c1 = (Course *)a;
    Course *c2 = (Course *)b;
    return c1->capacity > c2->capacity;
}

bool cmpRoomName(void *a, void *b)
{
    Room *r1 = (Room *)a;
    Room *r2 = (Room *)b;
    return strcmp(r1->name, r2->name) > 0;
}

bool cmpRoomId(void *a, void *b)
{
    Room *r1 = (Room *)a;
    Room *r2 = (Room *)b;
    return r1->roomid > r2->roomid;
}

bool cmpRoomCapacity(void *a, void *b)
{
    Room *r1 = (Room *)a;
    Room *r2 = (Room *)b;
    return r1->capacity > r2->capacity;
}

bool checkEnrollmentUserid(void *data, void *userid)
{
    Enrollment *enrollment = (Enrollment *)data;
    return enrollment->studentid == *(uint32_t *)userid;
}

bool checkEnrollmentCourseid(void *data, void *courseid)
{
    Enrollment *enrollment = (Enrollment *)data;
    return enrollment->courseid == *(uint32_t *)courseid;
}

bool checkEnrollment(void *data, void *enrollment)
{
    Enrollment *enrollment1 = (Enrollment *)data;
    Enrollment *enrollment2 = (Enrollment *)enrollment;
    return (enrollment1->studentid == enrollment2->studentid && enrollment1->courseid == enrollment2->courseid);
}

int userSort(char *data, char *field, bool reverse)
{
    if (isAdmin(USERID))
    {
        if (strcmp(data, "user") == 0)
        {
            if (strcmp(field, "name") == 0)
            {
                qsortcore(USERLIST->head->next, USERLIST->tail, cmpStudentName, reverse);
            }
            else if (strcmp(field, "id") == 0)
            {
                qsortcore(USERLIST->head->next, USERLIST->tail, cmpStudentId, reverse);
            }
            else
            {
                return -1;
            }
        }
        else if (strcmp(data, "course") == 0)
        {
            if (strcmp(field, "name") == 0)
            {
                nodeqsort(COURSES, cmpCourseName, reverse);
            }
            else if (strcmp(field, "id") == 0)
            {
                nodeqsort(COURSES, cmpCourseId, reverse);
            }
            else if (strcmp(field, "credit") == 0)
            {
                nodeqsort(COURSES, cmpCourseCredits, reverse);
            }
            else if (strcmp(field, "capacity") == 0)
            {
                nodeqsort(COURSES, cmpCourseCapacity, reverse);
            }
            else
            {
                return -1;
            }
        }
        else if (strcmp(data, "room") == 0)
        {
            // sort by room
            if (strcmp(field, "name") == 0)
            {
                nodeqsort(ROOMS, cmpRoomName, reverse);
            }
            else if (strcmp(field, "id") == 0)
            {
                nodeqsort(ROOMS, cmpRoomId, reverse);
            }
            else if (strcmp(field, "capacity") == 0)
            {
                nodeqsort(ROOMS, cmpRoomCapacity, reverse);
            }
            else
            {
                return 1;
            }
        }
        return 0;
    }
    else
    {
        printf("You are not an admin!\n");
        return -1;
    }
nodataerr:
    printf("No such data: %s\n", data);
    return -1;
}

int initData()
{
    TEACHERS = makeDLinkedList();
    TEACHERS->count = 0;
    STUDENTS = makeDLinkedList();
    STUDENTS->count = 0;
    COURSES = makeDLinkedList();
    COURSES->count = 0;
    ROOMS = makeDLinkedList();
    ROOMS->count = 0;
    ENROLLMENTS = makeDLinkedList();
    ENROLLMENTS->count = 0;
    REGISTRYS = makeDLinkedList();
    REGISTRYS->count = 0;
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
    Room room = {0};
    for (; fscanf(fpRooms, "%s %u %u", room.name, &room.roomid, &room.capacity) != EOF;)
    {
        Room *newroom = calloc(1, sizeof(Room));
        initDLinkedList(&newroom->courses);
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

    Course course = {0};
    uint32_t roomid = 0;
    for (; fscanf(fpCourses, "%s %u %s %f %u %hhu %hhu %hhu %hhu %hhu %u\n", course.name, &course.courseid, course.description, &course.credits, &course.capacity, &course.time.weekstart, &course.time.weekend, &course.time.weekday, &course.time.timestart, &course.time.timeend, &roomid) != EOF;)
    {
        Course *newcourse = calloc(1, sizeof(Course));
        memcpy(newcourse, &course, sizeof(Course));
        seekNode(ROOMS, checkRoomid, &roomid);
        newcourse->room = (Room *)ROOMS->ptr->data;
        Room *room = ROOMS->ptr->data;
        appendNode(&room->courses, newcourse);
        initDLinkedList(&newcourse->enrollment);
        appendNode(COURSES, newcourse);
    }
    fclose(fpCourses);

    // load enrollments
    if (COURSES->count > 0)
    {
        FILE *fpEnrollment = fopen("enrollments", "r");
        if (fpEnrollment == NULL)
        {
            printf("Cannot open enrollments file.\n");
            return -1;
        }
        Enrollment enrollment = {0};
        for (; fscanf(fpEnrollment, "%u %u", &enrollment.studentid, &enrollment.courseid) != EOF;)
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
        fclose(fpEnrollment);
    }
    // load registries
    if (COURSES->count > 0)
    {
        FILE *fpRegistry = fopen("registries", "r");
        if (fpRegistry == NULL)
        {
            printf("Cannot open registries file.\n");
            return -1;
        }
        Registry registry = {0};
        for (; fscanf(fpRegistry, "%u %u", &registry.courseid, &registry.teacherid) != EOF;)
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
}

int userAddCourse()
{
    if (isAdmin(USERID))
    {
        if (ROOMS->count == 0)
        {
            printf("No rooms available.\n");
            return -1;
        }
        char type = '\0';
        bool iscompulsory = false;
        CTime time;
        float credits;
        uint32_t capacity;
        uint32_t courseid;
        char coursename[MAX_LEN] = {0};
        char description[MAX_LEN] = {0};
        uint32_t roomid;
        printf("Course name: ");
        scanf("%s", coursename);
        while (getchar() != '\n')
            ;
        printf("Description: ");
        scanf("%s", description);
        while (getchar() != '\n')
            ;
        printf("Credits: ");
        scanf("%f", &credits);
        while (getchar() != '\n')
            ;
    inputroomid:
        printf("Roomid: ");
        scanf("%u", &roomid);
        if (seekNode(ROOMS, checkRoomid, &roomid) == -1)
        {
            printf("Error: roomid not found. Retry.\n");
            goto inputroomid;
        }
    inputcapacity:
        printf("Capacity: ");
        scanf("%u", &capacity);
        if (capacity > ((Room *)ROOMS->ptr->data)->capacity)
        {
            printf("Error: capacity is larger than room capacity. Retry.\n");
            goto inputcapacity;
        }
        getchar();
    inputcompulsory:
        printf("Type: [Compulsory(C), Elective(E)] ");
        scanf("%c", &type);
        switch (type)
        {
        case 'C':
            iscompulsory = true;
            break;
        case 'E':
            iscompulsory = false;
            break;
        default:
            printf("Error: invalid type. Retry.\n");
            goto inputcompulsory;
            break;
        }
    timeschedule:
        printf("Week start: ");
        scanf("%hhd", &time.weekstart);
        printf("Week end: ");
        scanf("%hhd", &time.weekend);
        printf("Weekday (1-7): ");
        scanf("%hhd", &time.weekday);
        printf("Class start at: ");
        scanf("%hhd", &time.timestart);
        printf("Class end at: ");
        scanf("%hhd", &time.timeend);
        while (getchar() != '\n')
            ;
        // check conflict
        switch (seekNode(COURSES, checkTimeConflict, &time))
        {
        case 0:
            printf("Error: time conflict. Retry.\n");
            goto timeschedule;
            break;
        default:
            break;
        }
        courseid = COURSES->count | iscompulsory << 31;
        addCourse(coursename, description, courseid, credits, capacity, time, roomid);
        printf("Course added.\n");
        return 0;
    }
    else
    {
        printf("You are not an admin.\n");
    }
}

int addCourse(char name[MAX_LEN], char description[MAX_LEN], uint32_t courseid, float credits, uint32_t capacity, CTime time, uint32_t roomid)
{
    Course *course = calloc(1, sizeof(Course));
    course->courseid = courseid;
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
    if (isAdmin(USERID))
    {
        Room *room = calloc(1, sizeof(Room));
        // char name[MAX_LEN] = {0};
        printf("Room name: ");
        scanf("%s", room->name);
        while (getchar() != '\n')
            ;
        printf("Capacity: ");
        scanf("%u", &room->capacity);
        while (getchar() != '\n')
            ;
        room->roomid = ROOMS->count;
        initDLinkedList(&room->courses);
        appendNode(ROOMS, room);
        printf("Room %s uid %u with capacity %u added.\n", room->name, room->roomid, room->capacity);
    }
    else
    {
        printf("You are not an admin.\n");
    }
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
    student->studentid = studentid;
    strcpy(student->name, name);
    initDLinkedList(&student->enrollment);
    appendNode(STUDENTS, student);
}

int addTeacher(char *name, uint32_t teacherid)
{
    Teacher *teacher = calloc(1, sizeof(Teacher));
    teacher->teacherid = teacherid;
    strcpy(teacher->name, name);
    initDLinkedList(&teacher->registry);
    appendNode(TEACHERS, teacher);
}

int userEnroll() // student
{
    uint32_t studentid = uid2subid(USERID);
    uint32_t courseid;
    // if students
    if (isStudent(USERID))
    {
    inputcourseid:
        printf("Courseid: ");
        scanf("%u", &courseid);
        while (getchar() != '\n')
            ;
        COURSES->ptr = COURSES->head;
        switch (seekNode(COURSES, &checkCourseid, &courseid))
        {
        case 0: // found
            break;
        case 1: // empty
            printf("Course is empty.\n");
            goto inputcourseid;
            break;
        case -1:
            printf("Error: courseid not found.\n");
            break;
        default:
            break;
        }
        if (((Course *)COURSES->ptr->data)->capacity <= ((Course *)COURSES->ptr->data)->enrollment.count)
        {
            printf("Sorry, the course is full. \n");
            return -1;
        }
        enroll(studentid, courseid);
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
    STUDENTS->ptr = STUDENTS->head;
    COURSES->ptr = COURSES->head;
    seekNode(STUDENTS, checkUserid, &userid);
    seekNode(COURSES, checkCourseid, &courseid);
    Student *student = (Student *)STUDENTS->ptr->data;
    Course *course = (Course *)COURSES->ptr->data;
    student->enrollment.ptr = student->enrollment.head;
    if (seekNode(&student->enrollment, checkEnrollment, enrollment) == 0)
    {
        printf("Error: already enrolled.\n");
        return -1;
    }
    appendNode(&student->enrollment, enrollment);
    appendNode(&course->enrollment, enrollment);
    appendNode(ENROLLMENTS, enrollment);
    printf("Enrolled course id %u successfully.\n", courseid);
}

int userDisenroll()
{
    uint32_t studentid = uid2subid(USERID);
    uint32_t courseid;
    if (isStudent(USERID))
    {
    inputcourseid:
        printf("Courseid: ");
        scanf("%u", &courseid);
        while (getchar() != '\n')
            ;
        COURSES->ptr = COURSES->head;
        if (seekNode(COURSES, checkCourseid, &courseid) == 0)
        {
            printf("Error: course not found. Retry.\n");
            goto inputcourseid;
        }
        disenroll(studentid, courseid);
        printf("Disenrolled course id %u successfully.\n", courseid);
        return 0;
    }
    else
    {
        printf("Sorry, but you are not a student.\n");
        return -1;
    }
}

int disenroll(uint32_t studentid, uint32_t courseid)
{
    Enrollment enrollment;
    enrollment.studentid = studentid;
    enrollment.courseid = courseid;
    STUDENTS->ptr = STUDENTS->head;
    COURSES->ptr = COURSES->head;
    seekNode(STUDENTS, checkUserid, &studentid);
    seekNode(COURSES, checkCourseid, &courseid);
    Student *student = (Student *)STUDENTS->ptr->data;
    Course *course = (Course *)COURSES->ptr->data;
    student->enrollment.ptr = student->enrollment.head;
    course->enrollment.ptr = course->enrollment.head;
    seekNode(&student->enrollment, checkEnrollment, &enrollment);
    seekNode(&course->enrollment, checkEnrollment, &enrollment);
    delNode(&student->enrollment);
    delNode(&course->enrollment);
    ENROLLMENTS->ptr = ENROLLMENTS->head;
    seekNode(ENROLLMENTS, checkEnrollment, &enrollment);
    free(ENROLLMENTS->ptr->data);
    delNode(ENROLLMENTS);
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
    if (isTeacher(USERID))
    {
    inputcourseid:
        printf("Courseid: ");
        scanf("%u", &courseid);
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
        printf("Course %u registered.\n", courseid);
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
    COURSES->ptr = COURSES->head; // reset ptr
    for (int i = 0; i < COURSES->count; i++)
    {
        Course *course = (Course *)COURSES->ptr->data;
        fprintf(fpCourse, "%s %u %s %f %u %hhu %hhu %hhu %hhu %hhu %u\n", course->name, course->courseid, course->description, course->credits, course->capacity, course->time.weekstart, course->time.weekend, course->time.weekday, course->time.timestart, course->time.timeend, course->room->roomid);
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
    ROOMS->ptr = ROOMS->head; // reset ptr
    for (int i = 0; i < ROOMS->count; i++)
    {
        Room *room = (Room *)ROOMS->ptr->data;
        fprintf(fpRoom, "%s %u %u\n", room->name, room->roomid, room->capacity);
        moveNext(ROOMS);
    }
    // save Enrollments
    FILE *fpEnrollment = fopen("enrollments", "w");
    if (fpEnrollment == NULL)
    {
        printf("Error opening enrollments file\n");
        return -1;
    }
    ENROLLMENTS->ptr = ENROLLMENTS->head; // reset ptr
    for (int i = 0; i < ENROLLMENTS->count; i++)
    {
        Enrollment *enrollment = (Enrollment *)ENROLLMENTS->ptr->data;
        fprintf(fpEnrollment, "%u %u\n", enrollment->studentid, enrollment->courseid);
        moveNext(ENROLLMENTS);
    }
    // save Registries
    FILE *fpRegistry = fopen("registries", "w");
    if (fpRegistry == NULL)
    {
        printf("Error opening registries file\n");
        return -1;
    }
    REGISTRYS->ptr = REGISTRYS->head; // reset ptr
    for (int i = 0; i < REGISTRYS->count; i++)
    {
        Registry *registry = (Registry *)REGISTRYS->ptr->data;
        fprintf(fpRegistry, "%u %u\n", registry->courseid, registry->teacherid);
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
    COURSES->ptr = COURSES->head;
    for (int i = 0; i < COURSES->count; i++)
    {
        Course *course = (Course *)COURSES->ptr->data;
        printf("%u: %s\n\t%s\n\n", course->courseid, course->name, course->description);
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
    ROOMS->ptr = ROOMS->head;
    for (int i = 0; i < ROOMS->count; i++)
    {
        Room *room = (Room *)ROOMS->ptr->data;
        printf("%u: %s\n", room->roomid, room->name);
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
    TEACHERS->ptr = TEACHERS->head;
    for (int i = 0; i < TEACHERS->count; i++)
    {
        Teacher *teacher = (Teacher *)TEACHERS->ptr->data;
        printf("%u: %s\n", teacher->teacherid, teacher->name);
        moveNext(TEACHERS);
    }
    return 0;
}

int listStudents()
{
    if (isAdmin(USERID))
    {
        if (STUDENTS->count == 0)
        {
            printf("No students found.\n");
            return -1;
        }
        printf("Students:\n");
        STUDENTS->ptr = STUDENTS->head;
        for (int i = 0; i < STUDENTS->count; i++)
        {
            Student *student = (Student *)STUDENTS->ptr->data;
            printf("%u: %s\n", student->studentid, student->name);
            moveNext(STUDENTS);
        }
        return 0;
    }
    else
    {
        printf("You are not an admin.\n");
        return -1;
    }
}

int showStudentInfo(uint32_t studentid)
{
    if (isAdmin(USERID))
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
        printf("UID: %u", ((UserInfo *)USERLIST->ptr->data)->userid);

        Student *student = (Student *)STUDENTS->ptr->data;
        printf("Student: %s\n", student->name);
        printf("Studentid: %u\n", student->studentid);
        printf("Enrollments:\n");
        for (int i = 0; i < student->enrollment.count; i++)
        {
            Enrollment *enrollment = (Enrollment *)student->enrollment.ptr->data;
            printf("%u: %s\n", enrollment->courseid, ((Course *)COURSES->ptr->data)->name);
            moveNext(&student->enrollment);
        }
        return 0;
    }
    else
    {
        printf("You are not an admin.\n");
        return -1;
    }
}

int showTeacherInfo(uint32_t teacherid)
{
    if (isAdmin(USERID))
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
        printf("UID: %u", ((UserInfo *)USERLIST->ptr->data)->userid);

        Teacher *teacher = (Teacher *)TEACHERS->ptr->data;
        printf("Teacher: %s\n", teacher->name);
        printf("Teacherid: %u\n", teacher->teacherid);
        printf("Courses:\n");
        teacher->registry.ptr = teacher->registry.head;
        for (int i = 0; i < teacher->registry.count; i++)
        {
            Registry *registry = (Registry *)teacher->registry.ptr->data;
            printf("%u: %s\n", registry->courseid, ((Course *)COURSES->ptr->data)->name);
            moveNext(&teacher->registry);
        }
        return 0;
    }
    else
    {
        printf("You are not an admin.\n");
        return -1;
    }
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
    printf("Courseid: %u\n", course->courseid);
    course->teachers.ptr = course->teachers.head;
    for (int i = 0; i < course->teachers.count; i++)
    {
        Registry *registry = (Registry *)course->teachers.ptr->data;
        printf("%u: %s\n", registry->teacherid, ((Teacher *)TEACHERS->ptr->data)->name);
        moveNext(&course->teachers);
    }
    printf("Enrollments: Total %u\n", course->enrollment.count);
    if (isAdmin(USERID))
    {
        course->enrollment.ptr = course->enrollment.head;
        for (int i = 0; i < course->enrollment.count; i++)
        {
            Enrollment *enrollment = (Enrollment *)course->enrollment.ptr->data;

            printf("%u: %s\n", enrollment->studentid, ((Student *)STUDENTS->ptr->data)->name);

            moveNext(&course->enrollment);
        }
    }
    return 0;
}

int me()
{
    if (isStudent(USERID))
    {
        float creditsum = 0, compcreditsum = 0, electcreditsum = 0;
        uint32_t studentid = uid2subid(USERID);
        seekNode(STUDENTS, checkStudentid, &studentid);
        Student *me = (Student *)STUDENTS->ptr->data;
        printf("Information about %s:\n", me->name);
        printf("Studentid: %u\n", me->studentid);
        printf("\nEnrollments:\n");
        me->enrollment.ptr = me->enrollment.head;
        for (int i = 0; i < me->enrollment.count; i++)
        {
            Enrollment *enrollment = (Enrollment *)me->enrollment.ptr->data;
            Course *course = (Course *)COURSES->ptr->data;
            printf("%u: %s %f\n", enrollment->courseid, course->name, course->credits);
            if (isCompulsory(enrollment->courseid))
            {
                compcreditsum += course->credits;
            }
            else
            {
                electcreditsum += course->credits;
            }
            creditsum += course->credits;
            moveNext(&me->enrollment);
        }
        printf("\nTotal: %f\nCompulsory: %f \nElective: %f\n", creditsum, compcreditsum, electcreditsum);
    }
    else
    {
        printf("You are not a student.\n");
    }
}