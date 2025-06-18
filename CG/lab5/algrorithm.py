from dataclasses import dataclass
import time
from config import *

@dataclass
class Point:
    x: int
    y: int

    def __init__(self, x=0, y=0):
        self.x = x
        self.y = y

@dataclass
class Node:
    x: float
    dx: float
    dy: int

    def __init__(self, x=0, dx=0, dy=0):
        self.x = x
        self.dx = dx
        self.dy = dy


def draw_line(canvas, ps, pe, colour):
    x_beg = ps.x + 0.5
    x_end = pe.x + 0.5
    y = ps.y
    canvas.create_line(x_beg, y, x_end, y, fill=colour)


def draw_edges(canvas, edges):
    for i in range(len(edges)):
        canvas.create_line(edges[i][0].x, edges[i][0].y,
                           edges[i][1].x, edges[i][1].y, fill="black")


# figures - массив всех замкнутых фигур
def make_edges_list(figures):
    edges = list()
    for fig in figures:
        amount_point = len(fig)
        for i in range(amount_point):
            if i + 1 > amount_point - 1:
                edges.append([fig[-1], fig[0]])
            else:
                edges.append([fig[i], fig[i + 1]])

    return edges


def find_extrimum_Y_figures(figures):
    yMin = figures[0][0].y
    yMax = figures[0][0].y
    for fig in figures:
        for p in fig:
            if p.y > yMax:
                yMax = p.y
            if p.y < yMin:
                yMin = p.y
    return yMin, yMax



# CAP
# ------------------------------------------------
def make_link_list(Ymin=0, Ymax=CANVAS_WIDTH):
    link_list = dict()
    for i in range(round(Ymax), round(Ymin), -1):
        link_list.update({i: list()})
    return link_list


def update_y_group(y_groups, x_start, y_start, x_end, y_end):
        if y_start > y_end:
            x_end, x_start = x_start, x_end
            y_end, y_start = y_start, y_end

        y_proj = abs(y_end - y_start)
        if y_proj != 0:
            x_step = -(x_end - x_start) / y_proj
            if y_end not in y_groups:
                y_groups[y_end] = [Node(x_end, x_step, y_proj)]
            else:
                y_groups[y_end].append(Node(x_end, x_step, y_proj))
                # y_groups[y_end].extend([[x_end, x_step, y_proj]])


def iterator_active_edges(active_edges):
    i = 0
    while i < len(active_edges):
        active_edges[i].x += active_edges[i].dx
        active_edges[i].dy -= 1
        if active_edges[i].dy < 1:
            active_edges.pop(i) # удаляем как в стеке
        else:
            i += 1


def add_active_edges(y_groups, active_edges, y):
    if y in y_groups:
        for y_group in y_groups.get(y):
            active_edges.append(y_group)
    active_edges.sort(key=lambda edge: edge.x)


def draw_act(canvas, active_edges, y, colour):
    len_edge = len(active_edges)
    #if len_edge % 2 != 0:
    #    len_edge -= 1
    for i in range(0, len_edge, 2):
        try:
            canvas.create_line(active_edges[i].x, y, active_edges[i + 1].x, y, fill=colour)
        except:
            canvas.create_line(active_edges[i].x, y, active_edges[i - 1].x, y, fill=colour)


def CAP_algorithm_with_ordered_list_of_edges(canvas, polygon, colour="black", delay=False):
    edges = make_edges_list(polygon)
    print("Все рёбра всех фигур: ")
    for edge in edges:
        print(edge)

    ymin, ymax = find_extrimum_Y_figures(polygon)
    y_groups = make_link_list(ymin, ymax)

    print("Все рёбра всех фигур: ")
    for edge in edges:
         print(edge)
        
    for edge in edges:
        update_y_group(y_groups, edge[0].x, edge[0].y, edge[1].x, edge[1].y)

    print(y_groups)

    y_end = ymax
    y_start = ymin
    active_edges = []
    while y_end > y_start:
        iterator_active_edges(active_edges)
        add_active_edges(y_groups, active_edges, y_end)

        print("Len egde:", len(active_edges))
        e = 1
        for i in active_edges:
            print("   ", e, ")", i)
            e += 1
        draw_act(canvas, active_edges, y_end, colour)
        y_end -= 1
        if delay:
            time.sleep(0.00001)
            canvas.update()
    draw_edges(canvas, edges)