#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TOTAL_SEATS 30  // Total seats per flight

typedef struct Waitlist {
    char name[50];
    int age;
    char flight[10];
    char departureLocation[50];
    char destinationLocation[50];
    char departureTime[20];
    char estimatedArrivalTime[20];
    float distance;
    char seatNumber[5];  // Seat number (e.g., 12A)
    struct Waitlist* next;
} Waitlist;

// Seat availability tracker (simple array for each flight)
bool seatMap[TOTAL_SEATS] = {false};  // false means seat is available

Waitlist* waitHead = NULL;
Waitlist* waitTail = NULL;

// Function to find the next available seat
const char* assignSeat() {
    static char seat[5];  // e.g., "12A"
    for (int i = 0; i < TOTAL_SEATS; i++) {
        if (!seatMap[i]) {
            int row = i / 2 + 1;         // Rows 1-15
            char seatLetter = (i % 2 == 0) ? 'A' : 'B';  // Seats A & B
            sprintf(seat, "%d%c", row, seatLetter);
            seatMap[i] = true; // Mark seat as taken
            return seat;
        }
    }
    return NULL;  // No seats available
}

void bookTicket() {
    char name[50], flight[10], depLoc[50], destLoc[50], depTime[20], eta[20];
    int age;
    float distance;

    printf("Enter passenger name: ");
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;

    printf("Enter age: ");
    scanf("%d", &age);
    getchar();

    printf("Enter flight code: ");
    fgets(flight, sizeof(flight), stdin);
    flight[strcspn(flight, "\n")] = 0;

    printf("Enter departure location: ");
    fgets(depLoc, sizeof(depLoc), stdin);
    depLoc[strcspn(depLoc, "\n")] = 0;

    printf("Enter destination location: ");
    fgets(destLoc, sizeof(destLoc), stdin);
    destLoc[strcspn(destLoc, "\n")] = 0;

    printf("Enter departure time (e.g. 10:00): ");
    fgets(depTime, sizeof(depTime), stdin);
    depTime[strcspn(depTime, "\n")] = 0;

    // Automatic distance calculation (placeholder)
    distance = 1000;  // Default placeholder distance

    // Assuming average speed of 500 km/h
    float travelHours = distance / 500.0;

    // Automatic ETA calculation
    int hours, minutes;
    sscanf(depTime, "%d:%d", &hours, &minutes);
    minutes += (int)(travelHours * 60);
    hours += minutes / 60;
    minutes %= 60;
    sprintf(eta, "%02d:%02d", hours, minutes);

    // Assign seat number
    const char* seat = assignSeat();
    if (seat == NULL) {
        printf("Sorry, no seats available on this flight.\n");
        return;
    }

    // Create new waitlist node
    Waitlist* newWaitlist = (Waitlist*)malloc(sizeof(Waitlist));
    if (!newWaitlist) {
        printf("Memory allocation failed!\n");
        return;
    }

    strcpy(newWaitlist->name, name);
    newWaitlist->age = age;
    strcpy(newWaitlist->flight, flight);
    strcpy(newWaitlist->departureLocation, depLoc);
    strcpy(newWaitlist->destinationLocation, destLoc);
    strcpy(newWaitlist->departureTime, depTime);
    strcpy(newWaitlist->estimatedArrivalTime, eta);
    newWaitlist->distance = distance;
    strcpy(newWaitlist->seatNumber, seat);
    newWaitlist->next = NULL;

    if (waitTail == NULL) {
        waitHead = waitTail = newWaitlist;
    } else {
        waitTail->next = newWaitlist;
        waitTail = newWaitlist;
    }

    printf("\nTicket booked! Details below:\n");
    printf("Passenger: %s | Age: %d | Flight: %s\n", name, age, flight);
    printf("From: %s -> To: %s\n", depLoc, destLoc);
    printf("Departs at: %s | ETA: %s | Distance: %.2f km\n", depTime, eta, distance);
    printf("Seat Number: %s\n", seat);
}

void displayBookings() {
    if (waitHead == NULL) {
        printf("No current bookings.\n");
        return;
    }

    printf("\n--- All Booked Passengers ---\n");
    Waitlist* current = waitHead;
    while (current != NULL) {
        printf("Passenger: %s | Age: %d | Flight: %s\n", current->name, current->age, current->flight);
        printf("From %s to %s | Departure: %s | ETA: %s | Distance: %.2f km\n",
               current->departureLocation, current->destinationLocation,
               current->departureTime, current->estimatedArrivalTime,
               current->distance);
        printf("Seat Number: %s\n\n", current->seatNumber);
        current = current->next;
    }
}

int main() {
    int choice;

    while (1) {
        printf("\n--- Airline Reservation System ---\n");
        printf("1. Book Ticket\n");
        printf("2. Display Bookings\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);
        getchar(); // clear newline

        switch (choice) {
            case 1: bookTicket(); break;
            case 2: displayBookings(); break;
            case 3: printf("Exiting...\n"); return 0;
            default: printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
