#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "image/genalg_image.h"
#include "genetic/genetic.h"

int main() {
    srand(time(NULL));

    printf("Hello, World!\n");

    genalg_image *image = genalg_load_image("../res/hill.png");

    genalg_genetic *genetic = genalg_genetic_init(image, 20);
    genalg_start(genetic);

    return 0;
}