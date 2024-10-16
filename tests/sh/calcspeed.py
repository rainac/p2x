import sys
inf = sys.argv[1]
outf = sys.argv[2]

with open(inf) as f:
    data = [ [ float(w) for w in v.split(' ') ] for v in f.readlines() if v ]

print(data)

Bps = [ round(v[0] / v[1]) for v in data ]

print(Bps)

print(f"{inf} speeds: min = {min(Bps)}, max = {max(Bps)}, mean = {sum(Bps)/len(data)} B/s")

with open(outf, "w") as f:
    for i in range(len(data)):
        f.write( ' '.join([repr(v) for v in data[i] + [Bps[i]]]) + '\n')
