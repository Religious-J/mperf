from matplotlib import pyplot as plt
from tqdm import tqdm

logfile = open("./log.log", "r")
dic = dict()
total = 0

xs = []
ys = []
deltas = []

def process_one_step(line):
    ret = 0
    words = line.split()
    if words[0] == "FREE":
        if words[2] in dic:
            ret = -dic.pop(words[2])

    elif words[0] == "ALLOC"  or words[0] == "CALLOC" or words[0] == "REALLOC":
        dic[words[2]] = int(words[4])
        ret = int(words[4])
    return ret

# tid = 0
# max = 0
lines = logfile.readlines()

for i in tqdm(range(0, len(lines))):
    try:
        delta = process_one_step(lines[i])
        total += delta
        if (total > max):
            max = total
            tid = i
        xs.append(i)
        ys.append(total)
        deltas.append(delta)
    except:
        continue

# print(max/1024/1024)
# print(total/1024/1024/1024)
# print(tid)

plt.plot(xs, ys)
plt.savefig('result.png')
plt.show()