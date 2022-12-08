#version 330 core //Version
//Point Light
uniform vec3 lightPos;
uniform vec3 lightColor;

uniform vec4 color;
uniform sampler2D tex0;

out vec4 FragColor; //Color of the Pixel
in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

uniform vec3 direction;

//Ambient
uniform float ambientStr;
uniform vec3 ambientColor;

//Direction Light
uniform float ambientStr2;
uniform vec3 ambientColor2;
uniform float specStr2;
uniform float specPhog2;
uniform vec3 lightColor2;
//Spec
uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhog;

//Vars needed for point light attenuation
uniform float constant; //light constant value
uniform float lin; //Linear var for light
uniform float quad; //Quadratic var for point light

vec4 pointlight(){
    vec4 pixelColor = texture(tex0, texCoord);
	if(pixelColor.a < 0.1){
		discard;
	}
    
    vec3 normal = normalize(normCoord);

    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(normal,lightDir), 0.0f);

    vec3 diffuse = diff * lightColor;
    vec3 ambientCol = ambientStr * ambientColor;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(
    max(
    dot(reflectDir, viewDir), 0.1f
      ), 
      specPhog
     );
     vec3 specCol = spec * specStr * lightColor;

     //Attenuation for point light intensity relative to distance from object
     //Compute distance from light and object
     float distance = length(lightPos - fragPos); 
     //Compute for reduction/increase of light based on distance between light and object
     float attenuation = 1.0f / (constant + lin * distance + quad * (distance * distance)); 

     //Apply attenuation to 3 types of lighting implemented
     diffuse *= attenuation;
     ambientCol *= attenuation;
     specCol *= attenuation;

     return vec4(diffuse + ambientCol + specCol, 1.0f) * pixelColor;
}

//Implement separate method for direction light to feed to FragColor
vec4 directionlight(){
    vec4 pixelColor = texture(tex0, texCoord);
	if(pixelColor.a < 0.1){
		discard;
	}
   vec3 normal = normalize(normCoord);

    vec3 lightDir = normalize(-direction);
    float diff = max(dot(normal,lightDir), 0.0f);

    vec3 diffuse = diff * lightColor2;
    vec3 ambientCol = ambientStr2 * ambientColor2;

    vec3 viewDir = normalize(cameraPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, normal);

    float spec = pow(
    max(
    dot(reflectDir, viewDir), 0.1f
      ), 
      specPhog2
     );
     vec3 specCol = spec * specStr2 * lightColor2;


     return vec4(diffuse + ambientCol + specCol, 1.0f) * pixelColor;
}

void main(){
   
    //FragColor = color;
   

     FragColor =  directionlight() + pointlight();
}