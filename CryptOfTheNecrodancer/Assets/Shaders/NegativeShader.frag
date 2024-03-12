uniform sampler2D texture;

void main()
{
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
	
	vec3 newColor = vec3(1.0, 1.0, 1.0);
	
	newColor.r = 1.0 - color.r;
	newColor.g = 1.0 - color.g;
	newColor.b = 1.0 - color.b;
	
	if (color.r == 0.0 && color.g == 0.0 && color.b == 0.0)
	{
		color.a = 0.0;
	}
	
	gl_FragColor = vec4(newColor, color.a);
}