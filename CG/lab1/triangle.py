from math import sqrt, acos, pi, atan2, degrees
from class_point import *

EPS = 1e-6


# вершина проведения с
def length_bisector(side_a, side_b, side_c):
    return sqrt(side_a * side_b * (side_a + side_b + side_c) * (side_a + side_b - side_c)) / (side_a + side_b)


def length_side_triangle(pa, pb):
    return sqrt((pb.x - pa.x) * (pb.x - pa.x) + (pb.y - pa.y) * (pb.y - pa.y))


def displacement_factor(side_a, side_b):
    if abs(side_a) < EPS or abs(side_b) < EPS:
        return 0.0
    if side_a >= side_b:
        k = side_a / side_b
    else:
        k = side_b / side_a

    return k


def is_triangle(side_a, side_b, side_c):
    if abs(side_a + side_b - side_c) < EPS or abs(side_b + side_c - side_a) < EPS or abs(side_a + side_c - side_b) < EPS:
        return False
    else:
        return True


def find_coordinate(x1, x2, k):
    return (x1 + k * x2) / (1 + k)


def find_middle_coordinate(x1, x2):
    return (x1 + x2) / 2


def find_orthocenter(pa, pb, pc):
    # Проверка на вырожденный треугольник
    def area(a, b, c):
        return abs((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) / 2

    if area(pa, pb, pc) < EPS:
        return None

    def line_equation(p1, p2):
        if abs(p2.x - p1.x) < EPS:
            return None, p1.x  # Вертикальная линия x = p1.x
        m = (p2.y - p1.y) / (p2.x - p1.x)
        b = p1.y - m * p1.x
        return m, b

    # Уравнение стороны bc
    m_bc, b_bc = line_equation(pb, pc)

    # Высота из pa (перпендикулярна bc)
    if m_bc is None:  # bc вертикальна => высота горизонтальна
        h1_m = 0
        h1_b = pa.y
    else:
        h1_m = -1 / m_bc if abs(m_bc) > EPS else None
        h1_b = pa.y - h1_m * pa.x if h1_m is not None else pa.x

    # Уравнение стороны ac
    m_ac, b_ac = line_equation(pa, pc)

    # Высота из pb (перпендикулярна ac)
    if m_ac is None:  # ac вертикальна => высота горизонтальна
        h2_m = 0
        h2_b = pb.y
    else:
        h2_m = -1 / m_ac if abs(m_ac) > EPS else None
        h2_b = pb.y - h2_m * pb.x if h2_m is not None else pb.x

    # Находим пересечение высот
    if h1_m is None:  # Высота 1 вертикальна
        x = h1_b
        y = h2_m * x + h2_b if h2_m is not None else h2_b
    elif h2_m is None:  # Высота 2 вертикальна
        x = h2_b
        y = h1_m * x + h1_b
    else:
        if abs(h1_m - h2_m) < EPS:
            return None  # Параллельные высоты
        x = (h2_b - h1_b) / (h1_m - h2_m)
        y = h1_m * x + h1_b

    return Point(x, y)


def calculate_angle(ortho):
    if abs(ortho.x) < EPS:
        return 90.0
    angle_rad = atan2(ortho.y, ortho.x)
    angle_deg = degrees(angle_rad)
    angle_to_y = 90.0 - angle_deg if angle_deg >= 0 else (90.0 + 360.0) - angle_deg
    return angle_to_y % 360
