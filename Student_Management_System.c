    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>

    #define MAX 100

    // ----------------- Linked List: Student Management -------------------
    struct Student {
        int id;
        char name[50];
        struct Student* next;
    };
    struct Student* studentHead = NULL;

    // ----------------- BST: Course Catalog -------------------
    struct Course {
        int id;
        char title[100];
        struct Course* left;
        struct Course* right;
    };
    struct Course* courseRoot = NULL;

    // ----------------- Queue: Enrollment System -------------------
    struct Enrollment {
        int studentId;
        int courseId;
        struct Enrollment* next;
    };
    struct Enrollment* front = NULL;
    struct Enrollment* rear = NULL;

    // ----------------- Stack: Course Video History -------------------
    char* videoStack[MAX];
    int top = -1;

    // ----------------- Graph: Student Connections -------------------
    int graph[MAX][MAX] = {0};

    // ----------------- Sign-in System -------------------
    // Sample user credentials
    const char *username = "Tarunno";
    const char *password = "password";

    // Function to authenticate user
    int signIn() {
        char inputUsername[50];
        char inputPassword[50];

        printf("Blended Learning Center\n");
        printf("===== Sign In =====\n");
        printf("Enter Username: ");
        fgets(inputUsername, sizeof(inputUsername), stdin);
        inputUsername[strcspn(inputUsername, "\n")] = 0;

        printf("Enter Password: ");
        fgets(inputPassword, sizeof(inputPassword), stdin);
        inputPassword[strcspn(inputPassword, "\n")] = 0;

        if (strcmp(inputUsername, username) == 0 && strcmp(inputPassword, password) == 0) {
            printf("Sign In Successful! Welcome, %s.\n\n", inputUsername);
            return 1;  // Successful login
        } else {
            printf("Invalid Username or Password.\n");
            return 0;  // Unsuccessful login
        }
    }
    
    // ----------------- Student Management -------------------
    // Function to register a student
    void registerStudent(int id, char name[]) {
        struct Student* newStudent = (struct Student*)malloc(sizeof(struct Student));
        if (!newStudent) {
            printf("Memory allocation failed!\n");
            return;
        }
        newStudent->id = id;
        strncpy(newStudent->name, name, sizeof(newStudent->name) - 1);
        newStudent->name[sizeof(newStudent->name) - 1] = '\0';
        newStudent->next = studentHead;
        studentHead = newStudent;
        printf("Student Registered: %s (ID: %d)\n", name, id);
    }

    // Function to view all students
    void viewStudents() {
        struct Student* temp = studentHead;
        if (!temp) {
            printf("No students registered yet.\n");
            return;
        }
        printf("\nRegistered Students:\n");
        while (temp) {
            printf("ID: %d, Name: %s\n", temp->id, temp->name);
            temp = temp->next;
        }
    }

    // ----------------- Course Catalog -------------------
    struct Course* insertCourse(struct Course* root, int id, char title[]) {
        if (!root) {
            struct Course* temp = (struct Course*)malloc(sizeof(struct Course));
            if (!temp) {
                printf("Memory allocation failed!\n");
                return NULL;
            }
            temp->id = id;
            strncpy(temp->title, title, sizeof(temp->title) - 1);  // Safe string copy
            temp->title[sizeof(temp->title) - 1] = '\0';           // Ensure null termination
            temp->left = temp->right = NULL;
            return temp;
        }
        if (id < root->id)
            root->left = insertCourse(root->left, id, title);
        else
            root->right = insertCourse(root->right, id, title);
        return root;
    }

    // Function to display courses in-order (sorted by ID)
    void inorderCourses(struct Course* root) {
        if (root != NULL) {
            inorderCourses(root->left);
            printf("Course ID: %d, Title: %s\n", root->id, root->title);
            inorderCourses(root->right);
        }
    }

    // ----------------- Enrollment System -------------------
    // Function to add an enrollment request
    void enqueue(int studentId, int courseId) {
        struct Enrollment* temp = (struct Enrollment*)malloc(sizeof(struct Enrollment));
        if (!temp) {
            printf("Memory allocation failed!\n");
            return;
        }
        temp->studentId = studentId;
        temp->courseId = courseId;
        temp->next = NULL;
        if (rear != NULL)
            rear->next = temp;
        else
            front = temp;
        rear = temp;
        printf("Enrollment Request Added: Student %d -> Course %d\n", studentId, courseId);
    }

    // Function to process an enrollment request
    void processEnrollment() {
        if (front == NULL) {
            printf("No pending enrollments.\n");
            return;
        }
        struct Enrollment* temp = front;
        printf("Processing Enrollment: Student %d enrolled in Course %d\n", temp->studentId, temp->courseId);
        front = front->next;
        if (front == NULL)
            rear = NULL;
        free(temp);
    }

    // ----------------- Video Stack -------------------
    void pushVideo(char* video) {
        if (top < MAX - 1) {
            top++;
            videoStack[top] = video;
            printf("Watched video added: %s\n", video);
        } else {
            printf("Video stack is full!\n");
        }
    }

    // Function to view the watched videos history
    void viewHistory() {
        if (top == -1) {
            printf("No videos watched yet.\n");
            return;
        }
        printf("\nRecently Watched Videos:\n");
        for (int i = top; i >= 0; i--)
            printf("-> %s\n", videoStack[i]);
    }

    // ----------------- Student Connections -------------------
    // Function to connect two students
    void connectStudents(int s1, int s2) {
        graph[s1][s2] = 1;
        graph[s2][s1] = 1;
        printf("Connected Student %d <--> Student %d\n", s1, s2);
    }

    // Function to show connections of a specific student
    void showConnections(int studentId) {
        printf("Connections of Student %d:\n", studentId);
        for (int i = 0; i < MAX; i++) {
            if (graph[studentId][i] == 1)
                printf("-> Student %d\n", i);
        }
    }

    // ----------------- Main Menu -------------------
    int main() {
        int choice, id, cid;
        char name[50], title[100];
        char* video;

        // Sign-in
        if (!signIn()) {
            return 0;
        }

        // Example calls for adding students, courses, enrollments, etc.
        registerStudent(101, "Mahfujul Haque");    // Registering a student
        registerStudent(102, "Saiful Islam");  // Registering another student

        courseRoot = insertCourse(courseRoot, 301, "Data Structure 101");  // Adding a course
        courseRoot = insertCourse(courseRoot, 201, "Mathematics 101");       // Adding another course

        enqueue(101, 301);    // Enrolling student 101 in course 301
        processEnrollment();  // Processing the first enrollment

        video = (char*)malloc(100);
        if (video) {
            strcpy(video, "Introduction to Programming");
            pushVideo(video);  // Adding video to stack
        }

        connectStudents(101, 102);  // Connecting two students
        connectStudents(101, 103);  // Connecting more students

        // Main menu for user interaction
        while (1) {
            printf("\n===== Blended Learning System =====\n");
            printf("1. Register Student\n");
            printf("2. View Students\n");
            printf("3. Add Course\n");
            printf("4. View All Courses\n");
            printf("5. Enroll in Course\n");
            printf("6. Process Enrollment\n");
            printf("7. Watch Video\n");
            printf("8. View Watch History\n");
            printf("9. Connect Students\n");
            printf("10. View Student Connections\n");
            printf("0. Exit\n");
            printf("Enter choice: ");
            scanf("%d", &choice);
            getchar();  // to clear newline

            switch (choice) {
                case 1:
                    printf("Enter Student ID and Name: ");
                    scanf("%d", &id);
                    getchar();
                    fgets(name, 50, stdin);
                    name[strcspn(name, "\n")] = 0;
                    registerStudent(id, name);
                    break;

                case 2:
                    viewStudents();
                    break;

                case 3:
                    printf("Enter Course ID and Title: ");
                    scanf("%d", &cid);
                    getchar();
                    fgets(title, 100, stdin);
                    title[strcspn(title, "\n")] = 0;
                    courseRoot = insertCourse(courseRoot, cid, title);
                    printf("Course Added Successfully.\n");
                    break;

                case 4:
                    inorderCourses(courseRoot);
                    break;

                case 5:
                    printf("Enter Student ID and Course ID to enroll: ");
                    scanf("%d %d", &id, &cid);
                    enqueue(id, cid);
                    break;

                case 6:
                    processEnrollment();
                    break;

                case 7:
                    video = (char*)malloc(100);
                    if (video) {
                        printf("Enter video name: ");
                        fgets(video, 100, stdin);
                        video[strcspn(video, "\n")] = 0;
                        pushVideo(video);
                    } else {
                        printf("Memory allocation failed for video.\n");
                    }
                    break;

                case 8:
                    viewHistory();
                    break;

                case 9:
                    printf("Enter two student IDs to connect: ");
                    scanf("%d %d", &id, &cid);
                    connectStudents(id, cid);
                    break;

                case 10:
                    printf("Enter student ID to view connections: ");
                    scanf("%d", &id);
                    showConnections(id);
                    break;

                case 0:
                    printf("Exiting program.\n");
                    exit(0);

                default:
                    printf("Invalid choice! Please try again.\n");
            }
        }

        return 0;
    }
