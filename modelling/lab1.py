from sympy.abc import t
from sympy.plotting import plot, plot3d
from sympy import Function, Derivative, Eq, N, dsolve

def plot_model(title, start_population, time, birth_death_rate):
    def create_coeffs(a, b):
        return (lambda t: a*t, lambda t: b*t)

    alpha, beta = create_coeffs(*birth_death_rate)

    population_func = Function('N')
    population_speed = Eq(Derivative(population_func(t), t),
                  (alpha(t)-beta(t))*population_func(t))
    solution = dsolve(population_speed).subs('C1', start_population)
    plot(solution.rhs,
         (t, *time),
         title=title,
         axis_center=(0,0))


plot_model(title='N(t)',
           start_population=5,
           time=(0,40),
           birth_death_rate=(4.1,4.15))
