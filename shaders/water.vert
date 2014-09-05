varying vec4 refrCoords; 
varying vec4 normCoords; 
varying vec4 viewCoords;
varying vec4 viewTangetSpace;
varying vec4 lightTangetSpace;

uniform vec4 lightPos, cameraPos;

void main()
{
	// The water is a plane, so we already know the tangent vector
	vec4 tangent = vec4(1.0, 0.0, 0.0, 0.0);
	vec4 normal = vec4(0.0, 1.0, 0.0, 0.0);
	vec4 biTangent = vec4(0.0, 0.0, 1.0, 0.0);

	// Calculate the view vector
	vec4 viewDir = cameraPos - gl_Vertex;

	// Transform the view vector into tangent space
	viewTangetSpace.x = dot(viewDir, tangent);
	viewTangetSpace.y = dot(viewDir, biTangent);
	viewTangetSpace.z = dot(viewDir, normal);
	viewTangetSpace.w = 1.0;

	// Calculate the vector that the light hits the vertex
	vec4 lightDir = lightPos - gl_Vertex;

	// Transform the light vector to tangent space
	lightTangetSpace.x = dot(lightDir, tangent);
	lightTangetSpace.y = dot(lightDir, biTangent);
	lightTangetSpace.z = dot(lightDir, normal);
	lightTangetSpace.w = 1.0;

	refrCoords = gl_MultiTexCoord1;
	normCoords = gl_MultiTexCoord2;

	viewCoords = gl_ModelViewProjectionMatrix * gl_Vertex;

	gl_Position = viewCoords;
}
