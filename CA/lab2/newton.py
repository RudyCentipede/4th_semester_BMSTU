import copy

EPS = 1e-8

x_column = [[0], [1], [2], [3], [4]]
y_column = [[0], [1], [2], [3], [4]]
z_column = [[0], [1], [2], [3], [4]]

def find_range(data: list, x: float, n: int):
    left = 0
    right = left + n

    if (x < -EPS or x - 4.2 > EPS):
        return None

    flag = True
    while flag and right < len(data) - 1:
        if x - data[right][0] <= EPS:
            flag = False
        else:
            left += 1
            right += 1

    if right < len(data) - 1 and n > 2:
        left += 1
        right += 1
    
    return left, right

def delta_calculate(x1: float, x2: float, y1: float, y2: float):
    return (y2 - y1) / (x2 - x1)

def newton_matrix_end_get(data: list, x: float, n: int):
    matrix_end = [] * (n + 1)
    left, right = find_range(data, x, n)

    # Добавляем X
    for i in range(left, right + 1):
        matrix_end.append([data[i][0]])

    # Добавляем Y
    for i in range(left, right + 1):
        matrix_end[i - left].append(data[i][1])

    args_shift = 1
    for iter in range(2, n + 2):
        for i in range(n - iter + 2):
            x1 = matrix_end[i][0]
            x2 = matrix_end[i + args_shift][0]
            y1 = matrix_end[i][iter - 1]
            y2 = matrix_end[i + 1][iter - 1]

            matrix_end[i].append(delta_calculate(x1, x2, y1, y2))

        args_shift += 1

    return matrix_end

def x_multi_get(matrix_end: list, count: int, x: float):
    c_list = matrix_end[0]
    res = 1

    for i in range(count):
        res *= x - matrix_end[i][0]
    
    return res

def res_by_newton_matrix_end(matrix_end: list, x: float):
    c_list = matrix_end[0]
    res = 0

    for i in range(len(matrix_end)):
        x_multi = x_multi_get(matrix_end, i, x)
        res += c_list[i + 1] * x_multi

    return res

def newton_interpolation(data: list, x: float, n: int):
    matrix_end = newton_matrix_end_get(data, x, n)

    result = res_by_newton_matrix_end(matrix_end, x)

    return result

def y_u_configure(field_data: list, j: int):
    y_u = copy.deepcopy(y_column)

    for i in range(len(field_data)):
        y_u[i].append(field_data[i][j])

    return y_u

def newton_field_interpolation(field_data: list, x: float, y: float, nx, ny):
    x_u = copy.deepcopy(x_column)

    for j in range(len(field_data[0])):
        y_u = y_u_configure(field_data, j)
        u = newton_interpolation(y_u, y, ny)

        x_u[j].append(u)

    u = newton_interpolation(x_u, x, nx)

    return u

def newton_cube_interpolation(cube_data: list, x: float, y: float, z: float, nx, ny, nz):
    z_u = copy.deepcopy(z_column)

    for i in range(len(z_u)):
        u = newton_field_interpolation(cube_data[i], x, y, nx, ny)

        z_u[i].append(u)

    result = newton_interpolation(z_u, z, nz)

    return result
