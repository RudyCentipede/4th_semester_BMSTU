from typing import Callable


def parse_file(filename: str):
    file = open(filename)

    dataMatrix = []
    for line in file:
        dataMatrix.append([float(x) for x in line.split()])

    file.close()

    return dataMatrix


def trapezoid_area(a: float, b: float, h: float) -> float:
    return (a + b) / 2 * h


def legendre_poly_factors(n: int):
    if n == 0:
        return [1]
    if n == 1:
        return [0, 1]
    
    prev_p = legendre_poly_factors(n - 2)
    cur_p = legendre_poly_factors(n - 1)
    
    prev_p = [0] * (n - 1 - len(prev_p)) + prev_p
    cur_p = [0] * (n - len(cur_p)) + cur_p

    next_p = [0] * (n + 1)
    for k in range(1, n + 1):
        next_p[k] += (2 * n - 1) * cur_p[k - 1] / n
    for k in range(n - 1):
        next_p[k] -= (n - 1) * prev_p[k] / n
    return next_p


def legendre_poly(n: int):
    factors = legendre_poly_factors(n)
    
    def poly(x: float):
        res = 0
        for i in range(len(factors)):
            res += x ** i * factors[i]
        return res

    return poly


def legendre_poly_deriv_factors(n: int):
    if n == 0:
        return [0]
    if n == 1:
        return [1]
    
    poly = legendre_poly_factors(n)
    deriv = [0] * n
    for k in range(1, len(poly)):
        deriv[k - 1] = k * poly[k]
    return deriv


def legendre_poly_derivative(n: int):
    factors = legendre_poly_deriv_factors(n)
    
    def poly_deriv(x: float):
        res = 0
        for i in range(len(factors)):
            res += x ** i * factors[i]
        return res
    return poly_deriv
    
    
def bisection(func: Callable, a: float, b: float):
    max_iter = 1000
    i = 0
    mid = (a + b) / 2
    res = func(mid)
    eps = 1e-8
    while i < max_iter and abs(res) > eps:
        if func(a) * res < 0:
            b = mid
        else:
            a = mid
        mid = (a + b) / 2
        res = func(mid)
    return mid


def legendre_roots(n: int):
    roots = []

    step = 0.01
    left_bound = -1
    right_bound = 1
    x = left_bound
    legendre = legendre_poly(n)
    y = legendre(x)

    while len(roots) < n and x < right_bound:
        x1 = x + step
        y1 = legendre(x1)

        if y * y1 < 0:
            roots.append(bisection(legendre, x, x1))
        x = x1
        y = y1
    
    return roots


def gauss_weights(n: int):
    roots = legendre_roots(n)
    legendre_deriv = legendre_poly_derivative(n)
    weights = []
    for x in roots:
        weight = 2 / ((1 - x ** 2) * legendre_deriv(x) ** 2)
        weights.append(weight)
    return weights


def trapezoid_integrate(func: Callable, a: float, b: float, node_count: int) -> float:
    res = 0
    sign = 1
    if a > b:
        a, b = b, a
        sign = -1

    n = node_count - 1
    h = (b - a) / n
    x = a

    for i in range(n):
        left = func(x)
        right = func(x + h)
        res += trapezoid_area(left, right, h)
        x += h

    return res * sign


def parabola_integrate(func: Callable, a: float, b: float) -> float:
    h = (b - a) / 2
    x0 = a
    x1 = a + h
    x2 = b

    return (h / 3) * (func(x0) + 4 * func(x1) + func(x2))


def simpson_integrate(func: Callable, a: float, b: float, node_count: int) -> float:
    n = node_count - 1
    h = (b - a) / n
    x = a

    res = 0
    for i in range(n):
        res += parabola_integrate(func, x, x + h)
        x += h

    return res


def gauss_integrate(func: Callable, a: float, b: float, node_count: int) -> float:
    nodes = legendre_roots(node_count)
    # print(nodes)
    weights = gauss_weights(node_count)

    factor1 = (b - a) / 2
    factor2 = (b + a) / 2

    scaled_nodes = [factor1 * x + factor2 for x in nodes]
    scaled_weights = [factor1 * w for w in weights]

    res = 0
    for i in range(len(scaled_nodes)):
        res += func(scaled_nodes[i]) * scaled_weights[i]

    return res
