import sys
from mpi4py import MPI


def create_integrals(_from, to, h, func):

    def middle_rectangle():

        def chunk1(i):
            return func(_from + (i - 1/2) * h)

        def chunk2(i):
            return 0

        def reduce(chunk_sum):
            return h*chunk_sum

        return (chunk1, chunk2, reduce)


    def trapezium():
        first_part = (func(_from)+func(to))/2

        def chunk1(i):
            return func(a+i*h)

        def chunk2(i):
            return 0

        def reduce(chunk_sum):
            return h*(first_part + chunk_sum)

        return (chunk1, chunk2, reduce)


    def simpson():
        first_part = (func(_from)+func(to))/2

        def chunk1(i):
            return 2*func(_from + (i - 1/2) * h)

        def chunk2(i):
            return func(a+i*h)

        def reduce(chunk_sum):
            return h/3 * (first_part + chunk_sum)

        return (chunk1, chunk2, reduce)


    return (middle_rectangle, trapezium, simpson)


REDUCER_RANK = int(sys.argv[1]) - 1

a = 0
b = 1
n = REDUCER_RANK
h = (b - a) / n
rect, trap, simps = create_integrals(a, b, h, lambda x: 1/(1+x*x))

FUNC_MAPS = {
    rect : rect(),
    trap : trap(),
    simps :simps(),
}

KEY = trap

chunk1_func = FUNC_MAPS[KEY][0]
chunk2_func = FUNC_MAPS[KEY][1]
reduce_func = FUNC_MAPS[KEY][2]

COMM = MPI.COMM_WORLD
RANK = COMM.Get_rank()

if RANK < n:
    chunk1_res = chunk1_func(RANK)
    chunk2_res = chunk2_func(RANK)
    COMM.send(chunk1_res, dest=REDUCER_RANK)
    COMM.send(chunk2_res, dest=REDUCER_RANK)

if RANK == REDUCER_RANK:
    chunk_sum = 0
    for i in range(2*REDUCER_RANK):
        recieved = COMM.recv(source=MPI.ANY_SOURCE, tag=MPI.ANY_TAG)
        chunk_sum += recieved
    print(4*reduce_func(chunk_sum))
