import itertools as it

import seaborn as sns
import matplotlib.pyplot as plt


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

    rand_generator = create_random(a=6364136223846793005,
                                   x0=99,
                                   c=1442695040888963407,
                                   m=2**64)

    sns.set_style('darkgrid')
    sns.displot(list(it.islice(rand_generator, 100000)),
                kde=True)
    plt.show()

first_task()
