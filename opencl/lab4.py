import cl_wrap as clw

import numpy as np

from cl_image import process_buffer

from icecream import ic
from PIL import Image

box_blur_program = {
  "defines" : """
              #define RED 0
              #define GREEN 1
              #define BLUE 2

              #define CREATE_SUM_NAME(COLOR) CREATE_READABLE_SUM_NAME(COLOR)
              #define CREATE_READABLE_SUM_NAME(COLOR) sum_ ## COLOR

              #define GET_COLOR_INDEX(COLOR, PTR, pixel_index) PTR[pixel_index + COLOR]
              #define FOREACH_PIXEL_IN_CHUNK(PIXEL_INDEX, BODY) \
                for(int PIXEL_INDEX = chunk_start; PIXEL_INDEX < chunk_end; PIXEL_INDEX += 3){ \
                  BODY; \
                }

              #define BLUR(COLOR) \
                FOREACH_PIXEL_IN_CHUNK(pixel_index, \
                                      CREATE_SUM_NAME(COLOR) += GET_COLOR_INDEX(COLOR, rgb_row, pixel_index); \
                                      ) \
                CREATE_SUM_NAME(COLOR) /= blur_size;
           """,
  "body" : """
           int gid = get_global_id(0);

           int chunk_start = gid * 3 * blur_size;
           int chunk_end = chunk_start + 3 * blur_size;
           int CREATE_SUM_NAME(RED) = 0;
           int CREATE_SUM_NAME(GREEN) = 0;
           int CREATE_SUM_NAME(BLUE) = 0;

           if(chunk_start > rgb_row_len){
             return;
           }

           BLUR(RED)
           BLUR(GREEN)
           BLUR(BLUE)

           FOREACH_PIXEL_IN_CHUNK(pixel_index, GET_COLOR_INDEX(RED, blur_row, pixel_index)   = CREATE_SUM_NAME(RED))
           FOREACH_PIXEL_IN_CHUNK(pixel_index, GET_COLOR_INDEX(GREEN, blur_row, pixel_index) = CREATE_SUM_NAME(GREEN))
           FOREACH_PIXEL_IN_CHUNK(pixel_index, GET_COLOR_INDEX(BLUE, blur_row, pixel_index)  = CREATE_SUM_NAME(BLUE))
           """,

  "signature" : """
                __kernel void box_blur(int rgb_row_len,
                                       __global char *rgb_row,
                                       int blur_size,
                                       __global char *blur_row)
                """,
  "kernel" : "box_blur"
}


grayscale_program = {
  "defines" : """
              #define max_of_three(m,n,p) ( (m) > (n) ? ((m) > (p) ? (m) : (p)) : ((n) > (p) ? (n) : (p)))
              #define min_of_three(m,n,p) ( (m) < (n) ? ((m) < (p) ? (m) : (p)) : ((n) < (p) ? (n) : (p)))
              """,
  "body" : """
           int gid = get_global_id(0);
           int rgb_chunk_start = gid * 3;
           if(rgb_chunk_start > rgb_row_len){
              return;
           }
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
           """,
  "signature" : """
                __kernel void make_grayscale(int rgb_row_len,
                                             __global char *rgb_row,
                                             __global char *gray_row)
                """,
  "kernel" : "make_grayscale"
}


def process_image_via_buffers(image_name):
    image = Image.open(image_name)
    data = np.asarray(image)
    box_blur_text = f"""{box_blur_program['defines']}
                        {box_blur_program['signature']}
                        {{
                          {box_blur_program['body']}
                        }}
                        """
    blurred_data, box_blur_time = process_buffer(box_blur_text, data, box_blur_program['kernel'], np.int32(10))

    grayscale_text = f"""{grayscale_program['defines']}
                         {grayscale_program['signature']}
                         {{
                           {grayscale_program['body']}
                         }}
                        """
    grayscaled_data, grayscaled_time = process_buffer(grayscale_text, blurred_data, grayscale_program['kernel'])

    Image.fromarray(grayscaled_data).save("blurred_and_gray.jpg")

    return box_blur_time + grayscaled_time

def process_image_via_pipes(image_name):
    image = Image.open(image_name)
    data = np.asarray(image)
    blur_and_grey_text = f"""
                        pipe char gray_pipe __attribute__((xcl_reqd_pipe_depth(1024)));
                        pipe char blur_pipe __attribute__((xcl_reqd_pipe_depth(1024)));

                        {box_blur_program['defines']}
                        {box_blur_program['signature']}
                        {{
                          {box_blur_program['body']}
                          FOREACH_PIXEL_IN_CHUNK(pixel_index,
                                                 write_pipe(gray_pipe, &GET_COLOR_INDEX(RED, blur_row, pixel_index));
                                                 write_pipe(gray_pipe, &GET_COLOR_INDEX(GREEN, blur_row, pixel_index));
                                                 write_pipe(gray_pipe, &GET_COLOR_INDEX(BLUE, blur_row, pixel_index));)

                          mem_fence(CLK_GLOBAL_MEM_FENCE);

                          FOREACH_PIXEL_IN_CHUNK(pixel_index,
                                                 read_pipe(blur_pipe, &GET_COLOR_INDEX(RED, blur_row, pixel_index));
                                                 read_pipe(blur_pipe, &GET_COLOR_INDEX(GREEN, blur_row, pixel_index));
                                                 read_pipe(blur_pipe, &GET_COLOR_INDEX(BLUE, blur_row, pixel_index));)
                        }}

                        {grayscale_program['defines']}
                         __kernel void make_grayscale()
                         {{
                           int gid = get_global_id(0);
                           int red, green, blue;
                           read_pipe(gray_pipe, &red);
                           read_pipe(gray_pipe, &green);
                           read_pipe(gray_pipe, &blue);

                           char max_light = max_of_three(red, green, blue);
                           char min_light = min_of_three(red, green, blue);
                           char gray_color = (min_light + max_light) / 2;

                           write_pipe(blur_pipe, gray_color);
                           write_pipe(blur_pipe, gray_color);
                           write_pipe(blur_pipe, gray_color);
                        }}
                        """


    blurred_and_grey_data, blurred_and_grey_time = process_buffer(blur_and_grey_text,
                                                                  data,
                                                                  [box_blur_program['kernel'],
                                                                   grayscale_program['kernel']],
                                                                  np.int32(10))

    Image.fromarray(blurred_and_grey_data).save("blurred_and_gray.jpg")

    return blurred_and_grey_time

#ic(process_image_via_buffers("image.jpg"))
ic(process_image_via_pipes("image.jpg"))
