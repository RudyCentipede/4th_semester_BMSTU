from integration import *
from interpolation import *


class App:
    def print_menu(self):
        print("\n1. Вычисление интеграла")
        print("2. Вычисление двукратного интеграла")
        print("0. Выход")

    def gauss_adaptive(self, func, a, b, node_count, eps):
        res = gauss_integrate(func, a, b, node_count)
        mid = (b + a) / 2
        leftRes = gauss_integrate(func, a, mid, node_count)
        rightRes = gauss_integrate(func, mid, b, node_count)
        if abs((leftRes + rightRes) - res) > eps:
            return self.gauss_adaptive(func, a, mid, node_count, eps) + self.gauss_adaptive(func, mid, b, node_count, eps)
        return leftRes + rightRes
        
    def simpson_adaptive(self, func, a, b, node_count, eps):
        res = simpson_integrate(func, a, b, node_count)
        mid = (b + a) / 2
        leftRes = simpson_integrate(func, a, mid, node_count)
        rightRes = simpson_integrate(func, mid, b, node_count)
        if abs((leftRes + rightRes) - res) > eps:
            return self.simpson_adaptive(func, a, mid, node_count, eps) + self.simpson_adaptive(func, mid, b, node_count, eps)
        return leftRes + rightRes

    def double_integral_calc(self):
        data = parse_file("data/task2.txt")
        print("Введите данные об ограничении области интегрирования")
        factor_alpha = float(input("Коэффициент первой функции: "))
        func1 = lambda x: factor_alpha * x ** 2
        factor_beta = float(input("Коэффициент второй функции: "))
        func2 = lambda x: factor_beta * x ** 2
        a = float(input("Первая ограничивающая прямая x = "))
        b = float(input("Вторая ограничивающая прямая x = "))

        eps = 1e-2

        def inner_gauss_integral(x, node_count):
            y1 = func1(x)
            y2 = func2(x)
            
            res = self.gauss_adaptive(lambda y: interpolate_newton_poly(data, Point(x, y), 2), y1, y2, node_count, eps)
            return res

        def inner_simpson_integral(x, node_count):
            y1 = func1(x)
            y2 = func2(x)
            
            res = self.simpson_adaptive(lambda y: interpolate_newton_poly(data, Point(x, y), 2), y1, y2, node_count, eps)
            return res
    
        node_count = 5
        
        print("Метод Гаусса -> Метод Гаусса:", 
              self.gauss_adaptive(lambda x: inner_gauss_integral(x, node_count), a, b, node_count, eps))
        print("Метод Симпсона -> Метод Симпсона:", 
              self.simpson_adaptive(lambda x: inner_simpson_integral(x, node_count), a, b, node_count, eps))
        print("Метод Симпсона -> Метод Гаусса:", 
              self.gauss_adaptive(lambda x: inner_simpson_integral(x, node_count), a, b, node_count, eps))
        print("Метод Гаусса -> Метод Симпсона:",
              self.simpson_adaptive(lambda x: inner_gauss_integral(x, node_count), a, b, node_count, eps))

    def integral_calc(self):
        print("1. 1")
        print("2. 2")
        flag = False
        func: Callable
        val: float
        while not flag:
            k = int(input("Выберите показатель степени функции: "))
            if k == 1 or k == 2:
                func = lambda x: abs(x) ** k
                if k == 1:
                    val = 1
                else:
                    val = 2 / 3
                flag = True
            else:
                print("Некорректная опция")
        
        methods = [trapezoid_integrate, simpson_integrate, gauss_integrate]
        labels = ["Метод трапеций", "Метод Симпсона", "Метод Гаусса"]

        a = -1
        b = 1
        node_count = 3
        for i in range(len(methods)):
            res = methods[i](func, a, b, node_count)
            print(f"{labels[i]}: {res} (Абсолютная погрешность = {val - res})")

    def menu_choice(self, choice: int):
        is_running = True
        match (choice):
            case 1:
                self.integral_calc()
            case 2:
                self.double_integral_calc()
            case 0:
                is_running = False
            case _:
                print("Некорректный ввод")
        return is_running

    def run(self):
        is_running = True
        while is_running:
            self.print_menu()
            try:
                menu_choice = int(input("Выберите опцию: "))
                is_running = self.menu_choice(menu_choice)
            except ValueError:
                print("Некорректный ввод")


app = App()
app.run()
