import os, re, subprocess, markdown

ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

with open(os.path.join(ROOT, 'docs', 'report.md'), 'r', encoding='utf-8') as f:
    md_content = f.read()

def get_svg(name):
    p = os.path.join(ROOT, 'results', name)
    with open(p, 'r', encoding='utf-8') as f:
        s = f.read()
    return re.sub(r'<\?xml[^>]*\?>', '', s)

svg1 = get_svg('fea_temperature_N80.svg')
svg2 = get_svg('fea_convergence.svg')
svg3 = get_svg('backprop_loss.svg')

fig1_2 = f'''
<div style="display:flex; gap:10px; margin:6px 0; justify-content:space-between;">
  <div style="flex:1; border:1px solid #cbd5e0; padding:4px; background:#fff; text-align:center;">
    <div style="max-height:135px; overflow:hidden;">{svg1}</div>
    <div style="font-size:7pt; color:#718096; margin-top:2px;">Figure 1: Temperature vs x (N=80 FEA vs Exact parabola)</div>
  </div>
  <div style="flex:1; border:1px solid #cbd5e0; padding:4px; background:#fff; text-align:center;">
    <div style="max-height:135px; overflow:hidden;">{svg2}</div>
    <div style="font-size:7pt; color:#718096; margin-top:2px;">Figure 2: Convergence: Max midpoint error vs N (O(h^2))</div>
  </div>
</div>
'''

fig3 = f'''
<div style="display:flex; gap:10px; margin:6px 0;">
  <div style="flex:1.2; border:1px solid #cbd5e0; padding:4px; background:#fff; text-align:center;">
    <div style="max-height:135px; overflow:hidden;">{svg3}</div>
    <div style="font-size:7pt; color:#718096; margin-top:2px;">Figure 3: Monotonic loss descent over 100 steps (log10 scale)</div>
  </div>
</div>
'''

html_body = markdown.markdown(md_content, extensions=['tables', 'fenced_code'])

if '<h3>Convergence Results (Midpoint Error)</h3>' in html_body:
    html_body = html_body.replace('<h3>Convergence Results (Midpoint Error)</h3>', '<h3>Convergence Results (Midpoint Error)</h3>' + fig1_2)

if '<h3>Parameter Update &amp; 100-Step Optimization</h3>' in html_body:
    html_body = html_body.replace('<h3>Parameter Update &amp; 100-Step Optimization</h3>', '<h3>Parameter Update &amp; 100-Step Optimization</h3>' + fig3)

# Page breaks before major sections to keep it strictly structured across pages
for sec in ['<h2>2.', '<h2>3.', '<h2>4.', '<h2>5.', '<h2>6.']:
    html_body = html_body.replace(sec, f'<div style="page-break-before: always; break-before: page;"></div>{sec}')

full_html = f'''<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>Unified Engineering Workbench — Technical Report</title>
<style>
  @page {{
    size: A4 portrait;
    margin: 14mm 16mm 14mm 16mm;
  }}
  * {{ box-sizing: border-box; -webkit-print-color-adjust: exact; print-color-adjust: exact; }}
  body {{
    font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Arial, sans-serif;
    font-size: 8.6pt;
    line-height: 1.34;
    color: #1a202c;
    margin: 0;
  }}
  h1 {{
    font-size: 13pt;
    color: #1a365d;
    margin: 0 0 2px 0;
    border-bottom: 2px solid #2b6cb0;
    padding-bottom: 2px;
  }}
  h2 {{
    font-size: 10pt;
    color: #2b6cb0;
    margin: 8px 0 2px 0;
    border-bottom: 1px solid #cbd5e0;
    padding-bottom: 2px;
    page-break-after: avoid;
    break-after: avoid;
  }}
  h3 {{
    font-size: 8.8pt;
    color: #2d3748;
    margin: 6px 0 2px 0;
    page-break-after: avoid;
    break-after: avoid;
  }}
  p {{ margin: 0 0 4px 0; text-align: justify; }}
  ul, ol {{ margin: 0 0 4px 0; padding-left: 18px; }}
  li {{ margin-bottom: 1.5px; }}
  table {{
    width: 100%;
    border-collapse: collapse;
    margin: 4px 0 5px 0;
    font-size: 7.6pt;
    page-break-inside: avoid;
    break-inside: avoid;
  }}
  th, td {{
    border: 1px solid #cbd5e0;
    padding: 2.5px 4px;
    text-align: left;
  }}
  th {{ background: #ebf8ff; color: #2b6cb0; font-weight: 700; }}
  tr:nth-child(even) {{ background: #f7fafc; }}
  code {{
    font-family: Consolas, "Courier New", monospace;
    font-size: 7.6pt;
    background: #edf2f7;
    padding: 1px 2px;
    border-radius: 2px;
    color: #805ad5;
  }}
  pre {{
    font-family: Consolas, "Courier New", monospace;
    font-size: 7pt;
    background: #2d3748;
    color: #f7fafc;
    padding: 5px;
    border-radius: 3px;
    margin: 3px 0;
    line-height: 1.25;
    overflow: hidden;
  }}
  svg {{ max-width: 100%; height: auto; }}
  hr {{ border: none; border-top: 1px solid #e2e8f0; margin: 5px 0 7px 0; }}
</style>
</head>
<body>
{html_body}
</body>
</html>'''

html_path = os.path.join(ROOT, 'docs', 'report.html')
pdf_path = os.path.join(ROOT, 'docs', 'report.pdf')

with open(html_path, 'w', encoding='utf-8') as f:
    f.write(full_html)
print(f'Wrote docs/report.html: {len(full_html)} bytes')

chrome = r'C:\Program Files\Google\Chrome\Application\chrome.exe'
html_uri = 'file:///' + os.path.abspath(html_path).replace('\\', '/')

subprocess.run([chrome, '--headless=new', '--disable-gpu', '--no-pdf-header-footer', f'--print-to-pdf={pdf_path}', html_uri], check=True)

with open(pdf_path, 'rb') as f:
    data = f.read()
pages = len(re.findall(rb'/Type\s*/Page\b', data))
print(f'Generated docs/report.pdf: {os.path.getsize(pdf_path)} bytes, {pages} pages')
if pages <= 6:
    print(f'GATE PASS: Report is {pages} pages (strictly <= 6 page limit)!')
else:
    print(f'WARNING: Report is {pages} pages (> 6 page limit)!')
