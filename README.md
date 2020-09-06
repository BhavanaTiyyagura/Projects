# Traffic Light Controller Design using FSM

Designed a traffic light controller for the intersection of two equally busy one-way streets. The goal is to maximize traffic flow, minimize waiting time at a red light, and avoid accidents.

The basic idea is to prevent southbound cars to enter the intersection at the same time as westbound cars. In this system, the light pattern defines which road has right of way over the other. Since an output pattern to the lights is necessary to remain in a state, we will solve this system with a Moore FSM. It will have two inputs (car sensors on North and East roads) and six outputs (one for each light in the traffic signal.)  The six traffic lights are interfaced to Port B bits 5–0, and the two sensors are connected to Port E bits 1–0 of the TM4C123 microcontroller. 

PE1=0, PE0=0 means no cars exist on either road
PE1=0, PE0=1 means there are cars on the East road
PE1=1, PE0=0 means there are cars on the North road
PE1=1, PE0=1 means there are cars on both roads

goN,                 PB5-0 = 100001 makes it green on North and red on East
waitN,            PB5-0 = 100010 makes it yellow on North and red on East
goE,                 PB5-0 = 001100 makes it red on North and green on East
waitE,            PB5-0 = 010100 makes it red on North and yellow on East

