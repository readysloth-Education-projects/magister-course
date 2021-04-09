import cl_wrap as clw
import numpy as np

from PIL import Image
from icecream import ic


def make_grayscale():
    image = Image.open('image.jpg')
    data = np.asarray(image)

    ctx, queue = clw.get_context_and_queue()
    prog = clw.build_program(ctx, """
                            #define max_of_three(m,n,p) ( (m) > (n) ? ((m) > (p) ? (m) : (p)) : ((n) > (p) ? (n) : (p)))
                            #define min_of_three(m,n,p) ( (m) < (n) ? ((m) < (p) ? (m) : (p)) : ((n) < (p) ? (n) : (p)))

                            __kernel void make_grayscale(__global char *rgb_row,
                                                         __global char *gray_row)
                            {
                              int gid = get_global_id(0);
                              int rgb_chunk_start = gid * 3;
                              char max_light = max_of_three(rgb_row[rgb_chunk_start],
                                                           rgb_row[rgb_chunk_start+1],
                                                           rgb_row[rgb_chunk_start+2]);
                              char min_light = min_of_three(rgb_row[rgb_chunk_start],
                                                           rgb_row[rgb_chunk_start+1],
                                                           rgb_row[rgb_chunk_start+2]);
                              char gray_color = (min_light + max_light) / 2;
                              gray_row[rgb_chunk_start]   = gray_color;
                              gray_row[rgb_chunk_start+1] = gray_color;
                              gray_row[rgb_chunk_start+2] = gray_color;
                            }
                            """)

    make_grayscale = prog.make_grayscale

    def process_rows(func, rows):

        flatten_rows = rows.flatten()
        row_cl = clw.bind_to_buffer(ctx, flatten_rows)
        gray_row, time = clw.execute_kernel(func, ctx, queue, flatten_rows, row_cl)

        return np.array(np.array_split(gray_row, len(gray_row) // 3)), time;

    time = 0
    columns = []
    for d in data:
        grayed, time1 = process_rows(make_grayscale, d)
        time += time1
        columns.append(grayed)

    new_data = np.array(columns)
    Image.fromarray(new_data).save("grayscale.jpg")

    return time

make_grayscale()
