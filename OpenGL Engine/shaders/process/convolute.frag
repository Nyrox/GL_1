#version 330 core
out vec4 FragColor;
in vec3 localPos;

uniform samplerCube tex2D_sourceMap;

const float PI = 3.14159265359f;

void main()
{		
    // the sample direction equals the hemisphere's orientation 
    vec3 N = normalize(localPos);
  
	vec3 irradiance = vec3(0.0);  

	vec3 up    = vec3(0.0, 1.0, 0.0);
	vec3 right = cross(up,N);
	up         = cross(N, right);

	float sampleDelta = 0.05f;
	float nrSamples = 0.0f; 
	for(float phi = 0.0; phi < 2.0 * PI; phi += sampleDelta) {
	    for(float theta = 0.0; theta < 0.5 * PI; theta += sampleDelta) {
	        // spherical to cartesian (in tangent space)
	        vec3 tangentSample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
	        // tangent space to world
	        vec3 sampleVec = tangentSample.x * right + tangentSample.y * up + tangentSample.z * N; 
	
	        irradiance += texture(tex2D_sourceMap, sampleVec).rgb * cos(theta) * sin(theta);
	        nrSamples++;
	    }
	}
	irradiance = PI * irradiance * (1.0 / float(nrSamples));
  
    FragColor = vec4(irradiance, 1.0);
}