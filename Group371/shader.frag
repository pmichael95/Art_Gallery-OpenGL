#version 330 core

struct Material {
	vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};  

struct Light {
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

	float constant;
    float linear;
    float quadratic;
};


in vec4 frag_position;
in vec3 frag_normal;
in vec2 frag_textureUV;

flat in ivec2 frag_textureMeta;
flat in ivec4 frag_textureID;

// Material in
in vec3  frag_ambient;
in vec3  frag_diffuse;
in vec3  frag_specular;
in float frag_shininess;

// Normal maps in
in mat3 TBN;




uniform vec4 camera_position;

#define POINT_LIGHT_COUNT 10
uniform Light lights[POINT_LIGHT_COUNT];


out vec4 color;

// Texture samplers
uniform sampler2D floor_d;
uniform sampler2D floor_n;
uniform sampler2D floor_s;
uniform sampler2D floor_h;

uniform sampler2D wall_d;
uniform sampler2D wall_n;
uniform sampler2D wall_s;
uniform sampler2D wall_h;

uniform sampler2D frame_d;
uniform sampler2D frame_n;
uniform sampler2D frame_s;
uniform sampler2D frame_h;


vec2 ParallaxMapping(vec2 texCoords, vec3 viewDir, sampler2D depthMap)
{ 
     // number of depth layers
    const float minLayers = 10;
	const float maxLayers = 20;
	float numLayers = mix(maxLayers, minLayers, abs(dot(vec3(0.0, 0.0, 1.0), viewDir)));  
    // calculate the size of each layer
    float layerDepth = 1.0 / numLayers;
    // depth of current layer
    float currentLayerDepth = 0.0;
    // the amount to shift the texture coordinates per layer (from vector P)
	// This is the height scale
    vec2 P = viewDir.xy * 0.125; 
    vec2 deltaTexCoords = P / numLayers;
	vec2  currentTexCoords     = texCoords;
	float currentDepthMapValue = texture(depthMap, currentTexCoords).r;
  
	while(currentLayerDepth < currentDepthMapValue)
	{
		// shift texture coordinates along direction of P
		currentTexCoords -= deltaTexCoords;
		// get depthmap value at current texture coordinates
		currentDepthMapValue = texture(depthMap, currentTexCoords).r;  
		// get depth of next layer
		currentLayerDepth += layerDepth;  
	}
	 //-- parallax occlusion mapping interpolation from here on
    // get texture coordinates before collision (reverse operations)
    vec2 prevTexCoords = currentTexCoords + deltaTexCoords;

    // get depth after and before collision for linear interpolation
    float afterDepth  = currentDepthMapValue - currentLayerDepth;
    float beforeDepth = texture(depthMap, prevTexCoords).r - currentLayerDepth + layerDepth;
 
    // interpolation of texture coordinates
    float weight = afterDepth / (afterDepth - beforeDepth);
    vec2 finalTexCoords = prevTexCoords * weight + currentTexCoords * (1.0 - weight);
	return currentTexCoords;
  
}

void main()
{

	Material m;
	
	// Lighting calcs
	vec3 pos3 = vec3(frag_position);
	vec3 camera_pos3 = vec3(camera_position);
	vec3 normal;
	// Convert to tangent space
	camera_pos3 = TBN * camera_pos3;
	pos3  = TBN * pos3;
	vec3 viewDir = normalize(camera_pos3 - pos3);
	vec2 uv = frag_textureUV;
	

	if (frag_textureMeta.x == 0) {
		// Use only the material
		m.ambient = frag_ambient;
		m.diffuse = frag_diffuse;
		m.specular = frag_specular;
		m.shininess = frag_shininess;
		normal = frag_normal;
	} else {

		// Use the texture
		if (frag_textureID.x == 1) {
			uv = ParallaxMapping(uv,  viewDir, floor_h);
			m.ambient = vec3(texture2D(floor_d, uv));
			m.diffuse = vec3(texture2D(floor_d, uv));
			m.specular = vec3(texture2D(floor_s, uv));
			m.shininess = 256.0f;
			normal = texture(floor_n, uv).rgb;
		} if (frag_textureID.x == 5) {
			uv = ParallaxMapping(uv,  viewDir, wall_h);
			m.ambient = vec3(texture2D(wall_d, uv));
			m.diffuse = vec3(texture2D(wall_d, uv));
			m.specular = vec3(texture2D(wall_s, uv));
			m.shininess = 200.0f;
			normal = texture(wall_n, uv).rgb;
		}

		if (frag_textureID.x == 9) {
			uv = ParallaxMapping(uv,  viewDir, frame_h);
			m.ambient = vec3(texture2D(frame_d, uv));
			m.diffuse = vec3(texture2D(frame_d, uv));
			m.specular = vec3(texture2D(frame_s, uv));
			m.shininess = 200.0f;
			normal = texture(wall_n, uv).rgb;
		}
		// mix
		if (frag_textureMeta.x == 2) {
			float mixValue = frag_textureMeta.y / 100.0f;
			m.ambient = mix(m.ambient, frag_ambient, mixValue);
			m.diffuse = mix(m.diffuse, frag_diffuse, mixValue);
			m.specular = mix (m.specular, frag_specular, mixValue);
			// Average shiniess
			m.shininess = (m.shininess + frag_shininess) / 2;
		}
		// Currently in range of [0,1]
		// Now convert to range [-1,1]
		normal = normalize(normal * 2.0 - 1.0);
		
		
	}

	vec4 result;
	for(int i = 0; i < POINT_LIGHT_COUNT; i++) {
	
		Light light = lights[i];
		light.position = TBN * light.position ;
		// If there is no light here
		if (light.constant >= 1.0) {
		
	
			// Ambient
			vec3 ambient = light.ambient * m.ambient;
  	
			// Diffuse 
			//normal = normalize(normal);
			vec3 lightDir = normalize(light.position - pos3);
			float diff = max(dot(normal, lightDir), 0.0);
			vec3 diffuse = light.diffuse * diff * m.diffuse;
    
			// Specular
			vec3 reflectDir = reflect(-lightDir, normal);  
			vec3 halfwayDir = normalize(lightDir + viewDir); 
			float spec = pow(max(dot(normal, halfwayDir), 0.0), m.shininess);
			vec3 specular = light.specular * spec * m.specular; 
	
			// Attenuation
			float distance    = length(light.position - pos3);
			float attenuation = 1.0f / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

			ambient  *= attenuation;  
			diffuse  *= attenuation;
			specular *= attenuation;  
        
			result += vec4(ambient + diffuse + specular, 1.0f);
		}
	}
	color = result;

	

	
} 
