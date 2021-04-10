from time import perf_counter_ns

import numpy as np
import pyopencl as cl

print(f"Using OpenCL version [{cl.VERSION_TEXT}]")

def get_context_and_queue():
    ctx = cl.create_some_context()
    queue = cl.CommandQueue(ctx)
    return (ctx, queue)


def build_program(ctx, kernel_text):
    return cl.Program(ctx, kernel_text).build()


def get_rw_buffer(ctx, size):
    return cl.Buffer(ctx, cl.mem_flags.READ_WRITE, size=size)


def get_w_buffer(ctx, size):
    return cl.Buffer(ctx, cl.mem_flags.WRITE_ONLY, size=size)


def bind_to_buffer(ctx, hostbuf):
    return cl.Buffer(ctx, cl.mem_flags.READ_ONLY | cl.mem_flags.COPY_HOST_PTR, hostbuf=hostbuf)


def execute_kernel(krnl, ctx, queue, sample, *args):
    out_array_cl = get_w_buffer(ctx, sample.nbytes)
    timer_start = perf_counter_ns()
    krnl(queue, sample.shape, None, *args, out_array_cl).wait()
    timer_stop = perf_counter_ns()
    output_array = np.empty_like(sample)
    cl.enqueue_copy(queue, output_array, out_array_cl)
    return output_array, timer_stop - timer_start
