#include "raylib.h"
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MAX_ANGLE_SPEED 3.0f

typedef struct CircleNode {
    float x, y;              // Position of the circle
    float radius;            // Radius of the circle
    float angle;             // Current angle for rotation
    float angularSpeed;      // Rotation speed
    float length;            // Length of the line connecting to the previous circle
    Color color;             // Color of the circle
    struct CircleNode* next; // Pointer to the next circle
} CircleNode;

// Function to create a new circle
CircleNode* CreateCircle(float x, float y, float radius, float length, float angularSpeed, Color color) {
    CircleNode* newCircle = (CircleNode*)malloc(sizeof(CircleNode));
    newCircle->x = x;
    newCircle->y = y;
    newCircle->radius = radius;
    newCircle->angle = 0;
    newCircle->angularSpeed = angularSpeed;
    newCircle->length = length;
    newCircle->color = color;
    newCircle->next = NULL;
    return newCircle;
}

// Function to add a circle to the tail of the linked list
void AddCircle(CircleNode* head) {
    CircleNode* current = head;
    while (current->next != NULL) {
        current = current->next;
    }

    // Generate random properties for the new circle
    float radius = 10 + rand() % 20;
    float length = 50 + rand() % 100;
    float angularSpeed = ((float)(rand() % 100) / 100.0f) * MAX_ANGLE_SPEED;
    Color colors[] = {
        RED,
        YELLOW,
        PURPLE,
        BLUE,
        WHITE
    };

    CircleNode* newCircle = CreateCircle(current->x, current->y, radius, length, angularSpeed, colors[rand() %5]);
    current->next = newCircle;
}

// Function to remove the tail circle from the linked list
void RemoveCircle(CircleNode* head) {
    if (head->next == NULL) return; // No tail to remove

    CircleNode* current = head;
    CircleNode* previous = NULL;

    while (current->next != NULL) {
        previous = current;
        current = current->next;
    }

    free(current);
    if (previous != NULL) previous->next = NULL;
}

// Function to update the positions of the circles
void UpdateCircles(CircleNode* head, float deltaTime) {
    CircleNode* current = head->next;
    CircleNode* previous = head;

    while (current != NULL) {
        current->angle += current->angularSpeed * deltaTime;
        current->x = previous->x + current->length * cos(current->angle);
        current->y = previous->y + current->length * sin(current->angle);
        previous = current;
        current = current->next;
    }
}

// Function to draw the circles and lines
void DrawCircles(CircleNode* head) {
    CircleNode* current = head;
    CircleNode* previous = NULL;

    // Draw lines
    while (current != NULL) {
        if (previous != NULL) {
            DrawLine((int)previous->x, (int)previous->y, (int)current->x, (int)current->y, GRAY);
        }
        previous = current;
        current = current->next;
    }

    // Draw circles
    current = head;
    while (current != NULL) {
        DrawCircle((int)current->x, (int)current->y, current->radius, current->color);
        current = current->next;
    }
}

int main() {
    // Initialize the window
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Multi-Arm Pendulum");
    SetTargetFPS(60);

    // Seed random number generator
    srand((unsigned int)time(NULL));

    // Create the root circle
    CircleNode* head = CreateCircle(SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2, 20, 0, 0, RED);

    while (!WindowShouldClose()) {
        // Handle input
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
            AddCircle(head);
        }
        if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
            RemoveCircle(head);
        }

        // Update circles
        float deltaTime = GetFrameTime();
        UpdateCircles(head, deltaTime);

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);
        DrawCircles(head);
        EndDrawing();
    }

    // Free memory
    while (head != NULL) {
        CircleNode* temp = head;
        head = head->next;
        free(temp);
    }

    CloseWindow();
    return 0;
}
