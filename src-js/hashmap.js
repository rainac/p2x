var P2X = P2X || {}

P2X.Error = function(msg) { return {msg: msg} }

P2X.binary_search_iterative = function(a, value) {
  var mid, lo = 0,
      hi = a.length - 1;

  while (lo <= hi) {
    mid = Math.floor((lo + hi) / 2);

    if (a[mid] > value) {
      hi = mid - 1;
    } else if (a[mid] < value) {
      lo = mid + 1;
    } else {
      return mid;
    }
  }
  return null;
}

P2X.binary_search_iterative_lb = function(a, value) {
  var mid, lo = 0,
      hi = a.length - 1;

  while (lo <= hi) {
    mid = Math.floor((lo + hi) / 2);

    if (a[mid] > value) {
      hi = mid - 1;
    } else if (a[mid] < value) {
      lo = mid + 1;
    } else {
      return mid;
    }
  }
  return lo;
}

P2X.HashMap = function(maxind) {
    if (maxind == undefined) maxind = 26*26
    var obj = {}
    obj.alpha = 'abcdefghijklmnopqrstuvwxyz'
    obj.themap = Array()
    obj.maxInd = maxind
    obj.mapKey = function(key) {
        if (key >= this.maxInd) {
            console.error ('Index too large: ' + key + ' (' + this.maxInd + ')')
            console.error (xxx.gg)
        }
        if (key == undefined) {
            console.error ('Index undefined: ' + key + ' (' + this.maxInd + ')')
            console.error (xxx.gg)
        }
        if (typeof key != 'number') {
            console.error ('Index must be number: ' + key + ' (' + this.maxInd + ')')
            console.error (xxx.gg)
        }
        if (key != Math.floor(key)) {
            console.error ('Index must be integer: ' + key + ' (' + this.maxInd + ')')
            console.error (xxx.gg)
        }
        return key
    }
    obj.insert = function(key, obj) {
        var mk = this.mapKey(key)
        this.themap[mk] = {key: key, value: obj}
    }
    obj.delete = function(key) {
        this.themap.erase(this.mapKey(key))
    }
    obj.get = function(key) {
        return this.themap[this.mapKey(key)].value
    }
    obj.first = function(it) {
        var keys = this.keys()
        return this.themap[keys[it]].key
    }
    obj.second = function(it) {
        var keys = this.keys()
        return this.themap[keys[it]].value
    }
    obj.find = function(key) {
        var keys = this.keys()
        var res = P2X.binary_search_iterative(keys, this.mapKey(key))
        if (res == null) {
            res = this.end()
        }
        return res
    }
    obj.lower_bound = function(key) {
        var keys = this.keys()
        return P2X.binary_search_iterative_lb(keys, this.mapKey(key))
    }
    obj.erase = function(from, to) {
        var k, ki, keys = Array()
        for (k = from; k < to; ++k) {
            keys[k-from] = this.mapKey(this.first(k))
        }
        keys.map(function(k) {
            delete this.themap[k]
        }, this)
    }
    obj.begin = function() {
        return 0
    }
    obj.end = function() {
        return this.keys().length
    }
    obj.length = function() {
        return this.keys().length
    }
    obj.keys = function() {
        var cmp = function(a,b){return Number(a) > Number(b)}
        return Object.keys(this.themap).sort(cmp)
    }
    obj.clone = function() {
        var res = P2X.HashMap()
        res.themap = {}
        this.keys().map(function(k) { res.themap[k] = this.themap[k] }, this)
        return res
    }
    return obj
}

if (typeof window == 'undefined') {
    exports.HashMap = P2X.HashMap;
    exports.binary_search_iterative = P2X.binary_search_iterative;
    exports.binary_search_iterative_lb = P2X.binary_search_iterative_lb;
}
