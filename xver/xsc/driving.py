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

with open("s4.csv", "r") as record:
    content = record.read()
    events = []
    for line in content.split('\n')[start:end]:
        try:
            sequencenum,timestamp,context,event,detail,timestampint,contextinint,contextint,contextoutint,eventint,eventoffset,eventsize,eventdata = line.split(",")
            events.append((sequencenum,timestamp,context,event,detail,timestampint,contextinint,contextint,contextoutint,eventint,eventoffset,eventsize,eventdata))
        except:
            pass

    logs = [ event for event in events if event[3] == '"Log"' ]
    CSE = [ log for log in logs if "CSE" in log[4] ]
    CST = [ log for log in logs if "CST" in log[4] ]

    se = []
    ce = []
    te = []
    for cv in CSE:
        _, soc, cap = cv[4].split(" ")
        se += [ float(soc)/1000 ]
        ce += [ float(cap[:-1]) ]
        te += [ s * makeHr(cv[1]) / 1000 ]

    st = []
    ct = []
    tt = []
    for cv in CST:
        _, soc, cap = cv[4].split(" ")
        st += [ float(soc)/1000 ]
        ct += [ float(cap[:-1]) ]
        tt += [ s * makeHr(cv[1]) / 1000 ]


plt.plot(te, se)
plt.plot(tt, st)
print("Est, True, Diff")
print(se[-1], st[-1], (se[-1] - st[-1])/se[-1])
plt.xlabel("Time [s]")
plt.ylabel("SOC [%]")
plt.legend(["Estimated", "True"])
plt.show()

fig, ax1 = plt.subplots()
ax1.plot(te, ce)
ax1.plot(tt, ct)
ax1.set_xlabel("Time [s]")
yhys = 0.01*(ce[0] - ce[-1])
ax1.set_ylim(ymax = ce[0] + yhys, ymin = ce[-1] - yhys)
ax1.set_ylabel("Capacity [As]")

ax2 = ax1.twinx()
yhys = 0.01*(1 - ce[-1]/28350)
ax2.set_ylim(ymax = 1 + yhys, ymin = ce[-1]/28350 - yhys)
ax2.set_ylabel("SOH")

ax1.legend(["Estimated", "True"])
plt.show()


