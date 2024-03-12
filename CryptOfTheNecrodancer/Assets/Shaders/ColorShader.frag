uniform sampler2D texture;

uniform float u_time;

const float PI = 3.1415926535;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	if (gl_Color.r == 0 && gl_Color.g == 0 && gl_Color.b == 0)
	{
		gl_Color.a = 0;
	}
	
	gl_Color.r = gl_Color.r + cos(u_time * (1 * PI / 2.0));
	gl_Color.g = gl_Color.g + cos(u_time * (2 * PI / 2.0));
	gl_Color.b = gl_Color.b + cos(u_time * (3 * PI / 2.0));
	
	float alpha = gl_Color.a;
	
	gl_Color = gl_Color / 1.5;
	
	gl_Color.a = alpha;

    gl_FragColor = gl_Color * pixel;
}