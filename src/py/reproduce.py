import sys, json

def dictOfLists2ListOfDicts(d):
    if type(d) != type({}):
        return d
    allKeys = list(d.keys())
    listKeys = []
    nlistKeys = []
    for k in range(len(allKeys)):
        key = allKeys[k]
        if type(d[key]) == type([]):
            if len(d[key]) == 1:
                d[key] = d[key][0]
        if type(d[key]) == type([]):
            listKeys += key
        else:
            nlistKeys += key
    res = []
    if len(listKeys) == 0:
        for k in range(len(allKeys)):
            key = allKeys[k]
            d[key] = dictOfLists2ListOfDicts(d[key])
        return d
    for k in range(len(d[listKeys[0]])):
        newObj = {}
        for lk in range(len(nlistKeys)):
            key = nlistKeys[lk]
            newObj[key] = dictOfLists2ListOfDicts(d[key])
        for lk in range(len(listKeys)):
            key = listKeys[lk]
            newObj[key] = dictOfLists2ListOfDicts(d[key][k])
        res += [newObj]
    return res

def reproduce(d):
    r = u"";
    def printTxt(d):
        r = u"";
        if 't' in d:
            r = r + d['t'];
        if 'i' in d:
            r = r + d['i'];
        return r
    if 'c' in d and len(d['c']):
        for k in range(len(d['c'])):
            if k > 0:
                r += printTxt(d)
            r += reproduce(d['c'][k]);
    if not 'c' in d or len(d['c']) <= 1:
        r += printTxt(d)
    return r

if __name__ == "__main__":
    s = sys.stdin.read()
    #d = eval(s)
    d = json.loads(s)
    d2 = dictOfLists2ListOfDicts(d)
    r = reproduce(d2)
    sys.stdout.write(r)
