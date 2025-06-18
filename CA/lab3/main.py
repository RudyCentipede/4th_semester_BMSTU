import copy
from data_funcs import *
from least_squares_1D import least_squares_method_1D, grafic_aprox_function_1D
from least_squares_2D import least_squares_method_2D, grafic_aprox_function_2D
from ODE import ODE
from matplotlib import pyplot as plt


def main_menu():
    current_data_1D = None
    current_data_2D = None
    data_loaded = False

    while True:
        print("\nГлавное меню:")
        print("1. Загрузить данные из файла")
        print("2. Одномерная аппроксимация")
        print("3. Двумерная аппроксимация")
        print("4. Решение дифференциального уравнения")
        print("5. Выход")

        choice = input("Выберите пункт: ")

        if choice == "1":
            file_path = input("Введите путь к файлу данных: ")
            try:
                # Пытаемся определить тип данных по содержимому файла
                with open(file_path) as f:
                    first_line = f.readline().strip()
                    cols = len(first_line.split())

                if cols == 3:
                    current_data_1D = generate_from_file_1D(file_path)
                    current_data_2D = None
                    print("Загружены одномерные данные")
                elif cols == 4:
                    current_data_2D = generate_from_file_2D(file_path)
                    current_data_1D = None
                    print("Загружены двумерные данные")
                data_loaded = True
            except Exception as e:
                print(f"Ошибка загрузки файла: {e}")

        elif choice == "2" and data_loaded and current_data_1D:
            n = int(input("Введите степень полинома: "))
            print_mode = input("Показать матрицу СЛАУ? (y/n): ").lower() == "y"

            # Создаем копию с весами 1
            data_w1 = copy.deepcopy(current_data_1D)
            set_all_weight(data_w1)

            # Аппроксимация с исходными весами
            grafic_aprox_function_1D(
                least_squares_method_1D(current_data_1D, n, print_mode),
                current_data_1D,
                color='blue',
                label=f"n={n} (custom weights)"
            )

            # Аппроксимация с весами=1
            grafic_aprox_function_1D(
                least_squares_method_1D(data_w1, n),
                data_w1,
                color='red',
                label=f"n={n} (weights=1)",
                is_empty=False
            )

            plt.show()

        elif choice == "3" and data_loaded and current_data_2D:
            n = int(input("Введите степень полинома: "))
            print_mode = input("Показать матрицу СЛАУ? (y/n): ").lower() == "y"

            # Построение графика
            axes = grafic_aprox_function_2D(
                least_squares_method_2D(current_data_2D, n, print_mode),
                current_data_2D,
                color='blue'
            )
            plt.show()

        elif choice == "4":
            x, y, y_2, y_3 = ODE()

            plt.figure("Решение ОДУ")
            plt.plot(x, y_2, color="blue", label="m=2")
            plt.plot(x, y_3, color="red", label="m=3")
            plt.plot(x, y, color="green", label="Точное решение")
            plt.legend()
            plt.grid(True)
            plt.show()

        elif choice == "5":
            break

        else:
            print("Некорректный ввод или данные не загружены")


if __name__ == "__main__":
    main_menu()