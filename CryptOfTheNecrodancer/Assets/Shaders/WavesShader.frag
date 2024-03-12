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
	
	vec2 coord = gl_FragCoord.xy / 1000.0;
	float color = 0.0;

	color += sin(coord.x * 6.0 + sin(u_time + coord.y * 90.0 + cos(coord.x * 30.0 + u_time * 2.0))) * 0.5;

	gl_FragColor = vec4(vec3(color + coord.x, color + coord.x, color), 1.0) * pixel;
}