//
// Created by luka on 3/28/19.
//

#include "genalg_image.h"
#include <stdlib.h>
#include <stdio.h>
#include <png.h>

genalg_image *genalg_load_image(char *path) {

    FILE *fp = fopen(path, "r");
    png_structp pngptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop pnginfo = png_create_info_struct(pngptr);
    png_set_palette_to_rgb(pngptr);
    png_init_io(pngptr, fp);
    png_bytepp rows;
    png_read_png(pngptr, pnginfo, PNG_TRANSFORM_IDENTITY, NULL);
    rows = png_get_rows(pngptr, pnginfo);

    int width = png_get_image_width(pngptr, pnginfo);
    int height = png_get_image_height(pngptr, pnginfo);
    genalg_image *image = malloc(sizeof(genalg_image));
    image->pixels = malloc(height * sizeof(genalg_color *));
    image->width = width;
    image->height = height;

    for (int i = 0; i < height; i++) {
        int y = 0;
        image->pixels[i] = malloc(width * sizeof(genalg_color));
        for (int j = 0; j < width * 3; j += 3) {
            genalg_color color;
            color.red = rows[i][j];
            color.green = rows[i][j + 1];
            color.blue = rows[i][j + 2];
            image->pixels[i][y] = color;
            y++;
        }
    }

    fclose(fp);

    return image;
}

void genalg_write_image(char *path, genalg_image *bitmap) {

    FILE *fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
    png_byte **row_pointers = NULL;
    int pixel_size = 3;
    int depth = 8;

    fp = fopen(path, "wb");

    png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);

    info_ptr = png_create_info_struct(png_ptr);

    png_set_IHDR(png_ptr,
                 info_ptr,
                 bitmap->width,
                 bitmap->height,
                 depth,
                 PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    row_pointers = png_malloc(png_ptr, bitmap->height * sizeof(png_byte *));
    for (y = 0; y < bitmap->height; y++) {
        png_byte *row =
                png_malloc(png_ptr, sizeof(unsigned char) * bitmap->width * pixel_size);
        row_pointers[y] = row;
        for (x = 0; x < bitmap->width; x++) {
            genalg_color pixel = bitmap->pixels[y][x];
            *row++ = pixel.red;
            *row++ = pixel.green;
            *row++ = pixel.blue;
        }
    }

    png_init_io(png_ptr, fp);
    png_set_rows(png_ptr, info_ptr, row_pointers);
    png_write_png(png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    for (y = 0; y < bitmap->height; y++) {
        png_free(png_ptr, row_pointers[y]);
    }
    png_free(png_ptr, row_pointers);

    png_destroy_write_struct(&png_ptr, &info_ptr);
    fclose(fp);
}
