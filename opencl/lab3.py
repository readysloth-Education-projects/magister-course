import cl_wrap as clw

import numpy as np

from cl_image import process_image

from icecream import ic


def box_blur():
    time = process_image("image.jpg",
    """
    #define RED 0
    #define GREEN 1
    #define BLUE 2

    #define CREATE_SUM_NAME(COLOR) CREATE_READABLE_SUM_NAME(COLOR)
    #define CREATE_READABLE_SUM_NAME(COLOR) sum_ ## COLOR

    #define GET_COLOR_INDEX(COLOR, PTR, pixel_index) PTR[pixel_index + COLOR]

    __kernel void box_blur(__global char *rgb_row,
                           int blur_size,
                           __global char *blur_row)
    {

      int gid = get_global_id(0);

      int chunk_start = gid * 3 * blur_size;
      int chunk_end = chunk_start + 3 * blur_size;
      int CREATE_SUM_NAME(RED) = 0;
      int CREATE_SUM_NAME(GREEN) = 0;
      int CREATE_SUM_NAME(BLUE) = 0;

      #define FOREACH_PIXEL_IN_CHUNK(PIXEL_INDEX, BODY) \
        for(int PIXEL_INDEX = chunk_start; PIXEL_INDEX < chunk_end; PIXEL_INDEX += 3){ \
          BODY; \
        }

      #define BLUR(COLOR) \
        FOREACH_PIXEL_IN_CHUNK(pixel_index, \
                              CREATE_SUM_NAME(COLOR) += GET_COLOR_INDEX(COLOR, rgb_row, pixel_index); \
                              ) \
        CREATE_SUM_NAME(COLOR) /= blur_size;


      BLUR(RED)
      BLUR(GREEN)
      BLUR(BLUE)

      FOREACH_PIXEL_IN_CHUNK(pixel_index, GET_COLOR_INDEX(RED, blur_row, pixel_index)   = CREATE_SUM_NAME(RED))
      FOREACH_PIXEL_IN_CHUNK(pixel_index, GET_COLOR_INDEX(GREEN, blur_row, pixel_index) = CREATE_SUM_NAME(GREEN))
      FOREACH_PIXEL_IN_CHUNK(pixel_index, GET_COLOR_INDEX(BLUE, blur_row, pixel_index)  = CREATE_SUM_NAME(BLUE))

    }""", "box_blur", "box_blur.jpg", np.int32(10))

    return time

box_blur()
