# minios
How to setup minios on windows

Building `minios` on Windows
Prerequisites
1. **Visual Studio**: Install Visual Studio with the 'Desktop development with C++' workload.
   - [Download Visual Studio](https://visualstudio.microsoft.com/)

2. **CMake**: Install CMake .
   - [Download CMake](https://cmake.org/download/)
Step-by-Step Guide to Building `minios`
Step 1: Set Up the Project Directory
1.	Unzip the minios file and 
2.	Open command prompt then go the folder of the unzipped minios. You can then run cd minios
3.	Remove the build folder

Step 2: Generate Build Files Using CMake
Command Prompt to generate the build files.
Using Command Prompt
1. Open **Command Prompt** and navigate to the `minios` directory.
   ```bash
   cd path\to\minios
   ```
2. Create a `build` directory.
   ```bash
   mkdir build
   cd build
   ```
3. Run the following CMake command to generate Visual Studio project files:
   ```bash
   cmake .. -G "Visual Studio 17 2022"
   ```
   Replace `"Visual Studio 17 2022"` with your installed version if different.

# Step 3: Build the Project
 Build with Command Line (MSBuild)

1. Navigate to the `build` directory:
   ```bash
   cd path\to\minios\build
   ```
2. Run the build command:
   ```bash
   msbuild minios.sln /p:Configuration=Release
   ```
   Replace `Release` with `Debug` if you want a debug build.
3. ```cd path\to\minios\build``` To run the executable program.

Note: make sure you add the path to your visual studio to your environment variables


# How to build on Mac

Install Homebrew if not already installed
```
/bin/bash -c "$(curl -fsSL https://raw.githubusercontent.com/Homebrew/install/HEAD/install.sh)"
```

1. Install CMake
```
brew install cmake
```

2. Go to the project folder
```
cd minios
```

3. Create the build script in your project root
```
touch build.sh
```

4. Copy the content below for the build script

```
#!/bin/bash

# build.sh - Build script for Mini OS Simulator

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m' # No Color

echo -e "${GREEN}Building Mini OS Simulator...${NC}"

# Create build directory if it doesn't exist
mkdir -p build
cd build

# Configure with CMake
echo -e "${GREEN}Configuring CMake...${NC}"
cmake .. || {
    echo -e "${RED}CMake configuration failed${NC}"
    exit 1
}

# Build the project
echo -e "${GREEN}Building project...${NC}"
make || {
    echo -e "${RED}Build failed${NC}"
    exit 1
}

echo -e "${GREEN}Build successful!${NC}"
echo -e "You can run the simulator with: ${GREEN}./minios${NC}"
```
5. Make it executable
```
chmod +x build.sh
```
6. run this to build the minios
```
./build.sh
```

6. Navigate to build directory
```
cd build
```

7. Run the simulator
./minios

# How to build on FreeBSD

1. Install required packages
```
pkg install cmake
pkg install gmake
pkg install git
pkg install llvm
pkg install googletest
```

2. Create a build script for FreeBSD freebsd_build.sh and copy the lines below to the file 
```
#!/bin/sh

# Create build directory
mkdir -p build
cd build

# Configure with CMake
cmake ..

# Build using gmake (FreeBSD's version of make)
gmake

# Run tests
gmake test
```

3. make the script executable

chmod +x freebsd_build.sh

4. Build the project

./freebsd_build.sh

5. Run the simulator

./build/minios

# DOCUMENTATION FILES CAN BE FOUND IN THE DOCUMENTATION FOLDER /DOCUMENTATION

# Here's a checklist link of the completed requirements
https://scandalous-droplet-fe4.notion.site/COSC-514-PROJECT-CHECKLIST-146477d01f628041af87faea91d75176

# Useage Expamles
```
# First check resources
os_sim> resources

# Create some processes
os_sim> create process1 10
os_sim> create process2 5

# Try allocating resources
os_sim> allocate 0 0
os_sim> allocate 1 1

# Check status
os_sim> status

# Try creating a deadlock
os_sim> allocate 0 1
os_sim> allocate 1 0
os_sim> deadlock

# Start the calculator process
os_sim> calculator

# Perform calculations within the calculator
> 5 + 3
Result: 8
> 10 ^ 2
Result: 100
> exit

# Exit back to the main simulator
os_sim>

# Design Decisions

## 1. Architecture Decisions
```
1.1 Modular Design
    - Separate components for process, thread, and resource management
    - Clear separation of concerns
    - Easy to extend and maintain
    Rationale: Facilitates maintainability and testing

1.2 Singleton Pattern
    - Used for ProcessManager and ResourceManager
    - Ensures single point of control
    - Thread-safe implementation
    Rationale: Centralized resource management and control

1.3 Thread Pool Design
    - Fixed-size thread pool
    - Task queue-based work distribution
    - FIFO scheduling
    Rationale: Efficient thread reuse and management

1.4 Resource Management
    - Hash table-based resource tracking
    - Graph-based deadlock detection
    - Priority inheritance for deadlock prevention
    Rationale: Efficient resource allocation and deadlock handling
```

## 2. Implementation Decisions
```
2.1 Data Structures
    - std::unordered_map for O(1) lookups
    - std::vector for ordered collections
    - std::queue for task management
    Rationale: Balance between performance and functionality

2.2 Synchronization
    - Mutex-based synchronization
    - Condition variables for thread coordination
    - Fine-grained locking
    Rationale: Prevent race conditions while minimizing contention

2.3 Error Handling
    - Enum-based error codes
    - Exception handling for critical errors
    - Detailed error reporting
    Rationale: Clear error communication and handling

2.4 Memory Management
    - RAII principles
    - Smart pointers for resource management
    - Clear ownership semantics
    Rationale: Prevent memory leaks and ensure cleanup
```

# Known Limitations

## 1. Technical Limitations
```
1.1 Scalability
    - Fixed thread pool size
    - No dynamic resource creation
    - Limited process priority levels

1.2 Performance
    - Potential mutex contention under high load
    - O(n log n) scheduling complexity
    - Memory overhead from data structures

1.3 Features
    - No virtual memory management
    - Simplified process scheduling
    - Basic IPC mechanisms only
```

## 2. Implementation Limitations
```
2.1 Process Management
    - No process hierarchies
    - Limited process states
    - No process migration

2.2 Resource Management
    - Simple deadlock resolution
    - No resource priorities
    - Limited resource types

2.3 Threading
    - No thread priorities
    - Fixed thread pool size
    - Limited thread states
```

## 3. Architectural Limitations
```
3.1 Design
    - Single-system design (no networking)
    - No distributed capabilities
    - Limited extensibility points

3.2 Security
    - No access control
    - No process isolation
    - No security features

3.3 Monitoring
    - Basic performance metrics only
    - Limited debugging capabilities
    - No system-wide monitoring
```

# References Used
## 1. Operating Systems Concepts
```
1.1 Books
    - "Operating System Concepts" by Silberschatz, Galvin, and Gagne
    - "Modern Operating Systems" by Tanenbaum
    - "Operating Systems: Three Easy Pieces" by Arpaci-Dusseau

1.2 Papers
    - "The Deadlock Problem in Operating Systems"
    - "Priority Inheritance Protocols"
    - "Resource Allocation Graphs"
```

## 2. Implementation References
```
2.1 C++ Standards
    - C++17 Standard Library
    - STL Documentation
    - C++ Concurrency in Action (Anthony Williams)

2.2 Design Patterns
    - "Design Patterns" by Gang of Four
    - "Pattern-Oriented Software Architecture"
    - "C++ Software Design Patterns"

2.3 Best Practices
    - Google C++ Style Guide
    - C++ Core Guidelines
    - Modern C++ Design (Andrei Alexandrescu)
```

## 3. Online Resources
```
3.1 Documentation
    - cppreference.com
    - C++ Standards Committee papers
    - Boost documentation

3.2 Articles
    - Thread Pool implementations
    - Deadlock detection algorithms
    - Process scheduling algorithms

3.3 Research Papers
    - "Implementation of Process Management in Operating Systems"
    - "Resource Allocation Techniques"
    - "Thread Pool Patterns for Service Oriented Architectures"
```
