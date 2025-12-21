//202220775 박민정 과제 38
#version 410 core
layout(location=0) in vec3 in_Position;
layout(location=1) in vec2 in_TexCoord;
layout(location=2) in vec3 in_Normal;

uniform mat4 viewMat = mat4(
                            1,0,0,0,
                            0,1,0,0,
                            0,0,1,0,
                            0,0,-3,1
                            );
uniform mat4 projMat = mat4(
                            1.299038, 0, 0, 0,
                            0, 1.732051, 0, 0,
                            0, 0, -1.002002, -1.0,
                            0, 0, -0.2002, 0
                            );
uniform mat4 modelMat = mat4(1);
uniform mat4 shadowProjMat;
uniform mat4 shadowViewMat;

uniform mat4 shadowBiasMVP;

out vec2 texCoord;
out vec3 normal;
out vec3 worldPos;

out vec4 shadowCoord;

void main(){
    vec4 worldCoord = modelMat * vec4(in_Position, 1.0);
    worldPos = worldCoord.xyz;

    normal = normalize( (modelMat * vec4(in_Normal, 0.0)).xyz );
    texCoord = in_TexCoord;

    gl_Position = projMat * viewMat * worldCoord;

    shadowCoord = shadowBiasMVP * worldCoord;
}
