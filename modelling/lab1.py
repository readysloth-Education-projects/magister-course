import functools as ft

from sympy.abc import t
from sympy.plotting import plot, plot3d
from sympy import Function, Derivative, Eq, E, dsolve

generic_plot = ft.partial(plot, legend=True, axis_center=(0,0), ylim=(0,100))

def plot_free_growth_model(title, start_population, time, birth_death_rate):
    def create_coeffs(a, b):
        return (lambda t: a*t, lambda t: b*t)

    alpha, beta = create_coeffs(*birth_death_rate)

    population_func = Function('N')
    population_speed = Eq(Derivative(population_func(t), t),
                  (alpha(t)-beta(t))*population_func(t))
    solution = dsolve(population_speed).subs('C1', start_population)
    print(solution)
    print(solution.rhs)
    generic_plot(solution.rhs,
                 (t, *time),
                 title=title,
                 label=str(solution))


def plot_limited_growth_model(title, N0, time, r, k, b):
    population_func = Function('N')
    z = E**(r*(t-N0))
    function = k*N0*z/(k+N0*(z-1))
    generic_plot(function,
                 (t, *time),
                 title='Bernoulli',
                 label=str(function))


def plot_model(name):
    if name == 'free_growth_model':
        plot_free_growth_model(title='N(t)',
                               start_population=25,
                               time=(0,50),
                               birth_death_rate=(0.1,0.15))
    elif name == 'limited_growth_model':
        plot_limited_growth_model(title='N(t)',
                                  N0=25,
                                  time=(0,50),
                                  r=0.05,
                                  k=40,
                                  b=1)



plot_model('free_growth_model')
plot_model('limited_growth_model')

