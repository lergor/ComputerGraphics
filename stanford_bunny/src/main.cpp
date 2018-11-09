#include "bunny_with_shadows.h"

const int  WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

//http://www.opengl-tutorial.org/
//https://learnopengl.com/
//https://triplepointfive.github.io/ogltutor/

int main() {
    BunnyWithShadows bunny(WINDOW_WIDTH, WINDOW_HEIGHT);
    bunny.show();
    return 0;
}