#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif


attribute highp vec4 vertex;
varying highp vec2 texCoord;

void main(void)
{
    gl_Position = vertex;
    texCoord.x = vertex.x * 0.5 + 0.5;
    texCoord.y = vertex.y * 0.5 + 0.5;
}
