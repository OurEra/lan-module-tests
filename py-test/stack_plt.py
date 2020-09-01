from matplotlib import pyplot as plt


x_times = [1, 2, 3, 4, 5, 6, 7, 8, 9]

player1 = [1, 2, 3, 3, 1, 2, 1, 1, 2]
player2 = [1, 1, 2, 2, 2, 4, 4, 4, 4]
player3 = [1, 1, 1, 1, 2, 2, 3, 3, 4]
lables = ['player1', 'player2', 'player3']

plt.style.use("fivethirtyeight")
plt.stackplot(x_times, player1, player2, player3, labels=lables)
plt.title("STACK PLT")
plt.legend()
plt.tight_layout()
plt.show()