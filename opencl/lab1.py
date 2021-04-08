import cl_wrap as clw

import numpy as np
import pyopencl as cl
import pyopencl.array as cl_array

from icecream import ic


i = np.array([2**n for n in range(2,20)])
x = np.array(range(1,10), dtype=np.int32)
y = np.array(range(1,10), dtype=np.int32)
z = np.array(range(1,10), dtype=np.int32)
Y = np.random.randint(100, size=(5,5))
a = 10
b = 10

ctx, queue = clw.get_context_and_queue()
prog = clw.build_program(ctx, """
                        __kernel void mul_by_coeff(__global int *input_array,
                                                   int coeff,
                                                   __global int *output_array)
                        {
                          int gid = get_global_id(0);
                          output_array[gid] = input_array[gid] * coeff;
                        }

                        __kernel void mul_arrays(__global int *input_array1,
                                                 __global int *input_array2,
                                                 __global int *output_array)
                        {
                          int gid = get_global_id(0);
                          output_array[gid] = input_array1[gid] * input_array2[gid];
                        }

                        __kernel void sum_arrays(__global int *input_array1,
                                                 __global int *input_array2,
                                                 __global int *output_array)
                        {
                          int gid = get_global_id(0);
                          output_array[gid] = input_array1[gid] + input_array2[gid];
                        }
                        """)

mul_by_coeff = prog.mul_by_coeff
mul_arrays = prog.mul_arrays
sum_arrays = prog.sum_arrays

i_cl = clw.bind_to_buffer(ctx, ic(i))
x_cl = clw.bind_to_buffer(ctx, ic(x))
y_cl = clw.bind_to_buffer(ctx, ic(y))
z_cl = clw.bind_to_buffer(ctx, ic(z))
Y_cl = clw.bind_to_buffer(ctx, ic(Y))

def calculate_first_task(print_intermediate=False):
    if not print_intermediate:
        ic = lambda e: e
    # y * z
    output_array, time1 = clw.execute_kernel(mul_arrays, ctx, queue, y, y_cl, z_cl)
    y_mul_z_cl = clw.bind_to_buffer(ctx, ic(output_array))

    # b * y * z
    output_array, time2 = clw.execute_kernel(mul_by_coeff, ctx, queue, y, y_mul_z_cl, np.int32(b))
    b_mul_y_mul_z_cl = clw.bind_to_buffer(ctx, ic(output_array))

    # a * x
    output_array, time3 = clw.execute_kernel(mul_by_coeff, ctx, queue, x, x_cl, np.int32(a))
    a_mul_x_cl = clw.bind_to_buffer(ctx, ic(output_array))

    # w = a * x + b * y * z
    output_array, time4 = clw.execute_kernel(sum_arrays, ctx, queue, x, a_mul_x_cl, b_mul_y_mul_z_cl)
    w_cl = clw.bind_to_buffer(ctx, ic(output_array))

    return time1 + time2 + time3 + time4

for count in i:
    print(f"[{count}] Затраченное время в секундах",
          sum([calculate_first_task() for i in range(count)]) // 10**9)
