 
# Waste Manager (C)


####Authors
Nathan Olff (nathan.olff@utbm.fr)<br>
Maxime Bourgeois (maxime.bourgeois@utbm.fr)

####Objective
This project was created as part of a course on system's architecture (on UNIX).<br>
The idea is to simulate the waste collection services of a city.<br>
A city consists of a certain number of homes.<br>
There is two types of trashes :
- individual trashes
- sorting center : trashes available for multiple users. Each sorting center is composed of several (big) trashes and is linked to some users (idea of neighborhood)
When a trash is full (or nearly full), it asks the collect manager to send a truck to empty it.

####Implementation
The course was about system's architecture (especially on UNIX/Linux).<br>
Each trash and eash home is an independant thread.<br>
These threads synchronize and communicate through monitors (with mutex and signal through system's calls).
