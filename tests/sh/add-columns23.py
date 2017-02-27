import sys, json, time

if __name__ == "__main__":
    t0 = time.time()
    d = json.load(open(sys.argv[1]))
    t1 = time.time()
    nlNode = d['c'][0]['c'][1];
    nitems = len(nlNode['c'])
    for k in range(nitems):
        n = nlNode['c'][k]
        if n.has_key('t'):
            num = float(n['t'][1])+float(n['t'][2])
            print num
    t2 = time.time()
    sys.stderr.write('JSON parse time: %g\n' % (t1 - t0))
    sys.stderr.write('Transform time: %g\n' % (t2 - t1))
