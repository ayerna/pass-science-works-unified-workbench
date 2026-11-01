# Unified Engineering Workbench (Pass Science Works, Common Internship Assignment)

## One-click steps (Windows 11 x64, target)
1. Unzip the submission ZIP into any folder.
2. Double-click `Launch.exe`.
3. The workbench opens in the default browser, with Geometry, Thermal, Performance and Hardware views. No terminal, PowerShell, admin rights, install or first-run build, and no internet is required for the standalone launcher.

WINDOWS STATUS (native, 2026-11-01): `workbench.exe all`, `validate-stl` and `validate-csv` tested natively on Windows 11 Home x64 (AMD Ryzen 5 5600H, 6C/12T, 15.3 GB RAM). All exit 0. FEA errors: 1.25 / 0.3125 / 0.078125 / 0.0195312 C. Backprop gradients verified. Serial/parallel agreement 0 C. Backend compute time: 66 ms. The binaries are PE32+ x86-64, statically linked (GCC runtime), importing only system DLLs (KERNEL32, msvcrt, SHELL32, USER32), so no MinGW DLLs are needed beside them. `Launch.exe` (GUI subsystem, 541 KB) calls `workbench.exe` with `CREATE_NO_WINDOW` then opens `frontend/index.html` via `ShellExecute`. Launch-to-ready time (double-click to browser showing the workbench): record this from the packaged ZIP as the final step before submission; the backend-only portion is 66 ms, the remainder is browser open time. Fill in the time in docs/windows-test-checklist.md and docs/screenshot-walkthrough.md. Native Windows build also available via `build_windows.bat`, packaged by `scripts/package_windows.ps1`.

## Source build (any platform)
```
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j4
./build/workbench all        # FEA + batch + backprop + mocks, writes results/
./build/workbench validate-stl data/example.stl
./build/workbench validate-csv data/ansys_temperature_synthetic.csv
bash tests/run_tests.sh
```
On Windows the build produces `build/Launch.exe` (copy of workbench.exe) next to the packaged app; copy `Launch.exe`, `workbench.exe`, `frontend/` and `results/` together for the one-click package, and rename the final archive to `CandidateID_Workbench.zip` once the CandidateID is known. Do NOT ship personal data or credentials in the ZIP.

## Editor one-action run
`.vscode/launch.json` runs `scripts/serve.py` (Python 3 standard library only, no extension beyond a Python/debugger setup if your VS Code needs one) which regenerates results with the built binary if present and serves the frontend on http://localhost:8000. The standalone launcher does not require VS Code.

## Architecture
- `src/fea.hpp`: 1D linear-element assembly, Dirichlet BCs moved to the RHS, midpoint error.
- `src/parsers.hpp`: ASCII STL and ANSYS CSV validation in C++.
- `src/parallel_batch.hpp`: deterministic batch (seed 42), bounded thread pool, input-order results.
- `src/backprop.hpp`: analytic gradients, central finite-difference check, 100-step loss trajectory.
- `src/main.cpp`: CLI only. `frontend/index.html`: presentation only, no backend logic in JS except drawing and the exact-solution curve for display.
- `data/`: self-created STL cube and a SYNTHETIC CSV (not a real ANSYS export).
- `results/`: regenerated CSVs, never hand-edited.
- `docs/`: report draft, AI-use log, embedded design notes.

## Supported formats and limits
ASCII STL only; ANSYS CSV with header `node_id,x_m,y_m,z_m,temperature_C` only. Native ANSYS and native CAD solids are out of scope per the brief. Tested here: 12-triangle STL, 25-node CSV. Larger files are parsed by the same C++ code but not benchmarked beyond a few thousand items, stated as a limitation.

## Known limitations
1. Launch-to-ready time from the packaged ZIP still needs a stopwatch measurement by the candidate (backend-only is 66 ms; total includes browser open time). Fill in docs/windows-test-checklist.md step 2 and docs/screenshot-walkthrough.md step 2 before submission. workbench.exe and validate-* pass natively on Windows 11 (see status above).
2. Handwritten backprop pages are not in this repo: they must be handwritten by the candidate (max 2 pages) and scanned in as the appendix.
3. Demo video not yet recorded: the brief allows a timestamped screenshot walkthrough with logs instead, which is the fallback in `docs/screenshot-walkthrough.md`.
4. No real hardware: Task E devices are mock adapters, clearly labelled simulated.
5. CandidateID, receipt date, actual effort log and measured launch-to-ready time are still to be filled in by the candidate. Do not invent them.
6. VS Code one-action run (`Run Workbench (one action)` in `.vscode/launch.json`) requires the Python extension and Python 3 in PATH; it runs `scripts/serve.py` which finds `workbench.exe` in the project root. This has been smoke-tested (serve.py candidate list fixed); a full end-to-end VS Code session remains for the candidate to verify.

