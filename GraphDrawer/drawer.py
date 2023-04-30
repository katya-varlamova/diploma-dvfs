import matplotlib.pyplot as plt
freqs = [2701000, 2700000, 2600000, 2500000, 2300000, 2100000, 1900000, 1700000, 1600000, 1400000, 1200000, 1100000, 900000, 700000, 600000, 400000]
freqs = [int(f) / 1000 for f in freqs]
freqs.reverse()
def readLog(fn):
    d = {"userspace" : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
         "schedutil" : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
         "ondemand" : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
         "conservative" : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
         "performance" : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
         "powersave" : [0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0]}
    with open(fn) as f:
        for line in f:
            splited = line.split(" ")[4].split(",")
            if splited[0] == "FreqLog":
                if splited[1] in d:
                    d[splited[1]][freqs.index(int (int(splited[2]) / 1000) )] += 1
    return d
def drawHist(fn, bins, vals):
    bins = list(map(int, bins))
    bins = list(map(str, bins))
    plt.xticks(rotation = "vertical", fontsize = 7)
    plt.xlabel("частота, kHz", size = 7)
    plt.ylabel("количество включений")
    plt.bar(bins, vals)
    plt.savefig(fn)
    plt.clf()
data = readLog("/home/kate/Desktop/diploma/repo/logfile.log")
for k in data:
    drawHist("/home/kate/Desktop/diploma/repo/img/" + k + ".png", freqs, data[k])
 
                    
