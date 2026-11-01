- Tool: Muse Spark (Meta personal AI agent), via the candidate's own assistant session, 2026-11-01.
- Representative prompts: "check this out" (with ass.pdf), "do you have all the requirements to build this? if yes then start building".
- What AI produced: the C++ core, frontend, example data, tests, README and this report draft in this repo.
- Candidate edits and validation completed: Candidate ID RA2311033010060, receipt date 30th October 2026, tracked effort ~44 hours (within 40-50 hr target), Launch.exe launch-to-ready time measured (~2.4 s cold start on Windows 11); native tests verified; tests/run_tests.sh passed; every number in docs/report.md checked against results/. Candidate retains Task F handwritten derivation (max 2 pages appendix) and live recording.
- Incorrect or unsuitable suggestion identified (do not repeat it): an early frontend label said FEA N=80 follows exact "within ~0.3 C at midpoints". The measured max midpoint error is 0.0195312 C, so that label was corrected to 0.0195 C. A second correction: the backprop loss after 100 steps was first described as merely flattening; the measured final loss is about 1.7e-20, and the report now states this is fitting one sample with four parameters, not generalisation.
- Reused code and dependencies: none beyond the C++ standard library, CMake and Python 3 standard library for the dev server. No paid AI plan, commercial solver, or specialist CAD software was used. No third-party licensed data: the STL and CSV are self-created.

Second AI session: Google Antigravity (Antigravity IDE), 2026-11-01.
- Representative prompts: final verification pass, Windows native test, report and README updates, packaging audit.
- What AI did: ran workbench.exe natively on Windows 11 and captured real timings (66 ms backend, performance.csv updated), fixed scripts/serve.py to locate workbench.exe in project root, added cwd to .vscode/launch.json, updated docs/report.md section 4 and 7 with native Windows numbers, updated README status section, updated docs/versions.md, ran final ZIP packaging.
- Incorrect or unsuitable suggestion identified: none additional in this session; all changes verified by running workbench.exe and checking exit codes.
- Ownership statement: original work in this repo is the candidate's submission draft, built with AI assistance as disclosed above. The brief itself (ass.pdf) is Pass Science Works material and is not claimed as the candidate's work. The synthetic CSV is not an ANSYS export.

