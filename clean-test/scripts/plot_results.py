#!/usr/bin/env python3
"""Generate labelled SVG plots from results/*.csv (standard library only).
Run from the project root: python3 scripts/plot_results.py"""
import csv, math, os

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

def svg_plot(path, title, xlabel, ylabel, series, width=640, height=360):
    # series: list of (label, colour, [(x, y), ...]); all axes linear except
    # backprop, which the caller pre-transforms to log10(loss).
    xs = [x for _, _, pts in series for x, _ in pts]
    ys = [y for _, _, pts in series for x, y in pts]
    xmin, xmax = min(xs), max(xs)
    ymin, ymax = min(ys), max(ys)
    if xmax == xmin: xmax = xmin + 1
    if ymax == ymin: ymax = ymin + 1
    pad = (ymax - ymin) * 0.05
    ymin -= pad; ymax += pad
    L, R, T, B = 70, 20, 40, 50
    def px(x): return L + (x - xmin) / (xmax - xmin) * (width - L - R)
    def py(y): return height - B - (y - ymin) / (ymax - ymin) * (height - T - B)
    out = [f'<svg xmlns="http://www.w3.org/2000/svg" width="{width}" height="{height}">']
    out.append(f'<rect width="100%" height="100%" fill="white"/>')
    out.append(f'<text x="{width//2}" y="22" text-anchor="middle" font-size="15" font-family="sans-serif">{title}</text>')
    out.append(f'<text x="{width//2}" y="{height-8}" text-anchor="middle" font-size="12" font-family="sans-serif">{xlabel}</text>')
    out.append(f'<text x="16" y="{height//2}" text-anchor="middle" font-size="12" font-family="sans-serif" transform="rotate(-90 16 {height//2})">{ylabel}</text>')
    out.append(f'<rect x="{L}" y="{T}" width="{width-L-R}" height="{height-T-B}" fill="none" stroke="#999"/>')
    for label, colour, pts in series:
        d = " ".join(f"{px(x):.1f},{py(y):.1f}" for x, y in pts)
        out.append(f'<polyline points="{d}" fill="none" stroke="{colour}" stroke-width="2"/>')
        out.append(f'<text x="{width-R-150}" y="{T+18+18*series.index((label, colour, pts))}" font-size="12" font-family="sans-serif" fill="{colour}">{label}</text>')
    out.append(f'<text x="{L}" y="{height-T-B+18}" font-size="11" font-family="sans-serif">{xmin:g}</text>')
    out.append(f'<text x="{width-R-30}" y="{height-T-B+18}" font-size="11" font-family="sans-serif">{xmax:g}</text>')
    out.append('</svg>')
    with open(path, 'w') as f:
        f.write("\n".join(out))
    print(f"wrote {path}")

def read_csv(name):
    with open(os.path.join(ROOT, 'results', name)) as f:
        return list(csv.DictReader(f))

# FEA temperature: N=80 nodes vs exact
rows = read_csv('fea_N80.csv')
fea = [(float(r['x_m']), float(r['temperature_C'])) for r in rows]
exact = [(float(r['x_m']), float(r['temperature_exact_C'])) for r in rows]
svg_plot(os.path.join(ROOT, 'results', 'fea_temperature_N80.svg'),
         'Thermal FEA: temperature vs x (N=80)', 'x (m)', 'Temperature (C)',
         [('FEA N=80', '#00c', fea), ('Exact', '#c00', exact)])

# Convergence: midpoint error vs N
rows = read_csv('fea_convergence.csv')
conv = [(float(r['elements']), float(r['max_midpoint_error_C'])) for r in rows]
svg_plot(os.path.join(ROOT, 'results', 'fea_convergence.svg'),
         'FEA convergence: max midpoint error vs N', 'Elements N', 'Max midpoint error (C)',
         [('Midpoint error', '#00c', conv)])

# Backprop loss, log10 scale so the fall to ~1e-20 is visible
rows = read_csv('backprop_loss.csv')
loss = [(float(r['step']), math.log10(max(float(r['loss']), 1e-22))) for r in rows]
svg_plot(os.path.join(ROOT, 'results', 'backprop_loss.svg'),
         'Backprop: log10(loss) vs step (100 steps)', 'Step', 'log10(loss)',
         [('Loss (log10)', '#00c', loss)])
