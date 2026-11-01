# Screenshot walkthrough (equivalent to a demo video, per the brief)

CandidateID: ______ (fill in before submission)
Machine and OS for this walkthrough: ______
Date and time started: ______
Launch-to-ready time (stopwatch, double-click to usable page): ______ seconds

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
Launch-to-ready time for this run: ______ seconds

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
Inspected node from this run: ______

## 5. Thermal: FEA curve and convergence
What to show: the Thermal view with the exact and FEA curves, plus the
convergence CSV values below.

[Screenshot: Thermal view]
Log / CSV from this run (paste results/fea_convergence.csv here):
```
(elements, max_midpoint_error_C)
```

## 6. Performance: serial vs parallel
What to show: the Performance view table, plus the batch log.

[Screenshot: Performance view]
Log from this run (paste the workbench.exe batch output here):
```
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
```
Validation from this run:
```
workbench.exe validate-stl data/example.stl
workbench.exe validate-csv data/ansys_temperature_synthetic.csv
```

## 9. What was not shown, honestly
- Handwritten backprop appendix: separate, scanned by the candidate (see Task F).
- Real Windows launch-to-ready time: recorded in step 2 above, from this
  machine, not from the Linux or Wine runs.
- Known limitations: see README.md (Windows editor config untested in VS Code,
  larger STL/CSV files not benchmarked beyond a few thousand items).
