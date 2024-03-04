uniform sampler2D texture;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	gl_Color.r = gl_Color.r * 0.4;
	gl_Color.g = gl_Color.g * 0.4;
	gl_Color.b = gl_Color.b * 0.8;

    gl_FragColor = gl_Color * pixel;
}