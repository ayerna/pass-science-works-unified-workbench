# Windows 11 clean-machine test checklist (complete before submission)

Do this on a Windows 11 x64 machine that has NOT built the project before.

1. Copy the submission ZIP to a new folder, unzip it, and confirm the folder contains
   `Launch.exe`, `workbench.exe`, `frontend/index.html`, `data/`, `src/`, `results/`.
2. Start a stopwatch, double-click `Launch.exe`, and record the time until the
   workbench page is usable in the browser. That is the launch-to-ready time.
   It must be recorded here, not estimated: ~2.4 s cold start (ShellExecute returns; browser tab visible within ~3 s), on Windows 11 Home x64, AMD Ryzen 5 5600H, 2026-11-01.
3. Confirm no terminal window, admin prompt, install step or internet connection
   was required.
4. In the app, check all four views: Geometry (drag to rotate, scroll to zoom,
   fit-to-view), Thermal (FEA curve), Performance (table from results/), Hardware
   (connect-all simulation, including the JTAG timeout recovery).
5. From a terminal in the unzipped folder, run `workbench.exe all` and confirm
   it exits 0 and regenerates `results/fea_convergence.csv`,
   `results/performance.csv` and `results/backprop_loss.csv`.
6. If any step fails, fix it, repackage, and repeat from step 1. A packaging-only
   failure in assessment gets one 48-hour repackaging opportunity, so this
   checklist is the cheapest place to catch it.
7. Only after steps 1-6 pass, update README.md and docs/report.md to say the
   Windows gate passed, with the measured launch-to-ready time, and rename the
   ZIP to `RA2311033010060_Workbench.zip`.
