#include <stdio.h>
#include <stdlib.h>
#include <png.h>
#include <assert.h>

int main(int argc, char *argv[]) {
    int width = 1408;
    int height = 3840;

    png_bytepp row_ptr = (png_bytepp)malloc(sizeof(png_bytep) * height);

    const int rowbytes = width * 2;
    for (int y=0; y<height; ++y) {
        row_ptr[y] = (png_bytep)malloc(rowbytes);
    }

    for (int y=0; y<height; ++y) {
        assert(width == fread(row_ptr[y], 2, width, stdin));
    }

    png_structp png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    assert(png_ptr);

    png_infop info_ptr = png_create_info_struct(png_ptr);
    assert(info_ptr);

    assert(0 == setjmp(png_jmpbuf(png_ptr)));

    png_init_io(png_ptr, stdout);

    png_set_IHDR(
            png_ptr,
            info_ptr,
            width,
            height,
            16,
            PNG_COLOR_TYPE_GRAY,
            PNG_INTERLACE_NONE,
            PNG_COMPRESSION_TYPE_DEFAULT, 
            PNG_FILTER_TYPE_DEFAULT);

    png_write_info(png_ptr, info_ptr);

    png_write_image(png_ptr, row_ptr);

    png_write_end(png_ptr, NULL);

    png_destroy_write_struct(&png_ptr, &info_ptr);

    return 0;
}
