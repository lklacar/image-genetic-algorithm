//
// Created by luka on 3/28/19.
//

#ifndef IMAGE_GENETIC_ALGORITHM_GENALG_IMAGE_H
#define IMAGE_GENETIC_ALGORITHM_GENALG_IMAGE_H

typedef struct genalg_color {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} genalg_color;

typedef struct genalg_image {
    genalg_color **pixels;
    int width;
    int height;
} genalg_image;

genalg_image *genalg_load_image(char *path);

void genalg_write_image(char *path, genalg_image *image);

#endif //IMAGE_GENETIC_ALGORITHM_GENALG_IMAGE_H
