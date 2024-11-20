/**
 * Title         : COSC 514 : Operating System
 * Specification : Struct for Preemptive SJF CPU Scheduling Algorithms
 *
 */

// struct definitions

// struct for Preemptive Shortest Job
// First cpu scheduling algorithm 
struct SjfPreReadyQueue {

    int burstTime;
    int processId;

};

// variable declaration section
int timeQuantum = 2; 
int iterationCounter = 0;
int iterationRepeatFlag = 1;

// struct variable declaration section
struct SjfPreReadyQueue sjfPreReadyQueueItem[100];
struct SjfPreReadyQueue sjfPreReadyQueueCpyItem[100];