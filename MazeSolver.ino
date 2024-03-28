#include <Zumo32U4.h>

Zumo32U4Motors motors;
Zumo32U4LCD lcd;
Zumo32U4LineSensors lineSensors;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4ButtonA buttonA;
Zumo32U4Buzzer buzzer;


unsigned int lineSensorValues[3];

// Define robot direction
enum Direction { NORTH, EAST, SOUTH, WEST };

// how many houses are detected
int detectedhouse = 0;

//define maze dimensions
bool** maze;
int mazeWidth;
int mazeHeight;

//define the robot position
int robotPositionX=0;
int robotPositionY=0;
Direction robotDirection = NORTH;

// Store original starting position
int originalPositionX = 0;
int originalPositionY = 0;

// Function to set robot position
void setRobotPosition(int x, int y) {
  robotPositionX = x;
  robotPositionY = y;
}



//Get robot position functions

int getRobotPositionX() {
  return robotPositionX;
}

int getRobotPositionY() {
  return robotPositionY;
}

// Function to update robot direction based on turn
void updateDirection(Direction newDirection) {
  robotDirection = newDirection;
}

void updatePosition() {
  switch (robotDirection) {
    case NORTH:
      robotPositionY--;
      break;
    case EAST:
      robotPositionX++;
      break;
    case SOUTH:
      robotPositionY++;
      break;
    case WEST:
      robotPositionX--;
      break;
  }
}

//defining maximum number of points
const int Max_points = 1000;
// Create an array to store points, each point represented by two integers (x, y)
int storedPoints[Max_points][2];
int numStoredPoints = 0; // Variable to keep track of the number of stored points

void storePoint() {
  if (numStoredPoints < Max_points) {
    // Store the current position as a point
    storedPoints[numStoredPoints][0] = robotPositionX;
    storedPoints[numStoredPoints][1] = robotPositionY;
    numStoredPoints++; // Increment the count of stored points
  } else {
    Serial.println("Maximum number of stored points reached!");
  }
}

void accessStoredPoints() {
  // Iterate over the stored points and print their coordinates
  for (int i = 0; i < numStoredPoints; ++i) {
    Serial.print("Stored Point ");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print("x = ");
    Serial.print(storedPoints[i][0]);
    Serial.print(", y = ");
    Serial.println(storedPoints[i][1]);
  }
}

void setup() {
  buttonA.waitForPress();
  lineSensors.initThreeSensors();
  proxSensors.initThreeSensors();
  buttonA.waitForRelease();
  originalPositionX = 0;
  originalPositionY = 0;
  setRobotPosition(originalPositionX, originalPositionY);
}

void loop() {
   while(detectedhouse <= 2) {
    solveMaze();
    if (detectHouse()) {
      detectedhouse++;

      delay(200);
      turnAround();
    }
  } 
}

  int t1 = 600; // Threshold for detecting a line (black space)
  int t2 = 250; // Lower threshold for detecting white space

void solveMaze() {
    // Read line sensor values from all three sensors
    lineSensors.read(lineSensorValues, QTR_EMITTERS_ON);

    // Thresholds for detecting lines and white spaces

    lcd.clear();
    lcd.print("Scanning...");
    if (lineSensorValues[0] > t1 || lineSensorValues[1] > t1 || lineSensorValues[2] > t1) {
    // Line detected by any sensor
    motors.setSpeeds(0, 0); // Stop to decide next move

    if (canTurnRight()) {
        // If right is clear, prefer turning right
        turnRight();
    } else if (canTurnLeft()) {
        // If cannot turn right but can go forward, proceed
        turnLeft();
    } else if (canGoForward()) {
        // If right and forward are blocked, try left
        goStraight();
    } else {
        // No option but to turn around
        goBackWard();
        delay(500); // Back up a bit before turning around for better maneuvering
        turnAround();
    }
} 
else {
    // If no line is detected, continue moving forward
    goStraight();
}
   
}


bool canTurnRight() {
  // Consider the rightmost sensors for turning right
  return lineSensorValues[2] < t2;
}


bool canTurnLeft() {
  // Use the leftmost sensors to determine if turning left is appropriate
  return lineSensorValues[0] < t2;
}

bool canGoForward() {
  // Use the leftmost sensors to determine if turning left is appropriate
  return lineSensorValues[1] < t2;
}

void goStraight() {
  motors.setSpeeds(60, 60);
  updatePosition();
  storePoint();
}

void turnRight() {
  motors.setSpeeds(100, -100);
  delay(250); // Adjust for a 90-degree turn
  motors.setSpeeds(0, 0);
  switch(robotDirection){
    case NORTH:
      updateDirection(EAST);
      break;
    case EAST:
      updateDirection(SOUTH);
      break;
    case SOUTH:
      updateDirection(WEST);
      break;
    case WEST:
      updateDirection(NORTH);
      break;
  }
}

void turnLeft() {
  motors.setSpeeds(-100, 100);
  delay(250); // Adjust for a 90-degree turn
  motors.setSpeeds(0, 0);
  switch(robotDirection){
    case NORTH:
      updateDirection(WEST);
      break;
    case EAST:
      updateDirection(NORTH);
      break;
    case SOUTH:
      updateDirection(EAST);
      break;
    case WEST:
      updateDirection(SOUTH);
      break;

  }
  
}

void turnAround() {
  motors.setSpeeds(100, -100);
  delay(500); // Adjust for a 180-degree turn
  motors.setSpeeds(0, 0);
}

void goBackWard() {
  motors.setSpeeds(-100, -100);
  delay(200);
  motors.setSpeeds(0, 0);
}

bool detectHouse() {
   proxSensors.read();
  int center_leftsensor = proxSensors.countsFrontWithLeftLeds();
  int center_rightsensor = proxSensors.countsFrontWithRightLeds();
  if (center_leftsensor == 6 && center_rightsensor == 6) {
      buzzer.playFromProgramSpace(PSTR("!>g32>>c32"));
      delay(4000);
      return true;
  }else{
    return false;
  }
}