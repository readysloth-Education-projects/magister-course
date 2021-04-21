import cl_wrap as clw

import numpy as np

from PIL import Image

def process_image(image, cl_text, function_name, output_img_name, *args):
    image = Image.open(image)
    data = np.asarray(image)
    ctx, queue = clw.get_context_and_queue()
    prog = clw.build_program(ctx, cl_text)

    cl_func = getattr(prog, function_name)

    def process_rows(func, rows):
        flatten_rows = rows.flatten()
        flatten_rows_len = np.int32(flatten_rows.shape[0])
        row_cl = clw.bind_to_buffer(ctx, flatten_rows)
        p_row, time = clw.execute_kernel(func,
                                         ctx,
                                         queue,
                                         flatten_rows,
                                         flatten_rows_len,
                                         row_cl,
                                         *args)

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
