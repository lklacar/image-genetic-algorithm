//
// Created by luka on 3/28/19.
//

#ifndef IMAGE_GENETIC_ALGORITHM_GENETIC_H
#define IMAGE_GENETIC_ALGORITHM_GENETIC_H

#include "../image/genalg_image.h"
#include <stdlib.h>

typedef struct genalg_genetic {
    genalg_image **population;
    int current_generation;
    int population_size;
    genalg_image *reference_image;
    int iteration_count;
} genalg_genetic;

genalg_genetic *genalg_genetic_init(genalg_image *reference_image, int population_size, int iteration_count);

void genalg_start(genalg_genetic *genetic);

int genalg_calculate_fitness(genalg_image *reference, genalg_image *image);

int roulette_choose(int *fitness_array, int size);

genalg_image** crossover(genalg_image *image1, genalg_image *image2);

#endif //IMAGE_GENETIC_ALGORITHM_GENETIC_H
