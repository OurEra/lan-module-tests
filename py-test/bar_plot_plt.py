import numpy
from matplotlib import pyplot as plt


dev_x = [25, 26, 27, 28, 29,
         30, 31, 32, 33, 34, 35]
range_x = numpy.arange(len(dev_x))
dev_y = [38496, 42000, 46752, 49320, 53200,
         56000, 62316, 64928, 67317, 68748, 73752]
        
py_dev_y = [45372, 48876, 53850, 57287, 63016,
            65998, 70003, 70000, 71496, 75370, 83640]

# draw plot
# plt.plot(dev_x, dev_y, color="#e5ae38", label="dev")
# plt.plot(dev_x, py_dev_y, color="#444444", label="py dev")

# draw bar
#plt.bar(range_x, dev_y, width = 0.25, color="#e5ae38", label="dev")
#plt.bar(range_x + 0.25, py_dev_y, width = 0.25, color="#444444", label="py dev")
#plt.xticks(ticks=range_x, labels=dev_x)

# draw scatter
plt.scatter(dev_x, dev_y)

plt.legend()
plt.show()
