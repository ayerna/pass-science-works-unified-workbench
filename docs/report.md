# Unified Engineering Workbench: Report (draft, to be kept to 6 pages in final PDF)

## 1. Design choices
A C++17 core (CLI, no GUI framework) plus a single-page HTML frontend. This keeps backend logic, numerical computation, file processing and concurrency in C++ as required, while the UI stays portable. The CLI is the testable source of truth; the browser only draws and validates previews, and the README states which check is authoritative. One-click delivery on Windows is a small launcher that regenerates results and opens the frontend; source build is via CMake.

## 2. FEA verification (Task C)
Rod: L = 1 m, A = 1e-4 m2, k = 10 W/(m K), q = 10,000 W/m3, T(0) = 100 C, T(L) = 20 C, insulated sides, constant properties. Element stiffness kA/h, element load qAh/2 per node, Dirichlet BCs moved to the RHS. Exact solution: T = 100 - 80x + 500x(1-x).

Measured max midpoint errors (FE linear interpolation, not nodes only):
- N=10: 1.25 C
- N=20: 0.3125 C
- N=40: 0.078125 C
- N=80: 0.0195312 C

Error quarters on each doubling, as expected for a smooth problem with linear elements. Assumptions: steady state, 1D, uniform properties, no lateral loss, no temperature-dependent k. Debugging insight: overwriting assembled BC rows double-counted neighbour stiffness; moving known terms to the RHS fixed it.

## 3. CAD / ANSYS visualisation (Task B)
Self-created 1 m cube STL (12 triangles, metres) and a synthetic CSV (25 nodes, metres and Celsius, labelled synthetic, never presented as a real ANSYS export). UI supports rotate, pan, zoom, fit-to-view, a temperature legend and click-to-inspect nodes. C++ parsers reject malformed, empty and non-finite inputs with line-specific messages; negative tests are in tests/run_tests.sh.

## 4. Performance (Task D)
Batch: 200 cases, mesh 40, fixed seed 42, k in 5..20, q in 5000..20000, identical cases and solver serial and parallel, bounded thread pool, input-order results. Machine (native Windows run): Windows 11 Home x64, AMD Ryzen 5 5600H (6 physical / 12 logical cores, 15.3 GB RAM), MinGW-w64 GCC 13 (-O2, statically linked). Warm-up excluded, 5 repetitions, median reported. Results in results/performance.csv: 1 worker 0.403 ms, 2 workers 0.471 ms (speedup 0.856, efficiency 0.428), 4 workers 0.382 ms (speedup 1.055, efficiency 0.264). Serial/parallel max difference 0 C, within the 1e-8 C limit. Limitation: the 200-case batch is small relative to thread-pool overhead; at 2 workers the median exceeds the serial time on this machine (overhead dominates on a fast CPU). This is expected and not hidden. A heavier workload or larger batch would show stronger scaling; the small batch was chosen to keep the demo fast. Launch-to-ready time on this Windows 11 machine: see README (recorded from double-click to workbench visible in browser).

## 5. Embedded design (Task E)
MCU board (STM32 Nucleo-style): USB-CDC host link, 3.3 V domain, ST-LINK owns SWD/JTAG. FPGA board (Artix-7-style): USB-JTAG host link, 3.3 V I/O with 1.8 V core, platform cable owns JTAG. JTAG debug adapter: host USB, target signals TCK/TMS/TDI/TDO/TRST, level-shifted to the target voltage. One common ground, separate 3.3 V domains, level shifting where a 5 V signal could meet a 3.3 V pin. No connector, voltage or protocol is assumed shared. A JTAG adapter is a debug interface, not a generic compute board: it does not run application code. Mock adapters demonstrate discovery, capability reporting, connect/disconnect and one telemetry message per device, plus a recoverable JTAG timeout (retry, then report). No real hardware, wiring, PCB or FPGA programming is claimed. Safe integration plan: connect ground first, verify target voltage before attaching JTAG, use level shifters, power boards from their own supplies, disconnect before rewiring.

## 6. Backprop (Task F)
h = tanh(0.5) = 0.462117, y_hat = 0.462117, loss = 1.182542. Gradients: dw = db = -1.209464, dv = -0.710682, dc = -1.537883. One simultaneous step at lr 0.1: w = 0.620946, b = 0.120946, v = 1.071068, c = 0.153788, new loss 0.685774. Finite-difference errors are about 1e-10, within the 1e-5 limit. Loss over 100 steps falls to about 1.7e-20: with 4 parameters fitting a single sample, this shows optimisation, not generalisation. Gradient direction: all gradients are negative because y_hat is below target, so gradient descent increases all four parameters. The handwritten derivation (max 2 pages) must be added by the candidate as the appendix; it is not in this draft.

## 7. Test outcomes and limitations
tests/run_tests.sh passes on Linux: positive and negative parser tests, FEA convergence, parallel agreement, gradient check and loss decrease. Windows delivery: `build_windows.bat` builds `Launch.exe` plus `workbench.exe` natively on Windows, `scripts/package_windows.ps1` packages them, and `toolchain-mingw.cmake` allows a MinGW cross-build from Linux. The cross-build produced PE32+ x86-64 `Launch.exe` (GUI subsystem, 541 KB) and `workbench.exe` (console, 2.7 MB), statically linked for the GCC runtime, importing only system DLLs (KERNEL32, msvcrt, SHELL32, USER32). Native Windows 11 verification (AMD Ryzen 5 5600H, 15.3 GB RAM): `workbench.exe all` exits 0, `validate-stl` and `validate-csv` exit 0, FEA errors identical (1.25 / 0.3125 / 0.078125 / 0.0195312 C), backprop gradients identical, serial/parallel agreement 0 C. Backend compute time: 66 ms. Launch-to-ready time (double-click Launch.exe to workbench visible in browser) must be measured by the candidate from the packaged ZIP on this machine (see docs/windows-test-checklist.md); the backend-only portion is 66 ms and the remainder is browser open time. Windows gate: all CLI tests pass natively; the browser-open step requires the candidate to record the wall-clock launch-to-ready time and claim the gate after the packaged unzip test. Remaining items for the candidate: CandidateID, receipt date, effort log, measured launch-to-ready time, demo video or completed screenshot walkthrough (docs/screenshot-walkthrough.md), and the handwritten backprop appendix. No personal data or credentials are included.

