import regex
from statistics import *
from math import *
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

offset = 2500

with open("a5.csv", "r") as record:
    content = record.read()
    events = []
    for line in content.split('\n')[offset:]:
        try:
            sequencenum,timestamp,context,event,detail,timestampint,contextinint,contextint,contextoutint,eventint,eventoffset,eventsize,eventdata = line.split(",")
            events.append((sequencenum,timestamp,context,event,detail,timestampint,contextinint,contextint,contextoutint,eventint,eventoffset,eventsize,eventdata))
        except:
            pass


    s = 1000

    __tasks = list(set((task_name, task_priority) for task_name, task_priority in regex.findall('"Task Info","(\w*) \(\w*\): Priority = (\w*)"', content)))

    _tasks = []
    for task_name, task_priority in __tasks:
        task = f'"{task_name}"'
        task_records = [event for event in events if event[2] == task]

        runs = [record for record in task_records if record[3] == '"Task Run"'] # Task run
        computation_times = []
        trigger_times = []
        period_times = []
        for run in runs:
            computation_times += [float(x)*units[y] for x, y in regex.findall('"Runs for (.*) (u|m| )s"', run[4])]
            trigger_times += [makeHr(run[1])]

        
        for index in range(len(trigger_times) - 1):
            period_times += [ trigger_times[index+1] - trigger_times[index] ]

        p = int(task_priority)
        T = s * mean(period_times)
        C = s * max(computation_times)
        U = C/T

        _tasks += [(task_name, p, T, C, U)]

    U_sum = 0
    tasks = []

    for L, p, T, C, U in sorted(_tasks, key=lambda x: -int(x[1])):
        hp = [t for t in _tasks if t[1] > p]
        R = C
        nR = 10e6
        while R != nR:
            nR = R
            R = C
            for _, _, hpT, hpC, _ in hp:
                R += ceil(nR/hpT)*hpC

        U_sum += U

        tasks += [(L, p, T, C, U, U_sum, R)]

    for L, p, T, C, U, U_sum, R in tasks:
        print(f'{L} & {p} & {T:.1f} & {C:.1f} & {U:.1f} & {U_sum:.1f} & {R:.1f} \\\\'.replace('_','\\_'))

    print("---"*10)


            
