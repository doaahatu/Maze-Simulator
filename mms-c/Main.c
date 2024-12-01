#include <stdio.h>
#include "API.h"
#include <stdlib.h>
#include <math.h>   // For fmin()

extern int mazeCosts[MAZE_SIZE][MAZE_SIZE];

// Custom logging function renamed to `log_message`
void log_message(char* text) {
    fprintf(stderr, "%s\n", text);
    fflush(stderr);
}

// Enum for direction
typedef enum {
    NORTH,
    EAST,
    SOUTH,
    WEST
} Direction;

// Left-Hand Rule implementation
void leftHandRule() {
    log_message("Executing Left-Hand Rule...");
    int leftWall = API_wallLeft();
    int frontWall = API_wallFront();
    int rightWall = API_wallRight();

    if (!leftWall) {
        log_message("No wall on the left. Turning left...");
        API_turnLeft();
        API_moveForward();
    } else if (!frontWall) {
        log_message("No wall in front. Moving forward...");
        API_moveForward();
    } else if (!rightWall) {
        log_message("No wall on the right. Turning right...");
        API_turnRight();
        API_moveForward();
    } else {
        log_message("Surrounded by walls! Turning right...");
        API_turnRight();
    }
}

// Right-Hand Rule implementation
void rightHandRule() {
    log_message("Executing Right-Hand Rule...");
    int leftWall = API_wallLeft();
    int frontWall = API_wallFront();
    int rightWall = API_wallRight();

    if (!rightWall) {
        log_message("No wall on the right. Turning right...");
        API_turnRight();
        API_moveForward();
    } else if (!frontWall) {
        log_message("No wall in front. Moving forward...");
        API_moveForward();
    } else if (!leftWall) {
        log_message("No wall on the left. Turning left...");
        API_turnLeft();
        API_moveForward();
    } else {
        log_message("Surrounded by walls! Turning left...");
        API_turnLeft();
    }
}



// Initialize maze costs with Manhattan distances
void initializeMazeCosts(int goalX, int goalY) {
    for (int x = 0; x < MAZE_SIZE; x++) {
        for (int y = 0; y < MAZE_SIZE; y++) {
            mazeCosts[x][y] = abs(goalX - x) + abs(goalY - y);
        }
    }
}

// Update maze costs based on walls around a given position
void updateCostsBasedOnWalls(int x, int y) {
    if (!API_wallFront()) {
        mazeCosts[x][y + 1] = fmin(mazeCosts[x][y + 1], mazeCosts[x][y] + 1);
    }
    if (!API_wallRight()) {
        mazeCosts[x + 1][y] = fmin(mazeCosts[x + 1][y], mazeCosts[x][y] + 1);
    }
    if (!API_wallLeft()) {
        mazeCosts[x - 1][y] = fmin(mazeCosts[x - 1][y], mazeCosts[x][y] + 1);
    }
}

// Determine next move based on flood fill algorithm
void floodFillNavigation(int* currentX, int* currentY, Direction* currentDirection, int goalX, int goalY) {
    log_message("Executing Flood Fill Navigation...");

    // If goal is reached
    if (*currentX == goalX && *currentY == goalY) {
        log_message("Goal reached!");
        return;
    }

    // Update maze costs based on walls
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (abs(dx) + abs(dy) != 1) continue; // Skip diagonals and the current cell
            int nx = *currentX + dx;
            int ny = *currentY + dy;

            if (nx < 0 || nx >= MAZE_SIZE || ny < 0 || ny >= MAZE_SIZE) continue; // Out of bounds
            if ((dx == 0 && dy == 1 && API_wallFront()) ||
                (dx == 1 && dy == 0 && API_wallRight()) ||
                (dx == -1 && dy == 0 && API_wallLeft())) continue; // Wall blocks the way

            mazeCosts[nx][ny] = fmin(mazeCosts[nx][ny], mazeCosts[*currentX][*currentY] + 1);
        }
    }

    // Find the next cell with the minimum cost
    int nextX = *currentX, nextY = *currentY, minCost = mazeCosts[*currentX][*currentY];
    for (int dx = -1; dx <= 1; dx++) {
        for (int dy = -1; dy <= 1; dy++) {
            if (abs(dx) + abs(dy) != 1) continue;
            int nx = *currentX + dx;
            int ny = *currentY + dy;

            if (nx < 0 || nx >= MAZE_SIZE || ny < 0 || ny >= MAZE_SIZE) continue;
            if (mazeCosts[nx][ny] < minCost) {
                nextX = nx;
                nextY = ny;
                minCost = mazeCosts[nx][ny];
            }
        }
    }

    // Determine movement direction
    if (nextX > *currentX) {
        while (*currentDirection != EAST) {
            API_turnRight();
            *currentDirection = (*currentDirection + 1) % 4;
        }
    } else if (nextX < *currentX) {
        while (*currentDirection != WEST) {
            API_turnLeft();
            *currentDirection = (*currentDirection + 3) % 4;
        }
    } else if (nextY > *currentY) {
        while (*currentDirection != NORTH) {
            API_turnLeft();
            *currentDirection = (*currentDirection + 3) % 4;
        }
    } else if (nextY < *currentY) {
        while (*currentDirection != SOUTH) {
            API_turnRight();
            *currentDirection = (*currentDirection + 1) % 4;
        }
    }

    if (API_moveForward()) {
        *currentX = nextX;
        *currentY = nextY;
        log_message("Moved to next cell.");
    } else {
        log_message("Failed to move forward.");
    }
}


int main(int argc, char* argv[]) {
    log_message("Starting simulation...");

    // Initial simulator configuration
    API_setColor(0, 0, 'G');
    API_setText(0, 0, "Start");

    int currentX = 0, currentY = 0;
    Direction currentDirection = NORTH; // Initialize direction
    int goalX = 7, goalY = 7;           // Example goal position

    initializeMazeCosts(goalX, goalY);

    while (1) {
        if (API_wasReset()) {
            log_message("Maze reset detected. Reinitializing...");
            initializeMazeCosts(goalX, goalY);
            currentX = 0;
            currentY = 0;
            currentDirection = NORTH;
            API_ackReset();
            continue;
        }

        floodFillNavigation(&currentX, &currentY, &currentDirection, goalX, goalY);
        if (currentX == goalX && currentY == goalY) {
            API_setColor(goalX, goalY, 'R');
            API_setText(goalX, goalY, "Goal");
            log_message("Reached the goal. Stopping simulation.");
            break;
        }

        // Call the navigation logic
        // Toggle between leftHandRule() and rightHandRule() as needed
        //leftHandRule();
        //rightHandRule();
    }

    log_message("Simulation ended.");
    return 0;
}