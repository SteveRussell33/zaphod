#!/usr/bin/env python3

import subprocess

files = [
    'FM',
    'knob18',
    'knob40',
    'knob50',
    'port'
]

for f in files:
    fin = 'res-src/' + f + '-src.svg'
    fout = 'res/' + f + '.svg'
    print('Processing ' + fout + '...')

    # read input
    lin = open(fin, 'r').readlines()
    
    # pre-process output
    lout = []
    for ln in lin:
        # skip the blueprint elements
        if 'class="blueprint"' not in ln:
            lout.append(ln)

    # write output
    open(fout, 'w').writelines(lout)

    # run it through inkscape
    subprocess.run([
        '/Applications/Inkscape.app/Contents/MacOS/inkscape', 
        '--batch-process', 
        "--actions=EditSelectAll;SelectionUnGroup;EditSelectAll;EditUnlinkClone;EditSelectAll;ObjectToPath;FileSave",
        fout],
        check=True)

print('Done.')
