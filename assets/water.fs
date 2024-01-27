
#version 330

in vec2 fragTexCoord;
in vec4 fragColor;

out vec4 finalColor;

uniform sampler2D texture0;
uniform vec4 colDiffuse;
uniform float time;

const vec4 uWaveStrengthX = vec4(4.15, 4.66, 0.0016, 0.0015);
const vec4 uWaveStrengthY = vec4(2.54, 6.33, 0.00102, 0.0025);

vec2 dist(vec2 uv) { 
    float uTime = time * 0.005;
    float noise = texture(texture0, uv + uTime).r;

    uv.y += (cos((uv.y + uTime * uWaveStrengthY.y + uWaveStrengthY.x * noise)) * uWaveStrengthY.z) +
        (cos((uv.y + uTime) * 10.0) * uWaveStrengthY.w);

    uv.x += (sin((uv.y + uTime * uWaveStrengthX.y + uWaveStrengthX.x * noise)) * uWaveStrengthX.z) +
        (sin((uv.y + uTime) * 15.0) * uWaveStrengthX.w);

    return uv;
}

void main()
{
    vec4 color = texture(texture0, dist(fragTexCoord));
    finalColor = color;
}