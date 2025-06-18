import numpy as np
from math import exp, sqrt, pi
import matplotlib.pyplot as plt
from half_division import HalfDivision
from newton_system import NewtonSystem
from sympson import SympsonIntegral


def main():
    while True:
        print("\nМеню:")
        print("1. Решить систему уравнений")
        print("2. Найти аргумент функции Лапласа")
        print("3. Решить краевую задачу и построить график")
        print("4. Выход")

        choice = input("Выберите пункт меню: ")

        if choice == "1":
            solve_system_equations()
        elif choice == "2":
            find_laplace_argument()
        elif choice == "3":
            solve_boundary_problem()
        elif choice == "4":
            break
        else:
            print("Неверный ввод. Попробуйте снова.")


def solve_system_equations():
    def f1(x, y, z):
        return x ** 2 + y ** 2 + z ** 2 - 1

    def f2(x, y, z):
        return 2 * x ** 2 + y ** 2 - 4 * z

    def f3(x, y, z):
        return 3 * x ** 2 - 4 * y + z ** 2

    def jacobian(x, y, z):
        return [
            [2 * x, 2 * y, 2 * z],
            [4 * x, 2 * y, -4],
            [6 * x, -4, 2 * z],
        ]

    print("\nРешение системы уравнений:")
    print("x² + y² + z² - 1 = 0")
    print("2x² + y² - 4z = 0")
    print("3x² - 4y + z² = 0")

    try:
        x0 = float(input("Введите начальное приближение x0: "))
        y0 = float(input("Введите начальное приближение y0: "))
        z0 = float(input("Введите начальное приближение z0: "))

        res, iters = NewtonSystem(jacobian, [f1, f2, f3], [x0, y0, z0], iter_limit=30)
        xres, yres, zres = res

        print("\nРезультат:")
        print(f"x = {xres:.6f}")
        print(f"y = {yres:.6f}")
        print(f"z = {zres:.6f}")
        print(f"Количество итераций: {iters}")

        print("\nПроверка:")
        print(f"f1(x, y, z) = {f1(xres, yres, zres):.6f}")
        print(f"f2(x, y, z) = {f2(xres, yres, zres):.6f}")
        print(f"f3(x, y, z) = {f3(xres, yres, zres):.6f}")

    except ValueError:
        print("Ошибка ввода. Введите числовые значения.")


def find_laplace_argument():
    def laplace_function(x, integralCount=10):
        def under_integral(t):
            return exp(-(t ** 2) / 2)

        linspace_to_x = list(np.linspace(0, x, integralCount))
        return 2 / sqrt(2 * pi) * SympsonIntegral(linspace_to_x, under_integral)

    print("\nНахождение аргумента функции Лапласа:")
    try:
        target = float(input("Введите значение функции Лапласа: "))
        start = float(input("Введите начальную границу поиска: "))
        end = float(input("Введите конечную границу поиска: "))

        x_found, iters = HalfDivision(laplace_function, target, start, end, iter_limit=30)

        print("\nРезультат:")
        print(f"x = {x_found:.6f}")
        print(f"Количество итераций: {iters}")
        print(f"Проверка: Ф({x_found:.6f}) = {laplace_function(x_found):.6f}")

    except ValueError:
        print("Ошибка ввода. Введите числовые значения.")


def solve_boundary_problem():
    print("\nРешение краевой задачи:")
    print("y'' - y³ = x², 0 ≤ x ≤ 1")
    print("y(0) = 1, y(1) = 3")

    try:
        N = int(input("Введите количество узлов сетки (N): "))
        if N < 2:
            print("N должно быть не менее 2.")
            return

        x0, y0 = 0, 1
        x1, y1 = 1, 3
        step = (x1 - x0) / N
        x = np.linspace(x0, x1, N + 1)

        def jacobian(*y):
            n = len(y)
            res = []
            res.append([1] + [0] * (n - 1))

            for i in range(1, n - 1):
                res.append(
                    [0] * (i - 1)
                    + [1 / step ** 2]
                    + [-2 / step ** 2 - 3 * y[i] ** 2]
                    + [1 / step ** 2]
                    + [0] * (n - i - 2)
                )

            res.append([0] * (n - 1) + [1])
            return res

        def f(n):                                 #Разностное уравнение
            if n == 0:
                return lambda *y: y[0] - y0
            elif n == N:
                return lambda *y: y[n] - y1
            else:
                return lambda *y: (y[n - 1] - 2 * y[n] + y[n + 1]) / step ** 2 - y[n] ** 3 - x[n] ** 2

        funcs = [f(n) for n in range(N + 1)]
        initial_guess = [2 * xp + 1 for xp in x]  # Линейное начальное приближение

        res, iters = NewtonSystem(jacobian, funcs, initial_guess, iter_limit=30, eps=1e-10)

        print(f"\nРешение найдено за {iters} итераций.")

        plt.plot(x, res, label='y(x)')
        plt.xlabel('x')
        plt.ylabel('y')
        plt.title('Решение краевой задачи')
        plt.grid(True)
        plt.legend()
        plt.show()

    except ValueError:
        print("Ошибка ввода. Введите целое число для N.")


if __name__ == "__main__":
    main()