#!/usr/bin/env python3

import subprocess

files = [
    'FM' 
]

for f in files:
    fin = 'res-src/' + f + '-src.svg'
    fout = 'res/' + f + '.svg'
    print('Creating ' + fout + '...')

    # read input
    lin = open(fin, 'r').readlines()
    
    # pre-process output
    lout = []
    for ln in lin:
        lout.append(ln)
        ## nuke the scaffolding
        #if 'class="scaffold' not in ln:
        #    #if 'coords' in ln:
        #    #    print(ln.rstrip())
        #    lout.append(ln)

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
