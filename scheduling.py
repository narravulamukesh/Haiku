
qu = []
rl = []
flag = 0
wt = 0
tat = 0
gantt = ""

class Roundrobin:
    def __init__(self, i, at, bt):
        self.pid = i
        self.at = at
        self.bt = bt
        self.rt = bt

n = int(input("Enter the number of processes: "))
remain = n
process = []
for i in range(n):
    print("Process", i + 1)
    at = int(input("Enter arrival time: "))
    bt = int(input("Enter burst time: "))
    process.append(Roundrobin(i + 1, at, bt))
qt = int(input("Enter time slice: "))
dl = process.copy()
t = 0
cnt = 0
while remain != 0:
    if len(dl) != 0:
        for i in dl:
            if i.at <= t:
                qu.append(i)
        to_remove = []
        for z in qu:
            to_remove.append(z)
        for element in to_remove:
            if element in dl:
                dl.remove(element)
        if len(qu) != 0:
            a = qu.pop(0)
            if a.rt <= qt and a.rt > 0:
                gantt += "P" + str(a.pid) + "|"
                t += a.rt
                a.rt = 0
                remain -= 1
                wt += t - a.at - a.bt
                tat += t - a.at
            elif a.rt > 0:
                gantt += "P" + str(a.pid) + "|"
                a.rt -= qt
                t += qt
                for j in dl:
                    if j.at <= t:
                        qu.append(j)
                to_remove = []
                for y in qu:
                    to_remove.append(y)
                for element in to_remove:
                    if element in dl:
                        dl.remove(element)
                qu.append(a)
        else:
            t += 1
print("-" * len(gantt))
print(gantt)
print("-" * len(gantt))
print("Average Waiting Time = ", wt * 1.0 / n)
print("Avg Turnaround Time = ", tat * 1.0 / n)

                

