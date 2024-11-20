/**
 * Title         : COSC 514 : Operating System
 * Specification : Struct for FCFS CPU Scheduling Algorithms
 *
 */

// struct definitions

// struct for First come first 
// serve cpu scheduling algorithm 
struct FcfsReadyQueue {

    int burstTime;
    int processId;

};

// struct variable declaration section
struct FcfsReadyQueue fcfsReadyQueueItem[100];

