import regex
from statistics import *
from math import *
from matplotlib import pyplot as plt
# assume rate monotonic

units = {"u" : 10e-7, "m" : 10e-4, " " : 10e-0}

def makeHr(x):
    u = list(map(float, x.replace(" ", "").split(":")))
    if len(u) == 4:
        return u[3] + 60*(u[2] + 60*(u[1] + 24*(u[0])))
    elif len(u) == 3:
        return u[2] + 60*(u[1] + 60*(u[0]))
    elif len(u) == 2:
        return u[1] + 60*(u[0])
    elif len(u) == 1:
        return u[0]
    else:
        return 0

start = 1000
end = -1
s = 1000 * 0.2 * 200/320

t = []
v = []
T = 0

with open("s3.csv", "r") as record:
    content = record.read()
    events = []
    for line in content.split('\n')[start:end]:
        try:
            sequencenum,timestamp,context,event,detail,timestampint,contextinint,contextint,contextoutint,eventint,eventoffset,eventsize,eventdata = line.split(",")
            events.append((sequencenum,timestamp,context,event,detail,timestampint,contextinint,contextint,contextoutint,eventint,eventoffset,eventsize,eventdata))
        except:
            pass

    logs = [ event for event in events if event[3] == '"Log"' ]
    cell_voltages = [ log for log in logs if "cell_voltages" in log[4] ]
    over_voltages = [ log for log in logs if "error 22" in log[4] ]

    k = 0
    h = 0
    t = []
    v = []
    _v = [ 0 for i in range(126)]
    __tr = lambda x: float(x) / 10000
    for cv in cell_voltages:
        _, i, v1, v2, v3, v4, v5, v6, v7 = cv[4].split(" ")
        i = int(i)
        _v[i+0] = __tr(v1)
        _v[i+1] = __tr(v2)
        _v[i+2] = __tr(v3)
        _v[i+3] = __tr(v4)
        _v[i+4] = __tr(v5)
        _v[i+5] = __tr(v6)
        _v[i+6] = __tr(v7[:-1])
        if i == 0:
            h = h + 1
            if h < 100:
                continue
            __v = 0
            for j in range(126):
                __v += _v[j]
            v.append(__v)
            t += [ s * makeHr(cv[1]) / 1000 ]

plt.plot(t, v)
plt.xlabel("Time [s]")
plt.ylabel("Accumulator voltage")
plt.show()
        


