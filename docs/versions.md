# Dependency and version information

No third-party libraries are used. The backend is C++17 standard library
only; the frontend is plain HTML/CSS/JavaScript with no framework; the
development server is Python 3 standard library only.

Build environment versions, verified on 2026-11-01:
- Linux compiler: g++ (Ubuntu 13.3.0-6ubuntu2~24.04.1) 13.3.0
- CMake: 3.28.3
- Windows cross-compiler: x86_64-w64-mingw32-g++ (GCC) 13-win32 (MinGW-w64 13.2.0)
- Wine (verification only, not a submission dependency): 9.0~repack-4build3
- Python (dev server and plot script only): 3.12.3
- C++ standard: C++17, optimisation -O2, warnings -Wall -Wextra
- Static linking for MinGW builds: -static, plus -mwindows for Launch.exe,
  so the Windows binaries import only system DLLs (KERNEL32, msvcrt,
  SHELL32, USER32)

On a native Windows build, CMake and a C++17 compiler are required, plus
the Visual C++ redistributable for that compiler if it is not statically
linked. The statically linked MinGW cross-build IS the tested Windows
deliverable; `build_windows.bat` also provides a native MSVC/MinGW path.

Native Windows 11 test: unconfirmed in this working copy. Do not claim a clean-machine
launch, timing, or native result until the checklist is run on that machine.

No paid AI plan, commercial solver licence, physical board or specialist
CAD software was used. No credentials or personal data are included.
