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