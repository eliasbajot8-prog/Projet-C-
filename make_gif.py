import numpy as np
import matplotlib.pyplot as plt
import imageio

data = np.loadtxt("data/P_xt.csv")

x = data[:,0]
t = data[:,1]
p = data[:,2]

unique_times = np.unique(t)

frames = []

for ti in unique_times[::80]:  # 1 image sur 80 pour alléger le GIF
    mask = t == ti

    plt.figure()
    plt.plot(x[mask], p[mask])
    plt.xlabel("x (m)")
    plt.ylabel("P(x,t)")
    plt.ylim(p.min(), p.max())
    plt.title(f"t = {ti:.3f} s")
    plt.grid()

    plt.savefig("tmp.png")
    plt.close()

    frames.append(imageio.imread("tmp.png"))

imageio.mimsave("P_wave.gif", frames, duration=0.05)