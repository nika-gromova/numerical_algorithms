import matplotlib.pyplot as plt
import re
from math import fabs
##import numpy as np

def check_is_num(number):
    return (re.fullmatch(r'[+-]?(?:\d+(?:\.\d*)?|\.\d+)', number))

def input_data(name):
    rc = 0
    x = []
    y = []
    r = []
    number_of_dots = 0
    try:
        file = open(name, 'r')
    except FileNotFoundError:
        rc = -2
    else:
        number_of_dots_str =(file.readline())
        if not re.fullmatch(r'\d+\n', number_of_dots_str):
            rc = -1
        else:
            number_of_dots = int(number_of_dots_str)
            if number_of_dots <= 0:
                rc = -1
            else:
                for i in range(number_of_dots):
                    if rc == 0:
                        cur_line = file.readline()
                        parse_line = cur_line.split()
                        if len(parse_line) == 0 or not check_is_num(parse_line[0]) or not check_is_num(parse_line[1]) or not check_is_num(parse_line[2]):
                            rc = -1
                        else:
                            x.append(float(parse_line[0]))
                            y.append(float(parse_line[1]))
                            r.append(float(parse_line[2]))

        file.close()
    return rc, number_of_dots, x, y, r

def get_matrix(N, n, x, y, r0):
    mtr = [[0.0] * (n + 1) for i in range(n + 1)]
    r_vector = [0] * (n + 1)
    for i in range(n + 1):
        for j in range(n + 1):
            tmp = 0.0
            for k in range(N):
                tmp += r0[k] * (x[k] ** (i + j))
            mtr[i][j] = tmp
    for i in range(n + 1):
        tmp = 0.0
        for j in range(N):
            tmp += r0[j] * y[j] * (x[j] ** i)
        r_vector[i] = tmp
    
    return mtr, r_vector


def gauss(mtr, r_vector, n):
    res = [0 for i in range(n + 1)]
    i = 0
    j = 0
    while i < (n + 1):
        max_elem = 0.0
        max_index = 0
        for k in range(i, n + 1):
            if fabs(mtr[k][i]) >= max_elem:
                max_elem = fabs(mtr[k][i])
                max_index = k
        #if max_elem < 1e-10:
            #return None
        if max_index != i:
            for k in range(n + 1):
                mtr[i][k], mtr[max_index][k] = mtr[max_index][k], mtr[i][k]
            r_vector[i], r_vector[max_index] = r_vector[max_index], r_vector[i]
        
        tmp = mtr[i][i]
        for k in range(i + 1, n + 1):
            coeff = mtr[k][i] / tmp
            for h in range(i, n + 1):
                mtr[k][h] -= coeff * mtr[i][h]
            r_vector[k] -= coeff * r_vector[i]
        i += 1

    for i in range(n, -1, -1):
        for j in range(n, i, -1):
            r_vector[i] -= res[j] * mtr[i][j];
        res[i] = r_vector[i] / mtr[i][i];
    return res

def draw_graph(x, y, a, n, N):
    x1 = []
    f = []
    i = x[0]
    while i <= x[N - 1]:
        x1.append(i)
        tmp = 0.0
        for j in range(n + 1):
            tmp += a[j] * i ** j
        f.append(tmp)
        i += 0.05
    plt.plot(x, y, 'r.', x1, f, 'g-')
    plt.xlabel("x")
    plt.ylabel("y")
    plt.title("Наилучшее среднеквадратичное приближение")
    plt.show()


############start############
input_file = input('Введите имя файла: ')
input_data(input_file)

rc, N, x, y, r0 = input_data(input_file)

if rc == 0:
    for i in range(N):
        print(x[i], y[i], r0[i], end = '\n')
    n_str = input("Введите степень полинома: ")
    if not re.fullmatch(r'\d+', n_str):
        rc = -1
    else:
        n = int(n_str)
        if n < 0:
            rc = -1
        else:
            mtr, r_vector = get_matrix(N, n, x, y, r0)
            a = gauss(mtr, r_vector, n)
            if a is not None:
                draw_graph(x, y, a, n, N)
                print("График построен!")
            else:
                print("Определитель Грама равен нулю. Вычисления остановлены")

if rc == -1:
    print("Некорректный ввод")
if rc == -2:
    print("Ошибка при открытии файла :(")


