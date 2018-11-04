#include "bunny_with_shadows.h"

const int  WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;

//http://www.opengl-tutorial.org/
//https://learnopengl.com/

int main() {
    BunnyWithShadows bunny(WINDOW_WIDTH, WINDOW_HEIGHT);
    bunny.show();
    return 0;
}