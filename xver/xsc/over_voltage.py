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

start = 14200
end = 15400
s = 1000*0.2 * 200/320

t = []
v = []
T = 0

with open("s1.csv", "r") as record:
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

    k = False
    for cv in cell_voltages:
        _, i, v1, v2, v3, v4, v5, v6, v7 = cv[4].split(" ")
        t += [ s * makeHr(cv[1]) / 1000 ]
        v += [ float(v1) / 10000 ]
        if v[-1] > 4.189:
            if k == False:
                T0 = s * makeHr(cv[1]) / 1000
                k = True
    T = s*makeHr(over_voltages[0][1]) / 1000

print(T0)
plt.plot(t, v)
plt.plot([T0 - 0.01, T0], [min(v), max(v)])
plt.plot([T - 0.01, T], [min(v), max(v)])
plt.legend(["Cell voltage", "Time of first over voltage", "Time of AMS error"])
plt.xticks(t, rotation=45)
plt.yticks([4.14, 4.15, 4.16, 4.17, 4.18, 4.19, 4.20, 4.21, 4.22, 4.23])
plt.grid('minor')
plt.xlabel("Time [s]")
plt.ylabel("Cell voltage")
plt.show()
        


