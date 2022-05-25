#version 330

// UV-coordinate of the fragment (interpolated by the rasterization stage)
in vec2 outUV;

// Color of the fragment received from the vertex shader (interpolated by the rasterization stage)
in vec3 outColor;

// normal vector
in vec3 outNormal;

in vec3 outPos;

in vec3 viewPos;

// Final color of the fragment that will be rendered on the screen
out vec4 fragColor;

// Texture unit of the texture
uniform sampler2D tex0;
//uniform sampler2D tex1;

uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 ambientLight;
uniform vec3 diffuseLight;

uniform vec3 matlAmbient;
uniform vec3 matlDiffuse;
uniform vec3 matlSpecular;
uniform float matlShiny;

void main()
{
    // Get pixel color of the texture at the current UV coordinate
    // and output it as our final fragment color
//    fragColor = mix(texture(tex0, outUV), texture(tex1, outUV), opacity) * vec4(outColor, 1.0);
//    fragColor = texture(tex0, outUV) * vec4(ambientLight, 1.0);
    vec3 normal = normalize(outNormal);
    vec3 lightDir = normalize(lightPos - outPos);
    
    vec3 viewDir = normalize(viewPos - outPos);
    vec3 refDir = reflect(-lightDir, normal);
    
    vec3 ambient = ambientLight * matlAmbient;
    
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = matlDiffuse * (diff * lightColor);
    
    float spec = pow(max(dot(viewDir, refDir), 0.0), matlShiny);
    vec3 specular = matlSpecular * (spec * lightColor);
    
    vec3 result = (ambient + diffuse + specular) * outColor;
    
    fragColor = texture(tex0, outUV) * vec4(result, 1.0);
}
