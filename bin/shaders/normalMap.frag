// Phong shader for simple game lighting
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec2 vTexCoord;
in vec3 vTangent;
in vec3 vBiTangent;

uniform sampler2D diffuseTexture;
uniform sampler2D specularTexture;
uniform sampler2D normalTexture;

uniform vec3 Ka; // The ambient color of the model's material
uniform vec3 Kd; // The diffuse color of the model's material
uniform vec3 Ks; // The specular color of the model's material
uniform float Ns; // The specular power of the model's material

uniform vec3 AmbientColor; // Ambient color of the ligtht
uniform vec3 LightColor; // Color of the light
uniform vec3 LightDirection;

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColor[MAX_LIGHTS];
uniform vec3 PointLightPosition[MAX_LIGHTS];


uniform vec3 CameraPosition; // Position of the viewport camera for specular calculations

out vec4 FragColor;

vec3 diffuse(vec3 direction, vec3 color, vec3 normal)
{
    return color * max(0,dot(normal, -direction));
}

vec3 specular(vec3 direction, vec3 color, vec3 normal, vec3 view)
{
    vec3 R = reflect(direction, normal);
    //calculate the specular term 

    float specTerm = pow(max(0,dot(R,view)), Ns);
    return specTerm * color;                                                                                
}

void main()
{
    // Make sure the normal and the light directions have been normalised
    vec3 N = normalize(vNormal);
    vec3 L = normalize(LightDirection);
    vec3 T = normalize(vTangent);
    vec3 B = normalize(vBiTangent);

    mat3 TBN = mat3(T, B, N);

    vec3 texDiffuse = texture(diffuseTexture, vTexCoord).rgb;
    vec3 texSpecular = texture(specularTexture, vTexCoord).rgb;
    vec3 texNormal = texture(normalTexture, vTexCoord).rgb;
    
    N = TBN * (texNormal * 2 - 1);

    //calculate the diffuse lighting from sunlight
    vec3 diffuseTotal = diffuse(L, LightColor, N);

    // Now we can calculate the lambert term, negative the light direction
    float lambertTerm = max(0, min(1, dot(N, -L)));

    // Calculate the view vector and the reflection vector
    vec3 V = normalize(CameraPosition - vPosition.xyz);
    vec3 R = reflect(L, N);

    vec3 specularTotal = specular(L, LightColor, N, V);

    for (int i = 0; i < numLights; i++)
    {
        vec3 direction = vPosition.xyz - PointLightPosition[i];
        float distance = length(direction);
        direction = direction / distance;
    

        //get the light intensity with the inverse square law
        vec3 color = PointLightColor[i] / (distance * distance);

        diffuseTotal += diffuse(direction, color, N);
        specularTotal += specular(direction, color, N, V);    
    }

    // Determine the value of the ambient
    vec3 ambient = AmbientColor * Ka;

    // Determine the value of the diffuse
    vec3 diffuse = Kd * texDiffuse * diffuseTotal;

    // Determnine the value of the specular
    vec3 specular = Ks * texSpecular * specularTotal;

    // Output the final color
    FragColor = vec4(ambient + diffuse + specular, 1);
}