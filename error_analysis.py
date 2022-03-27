from matplotlib import markers
import matplotlib.pyplot as plt
import numpy as np

with open("./results.txt","r") as fin:
    data = fin.readlines()
    data = [[int(item) for item in items.strip().split("\t")] for items in data]
data = np.array(data)
plt.plot(data[:,0],data[:,1],c='black',label='estimate',marker='^')
plt.plot(data[:,0],data[:,2],c='red',label='truth',marker='o')
plt.xlabel('Timestamp')
plt.ylabel("Count")
plt.title("error analysis")
plt.legend()
# plt.show()
plt.savefig("error_analysis.png")