from time import perf_counter_ns

import numpy as np
import pyopencl as cl

print(f"Using OpenCL version [{cl.VERSION_TEXT}]")

ctx = None
queue = None

def get_context_and_queue():
    global ctx
    global queue
    if ctx and queue:
        return (ctx, queue)
    ctx = cl.create_some_context()
    queue = cl.CommandQueue(ctx)
    return (ctx, queue)


def build_program(ctx, kernel_text, options=None):
    return cl.Program(ctx, kernel_text).build(options=options if options else [])


def get_rw_buffer(ctx, size):
    return cl.Buffer(ctx, cl.mem_flags.READ_WRITE, size=size)


def get_w_buffer(ctx, size):
    return cl.Buffer(ctx, cl.mem_flags.WRITE_ONLY, size=size)


def bind_to_buffer(ctx, hostbuf):
    return cl.Buffer(ctx, cl.mem_flags.READ_ONLY | cl.mem_flags.COPY_HOST_PTR, hostbuf=hostbuf)


def get_rw_pipe(ctx, element_size=1, capacity=1024):
    return cl.Pipe(ctx, cl.mem_flags.READ_WRITE, np.int32(element_size), np.int32(capacity), [])


def execute_kernel(krnl, ctx, queue, sample, *args):
    out_array_cl = get_w_buffer(ctx, sample.nbytes)
    timer_start = perf_counter_ns()
    krnl(queue, sample.shape, None, *args, out_array_cl).wait()
    timer_stop = perf_counter_ns()
    output_array = np.empty_like(sample)
    cl.enqueue_copy(queue, output_array, out_array_cl)
    return output_array, timer_stop - timer_start


def execute_n_kernels(krnls, ctx, queue, sample, *args):
    out_array_cl = get_w_buffer(ctx, sample.nbytes)
    wait_events = []
    timer_start = perf_counter_ns()
    for krnl in krnls:
        krnl(queue, sample.shape, None, *args, out_array_cl).wait()
    timer_stop = perf_counter_ns()

    output_array = np.empty_like(sample)
    cl.enqueue_copy(queue, output_array, out_array_cl)
    return output_array, timer_stop - timer_start
