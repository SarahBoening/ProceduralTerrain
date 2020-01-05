#version 330 core

// Fragment Shader Terrain

in float height;

// the space we are shading in does not really matter
// here are two possible solutions: world space and view space

// for shading in world space
uniform vec3 worldSunDirection;

in vec3 fWorldPos;
in vec3 fWorldNormal;
in vec3 fWorldCam;
in vec3 fWorldCamDir;

// for shading in view space
in vec3 fViewPos;
in vec3 fViewNormal;
in vec3 fViewSunDirection;

out vec3 fragmentColor;
void main()
{

	vec3 whiteColor = vec3(1.0, 1.0, 1.0);
	vec3 greenColor = vec3(0.25, 0.8, 0.25);
	vec3 brownColor = vec3(0.5, 0.5, 0.0);
	float shiny = 128.0;

	// terrain height: 80 -> height aus dem vertex shader aus [-80, 80]
	float t = height / 80.0;	// t aus [-2.0, 2.0]
	t = clamp(t, -1.0, 1.0);	// t aus [-1, 1] abgeschnitten -> oberes wei�, braun

	vec3 diffuseColor;
	vec3 specularColor;
	if (t < 0.0) 
	{
		diffuseColor = mix(greenColor, brownColor, -t);		
		specularColor = vec3(0.0);
	}
	else 
	{
		diffuseColor = mix(greenColor, whiteColor, t);
		specularColor = mix(vec3(0.0), vec3(0.75), t); 
	}

	//-------------------------------------------
	// shading in world space

	/*
	vec3 V = normalize(fWorldCam - fWorldPos);
	vec3 L_Sun = normalize(worldSunDirection);
	vec3 L_Spot = V; // Licht sitzt an der Kamera
	vec3 N = normalize(fWorldNormal);
	
	float Strength_Sun = 0.5;									// damit man den spot noch sieht
	float Strength_Spot = pow(dot(fWorldCamDir, -V), 128.0);	// cos vom winkel zur Kamerarichtung 
	*/

	//-------------------------------------------
	// shading in view space
	
	vec3 V = normalize(vec3(0.0) - fViewPos);
	vec3 L_Sun = normalize(fViewSunDirection);
	vec3 L_Spot = V; // Licht sitzt an der Kamera
	vec3 N = normalize(fViewNormal);

	float Strength_Sun = 0.5;				// damit man den spot noch sieht
	float Strength_Spot = pow(V.z, 128.0); // View vector z = cos vom winkel zur Kamerarichtung 
	
	//-------------------------------------------

	// light is additive
	fragmentColor = vec3(0.0);

	// sun light
	vec3 H_Sun = normalize(V + L_Sun);
	fragmentColor += diffuseColor * Strength_Sun * max(0.0, dot(N, L_Sun));					// diffuse
	fragmentColor += specularColor * Strength_Sun * pow(max(0.0, dot(N, H_Sun)), shiny);	// specular

	// spotlight
	vec3 H_Spot = normalize(V + L_Spot);
	fragmentColor += diffuseColor * Strength_Spot * max(0.0, dot(N, L_Spot));				// diffuse
	fragmentColor += specularColor * Strength_Spot * pow(max(0.0, dot(N, H_Spot)), shiny);	// specular

	//fragmentColor = L_Spot;
}
