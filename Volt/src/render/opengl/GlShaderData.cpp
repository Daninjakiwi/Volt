#pragma once

namespace volt {
	const char* vert_background = R"(
		#version 330 core
		layout (location = 0) in vec3 aPos;

		uniform mat4 projection;
		uniform mat4 view;

		out vec3 WorldPos;

		void main() {
			WorldPos = aPos;

			mat4 rotView = mat4(mat3(view));
			vec4 clipPos = projection * rotView * vec4(WorldPos, 1.0);

			gl_Position = clipPos.xyww;
		}
)";

	const char* frag_background = R"(
		#version 330 core
		out vec4 FragColor;
		in vec3 WorldPos;

		uniform samplerCube environmentMap;

		void main()
		{		
			vec3 envColor = textureLod(environmentMap, WorldPos, 0.0).rgb;
    
			// HDR tonemap and gamma correct
			envColor = envColor / (envColor + vec3(1.0));
			envColor = pow(envColor, vec3(1.0/2.2)); 
    
			FragColor = vec4(envColor, 1.0);
		}
)";

	const char* vert_2d = R"(
		#version 330 core

		layout (location=0) in vec2 position;
		layout (location=1) in vec2 tex_coord;
		layout (location=2) in vec4 colour;
		layout (location=3) in float tex_index;

		out vec2 v_tex_coord;
		out vec4 v_colour;
		out float v_tex_index;

		uniform mat4 u_projection;

		void main() {
			v_tex_coord = tex_coord;
			v_colour = colour;
			v_tex_index = tex_index;

			gl_Position = u_projection * vec4(position, 0.0,1.0);
		}
	)";

	const char* frag_2d = R"(
		#version 330 core
		
		layout (location=0) out vec4 col;
		
		in vec2 v_tex_coord;
		in vec4 v_colour;
		in float v_tex_index;

		uniform sampler2D u_textures[16];

		void main() {
			int index = int(v_tex_index);
			
			if (index > 0) {
				vec4 tex = texture(u_textures[index-1],v_tex_coord);
				float alpha = tex.a;
				if (v_colour.a > 0) {
					col = vec4(v_colour.r,v_colour.g,v_colour.b,v_colour.a * alpha);
				} else {
					col = tex;
				}

				
			} else {
				col = v_colour;
			}
		}		
	)";


	const char* vert_3d = R"(
		#version 330 core

		layout (location=0) in vec3 position;
		layout (location=1) in vec3 normal;
		layout (location=2) in vec2 tex_coord;

		out vec3 v_normal;
		out vec2 v_tex_coord;

		uniform mat4 u_model;
		uniform mat4 u_view;
		uniform mat4 u_projection;

		void main() {
			v_normal = normal;
			v_tex_coord = tex_coord;

			gl_Position = u_projection * u_view * u_model * vec4(position,1.0);
		}

	)";

	const char* frag_3d = R"(
		#version 330 core
		
		layout (location=0) out vec4 colour;

		in vec3 v_normal;
		in vec2 v_tex_coord;

		uniform sampler2D u_albedo;

		void main() {
			
			colour = texture(u_albedo, v_tex_coord);
		}		
	)";




	const char* vert_cubemap = R"(
#version 330 core
layout (location=0) in vec3 position;

out vec3 v_world_pos;

uniform mat4 u_projection;
uniform mat4 u_view;

void main() {
	v_world_pos = position;

	gl_Position = u_projection * u_view * vec4(position, 1.0);
}
)";

	const char* frag_equi_to_cubemap = R"(
#version 330 core
layout (location=0) out vec4 colour;

in vec3 v_world_pos;

uniform sampler2D u_equi_map;

const vec2 inverse_pi = vec2(0.1591,0.3183);

vec2 sampleMap(vec3 pos) {
	vec2 uv = vec2(atan(pos.z, pos.x), asin(pos.y));

	uv *= inverse_pi;
	uv += 0.5;
	return uv;
}

void main() {
	vec2 uv = sampleMap(normalize(v_world_pos));

	vec3 t = texture(u_equi_map,uv).rgb;

	colour = vec4(t,1.0);
}
)";

	const char* frag_irradiance_convolute = R"(
#version 330 core
layout (location=0) out vec4 colour;

in vec3 v_world_pos;

uniform samplerCube u_env_map;

const float PI = 3.14159265359;

void main() {
	vec3 N = normalize(v_world_pos);

	vec3 irradiance = vec3(0.0);
	
	vec3 up = vec3(0.0,1.0,0.0);
	vec3 right = cross(up,N);
	up = cross(N,right);

	float sample_delta = 0.025;
	float num_samples = 0.0f;

	for (float phi = 0.0; phi < 2.0 * PI; phi += sample_delta) {
		for (float theta = 0.0;theta < 0.5 * PI;theta += sample_delta) {
			vec3 tangent_sample = vec3(sin(theta) * cos(phi),  sin(theta) * sin(phi), cos(theta));
			vec3 sample = tangent_sample.x * right + tangent_sample.y * up + tangent_sample.z * N;

			irradiance += texture(u_env_map, sample).rgb * cos(theta) * sin(theta);
			num_samples++;
		}
	}
	irradiance = PI * irradiance * (1.0 / num_samples);

	colour = vec4(irradiance, 1.0);
}
)";

	const char* frag_prefilter = R"(
#version 330 core
layout (location=0) out vec4 colour;

in vec3 v_world_pos;

uniform samplerCube u_env_map;
uniform float u_roughness;

const float PI = 3.14159265359;

float ggxDistribution(vec3 N, vec3 H, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N,H),0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	
	return nom / denom;
}

float radicalInverse(uint bits) {
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);

	return float(bits) * 2.3283064365386963e-10;
}

vec2 hammersley(uint i, uint N) {
	return vec2(float(i)/float(N),radicalInverse(i));
}

vec3 ggxSample(vec2 Xi, vec3 N, float roughness) {
	float a = roughness*roughness;
	
	float phi = 2.0 * PI * Xi.x;
	float cos_theta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
	
	vec3 H;
	H.x = cos(phi) * sin_theta;
	H.y = sin(phi) * sin_theta;
	H.z = cos_theta;
	
	vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);
	
	vec3 sample = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sample);
}

void main() {
	vec3 N = normalize(v_world_pos);
	
	vec3 R = N;
	vec3 V = R;

	const uint SAMPLE_COUNT = 1024u;
	vec3 prefilter_colour = vec3(0.0);
	float total = 0.1;

	for(uint i = 0u; i < SAMPLE_COUNT;i++) {

        vec2 Xi = hammersley(i, SAMPLE_COUNT);
        vec3 H = ggxSample(Xi, N, u_roughness);
        vec3 L  = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(dot(N, L), 0.0);
        if(NdotL > 0.0) {
            float D   = ggxDistribution(N, H, u_roughness);
            float NdotH = max(dot(N, H), 0.0);
            float HdotV = max(dot(H, V), 0.0);
            float pdf = D * NdotH / (4.0 * HdotV) + 0.0001; 

            float resolution = 512.0;
            float texel  = 4.0 * PI / (6.0 * resolution * resolution);
            float sample = 1.0 / (float(SAMPLE_COUNT) * pdf + 0.0001);

            float mip_level = u_roughness == 0.0 ? 0.0 : 0.5 * log2(sample / texel); 
            
            prefilter_colour += textureLod(u_env_map, L, mip_level).rgb * NdotL;
            total += NdotL;
        }
    }

	prefilter_colour = prefilter_colour / total;
	
	colour = vec4(prefilter_colour,1.0);
}
)";

	const char* vert_brdf = R"(
#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec2 tex_coord;

out vec2 v_tex_coord;

void main() {
	v_tex_coord = tex_coord;
	
	gl_Position = vec4(position, 1.0);
}
)";

	const char* frag_brdf = R"(
#version 330 core

layout (location=0) out vec2 colour;

in vec2 v_tex_coord;

const float PI = 3.14159265359;

float radicalInverse(uint bits) {
	bits = (bits << 16u) | (bits >> 16u);
	bits = ((bits & 0x55555555u) << 1u) | ((bits & 0xAAAAAAAAu) >> 1u);
	bits = ((bits & 0x33333333u) << 2u) | ((bits & 0xCCCCCCCCu) >> 2u);
	bits = ((bits & 0x0F0F0F0Fu) << 4u) | ((bits & 0xF0F0F0F0u) >> 4u);
	bits = ((bits & 0x00FF00FFu) << 8u) | ((bits & 0xFF00FF00u) >> 8u);

	return float(bits) * 2.3283064365386963e-10;
}

vec2 hammersley(uint i, uint N) {
	return vec2(float(i)/float(N),radicalInverse(i));
}

vec3 ggxSample(vec2 Xi, vec3 N, float roughness) {
	float a = roughness*roughness;
	
	float phi = 2.0 * PI * Xi.x;
	float cos_theta = sqrt((1.0 - Xi.y) / (1.0 + (a*a - 1.0) * Xi.y));
	float sin_theta = sqrt(1.0 - cos_theta*cos_theta);
	
	vec3 H;
	H.x = cos(phi) * sin_theta;
	H.y = sin(phi) * sin_theta;
	H.z = cos_theta;
	
	vec3 up          = abs(N.z) < 0.999 ? vec3(0.0, 0.0, 1.0) : vec3(1.0, 0.0, 0.0);
	vec3 tangent   = normalize(cross(up, N));
	vec3 bitangent = cross(N, tangent);
	
	vec3 sample = tangent * H.x + bitangent * H.y + N * H.z;
	return normalize(sample);
}

float ggxGeometrySchlick(float NdotV, float roughness) {
	float a = roughness;
	float k = (a * a) / 2.0;
	
	float nom = NdotV;
	float denom = NdotV * (1.0 - k) + k;

	return nom / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NdotV = max(dot(N,V), 0.0);
	float NdotL = max(dot(N,L),0.0);
	
	float ggx2 = ggxGeometrySchlick(NdotV, roughness);
	float ggx1 = ggxGeometrySchlick(NdotL,roughness);

	return ggx1 * ggx2;
}

vec2 brdfIntegrate(float NdotV, float roughness) {
	vec3 V;
	V.x = sqrt(1.0 - NdotV*NdotV);
    V.y = 0.0;
    V.z = NdotV;

    float A = 0.0;
    float B = 0.0; 

    vec3 N = vec3(0.0, 0.0, 1.0);
    
    const uint SAMPLE_COUNT = 1024u;
    for(uint i = 0u; i < SAMPLE_COUNT;i++)
    {
        vec2 Xi = hammersley(i, SAMPLE_COUNT);
        vec3 H = ggxSample(Xi, N, roughness);
        vec3 L = normalize(2.0 * dot(V, H) * H - V);

        float NdotL = max(L.z, 0.0);
        float NdotH = max(H.z, 0.0);
        float VdotH = max(dot(V, H), 0.0);

        if(NdotL > 0.0)
        {
            float G = geometrySmith(N, V, L, roughness);
            float G_Vis = (G * VdotH) / (NdotH * NdotV);
            float Fc = pow(1.0 - VdotH, 5.0);

            A += (1.0 - Fc) * G_Vis;
            B += Fc * G_Vis;
        }
    }
    A /= float(SAMPLE_COUNT);
    B /= float(SAMPLE_COUNT);
    return vec2(A, B);
}

void main() {
	colour = brdfIntegrate(v_tex_coord.x, v_tex_coord.y);
}
)";

	const char* vert_pbr = R"(
#version 330 core

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 tex_coord;

out vec3 v_world_pos;
out vec3 v_normal;
out vec2 v_tex_coord;

uniform mat4 u_projection;
uniform mat4 u_view;
uniform mat4 u_model;

void main() {
	v_world_pos = vec3(u_model * vec4(position,1.0));
	v_normal = mat3(u_model) * normal;
	v_tex_coord = tex_coord;

	gl_Position = u_projection * u_view * vec4(v_world_pos,1.0);
}
)";

	const char* frag_pbr = R"(
#version 330 core

layout (location=0) out vec4 colour;

in vec3 v_world_pos;
in vec3 v_normal;
in vec2 v_tex_coord;

uniform sampler2D u_albedo;
uniform sampler2D u_normal;
uniform sampler2D u_metallic;
uniform sampler2D u_roughness;
uniform sampler2D u_ao;

uniform samplerCube u_irradiance;
uniform samplerCube u_prefilter;
uniform sampler2D u_brdf;

const int max_lights = 8;

uniform vec3 u_light_positions[max_lights];
uniform vec3 u_light_colours[max_lights];

uniform vec3 u_cam_pos;

const float PI = 3.14159265359;

vec3 getNormalFromMap() {
    vec3 tangent_normal = texture(u_normal, v_tex_coord).xyz * 2.0 - 1.0;

    vec3 Q1  = dFdx(v_world_pos);
    vec3 Q2  = dFdy(v_world_pos);
    vec2 st1 = dFdx(v_tex_coord);
    vec2 st2 = dFdy(v_tex_coord);

    vec3 N   = normalize(v_normal);
    vec3 T  = normalize(Q1*st2.t - Q2*st1.t);
    vec3 B  = -normalize(cross(N, T));
    mat3 TBN = mat3(T, B, N);

    return normalize(TBN * tangent_normal);
}

float ggxDistribution(vec3 N, vec3 H, float roughness) {
	float a = roughness * roughness;
	float a2 = a * a;
	float NdotH = max(dot(N,H),0.0);
	float NdotH2 = NdotH * NdotH;

	float nom = a2;
	float denom = (NdotH2 * (a2 - 1.0) + 1.0);
	denom = PI * denom * denom;
	
	return nom / denom;
}

float ggxGeometrySchlick(float NdotV, float roughness) {
	float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}

float geometrySmith(vec3 N, vec3 V, vec3 L, float roughness) {
	float NdotV = max(dot(N,V), 0.0);
	float NdotL = max(dot(N,L),0.0);
	
	float ggx2 = ggxGeometrySchlick(NdotV, roughness);
	float ggx1 = ggxGeometrySchlick(NdotL,roughness);

	return ggx1 * ggx2;
}

vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlickRoughness(float cosTheta, vec3 F0, float roughness)
{
    return F0 + (max(vec3(1.0 - roughness), F0) - F0) * pow(max(1.0 - cosTheta, 0.0), 5.0);
}

void main() {
	vec3 albedo = pow(texture(u_albedo, v_tex_coord).rgb, vec3(2.2));
    float metallic = texture(u_metallic, v_tex_coord).r;
    float roughness = texture(u_roughness, v_tex_coord).r;
    float ao = texture(u_ao, v_tex_coord).r;

    vec3 N = getNormalFromMap();
    vec3 V = normalize(u_cam_pos - v_world_pos);
    vec3 R = reflect(-V, N); 

    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

	vec3 Lo = vec3(0.0);
    for(int i = 0; i < max_lights; ++i) 
    {
        vec3 L = normalize(u_light_positions[i] - v_world_pos);
        vec3 H = normalize(V + L);
        float distance = length(u_light_positions[i] - v_world_pos);
        float attenuation = 1.0 / (distance * distance);
        vec3 radiance = u_light_colours[i] * attenuation;

        float NDF = ggxDistribution(N, H, roughness);   
        float G   = geometrySmith(N, V, L, roughness);    
        vec3 F    = fresnelSchlick(max(dot(H, V), 0.0), F0);        
        
        vec3 nominator    = NDF * G * F;
        float denominator = 4 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.001;
        vec3 specular = nominator / denominator;
        
        vec3 kS = F;
        vec3 kD = vec3(1.0) - kS;
        kD *= 1.0 - metallic;	                
            
        float NdotL = max(dot(N, L), 0.0);        

        Lo += (kD * albedo / PI + specular) * radiance * NdotL;
    }
	vec3 F = fresnelSchlickRoughness(max(dot(N, V), 0.0), F0, roughness);
    
	vec3 kS = F;
	vec3 kD = 1.0 - kS;
	kD *= 1.0 - metallic;	  
    
	vec3 irradiance = texture(u_irradiance, N).rgb;
	vec3 diffuse      = irradiance * albedo;

	const float MAX_REFLECTION_LOD = 4.0;
    vec3 prefilter_colour = textureLod(u_prefilter, R,  roughness * MAX_REFLECTION_LOD).rgb;    
    vec2 brdf  = texture(u_brdf, vec2(max(dot(N, V), 0.0), roughness)).rg;
    vec3 specular = prefilter_colour * (F * brdf.x + brdf.y);

    vec3 ambient = (kD * diffuse + specular) * ao;
    
    vec3 col = ambient + Lo;

    // HDR tonemapping
    col = col / (col + vec3(1.0));
    // gamma correct
    col = pow(col, vec3(1.0/2.2));
	
	float alpha = texture(u_albedo, v_tex_coord).a;

	colour = vec4(col, alpha);
}

)";

	const char* vert_test = R"(
#version 330 core

layout (location=0) in vec3 position;

uniform mat4 u_projection;
uniform mat4 u_view;

out vec3 v_world_pos;

void main() {
	v_world_pos = position;

	mat4 rotView = mat4(mat3(u_view));
	vec4 clipPos = u_projection * rotView * vec4(v_world_pos,1.0);

	gl_Position = clipPos.xyww;
}
)";

	const char* frag_test = R"(
#version 330 core

layout (location=0) out vec4 colour;

in vec3 v_world_pos;

uniform samplerCube u_env_map;

void main() {
	vec3 envColour = textureLod(u_env_map, v_world_pos,0.0).rgb;

	envColour = envColour / (envColour + vec3(1.0));
	envColour = pow(envColour,vec3(1.0/2.2));

	colour = vec4(envColour, 1.0);
}
)";


}
