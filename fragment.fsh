#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
varying highp vec2 texCoord;

void main(void)
{
    gl_FragColor = texture2D(texture, texCoord);
    //gl_FragColor = vec4(1.0, 1.0, 0.0, 1.0);
}
