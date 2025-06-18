class InterpolationError(Exception):
    def __init__(self, info):
        super().__init__(info)


class Point:
    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y


def select_nodes(data, order, x):
    res = []
    res_len = order + 1
    left_bound = 0
    right_bound = left_bound + res_len - 1
    pivot_offset = (right_bound - left_bound) // 2 + 1
    while data[left_bound + pivot_offset][0] < x and right_bound < len(data) - 1:
        right_bound += 1
        left_bound += 1
    for i in range(left_bound, right_bound + 1):
        res.append(data[i])
    return res


def approximate(table, y0, x):
    res = y0
    for i in range(1, len(table)):
        tmp = table[0][i]
        for j in range(i):
            tmp *= x - table[j][0]
        res += tmp
    
    return res


def div_difference_table(nodes):
    table = [[0] * (len(nodes) - i) for i in range(len(nodes))]

    for i in range(len(nodes)):
        table[i][0] = nodes[i][0]

    for i in range(1, len(nodes)):  # Столбцы
        for j in range(len(nodes) - i):  # Строки
            if i == 1:
                div_diff = (nodes[j + 1][1] - nodes[j][1]) / (table[j + 1][0] - table[j][0])
            else:
                div_diff = (table[j + 1][i - 1] - table[j][i - 1]) / (table[j + i][0] - table[j][0])
            table[j][i] = div_diff

    return table


def newton_poly(data, order, x):
    if order > len(data) - 1:
        raise InterpolationError("Порядок полинома Ньютона не может быть больше, чем количество узлов - 1")
    if order < 0:
        raise InterpolationError("Порядок полинома Ньютона не может быть отрицательным")
    
    data.sort(key=lambda x: x[0])
    if x < data[0][0] or x > data[-1][0]:
        raise InterpolationError("Экстраполяция запрещена")
    
    nodes = select_nodes(data, order, x)
    table = div_difference_table(nodes)
    res = approximate(table, nodes[0][1], x)
    
    return res


def interpolate_newton_poly(data: list, point: Point, poly_n: int):
    tmp = []
    for i in range(1, len(data[0])):
        tmp_data = []
        for j in range(1, len(data)):
            tmp_data.append([data[j][0], data[j][i]])
        tmp.append([data[0][i], newton_poly(tmp_data, poly_n, point.y)])

    return newton_poly(tmp, poly_n, point.x)
