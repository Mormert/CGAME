#version 330 core

uniform vec3 color;
uniform float time;

in vec2 TexCoords;

out vec4 FragColor;

float noise(vec2 uv) {
    return fract(sin(dot(uv, vec2(12.9898,78.233))) * 43758.5453123 + 5.0 * time);
}

void main()
{
    vec2 center = vec2(0.0);
    float radius = 0.9;
    vec2 uv = TexCoords.xy * 2.0 - 1.0;

    float dist = distance(uv, center);

    dist += noise(uv) * 0.05;// * sin(time * 5.0);

    if (dist > radius)
    {
        discard;
    }

    vec3 col = color;

    //float transition = smoothstep(0.8, 0.9, dist);
    if(dist > 0.8){
        col = vec3(0.0);
    }
   // vec3 c = mix(color, vec3(0.0, 0.0, 0.0), transition);

    FragColor = vec4(col, 1.0f);
}