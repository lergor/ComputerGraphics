#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec4 FragPosLightSpace;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    mat4 lightSpaceMatrix;
};

struct PointLight {
    vec3 position;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    mat4 lightSpaceMatrix;
};

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    mat4 lightSpaceMatrix;
};

#define NR_POINT_LIGHTS 1

uniform sampler2D shadowMap;
uniform bool blinn;

uniform vec3 viewPos;
uniform vec3 lightPos;

uniform Light light;
uniform Material material;
uniform DirLight dirLight;
uniform PointLight pointLights[NR_POINT_LIGHTS];

float ShadowCalculation(vec3 lightDir) {
//    vec4 FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);
    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(shadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    vec3 normal = normalize(Normal);

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = 0.0;

    vec2 texelSize = 1.0 / textureSize(shadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(shadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 0.7 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0) {
        shadow = 0.0;
    }
    return shadow;
}

vec3 phong_basic(vec3 light_ambient, vec3 light_diffuse, vec3 light_specular, vec3 lightDir) {
    vec3 ambient = light_ambient * material.ambient;

    vec3 norm = normalize(Normal);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = light_diffuse * (diff * material.diffuse);

    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = 0.0;
    if(blinn) {
        vec3 halfwayDir = normalize(lightDir + viewDir);
        spec = pow(max(dot(norm, halfwayDir), 0.0), material.shininess);
    } else {
        vec3 reflectDir = reflect(-lightDir, norm);
        spec = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
    }

    vec3 specular = light_specular * (spec * material.specular);

    vec3 res = ambient + diffuse + specular;
    float shadow = ShadowCalculation(lightDir);
//    res = (ambient + (1.0 - shadow) * (diffuse + specular));
    return res;
}

void main() {
    vec3 color = vec3(0.7, 0.7, 0.7);
    vec3 normal = normalize(Normal);
    vec3 lightColor = vec3(0.3);
    vec3 ambient = 0.15 * color;
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(lightDir, normal), 0.0);
    vec3 diffuse = diff * lightColor;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = 0.0;
    vec3 halfwayDir = normalize(lightDir + viewDir);
    spec = pow(max(dot(normal, halfwayDir), 0.0), 64.0);
    vec3 specular = spec * lightColor;

    float shadow = ShadowCalculation(lightDir);
    vec3 light_dir = normalize(light.position - FragPos);
    vec3 phong = phong_basic(light.ambient, light.diffuse, light.specular, light_dir);

    vec3 lighting = ((ambient + (1.0 - shadow) * (diffuse + specular)) * phong) + phong;
    FragColor = vec4(lighting, 1.0);
}