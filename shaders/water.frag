varying vec4 refrCoords; 
varying vec4 normCoords; 
varying vec4 viewCoords;
varying vec4 viewTangetSpace;
varying vec4 lightTangetSpace;

uniform sampler2D reflection;
uniform sampler2D refraction;
uniform sampler2D normalMap;
uniform sampler2D dudvMap;
uniform sampler2D depthMap;
uniform vec4 waterColor;

void main()
{
	// Specular light
	const float shine = 128.0;

	// Distortion constant that makes the water move
	const float distortion = 0.015;
	
	// Refraction constant
	const float refract = 0.009;

	// Compute the distortion wave offset and normalize it
	vec4 distOffset = texture2D(dudvMap, normCoords.xy) * distortion;
	vec4 dudvColor = texture2D(dudvMap, vec2(refrCoords + distOffset));
	dudvColor = normalize(dudvColor * 2.0 - 1.0) * refract;

	// Get the offset from the normal map
	vec4 normalVector = texture2D(normalMap, vec2(refrCoords + distOffset));
	normalVector = normalVector * 2.0 - 1.0;
	normalVector.a = 0.0;

	vec4 lightReflection = normalize( reflect(-lightTangetSpace, normalVector) );

	// Using a compressed version of Schlick's  approximation for the Fresnel term
	vec4 invertedFresnel = vec4( dot(normalVector, lightReflection ) );
	vec4 fresnelTerm = 1.0 - invertedFresnel;

	// Get the projection coordinates
	vec4 projCoord = viewCoords / viewCoords.q;
	projCoord = (projCoord + 1.0) * 0.5;
	projCoord += dudvColor;
	projCoord = clamp(projCoord, 0.001, 0.999);

	// Compute the reflection, refraction, and depth	
	vec4 reflectionColor  = texture2D(reflection, projCoord.xy);
	vec4 refractionColor  = texture2D(refraction, projCoord.xy);
	vec4 depthValue = texture2D(depthMap, projCoord.xy);
		
	vec4 invDepth = 1.0 - depthValue;
	refractionColor *= invertedFresnel * invDepth;
	refractionColor +=  waterColor * depthValue * invertedFresnel;

	reflectionColor *= fresnelTerm;

	vec4 localView = normalize(viewTangetSpace);	
	
	// Store the intensity of the specular ligth	
	float intensity = max(0.0, dot(lightReflection, localView) );

	// Compute the specular
	vec4 specular = vec4(pow(intensity, shine));

	// Finally, compute the fragment color
	gl_FragColor = refractionColor + reflectionColor + specular; 
}