from matplotlib import pyplot as plt

slices = [100, 20, 40, 200]
lables = ["A area", "B area", "C area", "D area"]
explode = [0.1, 0, 0, 0]

plt.pie(slices, labels=lables, explode=explode, shadow=True)
plt.title("PIE PLT")
plt.show()