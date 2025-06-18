import copy

EPS = 1e-8

x_column = [[0.0], [1.0], [2.0], [3.0], [4.0]]
y_column = [[0.0], [1.0], [2.0], [3.0], [4.0]]
z_column = [[0.0], [1.0], [2.0], [3.0], [4.0]]

def forward_way(x: list, y: list, n: int):
    ksi_list = [0 for i in range(n + 2)]
    n_list = [0 for i in range(n + 2)]

    for i in range(2, n + 1):
        if i == 2:
            n_list[i] = 0
            ksi_list[i] = 0
        else:
            h_i_bef = x[i - 1] - x[i - 2]
            h_i = x[i] - x[i - 1]
            f_i = 3 * ((y[i] - y[i - 1]) / h_i - (y[i - 1] - y[i - 2]) / h_i_bef)

            part_1 = f_i - h_i_bef * n_list[i]
            part_2 = h_i_bef * ksi_list[i] + 2 * (h_i_bef + h_i)
            
            n_list[i + 1] = part_1 / part_2

            part_1 = -h_i
            part_2 = h_i_bef * ksi_list[i] + 2 * (h_i_bef + h_i)

            ksi_list[i + 1] = part_1 / part_2
    
    return ksi_list, n_list

def reverse_way(x: list, y: list, ksi_list: list, n_list: list, n: int):
    c_list = [0 for i in range(n + 2)]

    for i in range(n, 1, -1):
        c_list[i] = ksi_list[i + 1] * c_list[i + 1] + n_list[i + 1]
    
    return c_list

def c_list(x: list, y: list, n: int):
    c_list_tmp = [0 for i in range(n + 2)]

    ksi_list, n_list = forward_way(x, y, n)

    c_list_tmp = reverse_way(x, y, ksi_list, n_list, n)

    return c_list_tmp

def d_list(x: list, y: list, c: list, n: int):
    d_list_tmp = [0 for i in range(n + 2)]

    for i in range(1, n + 1):
        h_i = x[i] - x[i - 1]

        d_list_tmp[i] = (c[i + 1] - c[i]) / (3 * h_i)
    
    return d_list_tmp

def b_list(x: list, y: list, c: list, n: int):
    b_list_tmp = [0 for i in range(n + 2)]

    for i in range(1, n + 1):
        h_i = x[i] - x[i - 1]

        part_1 = (y[i] - y[i - 1]) / h_i
        part_2 = h_i * (c[i + 1] - 2 * c[i]) / 3
        
        b_list_tmp[i] = part_1 - part_2
    
    return b_list_tmp

def a_list(y: list, n: int):
    a_list_tmp = [0 for i in range(n + 1)]

    for i in range(1, n + 1):
        a_list_tmp[i] = y[i - 1]
    
    return a_list_tmp

def end_index(x_list: list, x: float):
    i = 1
    while i < len(x_list) and x - x_list[i] > -EPS:
        i += 1

    if i == len(x_list):
        i -= 1

    return i

def polynome_x_multi(x: float, xi: float, n: int):
    return (x - xi) ** n

def polynome_value(a, b, c, d, x, xi):
    result = a
    result += b * polynome_x_multi(x, xi, 1)
    result += c * polynome_x_multi(x, xi, 2)
    result += d * polynome_x_multi(x, xi, 3)

    return result

def spline_interpolation(data: list, x: float, n: int):
    x_list = [stroke[0] for stroke in data]
    y_list = [stroke[1] for stroke in data]
    
    c = c_list(x_list, y_list, len(x_list) - 1)
    d = d_list(x_list, y_list, c, len(x_list) - 1)
    a = a_list(y_list, len(x_list) - 1)
    b = b_list(x_list, y_list, c, len(x_list) - 1)

    i = end_index(x_list, x)

    result = polynome_value(a[i], b[i], c[i], d[i], x, x_list[i - 1])

    return result

def y_u_configure(field_data: list, j: int):
    y_u = copy.deepcopy(y_column)

    for i in range(len(field_data)):
        y_u[i].append(field_data[i][j])

    return y_u

def spline_field_interpolation(field_data: list, x: float, y: float, nx, ny):
    x_u = copy.deepcopy(x_column)

    for j in range(len(field_data[0])):
        y_u = y_u_configure(field_data, j)
        u = spline_interpolation(y_u, y, ny)

        x_u[j].append(u)

    u = spline_interpolation(x_u, x, nx)

    return u

def spline_cube_interpolation(cube_data: list, x: float, y: float, z: float, nx, ny, nz):
    z_u = copy.deepcopy(z_column)

    for i in range(len(z_u)):
        u = spline_field_interpolation(cube_data[i], x, y, nx, ny)

        z_u[i].append(u)

    result = spline_interpolation(z_u, z, nz)

    return result

