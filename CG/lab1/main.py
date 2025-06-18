"""
На плоскости дано множество точек.
Найти такой треугольник с вершинами в этих точках,
у которого угол, образованный прямой, соединяющей точку пересечения высот и начало координат, и осью ординат максимален
"""
from tkinter import Tk, Canvas, Label, Entry, Button, messagebox, LAST, Listbox
from triangle import *

window = Tk()
WINDOW_WIDTH = window.winfo_screenwidth() - 50
WINDOW_HEIGHT = window.winfo_screenheight() - 120
AXIS_SPACE = 10

CANVAS_X = WINDOW_WIDTH
CANVAS_Y = WINDOW_HEIGHT - 120

numb_points = 0

point_list = []

coor_orthocenter = Point(0, 0)
max_angle = -1.0


def translate_to_normal_system(x, y):
    return transform_origin_x(x), transform_origin_y(y)


def transform_origin_x(x):
    return x * (x_max - x_min) / CANVAS_X + x_min


def transform_origin_y(y):
    return -(y * (y_max - y_min) / CANVAS_Y - y_max)


def translate_to_own_system(x, y):
    return transform_x(x), transform_y(y)


def transform_x(x):
    return (x - x_min) / (x_max - x_min) * CANVAS_X


def transform_y(y):
    return (y_max - y) / (y_max - y_min) * CANVAS_Y


def point_not_enter():
    messagebox.showwarning("Предупреждение!",
                           "Координаты точки не введены!")


def number_not_enter():
    messagebox.showwarning("Предупреждение!",
                           "Введите номер удаляемой точки!")


def point_not_exist():
    messagebox.showwarning("Предупреждение!",
                           "Под такой нумерации точки нет!")


def point_exist():
    messagebox.showwarning("Предупреждение!",
                           "Такая точка уже введена!")


def task():
    messagebox.showinfo("Условие задачи",
                        " На плоскости дано множество точек.\n"
                        " Найти такой треугольник с вершинами в этих точках,\n"
                        " у которого угол, образованный прямой, соединяющей точку пересечения высот"
                        " и начало координат, и осью ординат максимален\n"
                        " Автор: Куликов Н.В. ИУ7-43Б")


def clear_canvas_field():
    global numb_points
    global flag_find_triangle

    canvas.delete("all")
    clear_fields(del_point_txt)
    clear_fields(x_point_txt)
    clear_fields(y_point_txt)
    draw_axis()
    point_list.clear()
    numb_points = 0
    scroll_menu.delete(0, scroll_menu.size() - 1)
    flag_find_triangle = False


def clear_fields(field):
    string = field.get()
    len_str = len(string)

    while len_str >= 1:
        field.delete(len_str - 1)
        len_str -= 1


def draw_axis():
    for i in range(0, CANVAS_Y, 50):
        canvas.create_line(7, CANVAS_Y - i - AXIS_SPACE, 13, CANVAS_Y - i - AXIS_SPACE, width=2)
        if i != 0:
            canvas.create_text(25, CANVAS_Y - i - AXIS_SPACE, text=str(round(transform_origin_y(CANVAS_Y - i - AXIS_SPACE), 2)))

    for i in range(0, CANVAS_X, 50):
        canvas.create_line(i + AXIS_SPACE, CANVAS_Y - 13, i + AXIS_SPACE, CANVAS_Y - 7, width=2)
        if i != 0:
            canvas.create_text(i + AXIS_SPACE, CANVAS_Y - AXIS_SPACE - 10, text=round(transform_origin_x(i + AXIS_SPACE), 2))

    canvas.create_line(0, CANVAS_Y - AXIS_SPACE, CANVAS_X, CANVAS_Y - AXIS_SPACE, width=2, arrow=LAST)
    canvas.create_line(AXIS_SPACE, CANVAS_Y, AXIS_SPACE, 0, width=2, arrow=LAST)


def add_point():
    global numb_points

    x_text = x_point_txt.get()
    y_text = y_point_txt.get()

    if x_text == "" or y_text == "":
        point_not_enter()
    else:
        try:
            x, y = float(x_text), float(y_text)
            flag = False
            for p in point_list:
                flag = p.equalpoint(x, y)
                if flag:
                    break

            if point_list == [] or (not flag):
                point_list.append(Point(x, y))

                scroll_menu.insert(numb_points, str(numb_points + 1) + ".(" + str(round(x, 2)) + ";" + str(round(y, 2)) + ")")

                numb_points += 1

                #draw_point(x, y, numb_points, color_points_add)

                clear_fields(del_point_txt)

                del_point_txt.insert(0, numb_points)
            else:
                point_exist()
        except:
            messagebox.showwarning("Предупреждение!",
                                   "Введены недопустимые символы")
        clear_fields(x_point_txt)
        clear_fields(y_point_txt)


def del_point():
    global numb_points, flag_find_triangle

    del_text = del_point_txt.get()
    if del_text == '':
        number_not_enter()
    else:
        index_del = int(del_text) - 1
        if index_del >= len(point_list) or index_del < 0:
            point_not_exist()
        elif numb_points != 0:
            canvas.delete("all")
            draw_axis()

            point_list.pop(index_del)

            # for i in range(0, len(point_list)):
            #     draw_point(point_list[i].x, point_list[i].y, i + 1, color_points_add)

            scroll_menu.delete(index_del)
            clear_fields(del_point_txt)
            numb_points -= 1
            if numb_points != 0:
                del_point_txt.insert(0, numb_points)

            if index_del == numb_points:
                scroll_menu.delete(index_del)
            else:
                scroll_menu.delete(index_del, scroll_menu.size() - 1)
                for i in range(index_del, len(point_list)):
                    x, y = point_list[i].x, point_list[i].y
                    scroll_menu.insert(i, str(i + 1) + ".(" + str(round(x, 3)) + ";" + str(round(y, 3)) + ")")

            flag_find_triangle = False


def draw_point(x, y, name, color):
    xp, yp = transform_x(x), transform_y(y)
    coor = str(name) + ".(" + str(round(x, 2)) + ";" + str(round(y, 2)) + ")"
    canvas.create_oval(xp - 2, yp - 2, xp + 2, yp + 2, fill=color, outline=color, width=2)
    canvas.create_text(xp + 5, yp - 10, text=coor, fill=color, font=("Courier New", 10))


def draw_line(x1, y1, x2, y2, color):
    x1, y1 = translate_to_own_system(x1, y1)
    x2, y2 = translate_to_own_system(x2, y2)
    canvas.create_line(x1, y1, x2, y2, width=2, fill=color)


def draw_triangle(pa, pb, pc):
    draw_line(pa.x, pa.y, pb.x, pb.y, colour_triangle)
    draw_line(pc.x, pc.y, pb.x, pb.y, colour_triangle)
    draw_line(pc.x, pc.y, pa.x, pa.y, colour_triangle)
    # draw_point(pa.x, point_list[i].y, i + 1, color_points_add)



def check_one_line():
    amount = len(point_list)
    for i in range(amount - 2):
        for j in range(i + 1, amount - 1):
            for k in range(j + 1, amount):
                if abs((point_list[k].x - point_list[i].x) * (point_list[j].y - point_list[i].y) - \
                       (point_list[j].x - point_list[i].x) * (point_list[k].y - point_list[i].y)) > EPS:
                    return CORRECT
    return MISTAKE


def find_triangle():
    global flag_find_triangle, coor_triangle, max_angle, coor_orthocenter
    global x_min, x_max, y_min, y_max, x_c, y_c

    amount = len(point_list)
    if not point_list or numb_points < 3:
        messagebox.showwarning('Ошибка', 'Треугольник невозможно построить\nВведите три точки')
        return
    if check_one_line() == MISTAKE:
        messagebox.showwarning("Ошибка", "Все точки лежат на одной прямой\n(нельзя построить треугольник)")
        return

    if flag_find_triangle:
        canvas.delete("all")
        draw_axis()
        if point_list:
            for i in range(amount):
                draw_point(point_list[i].x, point_list[i].y, i + 1, color_points_add)
        flag_find_triangle = False

    max_angle = -1.0
    best_triangle = []
    best_ortho = Point(0, 0)

    for i in range(amount - 2):
        for j in range(i + 1, amount - 1):
            for k in range(j + 1, amount):
                pa = point_list[i]
                pb = point_list[j]
                pc = point_list[k]

                # Проверка на вырожденность
                if abs((pc.x - pa.x) * (pb.y - pa.y) - (pb.x - pa.x) * (pc.y - pa.y)) < EPS:
                    continue

                # Находим ортоцентр
                ortho = find_orthocenter(pa, pb, pc)
                if ortho is None:
                    continue

                # Вычисляем угол с осью Y
                angle = calculate_angle(ortho)

                if angle > max_angle:
                    max_angle = angle
                    best_triangle = [pa, pb, pc]
                    best_ortho = ortho

    if max_angle > -EPS:
        coor_triangle = best_triangle
        coor_orthocenter = best_ortho

        # Масштабируем
        all_points = coor_triangle + [coor_orthocenter]
        x_max = max(p.x for p in all_points)
        x_min = min(p.x for p in all_points)
        y_max = max(p.y for p in all_points)
        y_min = min(p.y for p in all_points)

        # Добавляем отступы
        x_padding = (x_max - x_min) * 0.1
        y_padding = (y_max - y_min) * 0.1
        x_max += x_padding
        x_min -= x_padding
        y_max += y_padding
        y_min -= y_padding

        canvas.delete("all")
        draw_axis()
        for i in range(amount):
            if point_list[i] in coor_triangle:
                draw_point(point_list[i].x, point_list[i].y, i + 1, color_points_add)

        draw_result()
        flag_find_triangle = True
    else:
        messagebox.showwarning("Ошибка", "Не найден подходящий треугольник")


def draw_result():
    draw_triangle(coor_triangle[0], coor_triangle[1], coor_triangle[2])
    # Рисуем ортоцентр и линию к (0,0)
    draw_point(coor_orthocenter.x, coor_orthocenter.y, "O", "red")
    draw_line(coor_orthocenter.x, coor_orthocenter.y, 0, 0, "blue")


def get_result():
    if flag_find_triangle:
        messagebox.showinfo("Результат!",
            f"Треугольник с вершинами:\n"
            f"1.({coor_triangle[0].x:.2f}, {coor_triangle[0].y:.2f})\n"
            f"2.({coor_triangle[1].x:.2f}, {coor_triangle[1].y:.2f})\n"
            f"3.({coor_triangle[2].x:.2f}, {coor_triangle[2].y:.2f})\n"
            f"Угол с осью Y: {max_angle:.2f}°")
    else:
        messagebox.showinfo("Ошибка", "Треугольник не найден")


if __name__ == "__main__":
    CORRECT = 1
    MISTAKE = 0
    colour_triangle = "green"
    color_points_add = "purple"
    kx_space = 1.01
    ky_space = 1.02

    x_max = 20
    x_min = 0
    y_max = 10
    y_min = 0

    x_c = (x_min + x_max) / 2
    x_min = x_c - (x_c - x_min) * kx_space
    x_max = x_c + (x_max - x_c) * kx_space

    y_c = (y_min + y_max) / 2
    y_min = y_c - (y_c - y_min) * ky_space
    y_max = y_c + (y_max - y_c) * ky_space

    coor_triangle = [Point(0, 0), Point(0, 0), Point(0, 0)]
    coor_start = Point(0, 0)
    flag_find_triangle = False

    window.title("Лабораторная работа №1")
    window.geometry("%dx%d" % (WINDOW_WIDTH, WINDOW_HEIGHT))
    window.resizable(False, False)

    canvas = Canvas(window, width=CANVAS_X, height=CANVAS_Y, bg="white")
    canvas.place(x=0, y=120)

    draw_axis()

    Label(window, text="X: ", font=("Courier New", 18)). \
        place(width=60, height=40, x=10, y=15)

    x_point_txt = Entry(window, font=("Courier New", 15))
    x_point_txt.place(width=100, height=40, x=50, y=15)

    Label(window, text="Y: ", font=("Courier New", 18)). \
        place(width=60, height=40, x=160, y=15)

    y_point_txt = Entry(window, font=("Courier New", 15))
    y_point_txt.place(width=100, height=40, x=210, y=15)

    Button(text="Добавить точку", font=("Courier New", 12), command=add_point). \
        place(width=180, height=40, x=50, y=60)

    Label(window, text="№:", font=("Courier New", 15)). \
        place(height=50, x=320, y=15)

    del_point_txt = Entry(window, font=("Courier New", 15))
    del_point_txt.place(width=100, height=40, x=350, y=15)

    Button(text="Удалить точку", font=("Courier New", 12), command=del_point). \
        place(width=180, height=40, x=270, y=60)

    Button(text="Условие задачи", font=("Courier New", 12), command=task). \
        place(width=240, height=40, x=480, y=15)

    Button(text="Очистить все поля", font=("Courier New", 12), command=clear_canvas_field). \
        place(width=240, height=40, x=740, y=15)

    Button(text="Построить треуг-к", font=("Courier New", 12), command=find_triangle). \
        place(width=240, height=40, x=480, y=60)

    Button(text="Вывести результаты", font=("Courier New", 11), command=get_result). \
        place(width=240, height=40, x=740, y=60)

    Label(window, text="Cписок всех точек:", font=("Courier New", 12)). \
        place(width=240, height=50, x=1000, y=35)
    scroll_menu = Listbox()
    scroll_menu.place(width=180, height=100, x=1240, y=15)

    window.mainloop()
