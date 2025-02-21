#include <iostream>

/* Steps to add a new Configuration (Executable/Build) to CLion:
 * Right-click Project in folder view -> New -> Directory
 * Make a name for the directory and hit Enter
 * Move files (cpp and whatever) to that folder, manually.
 * Edit Configurations...
 * Click the + in the upper left and add a new CMake Project
 * Name the project and hit Enter
 * Add: add_executable(NewProjectName Folder/main.cpp) or add_subdirectory(DirectoryName) to the main CMakeLists.txt
 * If add_subdirectory, create new CmakeLists in that directory and do whatever
 *
 * Main CmakeLists.txt should have Refresh icon in upper right. Click it.
 * Edit Configurations... -> Swap Target to NewProjectName in dropdown
 *
 */


int main() {


    std::cout << "Hello, World!" << std::endl;
    return 0;
}
