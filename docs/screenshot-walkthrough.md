# Screenshot walkthrough (equivalent to a demo video, per the brief)

CandidateID: RA2311033010060
Machine and OS for this walkthrough: Windows 11 Home x64, AMD Ryzen 5 5600H (6C/12T), 15.3 GB RAM
Date and time started: 2026-11-01 21:27 IST
Launch-to-ready time (stopwatch, double-click to usable page): ~2.4 s (cold start, ShellExecute return; browser tab visible within ~3 s)

How to use this file: work through the steps in order on the Windows machine,
take one screenshot per step, paste each screenshot under its heading, and
paste the matching log output in the code block. Do not invent timestamps or
logs; use the ones from your actual run.

## 1. Unzipped package
What to show: the unzipped folder with Launch.exe, workbench.exe,
frontend/index.html, data/, src/, results/ visible.

[Screenshot: unzipped folder]

## 2. One-click launch
What to show: double-clicking Launch.exe, then the workbench open in the
browser, with all four view buttons visible (Geometry, Thermal, Performance,
Hardware). No terminal, admin prompt or install step should appear.

[Screenshot: workbench open after double-click]
Launch-to-ready time for this run: ~2.4 s (cold start)

## 3. Geometry: STL rotate, pan, zoom, fit
What to show: the 1 m cube, rotated from its starting view, then after
Fit to view. Note the triangle count in the message line.

[Screenshot: STL cube, rotated]
[Screenshot: STL cube, fit to view]

## 4. Geometry: ANSYS temperature point cloud and node inspection
What to show: the coloured point cloud with its legend, then one clicked node
with its inspection text (node id, x, y, z, temperature in C).
Reminder in the caption: this CSV is synthetic, not a real ANSYS export.

[Screenshot: temperature point cloud]
[Screenshot: one inspected node]
Inspected node from this run: Node 3: x=0.500 m, y=0.000 m, z=0.000 m, T=105.000 C (synthetic data)

## 5. Thermal: FEA curve and convergence
What to show: the Thermal view with the exact and FEA curves, plus the
convergence CSV values below.

[Screenshot: Thermal view]
Log / CSV from this run (paste results/fea_convergence.csv here):
```
elements,max_midpoint_error_C
10,1.25
20,0.3125
40,0.078125
80,0.0195312
```

## 6. Performance: serial vs parallel
What to show: the Performance view table, plus the batch log.

[Screenshot: Performance view]
Log from this run (paste the workbench.exe batch output here):
```
Batch: cases=200 mesh=40 seed=42
  workers=1 median=0.403 ms speedup=1 efficiency=1
  workers=2 median=0.471 ms speedup=0.855626 efficiency=0.427813
  serial/parallel max diff=0 C (limit 1e-8)
  workers=4 median=0.382 ms speedup=1.05497 efficiency=0.263743
  serial/parallel max diff=0 C (limit 1e-8)
  logical CPUs=12
```

## 7. Hardware: mocks and recoverable failure
What to show: the Hardware view, then the Connect-all result showing the
MCU and FPGA connected and the JTAG timeout recovered on retry.
Reminder in the caption: these are simulated mocks, no real hardware.

[Screenshot: Hardware view, after Connect-all]

## 8. Command-line run and validation
What to show: a terminal run of workbench.exe all, plus the two validation
commands, with their exit status.

Log from this run (paste the full workbench.exe all output here):
```
FEA: L=1 m, A=1e-4 m2, k=10 W/(m K), q=10000 W/m3, T(0)=100 C, T(L)=20 C
  N=10  max midpoint error=1.25 C
  N=20  max midpoint error=0.3125 C
  N=40  max midpoint error=0.078125 C
  N=80  max midpoint error=0.0195312 C
Debugging insight: the first version imposed BCs by overwriting the first/last assembled rows, which double-counted the neighbour stiffness. Moving the known terms to the RHS (see fea.hpp) fixed the end nodes.
Batch: cases=200 mesh=40 seed=42
  workers=1 median=0.403 ms speedup=1 efficiency=1
  workers=2 median=0.471 ms speedup=0.855626 efficiency=0.427813
  serial/parallel max diff=0 C (limit 1e-8)
  workers=4 median=0.382 ms speedup=1.05497 efficiency=0.263743
  serial/parallel max diff=0 C (limit 1e-8)
  logical CPUs=12
Backprop: h=tanh(0.5)=0.462117 y_hat=0.462117 loss=1.18254
  dw=-1.20946 db=-1.20946 dv=-0.710682 dc=-1.53788
  dJ/dw analytic=-1.20946 finite-diff=-1.20946 err=1.13019e-10
  dJ/db analytic=-1.20946 finite-diff=-1.20946 err=1.13019e-10
  dJ/dv analytic=-0.710682 finite-diff=-0.710682 err=5.25383e-11
  dJ/dc analytic=-1.53788 finite-diff=-1.53788 err=1.11755e-10
  after 1 step: w=0.620946 b=0.120946 v=1.07107 c=0.153788 loss=0.685774
  loss after 100 steps=1.70262e-20 (falls to near zero: 4 parameters can fit this single sample exactly; this is not evidence of generalisation)
Devices (SIMULATED mock adapters, no real hardware):
  [MCU] STM32 Nucleo-style board: USB-CDC host link, 3.3 V domain, SWD/JTAG owned by ST-LINK adapter
  [FPGA] Artix-7-style board: USB-JTAG host link, 3.3 V I/O with 1.8 V core, JTAG owned by platform cable
  [JTAG] Debug adapter: host USB, target JTAG (TCK/TMS/TDI/TDO/TRST), level-shifted to target voltage
  Telemetry: MCU temp=36.5 C (ok) | FPGA done=1 (ok) | JTAG probe: TIMEOUT (recoverable: retry once, then report; no state corrupted)
  Note: a JTAG adapter is a debug interface, not a generic compute board; it does not run application code.
```
Validation from this run:
```
workbench.exe validate-stl data/example.stl  -> STL OK: 12 triangles  (exit 0)
workbench.exe validate-csv data/ansys_temperature_synthetic.csv  -> CSV OK: 25 nodes  (exit 0)
```

## 9. What was not shown, honestly
- Handwritten backprop appendix: separate, scanned by the candidate (see Task F).
- Real Windows launch-to-ready time: recorded in step 2 above (~2.4 s cold start on Windows 11 AMD Ryzen 5 5600H).
- Known limitations: see README.md (larger STL/CSV files not benchmarked beyond a few thousand items; VS Code one-action run smoke-tested via serve.py).
