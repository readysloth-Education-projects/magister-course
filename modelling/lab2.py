import itertools as it

import seaborn as sns
import matplotlib.pyplot as plt


def calculate_period_and_show(seq):
    period = len(list(it.takewhile(lambda e: e != seq[0],
                                   seq[1:]))) + 1
    print(seq[:20])
    print('Период генератора равен ', period)
    sns.set_style('darkgrid')
    sns.displot(seq, kde=True)
    plt.show()


def first_task():
    def create_random(a,x0,c,m):
        assert m  >= 2
        assert a  >= 0 and a  < m
        assert c  >= 0 and c  < m
        assert x0 >= 0 and x0 < m
        x_next = x0
        def rand_generator():
            nonlocal x_next
            while True:
                x_next = (a*x_next + c) % m
                yield x_next
        return rand_generator()

    # Хорошие значения
    rand_generator = create_random(a=6364136223846793005,
                                   x0=990,
                                   c=1442695040888963407,
                                   m=2**64)
    #rand_generator = create_random(a=10,
    #                               x0=90,
    #                               c=10,
    #                               m=111)
    random_seq = list(it.islice(rand_generator, 100000))
    random_sub_seq = [e % 10 for e in random_seq]
    calculate_period_and_show(random_seq)
    calculate_period_and_show(random_sub_seq)


def second_task():
    import random
    import math
    def rand_generator():
        while True:
            fi = random.random()
            r = random.random()
            yield math.cos(2*math.pi*fi)*math.sqrt(-2*math.log(r))
            yield math.sin(2*math.pi*fi)*math.sqrt(-2*math.log(r))

    random_seq = list(it.islice(rand_generator(), 100000))
    calculate_period_and_show(random_seq)

first_task()
second_task()
