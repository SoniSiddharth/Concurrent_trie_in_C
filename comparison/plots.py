import numpy as np
import matplotlib.pyplot as plt

slock_data = np.genfromtxt("s_lock.csv", delimiter=",", names=["no", "time"])
rwlock_data = np.genfromtxt("rw_lock.csv", delimiter=",", names=["no", "time"])
hohlock_data = np.genfromtxt("hoh_lock.csv", delimiter=",", names=["no", "time"])
plt.plot(slock_data["no"], slock_data["time"], label="s_lock")
plt.plot(rwlock_data["no"], rwlock_data["time"], label="rw_lock")
plt.plot(hohlock_data["no"], hohlock_data["time"], label="hoh_lock")
plt.xlabel("Number of threads")
plt.ylabel('Time')
plt.legend()
plt.show()