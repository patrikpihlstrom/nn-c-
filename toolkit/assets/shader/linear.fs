#version 330 core

uniform sampler2D texture;

out vec4 FragmentColor;

uniform float offset[5] = float[](0.0, 1.0, 2.0, 3.0, 4.0);
uniform float weight[5] = float[](0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);

void main(void)
{
	FragmentColor = texture2D(texture, vec2(gl_FragCoord.x/1600, gl_FragCoord.y/900))*weight[0];

	for (int i = 1; i < 5; i++)
	{
		FragmentColor += texture2D(texture, (vec2(gl_FragCoord.x/1600, gl_FragCoord.y/900)+vec2(offset[i]/1600, 0.0)))*weight[i];
		FragmentColor += texture2D(texture, (vec2(gl_FragCoord.x/1600, gl_FragCoord.y/900)-vec2(offset[i]/1600, 0.0)))*weight[i];
	}
}
