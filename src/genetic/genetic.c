//
// Created by luka on 3/28/19.
//

#include "genetic.h"
#include "../math/math.h"

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>


genalg_genetic *genalg_genetic_init(genalg_image *reference_image, int population_size, int iteration_count) {
    genalg_genetic *genetic = malloc(sizeof(genalg_genetic));
    genetic->population = malloc(population_size * sizeof(genalg_image));
    genetic->population_size = population_size;
    genetic->reference_image = reference_image;
    genetic->iteration_count = iteration_count;

    int a = 1;
    for (int i = 0; i < population_size; i++) {

        genalg_color c;
        c.red = random_int(0, 255);
        c.green = random_int(0, 255);
        c.blue = random_int(0, 255);
        genetic->population[i] = generate_color_image(reference_image->width, reference_image->height, c);

//        genetic->population[i] = generate_random_image(reference_image->width, reference_image->height);
    }

//    genetic->population[population_size - 1] = genalg_load_image("../res/cat.png");

    return genetic;
}

void mutate_if_needed(genalg_image *image) {
    if (random_int(0, 100) > 2) {
        return;
    }

    int rr = random_int(-255, 255);
    int rg = random_int(-255, 255);
    int rb = random_int(-255, 255);
    for (int i = 0; i < image->height; i++) {
        for (int j = 0; j < image->width; j++) {
            genalg_color c = image->pixels[i][j];
            c.red += rr;
            c.green += rg;
            c.blue += rb;
            image->pixels[i][j] = c;
        }
    }


//    if (random_int(0, 100) < 1) {
//        for (int i = 0; i < (image->width * image->height) / 10; i++) {
//            genalg_color c;
//            c.red = random_int(0, 255);
//            c.blue = random_int(0, 255);
//            c.green = random_int(0, 255);
//            image->pixels[random_int(0, image->height)][random_int(0, image->width)] = c;
//        }
//    }
}

int *calculate_fitness_for_popululation(genalg_image *refenrece_image, genalg_image **population, int size) {
    int *fitness_array = malloc(size * sizeof(int));

    for (int i = 0; i < size; i++) {
        fitness_array[i] = genalg_calculate_fitness(refenrece_image, population[i]);
    }

    return fitness_array;
}

int *find_elites(const int *fitness_array, int size) {
    int max1 = 0;
    int max2 = 0;
    int maxi1 = 0;
    int maxi2 = 0;

    int temp;

    for (int i = 0; i < size; i++) {
        temp = fitness_array[i];

        if (temp >= max1) {
            max2 = max1;
            maxi2 = maxi1;

            max1 = temp;
            maxi1 = i;
        } else if (temp > max2) {
            max2 = temp;
            maxi2 = i;
        }
    }

    int *m = malloc(2 * sizeof(int));
    m[0] = maxi1;
    m[1] = maxi2;
    return m;
}


genalg_image **generate_new_population(genalg_image **old_population, int *fitness_array, int size) {

    int added_elites = 0;

    genalg_image **new_population = malloc(size * sizeof(genalg_image *));
    for (int i = 0; i < size; i += 2) {
        if (added_elites == 0) {
            int *elites = find_elites(fitness_array, size);
            genalg_image *elite1 = old_population[elites[0]];
            genalg_image *elite2 = old_population[elites[1]];
            new_population[0] = elite1;
            new_population[1] = elite2;

            added_elites = 1;
        }


        int first = roulette_choose(fitness_array, size);
        int second;
        do {
            second = roulette_choose(fitness_array, size);
        } while (second == first);

        genalg_image *parent1 = old_population[first];
        genalg_image *parent2 = old_population[second];

        genalg_image **children = crossover(parent1, parent2);
        mutate_if_needed(children[0]);
        mutate_if_needed(children[1]);

        new_population[i] = children[0];
        new_population[i + 1] = children[1];
    }

    return new_population;
}

void genalg_start(genalg_genetic *genetic) {

    for (int i = 0; i < genetic->iteration_count; i++) {
        int *fitness_array = calculate_fitness_for_popululation(genetic->reference_image, genetic->population,
                                                                genetic->population_size);


        genalg_image **new_population = generate_new_population(genetic->population, fitness_array,
                                                                genetic->population_size);

        double sum = 0;
        for (int j = 0; j < genetic->population_size; j++) {
            sum += fitness_array[j];
            for (int k = 0; k < genetic->population[j]->height; k++) {
                free(genetic->population[j]->pixels[k]);
            }
            free(genetic->population[j]->pixels);
        }

        printf("%d %f\n", i, sum / genetic->population_size);



        genetic->population = new_population;
    }

    int *fitness_array = calculate_fitness_for_popululation(genetic->reference_image, genetic->population,
                                                            genetic->population_size);

    int *elites = find_elites(fitness_array, genetic->population_size);
    genalg_image *elite1 = genetic->population[elites[0]];
    genalg_image *elite2 = genetic->population[elites[1]];

    genalg_write_image("../out/1.png", elite1);
    genalg_write_image("../out/2.png", elite2);

//
//    for (int i = 0; i < genetic->population_size; i++) {
//        char buffer[sizeof(int) * 4 + 1];
//        sprintf(buffer, "../out/%d.png", i);
//        genalg_write_image(buffer, genetic->population[i]);
//    }
}


int genalg_calculate_fitness(genalg_image *reference, genalg_image *image) {
    int sum = 0;
    for (int i = 0; i < reference->height; i++) {
        for (int j = 0; j < reference->width; j++) {
            int red_difference = abs(reference->pixels[i][j].red - image->pixels[i][j].red);
            int green_difference = abs(reference->pixels[i][j].green - image->pixels[i][j].green);
            int blue_difference = abs(reference->pixels[i][j].blue - image->pixels[i][j].blue);

            if (red_difference < 10) {
                sum += 1;
            }

            if (green_difference < 10) {
                sum += 1;
            }

            if (blue_difference < 10) {
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

    double value = random_int(0, weight_sum);

    for (int i = 0; i < size; i++) {
        value -= fitness_array[i];
        if (value < 0) {
            return i;
        }
    }
    return fitness_array[size - 1];
}


int blend(int target, int blend) {
    // Color burn
    return blend;
    return 255.0 - (255.0 - target) / blend;
}

genalg_image **crossover(genalg_image *image1, genalg_image *image2) {
    int x1 = random_int(0, image1->width);
    int x2 = random_int(0, image1->width);
    int y1 = random_int(0, image1->height);
    int y2 = random_int(0, image1->height);


    if (x1 < x2) {
        int temp = x1;
        int x1 = x2;
        int x2 = temp;
    }

    if (y1 < y2) {
        int temp = y1;
        int y1 = y2;
        int y2 = temp;
    }


    genalg_image *child1 = init_empty_image(image1->width, image1->height);
    genalg_image *child2 = init_empty_image(image1->width, image1->height);


    for (int i = 0; i < image1->height; i++) {
        for (int j = 0; j < image2->width; j++) {
            if (i > y2 && i < y1 && j > x2 && j < x1) {

                child1->pixels[i][j].red = blend(child1->pixels[i][j].red, image1->pixels[i][j].red);
                child1->pixels[i][j].green = blend(child1->pixels[i][j].green, image1->pixels[i][j].green);
                child1->pixels[i][j].blue = blend(child1->pixels[i][j].blue, image1->pixels[i][j].blue);

                child2->pixels[i][j].red = blend(child2->pixels[i][j].red, image2->pixels[i][j].red);
                child2->pixels[i][j].green = blend(child2->pixels[i][j].green, image2->pixels[i][j].green);
                child2->pixels[i][j].blue = blend(child2->pixels[i][j].blue, image2->pixels[i][j].blue);

            } else {
                child1->pixels[i][j].red = blend(child1->pixels[i][j].red, image2->pixels[i][j].red);
                child1->pixels[i][j].green = blend(child1->pixels[i][j].green, image2->pixels[i][j].green);
                child1->pixels[i][j].blue = blend(child1->pixels[i][j].blue, image2->pixels[i][j].blue);

                child2->pixels[i][j].red = blend(child2->pixels[i][j].red, image1->pixels[i][j].red);
                child2->pixels[i][j].green = blend(child2->pixels[i][j].green, image1->pixels[i][j].green);
                child2->pixels[i][j].blue = blend(child2->pixels[i][j].blue, image1->pixels[i][j].blue);;
            }

//            child1->pixels[i][j].red = child1->pixels[i][j].red % 255;
//            child1->pixels[i][j].green = child1->pixels[i][j].green % 255;
//            child1->pixels[i][j].blue = child1->pixels[i][j].blue % 255;
//
//            child2->pixels[i][j].red = child2->pixels[i][j].red % 255;
//            child2->pixels[i][j].green = child2->pixels[i][j].green % 255;
//            child2->pixels[i][j].blue = child2->pixels[i][j].blue % 255;
        }
    }

    genalg_image **ret = malloc(2 * sizeof(genalg_image *));
    ret[0] = child1;
    ret[1] = child2;

    return ret;
}

