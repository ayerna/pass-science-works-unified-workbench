#!/bin/bash
# Positive, negative and numerical tests. Run from the project root.
set -e
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release > /dev/null
cmake --build build -j4
WB=./build/workbench
echo "--- positive parser tests ---"
$WB validate-stl data/example.stl
$WB validate-csv data/ansys_temperature_synthetic.csv
echo "--- negative parser tests (must fail) ---"
printf 'solid x\n facet normal 0 0 1\n  outer loop\n   vertex 0 0 0\n  endloop\n endfacet\n' > /tmp/bad.stl
if $WB validate-stl /tmp/bad.stl 2>/dev/null; then echo "FAIL: truncated STL accepted"; exit 1; fi
printf 'node_id,x,y,z,temp\n1,0,0,0,100\n' > /tmp/bad.csv
if $WB validate-csv /tmp/bad.csv 2>/dev/null; then echo "FAIL: wrong CSV header accepted"; exit 1; fi
printf 'node_id,x_m,y_m,z_m,temperature_C\n1,0,0,0,nan\n' > /tmp/bad2.csv
if $WB validate-csv /tmp/bad2.csv 2>/dev/null; then echo "FAIL: non-finite CSV accepted"; exit 1; fi
: > /tmp/empty.stl
if $WB validate-stl /tmp/empty.stl 2>/dev/null; then echo "FAIL: empty STL accepted"; exit 1; fi
echo "--- numerical tests ---"
$WB all
python3 - << 'PY'
import csv
rows=list(csv.DictReader(open('results/fea_convergence.csv')))
errs=[float(r['max_midpoint_error_C']) for r in rows]
print('convergence errors:', errs)
assert len(errs)==4 and all(errs[i+1] < errs[i] for i in range(3)), 'FEA error must decrease with N'
assert errs[-1] < 1.0, 'N=80 midpoint error should be well under 1 C'
perf=list(csv.DictReader(open('results/performance.csv')))
print('performance rows:', perf)
assert len(perf)>=2
loss=list(csv.DictReader(open('results/backprop_loss.csv')))
print('backprop steps:', len(loss), 'final loss:', loss[-1]['loss'])
assert len(loss)==101 and float(loss[-1]['loss']) < float(loss[0]['loss'])
print('ALL TESTS PASSED')
PY
