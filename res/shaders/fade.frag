#version 330

precision lowp float;

layout(location = 0) out vec4 o_color;

in vec2 i_tex;

uniform sampler2D texture;

const float fadePower = 0.01;

void main() {
    vec4 pixel = texture2D(texture, i_tex);

    pixel -= fadePower;

    o_color = pixel;
}