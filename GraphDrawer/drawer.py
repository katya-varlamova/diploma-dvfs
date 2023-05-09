import matplotlib.pyplot as plt
import sys
freqs = [2701000, 2700000, 2600000, 2500000, 2300000, 2100000, 1900000, 1700000, 1600000, 1400000, 1200000, 1100000, 900000, 700000, 600000, 400000]
freqs = [int(f) / 1000 for f in freqs]
freqs.reverse()
def readLog(fn, ids):
    d = {"userspace" : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
         "schedutil" : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
         "ondemand" : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
         "conservative" : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
         "performance" : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
         "powersave" : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]}
    mrpiFreqData = [[], []]
    with open(fn) as f:
        for line in f:
            splited = line.split(" ")[4].split(",")
            if splited[0] == "FreqLog":
                if splited[1] in d and int(splited[2]) in ids:
                    d[splited[1]][freqs.index(int (int(splited[3]) / 1000) )] += 1
                    if splited[1] == "userspace":
                        mrpiFreqData[0].append(int(splited[3]))
                        mrpiFreqData[1].append(float(splited[4]))
    return (d, mrpiFreqData)
def drawHist(fn, bins, vals):
    bins = list(map(int, bins))
    bins = list(map(str, bins))
    plt.xticks(rotation = "vertical", fontsize = 7)
    plt.xlabel("частота, kHz", size = 7)
    plt.ylabel("количество включений")
    plt.bar(bins, vals)
    plt.savefig(fn)
    plt.clf()
def drawMrpiFreqGraph(fn, mrpiFreqData):
    plt.xlabel("время, мс")
    t = [i * 20 for i in range(len(mrpiFreqData[0]))]
    fr = [i / 2701000 for i in mrpiFreqData[0]]
    max_mrpi = max(mrpiFreqData[1])
    mrpi = [i / max_mrpi for i in mrpiFreqData[1]]
    plt.ylabel("нормализованное значение")
    plt.plot(t, fr, label = "частота")
    plt.plot(t, mrpi, label = "LLCMPI")
    plt.legend()
    plt.savefig(fn)
    plt.clf()
ids = []
for i in range(1, len(sys.argv)):
    ids.append(int(sys.argv[i]))
data, mrpiFreqData = readLog("/home/kate/Desktop/diploma/repo/logfile.log", ids)
for k in data:
    drawHist("/home/kate/Desktop/diploma/repo/GraphDrawer/img/" + k + ".png", freqs, data[k])
if len(mrpiFreqData[0]) > 0:
    drawMrpiFreqGraph("/home/kate/Desktop/diploma/repo/GraphDrawer/img/freq_mrpi.png", mrpiFreqData)
 
                    
