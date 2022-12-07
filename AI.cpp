/*
 * Copyright 2022 University of Michigan EECS183
 *
 * AI.cpp
 * Project UID 28eb18c2c1ce490aada441e65559efdd
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */

#include "AI.h"
#include <cassert>

// This file is used only in the Reach, not the Core.
// You do not need to make any changes to this file for the Core

string getAIMoveString(const BuildingState& buildingState) {
    int floorWithMaxPeople = 0;
    int prevMaxPeople = 0;
    int totalAngerLevel = 0;
    int maxAngerLevel = 0;
    int secondMaxAngerLevel = 0;
    int thirdMaxAngerLevel = 0;
    int floorWithMaxAngerLevel = 0;
    int floorWithSecondMaxAngerLevel = 0;
    int floorWithThirdMaxAngerLevel = 0;
    int e0currentFloor = buildingState.elevators[0].currentFloor;
    int e1currentFloor = buildingState.elevators[1].currentFloor;
    int e2currentFloor = buildingState.elevators[2].currentFloor;
    int temp = 0;
    int e0floorAnger = 0;
    string AImove = "";
    string targetFloor = "";
    string targetFloor2 = "";
    string targetFloor3 = "";
    string elevatorInService = "";
    string elevatorToPickup = "";
    bool passmove = false;
    bool pickupmove = false;
    bool servicemove = false;

   
    //return pass move if all elevators are servicing
    if (buildingState.elevators[0].isServicing == true && buildingState.elevators[1].isServicing == true &&
        buildingState.elevators[2].isServicing == true) {
        return "";
    }
    //check if floors have no ppl
    /*passmove = true;
    for (int i = 0; i < NUM_FLOORS; i++) {
        if (buildingState.floors[i].numPeople != 0) {
            passmove = false;
        }
    }
    if (passmove = true) {
        return "";
    } */
    
    //NEW PASS FUNCTION DRAFT. MIGHT CAUSE ERRORS
    int totalNumPeopleInBuilding = 0;
    for (int i = 0; i < NUM_FLOORS; i++) {
        if (buildingState.floors[i].numPeople != 0) {
            totalNumPeopleInBuilding++;
        }
    }
    if (totalNumPeopleInBuilding == 0) {
        return "";
    }

    
    //finding the target floor by checking highest cummulative anger level on each floor
    for (int i = 0; i < NUM_FLOORS; i++) {
        totalAngerLevel = 0;
        for (int j = 0; j < buildingState.floors[i].numPeople; j++) {
            totalAngerLevel += buildingState.floors[i].people[j].angerLevel;
        }
        if (totalAngerLevel > maxAngerLevel) {
            maxAngerLevel = totalAngerLevel;
            floorWithMaxAngerLevel = i;
        }
        //may cause errors on the first for loop iteration.must make sure the first for loop doesnt iterate through this if statement somehow
        else if (totalAngerLevel > secondMaxAngerLevel) {
            secondMaxAngerLevel = totalAngerLevel;
            floorWithSecondMaxAngerLevel = i;
        }
        else if (totalAngerLevel > thirdMaxAngerLevel) {
            thirdMaxAngerLevel = totalAngerLevel;
            floorWithThirdMaxAngerLevel = i;
        }
    }

    //converting target floors from intergers to string
    targetFloor = to_string(floorWithMaxAngerLevel);
    targetFloor2 = to_string(floorWithSecondMaxAngerLevel);
    targetFloor3 = to_string(floorWithThirdMaxAngerLevel);
    //check if the currentfloor the elevator is on is "valuable" enough to pickup. change this . make 3 if statments to check if each elevator is on the target floor
    /*for (int i = 0; i < buildingState.floors[e0currentFloor].numPeople; i++) {
        e0floorAnger += buildingState.floors[e0currentFloor].people[i].angerLevel;
    }
    *///changed from 3 if statements, to an if else branch so that only the first available elevator goes? or it doesnt rlly matter tbh
    if (buildingState.elevators[0].currentFloor == floorWithMaxAngerLevel && !buildingState.elevators[0].isServicing
        && buildingState.floors[buildingState.elevators[0].currentFloor].numPeople != 0) {
        pickupmove = true;
        elevatorToPickup = "0";
    }
    if (buildingState.elevators[1].currentFloor == floorWithMaxAngerLevel && !buildingState.elevators[1].isServicing
        && buildingState.floors[buildingState.elevators[1].currentFloor].numPeople != 0) {
        pickupmove = true;
        elevatorToPickup = "1";
    }
    if (buildingState.elevators[2].currentFloor == floorWithMaxAngerLevel && !buildingState.elevators[2].isServicing
        && buildingState.floors[buildingState.elevators[2].currentFloor].numPeople != 0) {
        pickupmove = true;
        elevatorToPickup = "2";
    }

    if (pickupmove == true) {
        AImove = "e" + elevatorToPickup + "p";
        return AImove;
    }
    
    //if (buildingState.floors[e0currentFloor])
   
     //find out which elevators are idle, so that you can move that elevator
    if (buildingState.elevators[0].isServicing == false) {
        elevatorInService = "0";
    }
    else if (buildingState.elevators[1].isServicing == false) {
        elevatorInService = "1";
    }
    else if (buildingState.elevators[2].isServicing == false) {
        elevatorInService = "2";
    }
      

    /*for (int i = 0; i < NUM_FLOORS; i++) {
        totalAngerLevel = 0;
        for (int j = 0; j < buildingState.floors[i].numPeople; j++) {
            totalAngerLevel = buildingState.floors[i].people[j].angerLevel;
        }
    } */

        

        //if elevator is not servicing, then send it to floor 4(to stay idle)
        /*for (int i = 0; i < NUM_ELEVATORS; i++) {
            if (!buildingState.elevators[i].isServicing) {
                targetFloor = 4;
            }
        }*/

    
     //combine elements to make it a viable string move input
    
    
     AImove = "e" + elevatorInService + "f" + targetFloor;  

    return AImove;
}

string getAIPickupList(const Move& move, const BuildingState& buildingState, 
                       const Floor& floorToPickup) {

    int numPeopleonCurrentFloor = floorToPickup.getNumPeople();
    string goingUpString = "";
    string goingDownString = "";
    int numPeopleUp = 0;
    int numPeopleDown = 0;
    int difference = 0;
    
    for (int i = 0; i < numPeopleonCurrentFloor; i++) {
        difference = floorToPickup.getPersonByIndex(i).getTargetFloor() - floorToPickup.getPersonByIndex(i).getCurrentFloor();

        if (difference > 0) {
            numPeopleUp++;
            goingUpString += to_string(i);
        }
        if (difference < 0) {
            numPeopleDown++;
            goingDownString += to_string(i);
        }
    }

    if (numPeopleDown > numPeopleUp) {
        return goingDownString;
    }
    else {
        return goingUpString;
    }


    return "";
}
