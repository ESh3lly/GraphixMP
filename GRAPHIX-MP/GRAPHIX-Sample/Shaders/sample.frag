#version 330 core // Version

uniform sampler2D tex0;
uniform sampler2D norm_tex;

uniform vec3 lightPos;
uniform vec3 lightColor;

uniform float ambientStr;
uniform vec3 ambientColor;

uniform vec3 cameraPos;
uniform float specStr;
uniform float specPhog;

// Added Constant & Linear & Quadratic for the Attenuation formula
uniform float constant;
uniform float linear;
uniform float quadratic;

in vec2 texCoord;
in vec3 normCoord;
in vec3 fragPos;

in mat3 TBN;

out vec4 FragColor; // Color of the Pixel

void main() {
	//FragColor = vec4(0.5f, 0f, 0f, 1f);

	vec4 pixelColor = texture(tex0, texCoord);


	if(pixelColor.a < 0.1) {
		discard;
	}

	//vec3 normal = normalize(normCoord);

	vec3 normal = texture(norm_tex, texCoord).rgb;

	normal = normalize(normal * 2.0 - 1.0);

	normal = normalize(TBN * normal);

	vec3 lightDir = normalize(lightPos - fragPos);

	float diff = max(
		dot(normal, lightDir) , 0.0f
	);

	vec3 diffuse = diff * lightColor;

	vec3 ambientCol = ambientStr * ambientColor;

	vec3 viewDir = normalize(cameraPos - fragPos);
	vec3 reflectDir = reflect(-lightDir, normal);

	float spec = pow(
		max(
			dot(reflectDir, viewDir), 0.1f
		),
	specPhog );

	vec3 specCol = spec * specStr * lightColor;

	float distance = length(lightPos - fragPos);

	// Calculate the attenuation (Which is a property of light when it loses it intensity when further from the source)
	float attenuation = 5.0f / (constant + linear * distance + quadratic * (distance * distance));

	// Multiple it to ambient diffuse and specular to apply the attenuation effect
	ambientCol *= attenuation;
	diffuse *= attenuation;
	specCol *= attenuation;

	FragColor = vec4(specCol + diffuse + ambientCol, 1.0f) *  pixelColor;

}