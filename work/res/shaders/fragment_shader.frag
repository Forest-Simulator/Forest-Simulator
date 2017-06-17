//---------------------------------------------------------------------------
//
// Copyright (c) 2015 Taehyun Rhee, Joshua Scott, Ben Allen
//
// This software is provided 'as-is' for assignment of COMP308 in ECS,
// Victoria University of Wellington, without any express or implied warranty. 
// In no event will the authors be held liable for any damages arising from
// the use of this software.
//
// The contents of this file may not be copied or duplicated in any form
// without the prior permission of its owner.
//
//----------------------------------------------------------------------------

#version 120

// Constant across both shaders
uniform sampler2D texture0;
uniform float texture_multiplier;

// Values passed in from the vertex shader
varying vec3 vNormal;
varying vec3 vPosition;
varying vec2 vTextureCoord0;
varying vec2 vNormalMapCoord;

#define MAX_LIGHTS 3

vec4 calculateLightIntensity(int light, vec3 normal) {
	// Light direction, a normal vertex from the position of the light 
	// to the position of the vertex
	vec3 lightDirection = normalize(gl_LightSource[light].position.xyz - vPosition);

	// Reflection from the fragment in the direction of the light
	vec3 reflection = normalize(-reflect(lightDirection, normal));

	// Inverse of the position vector, when this and the reflection 
	// align then the specular light will be strongest
	vec3 viewNormal = normalize(-vPosition);

	vec4 ambientIntensity = gl_FrontLightProduct[light].ambient;
	vec4 diffuseIntensity = max(dot(normal, lightDirection), 0) * gl_FrontLightProduct[light].diffuse;
	diffuseIntensity = clamp(diffuseIntensity, 0.0, 1.0);
	vec4 specularIntensity = gl_FrontLightProduct[light].specular * 
		pow(
			max(dot(reflection, viewNormal), 0.0),
			gl_FrontMaterial.shininess
		);
	specularIntensity = clamp(specularIntensity, 0.0, 1.0);

	return ambientIntensity + diffuseIntensity + specularIntensity;
}

void main() {

	vec4 color = vec4(texture2D(texture0, vTextureCoord0 * texture_multiplier).rgb, 1.0);
	vec4 finalLightIntensity = vec4(0.0, 0.0, 0.0, 0.0);

	for(int i = 0; i < MAX_LIGHTS; i++) {
		// Light direction, a normal vertex from the position of the light 
		// to the position of the vertex
		vec3 lightDirection = normalize(gl_LightSource[i].position.xyz - vPosition);
		finalLightIntensity += calculateLightIntensity(i, vNormal);
	}

	gl_FragColor = color * finalLightIntensity;
}