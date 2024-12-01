#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define BUFFER_SIZE 256
#define MAZE_SIZE 8
int mazeCosts[MAZE_SIZE][MAZE_SIZE];
int currentX = 0, currentY = 0;


int mazeCosts[MAZE_SIZE][MAZE_SIZE];

void log_message(char* text);

int getInteger(char* command) {
    printf("%s\n", command);
    fflush(stdout);
    char response[BUFFER_SIZE];
    fgets(response, BUFFER_SIZE, stdin);
    int value = atoi(response);
    return value;
}

int getBoolean(char* command) {
    printf("%s\n", command);
    fflush(stdout);
    char response[BUFFER_SIZE];
    fgets(response, BUFFER_SIZE, stdin);
    int value = (strcmp(response, "true\n") == 0);
    return value;
}

int getAck(char* command) {
    printf("%s\n", command);
    fflush(stdout);
    char response[BUFFER_SIZE];
    fgets(response, BUFFER_SIZE, stdin);
    int success = (strcmp(response, "ack\n") == 0);
    return success;
}

int API_mazeWidth() {
    return getInteger("mazeWidth");
}

int API_mazeHeight() {
    return getInteger("mazeHeight");
}

// Function to check if there's a wall in front
int API_wallFront() {
    // Send the command to check for a wall in front
    printf("wallFront\n");
    fflush(stdout);  // Ensure the command is sent immediately

    // Read the response from the simulator
    char response[BUFFER_SIZE];
    if (fgets(response, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read response from simulator.\n");
        return -1;  // Error case
    }

    // Debug: Print raw response
    fprintf(stderr, "Raw response: '%s'\n", response);

    // Process the response
    if (strcmp(response, "true\n") == 0) {
        return 1;  // Wall is present
    } else if (strcmp(response, "false\n") == 0) {
        return 0;  // No wall
    } else {
        fprintf(stderr, "Error: Invalid response received: '%s'\n", response);
        return -1;  // Invalid response
    }
}

// API function to check for a wall on the right
int API_wallRight() {
    printf("wallRight\n");
    fflush(stdout);

    char response[BUFFER_SIZE];
    if (fgets(response, BUFFER_SIZE, stdin) == NULL) {
        fprintf(stderr, "Error: Failed to read response from simulator.\n");
        return -1;  // Error case
    }

    // Debug: Log raw response
    fprintf(stderr, "Raw response: '%s'\n", response);

    if (strcmp(response, "true\n") == 0) {
        return 1;  // Wall is present on the right
    } else if (strcmp(response, "false\n") == 0) {
        return 0;  // No wall on the right
    } else {
        fprintf(stderr, "Invalid response received: '%s'\n", response);
        return -1;  // Invalid response
    }
}

// API function to check for a wall on the left
int API_wallLeft() {
    printf("wallLeft\n");
    fflush(stdout);

    char response[BUFFER_SIZE];
    if (fgets(response, BUFFER_SIZE, stdin) == NULL) {
        log_message("Error: Failed to read response from simulator.");
        return -1;  // Error
    }

    if (strcmp(response, "true\n") == 0) {
        return 1;  // Wall on the left
    } else if (strcmp(response, "false\n") == 0) {
        return 0;  // No wall on the left
    } else {
        fprintf(stderr, "Invalid response received: '%s'\n", response);
        return -1;  // Invalid response
    }
}

int API_moveForward() {
    int ack = getAck("moveForward");
    if (ack == 1) {
        fprintf(stderr, "Moved forward successfully.\n");
    } else {
        fprintf(stderr, "Failed to move forward.\n");
    }
    return ack;
}

void API_turnRight() {
    fprintf(stderr, "Turning right...\n");
    getAck("turnRight");
}

void API_turnLeft() {
    fprintf(stderr, "Turning left...\n");
    getAck("turnLeft");
}

void API_setWall(int x, int y, char direction) {
    if (direction != 'N' && direction != 'E' && direction != 'S' && direction != 'W') {
        fprintf(stderr, "Invalid direction '%c' for setting wall.\n", direction);
        return;
    }
    printf("setWall %d %d %c\n", x, y, direction);
    fflush(stdout);
    fprintf(stderr, "Set wall at (%d, %d) facing %c.\n", x, y, direction);
}

void API_clearWall(int x, int y, char direction) {
    if (direction != 'N' && direction != 'E' && direction != 'S' && direction != 'W') {
        fprintf(stderr, "Invalid direction '%c' for clearing wall.\n", direction);
        return;
    }
    printf("clearWall %d %d %c\n", x, y, direction);
    fflush(stdout);
    fprintf(stderr, "Cleared wall at (%d, %d) facing %c.\n", x, y, direction);
}

void API_setColor(int x, int y, char color) {
    if (color != 'R' && color != 'G' && color != 'B') {
        fprintf(stderr, "Invalid color '%c'. Supported colors: R, G, B.\n", color);
        return;
    }
    printf("setColor %d %d %c\n", x, y, color);
    fflush(stdout);
    fprintf(stderr, "Set color %c at (%d, %d).\n", color, x, y);
}

void API_clearColor(int x, int y) {
    printf("clearColor %d %d\n", x, y);
    fflush(stdout);
    fprintf(stderr, "Cleared color at (%d, %d).\n", x, y);
}

void API_clearAllColor() {
    printf("clearAllColor\n");
    fflush(stdout);
    fprintf(stderr, "Cleared all colors.\n");
}

void API_setText(int x, int y, char* text) {
    if (text == NULL) {
        fprintf(stderr, "Cannot set text at (%d, %d): text is NULL.\n", x, y);
        return;
    }
    printf("setText %d %d %s\n", x, y, text);
    fflush(stdout);
    fprintf(stderr, "Set text '%s' at (%d, %d).\n", text, x, y);
}

void API_clearText(int x, int y) {
    printf("clearText %d %d\n", x, y);
    fflush(stdout);
    fprintf(stderr, "Cleared text at (%d, %d).\n", x, y);
}

void API_clearAllText() {
    printf("clearAllText\n");
    fflush(stdout);
    fprintf(stderr, "Cleared all text.\n");
}

int API_wasReset() {
    int wasReset = getBoolean("wasReset");
    fprintf(stderr, "Checked reset status: %s\n", wasReset ? "Yes" : "No");
    return wasReset;
}

void API_ackReset() {
    fprintf(stderr, "Acknowledging reset...\n");
    getAck("ackReset");
    fprintf(stderr, "Reset acknowledged.\n");
}