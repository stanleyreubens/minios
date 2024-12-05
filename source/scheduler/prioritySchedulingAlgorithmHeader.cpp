/**
 * Title         : COSC 514 : Operating System
 * Specification : Struct for Priority Non Preemptive CPU Scheduling Algorithms
 *
 */

// struct definitions

// struct for Priority
// cpu scheduling algorithm 
struct prReadyQueue {

    int burstTime;
    int processId;
	int priority;

};

// struct variable declaration section
struct prReadyQueue prReadyQueueItem[100];
struct prReadyQueue prReadyQueueCpyItem[100];

