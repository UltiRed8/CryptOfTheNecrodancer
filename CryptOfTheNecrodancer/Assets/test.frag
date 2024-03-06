uniform sampler2D texture;

uniform float colorR;
uniform float colorG;
uniform float colorB;

void main()
{
    vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	gl_Color.r = gl_Color.r * colorR;
	gl_Color.g = gl_Color.g * colorG;
	gl_Color.b = gl_Color.b * colorB;

    gl_FragColor = gl_Color * pixel;
}