#version 330 core

layout(location = 0) in vec2 vertex_pos_modelspace;
out float fragment_color;

uniform mat4 MVP_matrix;

uniform int max_iter;
uniform float max_radius;

uniform vec2 scale;
uniform vec2 shift;

void main(){

	gl_Position = MVP_matrix * vec4(vertex_pos_modelspace, 1, 1);
    int i = 0;
    vec2 z = vec2(0.0, 0.0);
    vec2 c = vec2(gl_Position.x * scale.x, gl_Position.y * scale.y) + shift;

    while (i < max_iter && sqrt(dot(z, z)) < max_radius) {
        z = vec2(z.x * z.x - z.y * z.y, 2.0 * z.x * z.y) + c;
        i++;
    }

	fragment_color = float(i)/float(max_iter);
}