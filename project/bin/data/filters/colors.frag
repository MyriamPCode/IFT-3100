#version 150

uniform vec2 u_resolution;
uniform float u_time;

out vec4 outputColor;

void main()
{
    vec2 st = gl_FragCoord.xy/u_resolution.xy;
    st.x = st.x * u_resolution.x/u_resolution.y;

    vec3 color = vec3(0.694,0.855,0.369);
    color = vec3(st.x,st.y,abs(sin(u_time)));

    outputColor = vec4(color.x, color.y, color.z, 1.0);
}