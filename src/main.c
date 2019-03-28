#include <stdio.h>
#include "image/genalg_image.h"

int main() {
    printf("Hello, World!\n");

    genalg_image *image = genalg_load_image("../res/hill.png");
    genalg_write_image("../out/hill.png", image);

    return 0;
}