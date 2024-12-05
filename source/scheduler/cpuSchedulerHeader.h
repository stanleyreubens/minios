/**
 * Title         : COSC 514 : Operating System
 * Specification : Implement the CPU Scheduling Algorithms
 *
 */

// header files includes
#include<time.h>
#include<stdio.h>
#include<cstring>
#include<fstream>
#include<iomanip>
#include<unistd.h>
#include<iostream>
#include<libutil.h>
#include<sys/wait.h>
#include<sys/user.h>
#include <libproc.h>
#include<sys/types.h>
#include<sys/sysctl.h>
#include<vm/vm_param.h>

// scheduling algorithm 
// structs files includes
#include "roundRobinSchedulingAlgorithmHeader.cpp"
#include "shortestJobFirstSchedulingAlgorithmHeader.cpp"
#include "firstComeFirstServerSchedulingAlgorithmHeader.cpp"
#include "shortestJobFirstPreemptiveSchedulingAlgorithmHeader.cpp"

using namespace std;




