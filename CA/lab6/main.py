import numpy as np
import matplotlib.pyplot as plt

x = [1, 2, 3, 4, 5, 6]
y = [0.571, 0.889, 1.091, 1.231, 1.333, 1.412]
table = [['-'] * 7 for i in range(len(x))]
for i in range(len(table)):
    table[i][0] = x[i]
    table[i][1] = y[i]

for i in range(len(table) - 1):
    table[i][2] = (table[i + 1][1] - table[i][1]) / (table[i + 1][0] - table[i][0])

for i in range(1, len(table) - 1):
    table[i][3] = (table[i + 1][1] - table[i - 1][1]) / (table[i + 1][0] - table[i - 1][0])

for i in range(len(table) - 2):
    table[i][4] = 2 * table[i][2] - (table[i + 2][1] - table[i][1]) / (table[i + 2][0] - table[i][0])

for i in range(len(table) - 1):
    eta = 1 / table[i][1]
    xi = 1 / table[i][0]
    next_eta = 1 / table[i + 1][1]
    next_xi = 1 / table[i + 1][0]
    table[i][5] = (next_eta - eta) / (next_xi - xi) * (table[i][1] / table[i][0]) ** 2

h = table[1][0] - table[0][0]
for i in range(1, len(table) - 1):
    table[i][6] = (table[i - 1][1] - 2 * table[i][1] + table[i + 1][1]) / h ** 2

headers = ["x", "y", "1", "2", "3", "4", "5"]
print("".join(f"{h:<12}" for h in headers))
for row in table:
    for item in row:
        if item != "-":
            print(f"{item:<12.6f}", end="")
        else:
            print(f"{'-':<12}", end="")
    print()


def solve_boundary_value_problem(alpha, beta, gamma, N=100):

    h = 1.0 / (N - 1)
    x = np.linspace(0, 1, N)

    A = np.zeros((N, N))
    b = np.zeros(N)

    for i in range(1, N - 1):
        xi = x[i]

        A[i, i - 1] = 1 / h ** 2
        A[i, i] = -2 / h ** 2 - 2 * xi ** 2 * (-1 / (2 * h)) + 4
        A[i, i + 1] = 1 / h ** 2 + 2 * xi ** 2 * (1 / (2 * h))

        b[i] = 2 * xi + np.exp(-xi)

    A[0, 0] = -3 / (2 * h)
    A[0, 1] = 4 / (2 * h)
    A[0, 2] = -1 / (2 * h)
    b[0] = alpha

    A[-1, -3] = 1 / (2 * h)
    A[-1, -2] = -4 / (2 * h)
    A[-1, -1] = 3 / (2 * h) - beta
    b[-1] = gamma

    u = np.linalg.solve(A, b)

    return x, u


alpha = 1.0
beta = 0.5
gamma = 0.2
x, u = solve_boundary_value_problem(alpha, beta, gamma)

plt.plot(x, u, label='Приближенное решение')
plt.xlabel('x')
plt.ylabel('u(x)')
plt.title('Решение краевой задачи')
plt.legend()
plt.grid()
plt.show()