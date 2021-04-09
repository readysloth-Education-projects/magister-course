import cl_wrap as clw
import numpy as np

from PIL import Image
from icecream import ic


def make_grayscale():
    image = Image.open('image.jpg')
    data = np.asarray(image)
    rows = [r.flatten() for r in data]

    ctx, queue = clw.get_context_and_queue()
    prog = clw.build_program(ctx, """
                            __kernel void make_grayscale(__global int *rgb_row,
                                                         __global int *gray_row)
                            {
                              int gid = get_global_id(0);
                              int rgb_chunk_start = gid * 3;
                              int gray_color = (rgb_row[rgb_chunk_start] +
                                                rgb_row[rgb_chunk_start+1] +
                                                rgb_row[rgb_chunk_start+2]) / 3;
                              gray_row[rgb_chunk_start]   = gray_color;
                              gray_row[rgb_chunk_start+1] = gray_color;
                              gray_row[rgb_chunk_start+2] = gray_color;
                            }
                            """)
    m_grayscale = prog.make_grayscale
    gray_rows = []
    time = 0
    for row in rows:
        row_cl = clw.bind_to_buffer(ctx, row)
        gray_row, time1 = clw.execute_kernel(m_grayscale, ctx, queue, row, row_cl)
        gray_rows.append(gray_row)

        time += time1

    data = np.array([np.array_split(g_row, 3) for g_row in gray_rows])

    Image.fromarray(data).save("grayscale.jpg")

make_grayscale()
