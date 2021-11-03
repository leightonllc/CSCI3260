#version 430

in layout(location=0) vec3 position;
in layout(location=1) vec2 vertexColor;
in layout(location=2) vec3 normal;

uniform mat4 modelTransformMatrix;
uniform mat4 projectionMatrix;
uniform mat4 cameraMatrix;

out vec2 Color;
out vec3 normalWorld;
out vec3 vertexPositionWorld;

void main()
{
	vec4 v = vec4(position, 1.0);
	vec4 newPosition = modelTransformMatrix * v;
	vec4 cameraposition = cameraMatrix * newPosition;
	gl_Position = projectionMatrix * cameraposition;
	
	Color = vertexColor;

	vec4 normal_temp = modelTransformMatrix * vec4(normal, 0);
	normalWorld = normal_temp.xyz;

	vertexPositionWorld = newPosition.xyz;
}