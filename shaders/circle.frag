#version 330 core

in vec2 TexCoords;

out vec4 FragColor;

void main()
{
    vec2 center = vec2(0.0);
    float radius = 1.0;
    vec2 uv = TexCoords.xy * 2.0 - 1.0;

    float dist = distance(uv, center);
    if (dist > radius)
    {
        discard;
    }

    FragColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);

}