import sys
import os
import subprocess

script = sys.argv[1]
objcopy = sys.argv[2]
lib = sys.argv[3]

base = os.path.dirname(lib)
outfile = f"{base}/debug_gdb_scripts"

with open(script, 'r') as f:
    content = f.read()


with open(outfile, 'w') as f:
    f.write("\x04")
    f.write("markov.gdb\n")
    f.write(content)
    f.write("\x00")

subprocess.run([objcopy, '--add-section', f'.debug_gdb_scripts={outfile}', lib])

