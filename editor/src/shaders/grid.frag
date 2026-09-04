#version 330

in vec3 fragPosition;
in vec2 fragTexCoord;
in vec4 fragColor;
in vec3 fragNormal;

out vec4 finalColor;

uniform vec3 camera_pos;

float max2(vec2 v)
{
    return max(v.x, v.y);        
}

vec2 satv(vec2 v)
{
    return vec2(clamp(v.x, 0.0, 1.0), clamp(v.y, 0.0, 1.0));        
}

float satf(float x)
{
    return clamp(x, 0.0, 1.0);
}

float log10(float x)
{
    float f = log(x) / log(10.0);
    return f;
}

void main()
{
    const float grid_size = 0.5;
    const vec4 thick_lines = vec4(0.0, 1.0, 1.0, 1.0);
    const vec4 thin_lines = vec4(0.0, 0.3, 0.3, 1.0);
    
    vec2 dvx = vec2(dFdx(fragPosition.x), dFdy(fragPosition.x));
    vec2 dvy = vec2(dFdx(fragPosition.z), dFdy(fragPosition.z));

    float lx = length(dvx);
    float ly = length(dvy);

    vec2 dudv = vec2(lx, ly);
    float l = length(dudv);

    float LOD = max(0.0, log10(l * 2 / grid_size) + 1.0);
    float cell_size_lod0 = grid_size * pow(10, floor(LOD));
    float cell_size_lod1 = cell_size_lod0 * 10.0;
    float cell_size_lod2 = cell_size_lod1 * 10.0;

    // More lines
    dudv *= 4.0;

    vec2 mod_div_dudv =  mod(fragPosition.xz, cell_size_lod0) / dudv;
    float lod0a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)));
    
    mod_div_dudv =  mod(fragPosition.xz, cell_size_lod1) / dudv;
    float lod1a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)));
    
    mod_div_dudv =  mod(fragPosition.xz, cell_size_lod2) / dudv;
    float lod2a = max2(vec2(1.0) - abs(satv(mod_div_dudv) * 2.0 - vec2(1.0)));

    float LOD_fade = fract(LOD);

    vec4 color;
    if (lod2a > 0.0)
    {
        color = thick_lines;
        color.a *= lod2a;
    }
    else if (lod1a > 0.0)
    {
        color = mix(thick_lines, thin_lines, LOD_fade);
        color.a *= lod1a;
    }
    else
    {
        color = thin_lines;
        color.a *= lod0a * (1.0 - LOD_fade);
    }

    float falloff = (1.0 - satf(0.0006 * length(fragPosition.xz - camera_pos.xz) / grid_size));
    color.a *= falloff;
        
    finalColor = color;
    finalColor.rgb = pow(finalColor.rgb, vec3(1.0/2.2));
}
