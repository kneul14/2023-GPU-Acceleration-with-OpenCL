#version 430 core

#define SPHERE 0
#define FUNGUS 1

in vec2 texCoordsExport;
in vec3 normalExport;
in vec3 modelExport;
in vec3 eyeExport;

struct Light
{
   vec4 ambCols;
   vec4 difCols;
   vec4 specCols;
   vec4 coords;
};

struct Material
{
   vec4 ambRefl;
   vec4 difRefl;
   vec4 specRefl;
   vec4 emitCols;
   float shininess;
};

uniform Light light0;
uniform vec4 globAmb;
uniform Material sphereFandB;


uniform sampler2D fungusTex; //singular texture in
uniform uint object;

uniform vec4 sphereColour;

out vec4 colorsOut;

vec4 sphereTexColor, fungusTexColor;
vec3 normal, lightDirection;
vec3 eyeDirection;
vec3 halfway;
vec4 fAndBDif;
vec4 fAndBSpec;

void main(void)
{     
   fungusTexColor = texture(fungusTex, texCoordsExport);   
   
   if (object == SPHERE) 
	{
    normal = normalize(normalExport);
	lightDirection = normalize(vec3(light0.coords));
	fAndBDif = max(dot(normal, lightDirection), 0.0f) * (light0.difCols * sphereFandB.difRefl); //diffuse and ambient
	//specular
	halfway = (length(lightDirection + eyeDirection) == 0.0f) ? vec3(0.0) : (lightDirection + eyeDirection) / length(lightDirection + eyeDirection);
	fAndBSpec = pow(max(dot(normal, halfway), 0.0f), sphereFandB.shininess) * light0.specCols * sphereFandB.specRefl;
	colorsOut =  sphereColour*vec4(vec3(min(fAndBDif + fAndBSpec, vec4(1.0))), 1.0);
   }
   if (object == FUNGUS) colorsOut = fungusTexColor;
}