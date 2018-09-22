#include "src/fractal.h"

const int windowWidth = 850;
const int windowHeight = 550;
const int maxIteration = 200;
const float radiusLimit = 2.0;

int main() {
    Fractal l(windowWidth, windowHeight, maxIteration, radiusLimit);
    l.show();
    return 0;
}
