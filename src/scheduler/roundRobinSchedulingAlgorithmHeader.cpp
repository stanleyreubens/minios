/**
 * Title         : COSC 514 : Operating System
 * Specification : Struct for Round Robin CPU Scheduling Algorithms
 *
 */

// struct definitions

// struct for round robin
// cpu scheduling algorithm 
struct RrReadyQueue {

    int burstTime;
    int processId;

};

// variable declaration section
int rrTimeQuantum = 2; 
int rrIterationCounter = 0;
int rrIterationRepeatFlag = 1;

// struct variable declaration section
struct RrReadyQueue rrReadyQueueItem[100];
struct RrReadyQueue rrReadyQueueCpyItem[100];