from matplotlib import pyplot as plt

N = []
time = []
for i, line in enumerate(open("perf_test/log.txt", "rt")):
    N.append(50 + 50 * i)
    time.append(int(line))

plt.plot(N, time, label="mysort")

N = []
time = []
for i, line in enumerate(open("perf_test/log_qsort.txt", "rt")):
    N.append(50 + 50 * i)
    time.append(int(line))

plt.plot(N, time, label="qsort")

plt.legend()
plt.show()
