#version 430
out vec3 outColor;

in vec2 Color;
in vec3 normalWorld;
in vec3 vertexPositionWorld;

uniform sampler2D myTextureSampler;

uniform vec3 diffuseLightColor1;
uniform vec3 diffuseLightColor2;
uniform vec3 diffuseLightColor3;
uniform vec3 lightPositionWorld1;
uniform vec3 lightPositionWorld2;
uniform vec3 lightPositionWorld3;
uniform vec3 eyePositionWorld;
uniform float directionalLightBrightness;
uniform float ambientLight;

void main()
{
	vec3 Texture = texture(myTextureSampler, Color).rgb;

	vec3 lightVectorWorld1 = normalize(lightPositionWorld1 - vertexPositionWorld);
	float brightness1 = dot(lightVectorWorld1, normalize(normalWorld)) * directionalLightBrightness;
	vec3 diffuseLight1 = vec3(brightness1);

	vec3 lightVectorWorld2 = normalize(lightPositionWorld2 - vertexPositionWorld);
	float brightness2 = dot(lightVectorWorld2, normalize(normalWorld)) * directionalLightBrightness;
	vec3 diffuseLight2 = vec3(brightness2);

	vec3 lightVectorWorld3 = normalize(lightPositionWorld3 - vertexPositionWorld);
	float brightness3 = dot(lightVectorWorld3, normalize(normalWorld)) * directionalLightBrightness;
	vec3 diffuseLight3 = vec3(brightness3);

	vec3 eyeVectorWorld;

	vec3 reflectedLightVectorWorld1 = reflect(-lightVectorWorld1, normalWorld);
	eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s1 = pow(clamp(dot(reflectedLightVectorWorld1, eyeVectorWorld),0,1),50);
	vec3 specularLight1 = vec3(s1);

	vec3 reflectedLightVectorWorld2 = reflect(-lightVectorWorld2, normalWorld);
	eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s2 = pow(clamp(dot(reflectedLightVectorWorld2, eyeVectorWorld),0,1),50);
	vec3 specularLight2 = vec3(s2);

	vec3 reflectedLightVectorWorld3 = reflect(-lightVectorWorld3, normalWorld);
	eyeVectorWorld = normalize(eyePositionWorld - vertexPositionWorld);
	float s3 = pow(clamp(dot(reflectedLightVectorWorld3, eyeVectorWorld),0,1),50);
	vec3 specularLight3 = vec3(s3);

	outColor = vec3(ambientLight);
	outColor = outColor + clamp(diffuseLight1,0,1) * diffuseLightColor1 + clamp(diffuseLight2,0,1) * diffuseLightColor2 + clamp(diffuseLight3,0,1) * diffuseLightColor3;
	outColor = outColor + specularLight1 * diffuseLightColor1 + specularLight2 * diffuseLightColor2 + specularLight3 * diffuseLightColor3;
	outColor = outColor * Texture;
}