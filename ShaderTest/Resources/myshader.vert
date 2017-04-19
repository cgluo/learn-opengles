attribute vec4 a_position; // 1
attribute vec4 a_color; // 2
attribute vec2 TextureCoord;

varying vec4 DestinationColor; // 3
varying vec2 v_texCoord;


void main(void) { // 4
    DestinationColor = a_color; // 5
    v_texCoord = TextureCoord;
    gl_Position = CC_MVPMatrix * a_position; // 6
}