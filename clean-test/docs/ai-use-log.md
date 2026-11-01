- Tool: Muse Spark (Meta personal AI agent), via the candidate's own assistant session, 2026-11-01.
- Representative prompts: "check this out" (with ass.pdf), "do you have all the requirements to build this? if yes then start building".
- What AI produced: the C++ core, frontend, example data, tests, README and this report draft in this repo.
- Candidate edits and validation completed: identity and receipt metadata are now marked unconfirmed; clean-machine Windows launch and screenshot evidence are not present in this working copy. The supplied source retains the stated numerical result files. The referenced handwritten appendix scans and live recording were not supplied here.
- Incorrect or unsuitable suggestion identified (do not repeat it): an early frontend label said FEA N=80 follows exact "within ~0.3 C at midpoints". The measured max midpoint error is 0.0195312 C, so that label was corrected to 0.0195 C. A second correction: the backprop loss after 100 steps was first described as merely flattening; the measured final loss is about 1.7e-20, and the report now states this is fitting one sample with four parameters, not generalisation.
- Reused code and dependencies: none beyond the C++ standard library, CMake and Python 3 standard library for the dev server. No paid AI plan, commercial solver, or specialist CAD software was used. No third-party licensed data: the STL and CSV are self-created.

Second AI session: Google Antigravity (Antigravity IDE), 2026-11-01.
- Representative prompts: final verification pass, Windows native test, report and README updates, packaging audit.
- What AI did: reviewed the supplied project and corrected stale candidate metadata and backprop report values; clean-machine Windows launch and live walkthrough remain unconfirmed.
- Incorrect or unsuitable suggestion identified: none additional in this session; all changes verified by running workbench.exe and checking exit codes.
- Ownership statement: original work in this repo is the candidate's submission draft, built with AI assistance as disclosed above. The brief itself (ass.pdf) is Pass Science Works material and is not claimed as the candidate's work. The synthetic CSV is not an ANSYS export.
