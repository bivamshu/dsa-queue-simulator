#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "traffic_simulation.h"

// Global queues for lanes
Queue laneQueues[4];         // Queues for lanes A, B, C, D
int lanePriorities[4] = {0}; // Priority levels for lanes (0 = normal, 1 = high)

const SDL_Color VEHICLE_COLORS[] = {
    {0, 0, 255, 255}, // REGULAR_CAR: Blue
    {255, 0, 0, 255}, // AMBULANCE: Red
    {0, 0, 128, 255}, // POLICE_CAR: Dark Blue
    {255, 69, 0, 255} // FIRE_TRUCK: Orange-Red
};

void initializeTrafficLights(TrafficLight *lights)
{
    // Define the size of the traffic light (square)
    int trafficLightSize = 20; // Width and height of the traffic light (square)

    // Adjust the positions to center the traffic lights on the middle lane
    lights[0] = (TrafficLight){
        .state = RED,
        .timer = 0,
        .position = {INTERSECTION_X - trafficLightSize / 2, INTERSECTION_Y - LANE_WIDTH - trafficLightSize, trafficLightSize, trafficLightSize},
        .direction = DIRECTION_NORTH};

    lights[1] = (TrafficLight){
        .state = RED,
        .timer = 0,
        .position = {INTERSECTION_X - trafficLightSize / 2, INTERSECTION_Y + LANE_WIDTH, trafficLightSize, trafficLightSize},
        .direction = DIRECTION_SOUTH};

    lights[2] = (TrafficLight){
        .state = GREEN,
        .timer = 0,
        .position = {INTERSECTION_X + LANE_WIDTH, INTERSECTION_Y - trafficLightSize / 2, trafficLightSize, trafficLightSize},
        .direction = DIRECTION_EAST};

    lights[3] = (TrafficLight){
        .state = GREEN,
        .timer = 0,
        .position = {INTERSECTION_X - LANE_WIDTH - trafficLightSize, INTERSECTION_Y - trafficLightSize / 2, trafficLightSize, trafficLightSize},
        .direction = DIRECTION_WEST};
}