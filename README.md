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
Step 3: Build the Project
Option B: Build with Command Line (MSBuild)
1. Open the **Developer Command Prompt for Visual Studio**.
2. Navigate to the `build` directory:
   ```bash
   cd path\to\minios\build
   ```
3. Run the build command:
   ```bash
   msbuild minios.sln /p:Configuration=Release
   ```
   Replace `Release` with `Debug` if you want a debug build.
.

