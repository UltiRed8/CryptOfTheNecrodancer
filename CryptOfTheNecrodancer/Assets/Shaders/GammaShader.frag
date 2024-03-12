uniform sampler2D texture;

uniform float gamma;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	vec4 newColor = gl_Color * gamma;
	newColor.a = gl_Color.a;

    gl_FragColor = newColor * pixel;
}