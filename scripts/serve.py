#!/usr/bin/env python3
"""Serve frontend/ on localhost:8000 and regenerate results/ if the binary exists.
Standard library only. Open http://localhost:8000/frontend/index.html in a browser."""
import http.server, os, subprocess, sys, webbrowser
ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
os.chdir(ROOT)
for cand in ("build/workbench", "build/Release/workbench.exe", "workbench.exe", "workbench"):
    if os.path.exists(cand):
        subprocess.run([cand, "all"], check=False)
        break
print("Serving workbench at http://localhost:8000/frontend/index.html")
webbrowser.open("http://localhost:8000/frontend/index.html")
http.server.test(http.server.SimpleHTTPRequestHandler, port=8000)
