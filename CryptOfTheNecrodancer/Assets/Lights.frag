#define MAX_LIGHTS 3
uniform sampler2D texture;

uniform vec2 lightPositions[MAX_LIGHTS];
uniform vec3 lightColors[MAX_LIGHTS];
uniform float lightRanges[MAX_LIGHTS];

void main() {
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    vec3 finalColor = vec3(0.1);

    for (int i = 0; i < MAX_LIGHTS; ++i) {
        vec2 toLight = lightPositions[i] - gl_FragCoord.xy;
        float distance = length(toLight);

		float brightness = max(0.0, 1.0 - distance / lightRanges[i]);
		vec3 lightContribution = lightColors[i] * brightness;

        finalColor += lightContribution;
    }
	
	gl_FragColor = vec4(clamp(finalColor, 0.0, 1.0), 1.0) * pixel;
}