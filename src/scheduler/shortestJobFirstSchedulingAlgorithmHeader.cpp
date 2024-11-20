/**
 * Title         : COSC 514 : Operating System
 * Specification : Struct for SJF CPU Scheduling Algorithms
 *
 */

// struct definitions

// struct for Shortest Job First
// cpu scheduling algorithm 
struct SjfReadyQueue {

    int burstTime;
    int processId;

};

// struct variable declaration section
struct SjfReadyQueue sjfReadyQueueItem[100];
struct SjfReadyQueue sjfReadyQueueCpyItem[100];

