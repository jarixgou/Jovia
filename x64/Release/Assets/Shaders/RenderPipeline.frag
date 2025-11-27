#version 120

uniform sampler2D texture;
uniform sampler2D lightMap;

void main()
{
    vec4 sceneColor = texture2D(texture, gl_TexCoord[0].xy);
    vec4 lightColor = texture2D(lightMap, gl_TexCoord[0].xy);
    
    // Combiner la scène avec la lumière
    gl_FragColor = sceneColor * lightColor * 4;
}