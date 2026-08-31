#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

out vec4 finalColor;

void main()
{
    finalColor = vec4(0.0, 1.0, 1.0, 1.0);
    finalColor.rgb = pow(finalColor.rgb, vec3(1.0/2.2));
}
