// Daffodil Flight  Reservation System

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define MAX_AIRPORTS 10

typedef struct Flight {
    int flightNo;
    char origin[MAX_NAME], destination[MAX_NAME];
    struct Flight *left, *right;
} Flight;

Flight* createFlight(int no, char origin[], char dest[]) {
    Flight* f = malloc(sizeof(Flight));
    f->flightNo = no;
    strcpy(f->origin, origin);
    strcpy(f->destination, dest);
    f->left = f->right = NULL;
    return f;
}

Flight* insertFlight(Flight* root, int no, char origin[], char dest[]) {
    if (!root) return createFlight(no, origin, dest);
    if (no < root->flightNo)
        root->left = insertFlight(root->left, no, origin, dest);
    else if (no > root->flightNo)
        root->right = insertFlight(root->right, no, origin, dest);
    return root;
}

void displayFlights(Flight* root) {
    if (root) {
        displayFlights(root->left);
        printf("Flight %d: %s -> %s\n", root->flightNo, root->origin, root->destination);
        displayFlights(root->right);
    }
}

// ------------------- Queue for Passengers -------------------
typedef struct Passenger {
    char name[MAX_NAME];
    struct Passenger* next;
} Passenger;

typedef struct {
    Passenger *front, *rear;
} Queue;

void initQueue(Queue* q) { q->front = q->rear = NULL; }

void enqueue(Queue* q, char name[]) {
    Passenger* p = malloc(sizeof(Passenger));
    strcpy(p->name, name);
    p->next = NULL;
    if (!q->rear)
        q->front = q->rear = p;
    else {
        q->rear->next = p;
        q->rear = p;
    }
}

void displayQueue(Queue q) {
    if (!q.front) {
        printf("No passengers in queue.\n");
        return;
    }
    printf("Passenger Queue:\n");
    for (Passenger* p = q.front; p; p = p->next)
        printf("- %s\n", p->name);
}

// ------------------- Graph for Flight Routes -------------------
int graph[MAX_AIRPORTS][MAX_AIRPORTS];
char airports[MAX_AIRPORTS][MAX_NAME];
int airportCount = 0;

int getAirportIndex(char name[]) {
    for (int i = 0; i < airportCount; i++)
        if (strcmp(airports[i], name) == 0) return i;
    return -1;
}

void addAirport(char name[]) {
    if (airportCount >= MAX_AIRPORTS) return;
    if (getAirportIndex(name) != -1) return;
    strcpy(airports[airportCount++], name);
}

void addRoute(char from[], char to[]) {
    int i = getAirportIndex(from), j = getAirportIndex(to);
    if (i == -1 || j == -1) return;
    graph[i][j] = 1;
}

void displayGraph() {
    printf("\nFlight Network:\n     ");
    for (int i = 0; i < airportCount; i++)
        printf("%-10s", airports[i]);
    printf("\n");
    for (int i = 0; i < airportCount; i++) {
        printf("%-10s", airports[i]);
        for (int j = 0; j < airportCount; j++)
            printf("   %d      ", graph[i][j]);
        printf("\n");
    }
}

void input(char* prompt, char* buffer) {
    printf("%s", prompt);
    fgets(buffer, MAX_NAME, stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
}

// ------------------- Load Example Data -------------------
void loadExampleData(Flight** schedule, Queue* q) {
    // Flights
    *schedule = insertFlight(*schedule, 101, "Dhaka", "Chittagong");
    *schedule = insertFlight(*schedule, 102, "Dhaka", "Sylhet");
    *schedule = insertFlight(*schedule, 103, "Sylhet", "Rajshahi");

    // Passengers
    enqueue(q, "Salehin Ontu");
    enqueue(q, "Akash Das");
    enqueue(q, "Rofiqul Rahman");

    // Airports
    addAirport("Dhaka");
    addAirport("Chittagong");
    addAirport("Sylhet");
    addAirport("Rajshahi");

    // Routes
    addRoute("Dhaka", "Chittagong");
    addRoute("Dhaka", "Sylhet");
    addRoute("Sylhet", "Rajshahi");
}

// ------------------- Main -------------------
int main() {
    Flight* schedule = NULL;
    Queue queue;
    initQueue(&queue);
    int choice;

    loadExampleData(&schedule, &queue);

    while (1) {
        printf("\n--- Daffodil Flight Reservation System ---\n");
        printf("1. Add Flight\n2. View Flights\n3. Book Passenger\n");
        printf("4. View Passengers\n5. Add Airport\n6. Add Route\n");
        printf("7. View Network\n0. Exit\nChoice: ");
        scanf("%d", &choice); getchar();

        if (choice == 0) break;

        char input1[MAX_NAME], input2[MAX_NAME];
        int no;

        switch (choice) {
            case 1:
                printf("Flight No: "); scanf("%d", &no); getchar();
                input("Origin: ", input1);
                input("Destination: ", input2);
                schedule = insertFlight(schedule, no, input1, input2);
                break;
            case 2:
                printf("Flight Schedule:\n");
                displayFlights(schedule);
                break;
            case 3:
                input("Passenger Name: ", input1);
                enqueue(&queue, input1);
                break;
            case 4:
                displayQueue(queue);
                break;
            case 5:
                input("Airport Name: ", input1);
                addAirport(input1);
                break;
            case 6:
                input("From Airport: ", input1);
                input("To Airport: ", input2);
                addRoute(input1, input2);
                break;
            case 7:
                displayGraph();
                break;
            default:
                printf("Invalid choice.\n");
        }
    }

    printf("Goodbye!\n");
    return 0;
}