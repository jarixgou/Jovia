#version 120

uniform sampler2D texture;

uniform float intensity;
uniform float volumetricIntensity;
uniform float radialFalloff;
uniform float angularFalloff;

// couleur de l'objet (à fournir depuis le C++)
uniform vec3 baseColor;

void main()
{
    vec4 tex = texture2D(texture, gl_TexCoord[0].xy);

    vec3 lightTint = tex.rgb * baseColor;

    float finalIntensity = intensity * radialFalloff * angularFalloff;
    vec3 lightColor = finalIntensity * lightTint;

    // calcul du shading : la couleur finale est baseColor modulée par la lumière
    vec3 shadedColor = baseColor * lightColor;

    // ajouter l'effet volumétrique
    shadedColor += lightColor * volumetricIntensity;

    gl_FragColor = vec4(shadedColor, tex.a) * intensity;
}