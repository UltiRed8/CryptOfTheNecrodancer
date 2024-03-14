#define RADIUS 300
#define COLOR vec3(1.0, 1.0, 0.9)
#define BASE_COLOR vec3(0.1, 0.1, 0.1)

uniform sampler2D texture;

uniform vec2 lightPosition;

void main() {
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    vec3 finalColor = BASE_COLOR;

    vec2 toLight = lightPosition - gl_FragCoord.xy;
    float distance = length(toLight);

	float brightness = max(0.0, 1.0 - distance / RADIUS);
	vec3 lightContribution = COLOR * brightness;

    finalColor += lightContribution;
	
	gl_FragColor = vec4(clamp(finalColor, 0.0, 1.0), 1.0) * pixel;
}