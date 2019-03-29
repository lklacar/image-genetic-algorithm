//
// Created by luka on 3/28/19.
//

#include "genetic.h"

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>

genalg_genetic *genalg_genetic_init(genalg_image *reference_image, int population_size, int iteration_count) {
    genalg_genetic *genetic = malloc(sizeof(genalg_genetic));
    genetic->population = malloc(population_size * sizeof(genalg_image));
    genetic->population_size = population_size;
    genetic->reference_image = reference_image;
    genetic->iteration_count = iteration_count;

    for (int i = 0; i < population_size; i++) {
        genetic->population[i] = generate_random_image(reference_image->width, reference_image->height);
    }

    return genetic;
}

void mutate_if_needed(genalg_image *image) {
    if (rand() % 100 < 5) {
        for (int i = 0; i < (image->width * image->height) / 10; i++) {
            genalg_color c;
            c.red = rand();
            c.blue = rand();
            c.green = rand();
            image->pixels[rand() % image->height][rand() % image->width] = c;
        }
    }
}

void genalg_start(genalg_genetic *genetic) {
    int fitness_array[genetic->population_size];


    for (int i = 0; i < genetic->iteration_count; i++) {
        double sum = 0;
        for (int j = 0; j < genetic->population_size; j++) {
            fitness_array[j] = genalg_calculate_fitness(genetic->reference_image, genetic->population[j]);
            sum += fitness_array[j];
        }

        printf("Generation %d:\t%f\n", i, sum / genetic->population_size);

        int a = roulette_choose(fitness_array, genetic->population_size);
        int b = roulette_choose(fitness_array, genetic->population_size);
        genalg_image *image1 = genetic->population[a];
        genalg_image *image2 = genetic->population[b];

        crossover(image1, image2);

        mutate_if_needed(image1);
        mutate_if_needed(image2);
    }

    for (int i = 0; i < genetic->population_size; i++) {
        char buffer[sizeof(int) * 4 + 1];
        sprintf(buffer, "../out/%d.png", i);
        genalg_write_image(buffer, genetic->population[i]);
    }
}


int genalg_calculate_fitness(genalg_image *reference, genalg_image *image) {
    int sum = 0;
    for (int i = 0; i < reference->height; i++) {
        for (int j = 0; j < reference->width; j++) {
            int red_difference = abs(reference->pixels[i][j].red - image->pixels[i][j].red);
            int green_difference = abs(reference->pixels[i][j].green - image->pixels[i][j].green);
            int blue_difference = abs(reference->pixels[i][j].blue - image->pixels[i][j].blue);

            if (red_difference < 30) {
                sum += 1;
            }

            if (green_difference < 30) {
                sum += 1;
            }

            if (blue_difference < 30) {
                sum += 1;
            }
        }
    }

    return sum;
}

int roulette_choose(int fitness_array[], int size) {
    int weight_sum = 0;

    for (int i = 0; i < size; i++) {
        weight_sum += fitness_array[i];
    }

    double value = rand() % weight_sum;

    for (int i = 0; i < size; i++) {
        value -= fitness_array[i];
        if (value < 0) {
            return i;
        }
    }
    return fitness_array[size - 1];
}

void crossover(genalg_image *image1, genalg_image *image2) {
    int pivot = rand() % (image1->width * image1->height);

    int a = 0;

    for (int i = 0; i < image1->height; i++) {
        for (int j = 0; j < image2->width; j++) {
            if (a > pivot) {
                genalg_color temp = image1->pixels[i][j];
                image1->pixels[i][j] = image2->pixels[i][j];
                image2->pixels[i][j] = temp;
            }
            a++;
        }
    }
}

