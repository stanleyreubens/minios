/**
 * Problem Statement : Driver file for CPU scheduling algorithm.
 * Author            : Saurabh Shirgaonkar
 * Date              : 01 Nov, 2024, 12:00 noon 
 *
 */

// header files
#include "cpuSchedulerHeader.h"

// main
int main(int argc, char* argv[]) {
		
	// variable declaration section
	int userOption;
	int processCount;
	int waitTime=0;
	int turnAroundTime=0;
	
	cout<< endl << "Real Time CPU Scheduling Simulator:" << endl; 
	cout<< endl << "Menu:";
	cout<< endl << "-----------------------------------------------------------------"; 
	cout<< endl << "1. First Come First Serve CPU Scheduling Algorithm"; 
	cout<< endl << "2. Shortest Job First (Non-Preemptive) CPU Scheduling Algorithm";
	cout<< endl << "3. Shortest Job First (Preemptive) CPU Scheduling Algorithm";
	cout<< endl << "4. Round Robin CPU Scheduling Algorithm";
	cout<< endl << endl << "-----------------------------------------------------------------"; 
	cout<< endl << "Enter option and press enter: ";
			
	cin >> userOption;	
    cout<< endl << "-----------------------------------------------------------------"; 
    cout<< endl << "User selected option: " << userOption;  	
	cout<< endl;
	
	if(argc == 1) {

        cout<< endl << "-----------------------------------------------------------------"; 
		cout<< endl << "Enter the number of processes: ";
		cin >> processCount;
	   	
	} else if(argc == 2) {
	
       processCount = atoi(argv[1]);
	
	}
	
	int day = userOption;
	
	switch (day) {
	
    	case 1:
		
		    cout<< endl << "-----------------------------------------------------------------"; 
	        cout<< endl << "First Come First Serve CPU Scheduling Algorithm" << endl;
			
			cout<< endl << "-----------------------------------------------------------------"; 
			cout<< endl << "Enter process related information: " << endl << endl;
				
			for(int fcfsProcessCounter=0; fcfsProcessCounter<processCount; fcfsProcessCounter++) {
				
				cout << "Process " << fcfsProcessCounter+1 << " id: ";
				cin >> fcfsReadyQueueItem[fcfsProcessCounter].processId;
				cout << "Process " << fcfsProcessCounter+1 << " burst time in milliseconds: ";
				cin >> fcfsReadyQueueItem[fcfsProcessCounter].burstTime;
								
			}
			
			cout<< endl << "-----------------------------------------------------------------"; 
			cout<< endl << "User entered process related information is as follows:" << endl;
			for(int fcfsProcessCounter=0; fcfsProcessCounter<processCount; fcfsProcessCounter++) {
				
				cout << endl << "Process " << fcfsProcessCounter+1 << " id: " << fcfsReadyQueueItem[fcfsProcessCounter].processId;
				cout << endl << "Process " << fcfsProcessCounter+1 << " burst time in milliseconds: " << fcfsReadyQueueItem[fcfsProcessCounter].burstTime;
								
			}
			
			cout<< endl << endl << "-----------------------------------------------------------------"; 
			cout<< endl << "Waiting time & turnaround time:" << endl;
			
			for(int fcfsProcessCounter=0; fcfsProcessCounter<processCount; fcfsProcessCounter++) {
				
				cout << endl << "Process id: " << fcfsReadyQueueItem[fcfsProcessCounter].processId;				
				turnAroundTime = turnAroundTime + fcfsReadyQueueItem[fcfsProcessCounter].burstTime;
				cout << endl << "Waiting time in milliseconds: " << waitTime;
				cout << endl << "Turnaround time in milliseconds: " << turnAroundTime;
				waitTime = waitTime + fcfsReadyQueueItem[fcfsProcessCounter].burstTime;
								
			}
			
			cout<< endl << endl << "-----------------------------------------------------------------"; 
						
		    break;
	  
	    case 2:
		
		    cout<< endl << "-----------------------------------------------------------------"; 
	        cout<< endl << "Shortest Job First CPU Scheduling Algorithm" << endl;
			
			cout<< endl << "-----------------------------------------------------------------"; 
			cout<< endl << "Enter process related information: " << endl << endl;
				
			for(int sjfProcessCounter=0; sjfProcessCounter<processCount; sjfProcessCounter++) {
				
				cout << "Process " << sjfProcessCounter+1 << " id: ";
				cin >> sjfReadyQueueItem[sjfProcessCounter].processId;
				cout << "Process " << sjfProcessCounter+1 << " burst time in milliseconds: ";
				cin >> sjfReadyQueueItem[sjfProcessCounter].burstTime;
								
			}
			
			cout<< endl << "-----------------------------------------------------------------"; 
			cout<< endl << "User entered process related information is as follows:" << endl;
			for(int sjfProcessCounter=0; sjfProcessCounter<processCount; sjfProcessCounter++) {
				
				cout << endl << "Process " << sjfProcessCounter+1 << " id: " << sjfReadyQueueItem[sjfProcessCounter].processId;
				cout << endl << "Process " << sjfProcessCounter+1 << " burst time in milliseconds: " << sjfReadyQueueItem[sjfProcessCounter].burstTime;
                sjfReadyQueueCpyItem[sjfProcessCounter].processId = sjfReadyQueueItem[sjfProcessCounter].processId;
                sjfReadyQueueCpyItem[sjfProcessCounter].burstTime = sjfReadyQueueItem[sjfProcessCounter].burstTime;					
							
			}
			
			// code for sorting
			
			for(int sjfProcessCounterOuter=0; sjfProcessCounterOuter<processCount; sjfProcessCounterOuter++) {
			
				for(int sjfProcessCounter=0; sjfProcessCounter<processCount-1;sjfProcessCounter++) {
					
					int iThProcessId, iThBurstTime;	
																
					if(sjfReadyQueueCpyItem[sjfProcessCounter].burstTime > sjfReadyQueueCpyItem[sjfProcessCounter+1].burstTime) {
						
						iThProcessId = sjfReadyQueueCpyItem[sjfProcessCounter].processId;
						iThBurstTime = sjfReadyQueueCpyItem[sjfProcessCounter].burstTime;
						
						sjfReadyQueueCpyItem[sjfProcessCounter].processId = sjfReadyQueueCpyItem[sjfProcessCounter+1].processId;
						sjfReadyQueueCpyItem[sjfProcessCounter].burstTime = sjfReadyQueueCpyItem[sjfProcessCounter+1].burstTime;
						
						sjfReadyQueueCpyItem[sjfProcessCounter+1].processId = iThProcessId;
						sjfReadyQueueCpyItem[sjfProcessCounter+1].burstTime = iThBurstTime;
						
					}
													
				}
				
	        }
			
			cout<< endl << endl << "-----------------------------------------------------------------"; 
			cout<< endl << "Waiting time & turnaround time:" << endl;
			
			for(int sjfProcessCounter=0; sjfProcessCounter<processCount; sjfProcessCounter++) {
				
				cout << endl << "Process id: " << sjfReadyQueueCpyItem[sjfProcessCounter].processId;				
				turnAroundTime = turnAroundTime + sjfReadyQueueCpyItem[sjfProcessCounter].burstTime;
				cout << endl << "Waiting time in milliseconds: " << waitTime;
				cout << endl << "Turnaround time in milliseconds: " << turnAroundTime;
				waitTime = waitTime + sjfReadyQueueCpyItem[sjfProcessCounter].burstTime;
								
			}
			
			cout<< endl << endl << "-----------------------------------------------------------------"; 
			
		    break;
			
		case 3:
			
			cout<< endl << "-----------------------------------------------------------------"; 
	        cout<< endl << "Pre-emptive Shortest Job First CPU Scheduling Algorithm" << endl;
			
			cout<< endl << "-----------------------------------------------------------------"; 
		    cout<< endl << "Enter the time quantum  in milliseconds: ";
		    cin >> timeQuantum;
			
			cout<< endl << "-----------------------------------------------------------------"; 
			cout<< endl << "Enter process related information: " << endl << endl;
				
			for(int sjfPreProcessCounter=0; sjfPreProcessCounter<processCount; sjfPreProcessCounter++) {
				
				cout << "Process " << sjfPreProcessCounter+1 << " id: ";
				cin >> sjfPreReadyQueueItem[sjfPreProcessCounter].processId;
				cout << "Process " << sjfPreProcessCounter+1 << " burst time in milliseconds: ";
				cin >> sjfPreReadyQueueItem[sjfPreProcessCounter].burstTime;
								
			}
			
			cout<< endl << "-----------------------------------------------------------------"; 
			cout<< endl << "User entered process related information is as follows:" << endl;
			for(int sjfPreProcessCounter=0; sjfPreProcessCounter<processCount; sjfPreProcessCounter++) {
				
				cout << endl << "Process " << sjfPreProcessCounter+1 << " id: " << sjfPreReadyQueueItem[sjfPreProcessCounter].processId;
				cout << endl << "Process " << sjfPreProcessCounter+1 << " burst time in milliseconds: " << sjfPreReadyQueueItem[sjfPreProcessCounter].burstTime;
                sjfPreReadyQueueCpyItem[sjfPreProcessCounter].processId = sjfPreReadyQueueItem[sjfPreProcessCounter].processId;
                sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime = sjfPreReadyQueueItem[sjfPreProcessCounter].burstTime;					
							
			}
			
			// code for sorting
			
			for(int sjfPreProcessCounterOuter=0; sjfPreProcessCounterOuter<processCount; sjfPreProcessCounterOuter++) {
			
				for(int sjfPreProcessCounter=0; sjfPreProcessCounter<processCount-1;sjfPreProcessCounter++) {
					
					int iThProcessId, iThBurstTime;	
																
					if(sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime > sjfPreReadyQueueCpyItem[sjfPreProcessCounter+1].burstTime) {
						
						iThProcessId = sjfPreReadyQueueCpyItem[sjfPreProcessCounter].processId;
						iThBurstTime = sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime;
						
						sjfPreReadyQueueCpyItem[sjfPreProcessCounter].processId = sjfPreReadyQueueCpyItem[sjfPreProcessCounter+1].processId;
						sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime = sjfPreReadyQueueCpyItem[sjfPreProcessCounter+1].burstTime;
						
						sjfPreReadyQueueCpyItem[sjfPreProcessCounter+1].processId = iThProcessId;
						sjfPreReadyQueueCpyItem[sjfPreProcessCounter+1].burstTime = iThBurstTime;
						
					}
													
				}
				
	        }
			
			cout<< endl << endl << "-----------------------------------------------------------------"; 
			cout<< endl << "Waiting time & turnaround time:" << endl;
			
			while (iterationRepeatFlag == 1) {
				
				if(iterationCounter==0) {
					iterationCounter = iterationCounter + 1;
					iterationRepeatFlag = 0;
				} else {
				    iterationCounter = iterationCounter + 1;	
				}
			
				for(int sjfPreProcessCounter=0; sjfPreProcessCounter<processCount; sjfPreProcessCounter++) {
									
					if(sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime > 0) {
						
						if(sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime >= timeQuantum) {
						
						    cout << endl << "Process id: " << sjfPreReadyQueueCpyItem[sjfPreProcessCounter].processId << " burst time : "  << sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime;				
							//turnAroundTime = turnAroundTime + sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime;
							//cout << endl << "Waiting time in milliseconds: " << waitTime;
							//cout << endl << "Turnaround time in milliseconds: " << turnAroundTime;
							//waitTime = waitTime + sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime;
						
						    sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime = sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime - timeQuantum;
							iterationRepeatFlag = 1;
						
						} else {

						    cout << endl << "Process id: " << sjfPreReadyQueueCpyItem[sjfPreProcessCounter].processId << " burst time : "  << sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime;				
                            sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime = 0;							
							
						}	
																		
					} else {
						
					    if(iterationCounter > 100) {
							iterationRepeatFlag = 0;
						}	
						
					}
									
				}
				
			}	
			
			cout<< endl << endl << "-----------------------------------------------------------------"; 
					    		
			break;
		
		case 4:
			
			cout<< endl << "-----------------------------------------------------------------"; 
	        cout<< endl << "Pre-emptive Round Robin CPU Scheduling Algorithm" << endl;
			
			cout<< endl << "-----------------------------------------------------------------"; 
		    cout<< endl << "Enter the time quantum  in milliseconds: ";
		    cin >> rrTimeQuantum;
			
			cout<< endl << "-----------------------------------------------------------------"; 
			cout<< endl << "Enter process related information: " << endl << endl;
				
			for(int rrProcessCounter=0; rrProcessCounter<processCount; rrProcessCounter++) {
				
				cout << "Process " << rrProcessCounter+1 << " id: ";
				cin >> rrReadyQueueItem[rrProcessCounter].processId;
				cout << "Process " << rrProcessCounter+1 << " burst time in milliseconds: ";
				cin >> rrReadyQueueItem[rrProcessCounter].burstTime;
								
			}
			
			cout<< endl << "-----------------------------------------------------------------"; 
			cout<< endl << "User entered process related information is as follows:" << endl;
			for(int rrProcessCounter=0; rrProcessCounter<processCount; rrProcessCounter++) {
				
				cout << endl << "Process " << rrProcessCounter+1 << " id: " << rrReadyQueueItem[rrProcessCounter].processId;
				cout << endl << "Process " << rrProcessCounter+1 << " burst time in milliseconds: " << rrReadyQueueItem[rrProcessCounter].burstTime;
                rrReadyQueueCpyItem[rrProcessCounter].processId = rrReadyQueueItem[rrProcessCounter].processId;
                rrReadyQueueCpyItem[rrProcessCounter].burstTime = rrReadyQueueItem[rrProcessCounter].burstTime;					
							
			}
									
			cout<< endl << endl << "-----------------------------------------------------------------"; 
			cout<< endl << "Waiting time & turnaround time:" << endl;
			
			while (rrIterationRepeatFlag == 1) {
				
				if(rrIterationCounter==0) {
					rrIterationCounter = rrIterationCounter + 1;
					rrIterationRepeatFlag = 0;
				} else {
				    rrIterationCounter = rrIterationCounter + 1;	
				}
			
				for(int rrProcessCounter=0; rrProcessCounter<processCount; rrProcessCounter++) {
									
					if(rrReadyQueueCpyItem[rrProcessCounter].burstTime > 0) {
						
						if(rrReadyQueueCpyItem[rrProcessCounter].burstTime >= timeQuantum) {
						
						    cout << endl << "Process id: " << rrReadyQueueCpyItem[rrProcessCounter].processId << " burst time : "  << rrReadyQueueCpyItem[rrProcessCounter].burstTime;				
							//turnAroundTime = turnAroundTime + sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime;
							//cout << endl << "Waiting time in milliseconds: " << waitTime;
							//cout << endl << "Turnaround time in milliseconds: " << turnAroundTime;
							//waitTime = waitTime + sjfPreReadyQueueCpyItem[sjfPreProcessCounter].burstTime;
						
						    rrReadyQueueCpyItem[rrProcessCounter].burstTime = rrReadyQueueCpyItem[rrProcessCounter].burstTime - timeQuantum;
							rrIterationRepeatFlag = 1;
						
						} else {

						    cout << endl << "Process id: " << rrReadyQueueCpyItem[rrProcessCounter].processId << " burst time : "  << rrReadyQueueCpyItem[rrProcessCounter].burstTime;				
                            rrReadyQueueCpyItem[rrProcessCounter].burstTime = 0;							
							
						}	
																		
					} else {
						
					    if(rrIterationCounter > 100) {
							rrIterationRepeatFlag = 0;
						}	
						
					}
									
				}
				
			}	
			
			cout<< endl << endl << "-----------------------------------------------------------------"; 
					    		
			break;
					
		default: 
		    cout << "Default";
			
	}
	
	cout << endl << endl;				
    return 0;

}