import cl_wrap as clw
import numpy as np

from PIL import Image
from icecream import ic

def process_image(cl_text, function_name, output_img_name):
    image = Image.open('image.jpg')
    data = np.asarray(image)
    ctx, queue = clw.get_context_and_queue()
    prog = clw.build_program(ctx, cl_text)

    cl_func = getattr(prog, function_name)

    def process_rows(func, rows):

        flatten_rows = rows.flatten()
        row_cl = clw.bind_to_buffer(ctx, flatten_rows)
        p_row, time = clw.execute_kernel(func, ctx, queue, flatten_rows, row_cl)

        return np.array(np.array_split(p_row, len(p_row) // 3)), time;

    time = 0
    columns = []
    for d in data:
        processed, time1 = process_rows(cl_func, d)
        time += time1
        columns.append(processed)

    new_data = np.array(columns)
    Image.fromarray(new_data).save(output_img_name)

    return time

def make_grayscale():
    time = process_image( """
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
    }""", "make_grayscale", "grayscale.jpg")

    return time

def make_negative():
    time = process_image( """
    __kernel void make_negative(__global char *rgb_row,
                                __global char *negative_row)
    {
      int gid = get_global_id(0);
      int rgb_chunk_start = gid * 3;
      int max_intensity = 255 - 1;
      negative_row[rgb_chunk_start]   = max_intensity - rgb_row[rgb_chunk_start];
      negative_row[rgb_chunk_start+1] = max_intensity - rgb_row[rgb_chunk_start+1];
      negative_row[rgb_chunk_start+2] = max_intensity - rgb_row[rgb_chunk_start+2];
    }""", "make_negative", "negative.jpg")

    return time


ic(make_grayscale())
ic(make_negative())
