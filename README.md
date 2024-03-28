# Maze-Solver
Solving a maze using Arduino Zumo32u4


enum direction defines the robot direction so that it can know the direction it is going.
It is defined on line 24 and is first set to north meaning if you pick the robot up you should place it in the direction it is facing.

lines 20-22 were supposed to be for the robot to be able to detect the size of the maze regardless of the maze its in but does not work.

lines 33-36 is the function to store the robots current position. To get the robots current x and y there are 2 get functions for the robots position lines 41 to 48.

line 51 is a function that changes the value of the direction the robot is facing whenever it makes a turn. As the robot can turn many times during a bend it can have off readings for its true cardinal direction.

56-69 updates the position for the x and y of the robot so it can track the distance its traveled in certain directions. Does not really work as intended and an incremenetal counter with uniform movements most likely wouldve been much better.

74 - 77 is a 2 point array that can store 1000 values(in case it loses track and has to turn a lot to get to the house. Itll store the x and y values of the robot whenever it goes straight.

79-86 is the function to store a point and if the maximum number of points is reached itll print an error message saying so

90-99 prints out the coordinates of stored points although not much has been done with the points.

113 to 120 is the main loop and simply calls the functions and checks if theres a house in front. The house is usually detected very early and means the program doesnt run as intended.

128 to 153 is the solve maze loop which is the logic for the robot to traverse the maze. It will try to turn right if possible but if it can go straight it will do so. The logic depends on the line sensors of the maze but the maze is too wide for the sensors to activate a lot of the time so the car will get lost often. It defaults to turning right and going straight

164 to 177 is the sensors for checking if the robot can turn in that direction.

180 to 183 is letting the car go straight , updating its current position and storing the point it currently is at the go forward which was supposed to be for the recalling fucntion but was not able to work.

186 to 222 is the robot adjusting for 90 degree turns and a switch case statement so the robot knows its cardinal direction but in dead ends and when going diagonally it will get  lose track of it

288 to 237 are just functions for trying to turn. Alot of the time the robot will not be able to go backwards or turn around as 1 sensor will usually hit first.


240 to 249 detects the house and plays a sound but the house is detected from way too far away to be considered for the scenario.
