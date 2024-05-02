 # dispatch.py
import sys

errlog = open("./err.log", "w")
log = open("./log.log", "w")

for line in sys.stdin:
    words = line.split()
    if len(words) == 0:
        continue
    if words[0] == "ALLOC" or words[0] == "FREE" or words[0] == "CALLOC" or words[0] == "REALLOC":
        log.write(line)
    elif words[0] == "NO" or words[0] == "ERROR":
        errlog.write(line)
    else:
        print(line.strip())
