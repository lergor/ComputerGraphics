#version 330 core

out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;

struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
    sampler2D shadowMap;

};

struct PointLight {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;

    mat4 lightSpaceMatrix;
    sampler2D shadowMap;

};

struct DirLight {
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

#define NR_POINT_LIGHTS 2

uniform bool blinn;
uniform vec3 viewPos;
uniform Material material;
uniform PointLight lights[NR_POINT_LIGHTS];
uniform DirLight dirLight;
uniform sampler2D gShadowMap;
uniform mat4 lightSpaceMatrix;

float ShadowCalculation(vec3 lightDir) {
    vec4 FragPosLightSpace = lightSpaceMatrix * vec4(FragPos, 1.0);

    vec3 projCoords = FragPosLightSpace.xyz / FragPosLightSpace.w;
    projCoords = projCoords * 0.5 + 0.5;

    float closestDepth = texture(gShadowMap, projCoords.xy).r;
    float currentDepth = projCoords.z;
    vec3 normal = normalize(Normal);

    float bias = max(0.05 * (1.0 - dot(normal, lightDir)), 0.005);
    float shadow = 0.0;

    vec2 texelSize = 1.0 / textureSize(gShadowMap, 0);
    for(int x = -1; x <= 1; ++x) {
        for(int y = -1; y <= 1; ++y) {
            float pcfDepth = texture(gShadowMap, projCoords.xy + vec2(x, y) * texelSize).r;
            shadow += currentDepth - bias > pcfDepth  ? 0.7 : 0.0;
        }
    }
    shadow /= 9.0;

    if(projCoords.z > 1.0) {
        shadow = 0.0;
    }
    return 1.0 - shadow;
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

//    vec3 res = ambient + diffuse + specular;
    float shadow = ShadowCalculation(lightDir);
    vec3 res = (ambient + shadow * (diffuse + specular));
    return res;
}

vec3 CalcDirLight(DirLight light) {
    vec3 lightDir = normalize(-light.direction);
    vec3 res = phong_basic(light.ambient, light.diffuse, light.specular, lightDir); // ?
    return res;
}

vec3 CalcPointLight(PointLight light) {
    vec3 lightDir = normalize(light.position - FragPos);
    float distance = length(light.position - FragPos);

    float attenuation = light.constant + light.linear * distance + light.quadratic * (distance * distance);
    vec3 basic = phong_basic(light.ambient, light.diffuse, light.specular, lightDir);
    return basic / attenuation;
}

void main() {

    vec3 point_lightning = vec3(0.0f);
    point_lightning += CalcDirLight(dirLight);
    for(int i = 0; i < NR_POINT_LIGHTS; i++){
        point_lightning += CalcPointLight(lights[i]);
    }
    FragColor = vec4(point_lightning, 1.0);
}