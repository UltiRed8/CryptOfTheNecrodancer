uniform sampler2D texture;

uniform float u_time;

const float PI = 3.1415926535;

void main()
{
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

    vec2 coord = 6.0 * gl_TexCoord[0].xy;

	if (pixel.r == 0 && pixel.g == 0 && pixel.b == 0)
	{
		return;
	}

	for (int n = 1; n < 8; n++)
	{
		float i = float(n);
		coord += vec2(0.7 / i * sin(i * coord.y + u_time + 0.3 * i) + 0.8, 0.4 / i * sin(coord.x + u_time + 0.3) + 1.6);
	}

	vec3 color = vec3(0.5 * sin(coord.x) + 0.5, 0.5 * sin(coord.y) + 0.5, sin(coord.x + coord.y));

    gl_FragColor = vec4(color, 1.0) * pixel;
}