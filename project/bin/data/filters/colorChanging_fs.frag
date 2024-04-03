#ifdef GL_ES
precision mediump float;
#endif

uniform vec2 u_resolution;
uniform float u_time;

out vec4 FragColor;

void main() {

    vec3 color = vec3(0.694,0.855,0.369);
    
    FragColor = vec4(color,1.016);
}