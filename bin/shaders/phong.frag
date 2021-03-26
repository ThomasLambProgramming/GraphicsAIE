//a simple shader for a Phong implementation 
#version 410

in vec4 vPosition;
in vec3 vNormal;
in vec3 vTangent;
in vec3 vBiTangent;

uniform vec3 Ka; //ambient color of the models material
uniform vec3 Kd; // the diffuse color of the models material
uniform vec3 Ks; // the specularcolor of the moddels material
uniform float Ns; // the specular power of the models material 

uniform vec3 AmbientColor; //ambient color of the lgight
uniform vec3 LightColor; // color of hte light
uniform vec3 LightDirection;

const int MAX_LIGHTS = 4;
uniform int numLights;
uniform vec3 PointLightColor[MAX_LIGHTS];
uniform vec3 PointLightPosition[MAX_LIGHTS];

uniform vec3 CameraPosition; //position of the viewport camera for the specular calculations

out vec4 FragColor;
//probs go back on the lighting tutorial later 

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
    vec3 diffuse = Kd * diffuseTotal;

    // Determnine the value of the specular
    vec3 specular = Ks * specularTotal;
    // Output the final color
    FragColor = vec4(ambient + diffuse + specular, 1);
}