import sys
import numpy as np

from icecream import ic
from mpi4py import MPI

def create_matrix(n):
    return np.array([list(map(lambda e: e * 100, r)) for r in np.random.rand(n, n)])


def mul_collection(coll, number):
    return [e * number for e in coll]


def divide_collection(coll, number):
    return [e // number for e in coll]


def divide_column(matrix, column_index, number):
    column = [row[column_index] for row in matrix]
    divided_column = divide_collection(column, number)
    for div, row in zip(divided_column, matrix):
        row[column_index] = div
    return matrix


def step(i, step_row, subtract_row):
    mi = subtract_row[i] / step_row[i]
    mul_step_row = mul_collection(step_row, mi)
    return (mi, [e - mul
                 for mul, e in zip(mul_step_row,
                                   subtract_row)])


COMM = MPI.COMM_WORLD
RANK = COMM.Get_rank()
MATRIX = None
ROOT_RANK = 0
N = 0
if RANK == ROOT_RANK:
    N = int(sys.argv[1])
    MATRIX = create_matrix(N)
    COMM.bcast(MATRIX, root=ROOT_RANK)
    COMM.bcast(N, root=ROOT_RANK)
    L = []
    U = []
    # нужно N-1 раз сделать шаг алгоритма
    for i in range(N-1):
        for j, row in enumerate(MATRIX):
            COMM.isend((MATRIX[i], row), dest=j, tag=i)
        for j in range(N):
            mi, new_row = COMM.irecv(source=j).wait()
            L.append(mi)
            MATRIX[j] = new_row
    print(L)
    U = MATRIX
    print(U)
else:
    # получаем матрицу
    MATRIX = COMM.bcast(MATRIX, root=ROOT_RANK)
    N = COMM.bcast(N, root=ROOT_RANK)
    for i in range(N-1):
        step_row, sub_row = COMM.irecv(source=ROOT_RANK,
                                       tag=i).wait()
        mi, new_row = step(i, step_row, sub_row)
        COMM.isend((mi, new_row), dest=ROOT_RANK)

