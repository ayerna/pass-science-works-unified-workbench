cmake_minimum_required(VERSION 3.16)
project(UnifiedEngineeringWorkbench CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()
add_executable(workbench src/main.cpp)
target_include_directories(workbench PRIVATE src)
if(MSVC)
  target_compile_options(workbench PRIVATE /W4)
else()
  target_compile_options(workbench PRIVATE -Wall -Wextra -O2)
endif()
# Windows one-click target: Launch.exe is the launcher (src/launcher.cpp);
# workbench.exe is the C++ core it runs. Both must sit next to frontend/.
# See README "Windows one-click build" and build_windows.bat.
if(WIN32)
  add_executable(Launch src/launcher.cpp)
  target_link_libraries(Launch PRIVATE shell32)
endif()
# MinGW cross-build: static-link the GCC/libstdc++ runtime so the package
# runs on a clean Windows machine with no MinGW DLLs beside it, and make
# Launch a GUI-subsystem executable (no console window on double-click).
if(MINGW)
  target_link_options(workbench PRIVATE -static)
  target_link_options(Launch PRIVATE -static -mwindows)
endif()
