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

	float angle = atan(-coord.y + 0.25, coord.x - 0.5) * 0.1;
	float len = length(coord - vec2(0.5, 0.25));

	color.r += sin(len * 40.0 + angle * 40.0 + u_time);
	color.g += cos(len * 30.0 + angle * 60.0 - u_time);
	color.b += sin(len * 50.0 + angle * 50.0 + 3.0);

	gl_FragColor = vec4(color, 1.0) * pixel;
}