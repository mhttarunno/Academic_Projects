#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_USERS 10
#define MAX_HALLS 3
#define FLOORS 12
#define ROOMS_PER_FLOOR 26
#define SEATS_PER_ROOM 4
#define USERNAME_LENGTH 30
#define PASSWORD_LENGTH 20
#define HALL_NAME_LENGTH 10
#define MAX_NOTICES 20
#define MAX_COMPLAINTS 50
#define NOTICE_LENGTH 100
#define COMPLAINT_LENGTH 150

typedef struct
{
    int floor;
    int room;
    int seat;
} Seat;

typedef struct
{
    char hallName[HALL_NAME_LENGTH];
    Seat reservedSeat;
    char reservedBy[USERNAME_LENGTH];
} Reservation;

typedef struct
{
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];
} User;

typedef struct
{
    char notice[NOTICE_LENGTH];
    char postedBy[USERNAME_LENGTH];
} Notice;

typedef struct
{
    char complaint[COMPLAINT_LENGTH];
    char complainedBy[USERNAME_LENGTH];
} Complaint;

// Global variables
User users[MAX_USERS];
Reservation reservations[MAX_HALLS * FLOORS * ROOMS_PER_FLOOR * SEATS_PER_ROOM];
Notice notices[MAX_NOTICES];
Complaint complaints[MAX_COMPLAINTS];
int userCount = 0;
int reservationCount = 0;
int noticeCount = 0;
int complaintCount = 0;

// Function prototypes
void initializeSystem();
int loginUser();
void viewReservations();
void makeReservation(int userIndex);
void cancelReservation(int userIndex);
void showMenu(int userIndex);
void addUser();
void registerUser();
void recoverPassword();
void changePassword(int userIndex);
void viewUserDetails();
void saveData();
void loadData();
void initializeHalls();
int isValidFloor(int floor);
int isValidRoom(int room);
int isValidSeat(int seat);
int getHallIndex(const char *hallName);
void postNotice(int userIndex);
void deleteNotice(int userIndex);
void viewNotices();
void submitComplaint(int userIndex);
void viewComplaints(int userIndex);
void deleteComplaint(int userIndex);

const char *hallNames[MAX_HALLS] = {"YKSG-01", "YKSG-02", "RASG"};

int main()
{
    int userIndex;

    // Load user and reservation data
    loadData();

    printf("Welcome to the Hall Management System!\n");

    // Allow users to log in or register
    while ((userIndex = loginUser()) == -1)
    {
        printf("Invalid login. Would you like to register a new account? (y/n): ");
        char choice;
        scanf(" %c", &choice);

        if (choice == 'y' || choice == 'Y')
        {
            registerUser();
        }
        else
        {
            printf("Exiting...\n");
            return 0;
        }
    }

    viewNotices();
    showMenu(userIndex);

    // Save user and reservation data
    saveData();

    return 0;
}

void initializeSystem()
{
    // Add a default user
    strcpy(users[0].username, "admin");
    strcpy(users[0].password, "adminpass");
    userCount = 1;
    initializeHalls();
}

void initializeHalls()
{
    // Placeholder function: Initialize hall names or other details if needed.
}

int isValidFloor(int floor)
{
    return floor >= 1 && floor <= FLOORS;
}

int isValidRoom(int room)
{
    return room >= 1 && room <= ROOMS_PER_FLOOR;
}

int isValidSeat(int seat)
{
    return seat >= 1 && seat <= SEATS_PER_ROOM;
}

int getHallIndex(const char *hallName)
{
    for (int i = 0; i < MAX_HALLS; i++)
    {
        if (strcmp(hallNames[i], hallName) == 0)
        {
            return i;
        }
    }
    return -1;
}

int loginUser()
{
    char username[USERNAME_LENGTH];
    char password[PASSWORD_LENGTH];

    printf("Enter username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0 && strcmp(users[i].password, password) == 0)
        {
            return i;
        }
    }
    return -1;
}

void registerUser()
{
    if (userCount >= MAX_USERS)
    {
        printf("User limit reached. Cannot register new users.\n");
        return;
    }

    User newUser;
    printf("Enter new username: ");
    scanf("%s", newUser.username);
    printf("Enter new password: ");
    scanf("%s", newUser.password);

    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, newUser.username) == 0)
        {
            printf("Username already exists.\n");
            return;
        }
    }

    users[userCount] = newUser;
    userCount++;
    printf("Registration successful!\n");
}

void recoverPassword()
{
    char username[USERNAME_LENGTH];
    char newPassword[PASSWORD_LENGTH];

    printf("Enter username of the account to recover: ");
    scanf("%s", username);

    for (int i = 0; i < userCount; i++)
    {
        if (strcmp(users[i].username, username) == 0)
        {
            printf("Enter new password: ");
            scanf("%s", newPassword);
            strcpy(users[i].password, newPassword);
            printf("Password recovered successfully!\n");
            return;
        }
    }
    printf("User not found.\n");
}

void changePassword(int userIndex)
{
    char currentPassword[PASSWORD_LENGTH];
    char newPassword[PASSWORD_LENGTH];

    printf("Enter current password: ");
    scanf("%s", currentPassword);

    if (strcmp(users[userIndex].password, currentPassword) != 0)
    {
        printf("Incorrect current password.\n");
        return;
    }

    printf("Enter new password: ");
    scanf("%s", newPassword);
    strcpy(users[userIndex].password, newPassword);
    printf("Password changed successfully!\n");
}

void viewUserDetails()
{
    printf("User Details:\n");
    for (int i = 0; i < userCount; i++)
    {
        printf("Username: %s\n", users[i].username);
    }
}

void viewReservations()
{
    printf("Current Reservations:\n");
    if (reservationCount == 0)
    {
        printf("No reservations yet.\n");
        return;
    }
    for (int i = 0; i < reservationCount; i++)
    {
        printf("Hall: %s, Floor: %d, Room: %d, Seat: %d, Reserved by: %s\n",
               reservations[i].hallName,
               reservations[i].reservedSeat.floor + 1,
               reservations[i].reservedSeat.room + 1,
               reservations[i].reservedSeat.seat + 1,
               reservations[i].reservedBy);
    }
}

void makeReservation(int userIndex)
{
    if (reservationCount >= MAX_HALLS * FLOORS * ROOMS_PER_FLOOR * SEATS_PER_ROOM)
    {
        printf("Reservation limit reached.\n");
        return;
    }

    int hallChoice;
    int floor, room, seat;
    int found = 0;

    printf("Select hall:\n");
    for (int i = 0; i < MAX_HALLS; i++)
    {
        printf("%d. %s\n", i + 1, hallNames[i]);
    }
    printf("Enter choice (1-%d): ", MAX_HALLS);
    scanf("%d", &hallChoice);

    if (hallChoice < 1 || hallChoice > MAX_HALLS)
    {
        printf("Invalid hall choice.\n");
        return;
    }

    const char *hallName = hallNames[hallChoice - 1];

    printf("Enter floor (1-12): ");
    scanf("%d", &floor);
    printf("Enter room (1-26): ");
    scanf("%d", &room);
    printf("Enter seat (1-4): ");
    scanf("%d", &seat);

    // Convert to 0-based indexing for internal processing
    floor -= 1;
    room -= 1;
    seat -= 1;

    // Validate input
    if (!isValidFloor(floor + 1) || !isValidRoom(room + 1) || !isValidSeat(seat + 1))
    {
        printf("Invalid reservation details.\n");
        return;
    }

    // Check if reservation already exists
    for (int i = 0; i < reservationCount; i++)
    {
        if (strcmp(reservations[i].hallName, hallName) == 0 &&
            reservations[i].reservedSeat.floor == floor &&
            reservations[i].reservedSeat.room == room &&
            reservations[i].reservedSeat.seat == seat)
        {
            printf("The seat is already reserved.\n");
            found = 1;
            break;
        }
    }

    if (!found)
    {
        strcpy(reservations[reservationCount].hallName, hallName);
        reservations[reservationCount].reservedSeat.floor = floor;
        reservations[reservationCount].reservedSeat.room = room;
        reservations[reservationCount].reservedSeat.seat = seat;
        strcpy(reservations[reservationCount].reservedBy, users[userIndex].username);
        reservationCount++;
        printf("Reservation made successfully!\n");
    }
}

void cancelReservation(int userIndex)
{
    int hallChoice;
    int floor, room, seat;
    int found = 0;

    printf("Select hall to cancel reservation:\n");
    for (int i = 0; i < MAX_HALLS; i++)
    {
        printf("%d. %s\n", i + 1, hallNames[i]);
    }
    printf("Enter choice (1-%d): ", MAX_HALLS);
    scanf("%d", &hallChoice);

    if (hallChoice < 1 || hallChoice > MAX_HALLS)
    {
        printf("Invalid hall choice.\n");
        return;
    }

    const char *hallName = hallNames[hallChoice - 1];

    printf("Enter floor (1-12): ");
    scanf("%d", &floor);
    printf("Enter room (1-26): ");
    scanf("%d", &room);
    printf("Enter seat (1-4): ");
    scanf("%d", &seat);

    // Convert to 0-based indexing for internal processing
    floor -= 1;
    room -= 1;
    seat -= 1;

    // Validate input
    if (!isValidFloor(floor + 1) || !isValidRoom(room + 1) || !isValidSeat(seat + 1))
    {
        printf("Invalid reservation details.\n");
        return;
    }

    for (int i = 0; i < reservationCount; i++)
    {
        if (strcmp(reservations[i].hallName, hallName) == 0 &&
            reservations[i].reservedSeat.floor == floor &&
            reservations[i].reservedSeat.room == room &&
            reservations[i].reservedSeat.seat == seat &&
            strcmp(reservations[i].reservedBy, users[userIndex].username) == 0)
        {
            for (int j = i; j < reservationCount - 1; j++)
            {
                reservations[j] = reservations[j + 1];
            }
            reservationCount--;
            printf("Reservation canceled successfully!\n");
            found = 1;
            break;
        }
    }

    if (!found)
    {
        printf("Reservation not found or you are not the one who reserved it.\n");
    }
}

void showMenu(int userIndex)
{
    int choice;
    do
    {
        printf("\nMenu:\n");
        printf("1. View Reservations\n");
        printf("2. Make Reservation\n");
        printf("3. Cancel Reservation\n");
        printf("4. Change Password\n");
        printf("5. View User Details (Admin Only)\n");
        printf("6. Post Notice (Admin Only)\n");
        printf("7. Delete Notice (Admin Only)\n");
        printf("8. View Notices\n");
        printf("9. Submit Complaint\n");
        printf("10. View Complaints (Admin Only)\n");
        printf("11. Delete Complaint (Admin Only)\n");
        printf("0. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice)
        {
        case 1:
            viewReservations();
            break;
        case 2:
            makeReservation(userIndex);
            break;
        case 3:
            cancelReservation(userIndex);
            break;
        case 4:
            changePassword(userIndex);
            break;
        case 5:
            if (strcmp(users[userIndex].username, "admin") == 0)
            {
                viewUserDetails();
            }
            else
            {
                printf("Access denied.\n");
            }
            break;
        case 6:
            if (strcmp(users[userIndex].username, "admin") == 0)
            {
                postNotice(userIndex);
            }
            else
            {
                printf("Access denied.\n");
            }
            break;
        case 7:
            if (strcmp(users[userIndex].username, "admin") == 0)
            {
                deleteNotice(userIndex);
            }
            else
            {
                printf("Access denied.\n");
            }
            break;
        case 8:
            viewNotices();
            break;
        case 9:
            submitComplaint(userIndex);
            break;
        case 10:
            if (strcmp(users[userIndex].username, "admin") == 0)
            {
                viewComplaints(userIndex);
            }
            else
            {
                printf("Access denied.\n");
            }
            break;
        case 11:
            if (strcmp(users[userIndex].username, "admin") == 0)
            {
                deleteComplaint(userIndex);
            }
            else
            {
                printf("Access denied.\n");
            }
            break;
        case 0:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            break;
        }
    } while (choice != 0);
}

void postNotice(int userIndex)
{
    if (noticeCount >= MAX_NOTICES)
    {
        printf("Notice limit reached.\n");
        return;
    }

    printf("Enter notice: ");
    getchar(); // Clear the newline character left by previous scanf
    fgets(notices[noticeCount].notice, NOTICE_LENGTH, stdin);
    // Remove newline character if present
    notices[noticeCount].notice[strcspn(notices[noticeCount].notice, "\n")] = 0;
    strcpy(notices[noticeCount].postedBy, users[userIndex].username);
    noticeCount++;
    printf("Notice posted successfully!\n");
}

void deleteNotice(int userIndex)
{
    int noticeIndex;
    printf("Enter notice index to delete (1-%d): ", noticeCount);
    scanf("%d", &noticeIndex);
    noticeIndex--; // Convert to 0-based index

    if (noticeIndex < 0 || noticeIndex >= noticeCount)
    {
        printf("Invalid notice index.\n");
        return;
    }

    for (int i = noticeIndex; i < noticeCount - 1; i++)
    {
        notices[i] = notices[i + 1];
    }
    noticeCount--;
    printf("Notice deleted successfully!\n");
}

void viewNotices()
{
    printf("Notices:\n");
    if (noticeCount == 0)
    {
        printf("No notices available.\n");
        return;
    }
    for (int i = 0; i < noticeCount; i++)
    {
        printf("%d. %s (Posted by: %s)\n", i + 1, notices[i].notice, notices[i].postedBy);
    }
}

void submitComplaint(int userIndex)
{
    if (complaintCount >= MAX_COMPLAINTS)
    {
        printf("Complaint box is full.\n");
        return;
    }

    printf("Enter your complaint: ");
    getchar(); // Clear the newline character left by previous scanf
    fgets(complaints[complaintCount].complaint, COMPLAINT_LENGTH, stdin);
    // Remove newline character if present
    complaints[complaintCount].complaint[strcspn(complaints[complaintCount].complaint, "\n")] = 0;
    strcpy(complaints[complaintCount].complainedBy, users[userIndex].username);
    complaintCount++;
    printf("Complaint submitted successfully!\n");
}

void viewComplaints(int userIndex)
{
    if (strcmp(users[userIndex].username, "admin") != 0)
    {
        printf("Access denied.\n");
        return;
    }

    printf("Complaints:\n");
    if (complaintCount == 0)
    {
        printf("No complaints available.\n");
        return;
    }
    for (int i = 0; i < complaintCount; i++)
    {
        printf("%d. %s (Complained by: %s)\n", i + 1, complaints[i].complaint, complaints[i].complainedBy);
    }
}

void deleteComplaint(int userIndex)
{
    if (strcmp(users[userIndex].username, "admin") != 0)
    {
        printf("Access denied.\n");
        return;
    }

    int complaintIndex;
    printf("Enter complaint index to delete (1-%d): ", complaintCount);
    scanf("%d", &complaintIndex);
    complaintIndex--; // Convert to 0-based index

    if (complaintIndex < 0 || complaintIndex >= complaintCount)
    {
        printf("Invalid complaint index.\n");
        return;
    }

    for (int i = complaintIndex; i < complaintCount - 1; i++)
    {
        complaints[i] = complaints[i + 1];
    }
    complaintCount--;
    printf("Complaint deleted successfully!\n");
}

void saveData()
{
    FILE *file;

    // Save users
    file = fopen("users.dat", "wb");
    fwrite(&userCount, sizeof(int), 1, file);
    fwrite(users, sizeof(User), userCount, file);
    fclose(file);

    // Save reservations
    file = fopen("reservations.dat", "wb");
    fwrite(&reservationCount, sizeof(int), 1, file);
    fwrite(reservations, sizeof(Reservation), reservationCount, file);
    fclose(file);

    // Save notices
    file = fopen("notices.dat", "wb");
    fwrite(&noticeCount, sizeof(int), 1, file);
    fwrite(notices, sizeof(Notice), noticeCount, file);
    fclose(file);

    // Save complaints
    file = fopen("complaints.dat", "wb");
    fwrite(&complaintCount, sizeof(int), 1, file);
    fwrite(complaints, sizeof(Complaint), complaintCount, file);
    fclose(file);
}

void loadData()
{
    FILE *file;

    // Load users
    file = fopen("users.dat", "rb");
    if (file)
    {
        fread(&userCount, sizeof(int), 1, file);
        fread(users, sizeof(User), userCount, file);
        fclose(file);
    }
    else
    {
        initializeSystem(); // Initialize with default user if no data file is found
    }

    // Load reservations
    file = fopen("reservations.dat", "rb");
    if (file)
    {
        fread(&reservationCount, sizeof(int), 1, file);
        fread(reservations, sizeof(Reservation), reservationCount, file);
        fclose(file);
    }

    // Load notices
    file = fopen("notices.dat", "rb");
    if (file)
    {
        fread(&noticeCount, sizeof(int), 1, file);
        fread(notices, sizeof(Notice), noticeCount, file);
        fclose(file);
    }

    // Load complaints
    file = fopen("complaints.dat", "rb");
    if (file)
    {
        fread(&complaintCount, sizeof(int), 1, file);
        fread(complaints, sizeof(Complaint), complaintCount, file);
        fclose(file);
    }
}
