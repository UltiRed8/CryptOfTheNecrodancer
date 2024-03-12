uniform sampler2D texture;

void main()
{
    //vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);
	
	//gl_Color.rgb = vec3(0.299 * (gl_Color.r * 255), 0.587 * (gl_Color.g * 255), 0.114 * (gl_Color.b * 255));

    //gl_FragColor = gl_Color * pixel;
	
	
	
	vec4 color = texture2D(texture, gl_TexCoord[0].xy);
	float avg = (color.r + color.g + color.b) / 3.0;
	if (color.r == 0.0 && color.g == 0.0 && color.b == 0.0)
	{
		color.a = 0.0;
	}
	gl_FragColor = vec4(avg, avg, avg, color.a);
}