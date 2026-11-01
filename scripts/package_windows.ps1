# Package the one-click Windows app. Run after build_windows.bat.
# Output: RA2311033010060_Workbench.zip in the project folder.
param([string]$CandidateId = "RA2311033010060")
$ErrorActionPreference = "Stop"
$root = Split-Path -Parent $PSScriptRoot
$stage = Join-Path $env:TEMP "workbench-stage"
if (Test-Path $stage) { Remove-Item $stage -Recurse -Force }
New-Item $stage -ItemType Directory | Out-Null
Copy-Item (Join-Path $root "Launch.exe") $stage
Copy-Item (Join-Path $root "workbench.exe") $stage
Copy-Item (Join-Path $root "frontend") (Join-Path $stage "frontend") -Recurse
Copy-Item (Join-Path $root "results") (Join-Path $stage "results") -Recurse
Copy-Item (Join-Path $root "src") (Join-Path $stage "src") -Recurse
Copy-Item (Join-Path $root "data") (Join-Path $stage "data") -Recurse
Copy-Item (Join-Path $root "tests") (Join-Path $stage "tests") -Recurse
Copy-Item (Join-Path $root "docs") (Join-Path $stage "docs") -Recurse
Copy-Item (Join-Path $root "scripts") (Join-Path $stage "scripts") -Recurse
Copy-Item (Join-Path $root "README.md") $stage
Copy-Item (Join-Path $root "CMakeLists.txt") $stage
Copy-Item (Join-Path $root "build_windows.bat") $stage
Copy-Item (Join-Path $root "toolchain-mingw.cmake") $stage
# Git bundle (readable commit log, required by submission spec)
$bundle = Join-Path $root "workbench.bundle"
if (Test-Path $bundle) { Copy-Item $bundle $stage }
# VS Code one-action run config
$vscode = Join-Path $root ".vscode"
if (Test-Path $vscode) { Copy-Item $vscode (Join-Path $stage ".vscode") -Recurse }
$zip = Join-Path $root "$CandidateId`_Workbench.zip"
if (Test-Path $zip) { Remove-Item $zip -Force }
Compress-Archive -Path "$stage\*" -DestinationPath $zip
Write-Host "Wrote $zip"
Write-Host "Before submitting: unzip it in a clean folder, double-click Launch.exe, and record the launch-to-ready time."
