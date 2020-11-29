import numpy as np
import matplotlib.pyplot as plt

data = np.genfromtxt("plots.csv", delimiter=",", names=["x", "LRU", "approx", "random", "fifo"])
plt.plot(data['x'], data['LRU'], label="LRU")
plt.plot(data['x'], data['approx'], label="LRU approx")
plt.plot(data['x'], data['random'], label="Random")
plt.plot(data['x'], data['fifo'], label="FIFO")
plt.xlabel('Cache Size')
plt.ylabel('Hit Rate %')
plt.legend()
plt.show()