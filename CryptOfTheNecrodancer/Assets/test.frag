uniform sampler2D texture;

uniform float u_time;

const float PI = 3.1415926535;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	if (pixel.r == 0 && pixel.g == 0 && pixel.b == 0)
	{
		return;
	}
	
	vec2 coord = gl_FragCoord.xy / 1300;
	vec3 color = vec3(0.0);
	vec2 translate = vec2(-0.5, -0.5);
	coord += translate;

	for(int i = 0; i < 40; i++){
		float radius = 0.3;
		float rad = radians(360.0 / 40.0) * float(i);
		
		color += 0.003 / length(coord + vec2(radius * cos(rad), radius * sin(rad))) * vec3(i % 3 == 0 ? 1 : 0, i % 3 == 1 ? 1 : 0, i % 3 == 2 ? 1 : 0);
		gl_FragColor = vec4(color, 1.0) * pixel;
	}
}
