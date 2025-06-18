from math import *
import newton
import spline

def filedata_read():
    cube_data = [[] for i in newton.z_column]

    f = open("data.txt", "r")

    for i in range(len(cube_data)):
        while (stroke := f.readline()) != "" and stroke != "\n":
            stroke = stroke.rstrip()
            stroke = stroke.split()
            stroke = list(map(float, stroke))

            cube_data[i].append(stroke)

    f.close()

    return cube_data

def matrix_output(matrix: list):
    for i in range(len(matrix)):
        print(*matrix[i])

# ------------------------------------------------------------------

def menu():
    print("MENU")
    print("1. Посчитать u(x, y, z) для заданного значения аргумента x, y, z (0.0 - 4.0) полиномом Ньютона")
    print("2. Посчитать u(x, y, z) для заданного значения аргумента x, y, z (0.0 - 4.0) сплайнами")
    print("3. Посчитать u(x, y, z) для заданного значения аргумента x, y, z (0.0 - 4.0) смешанными способами")
    print("4. Выйти из программы")

    code = int(input())

    return code

# -----------------------------------------------

def run():
    flag = True

    cube_data = filedata_read()

    while flag:
        action = menu()

        match action:
            case 1:
                x = float(input("X: "))
                y = float(input("Y: "))
                z = float(input("Z: "))
                nx = int(input("Nx: "))
                ny = int(input("Ny: "))
                nz = int(input("Nz: "))
                
                u = newton.newton_cube_interpolation(cube_data, x, y, z, nx, ny, nz)
                print(f"u({x:.6g}, {y:.6g}, {z:.6g}) = {u}")
            case 2:
                x = float(input("X: "))
                y = float(input("Y: "))
                z = float(input("Z: "))
                nx = 0
                ny = 0
                nz = 0
                
                u = spline.spline_cube_interpolation(cube_data, x, y, z, nx, ny, nz)
                print(f"u({x:.6g}, {y:.6g}, {z:.6g}) = {u}")

            case 4:
                flag = False
            case _:
                print("\nUndefined code\n")
        
    return 0
