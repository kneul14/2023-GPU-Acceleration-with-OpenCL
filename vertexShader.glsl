#version 430 core

#define SPHERE 0
#define Fungus 1

layout(location = 0) in vec4 Coords;
layout(location = 1) in vec2 TexCoords;
layout(location = 2) in vec4 sphereCoords;
layout(location = 3) in vec3 sphereNormals;

layout(location = 4) in vec3 objCoords;
layout(location = 5) in vec3 objNormals;
layout(location = 6) in vec2 objTexCoords;

uniform mat4 modelViewMat;
uniform mat4 projMat;
uniform uint object;
uniform float yPos;
uniform mat4 modelMat;
uniform mat4 viewMat;
uniform vec3 eyePos;

out vec2 texCoordsExport;
out vec3 normalExport;
out vec3 eyeExport;
out vec3 modelExport;

vec4 coords;

void main(void)
{

    if (object == SPHERE)
    {
        coords          = sphereCoords;
        normalExport    = sphereNormals;
        texCoordsExport = TexCoords;
    }
    if (object == Fungus) 
    {
        coords          = sphereCoords;
        texCoordsExport = TexCoords;
    }
    
    modelExport = vec3(modelViewMat * vec4(objCoords, 1.0f));
    eyeExport = eyePos;
    
    gl_Position = projMat * viewMat * modelMat * coords;
    
}
