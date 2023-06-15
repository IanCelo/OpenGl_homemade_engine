#version 450 core

struct Map
{
	sampler2D text;
	vec2 offset;
	vec2 tiling;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;

	float shininess;
	
	Map diffuseMap;
	Map normalMap;
	Map specularMap;
	Map emissiveMap;
	Map maskMap;
};

struct Light
{
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	vec3 attenuation;
	vec3 direction;

	float power;
	float cutOff;
	float outerCutOff;

	int lightType;
};

#define MAX_LIGHTS 8
uniform Light light_list[MAX_LIGHTS];

uniform int LightNumber;

uniform Material mat;

uniform vec4 ParticleColor;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos; 

uniform vec3 ViewPos;

uniform bool has_diffuseMap;
uniform bool has_normalMap;
uniform bool has_specularMap;
uniform bool has_emissiveMap;
uniform bool has_maskMap;

uniform bool is_affected_by_light;


out vec4 FragColor;

//	Get text coord
vec2 getTextCoord(in Map in_map)
{
	return TexCoord * in_map.tiling + in_map.offset;
}


//	Set PointLight
void SetPointLight(inout vec3	ambient, inout vec3 diffuse, inout vec3 specular, in int i, in vec3 ligthWorldPosition)
{
	float dist    = length(ligthWorldPosition - FragPos);
	float attenuation = 1.0 / (light_list[i].attenuation.x + light_list[i].attenuation.y * dist + light_list[i].attenuation.z * (dist * dist));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
}

//	Set SpotLight
void setSpotLight(inout vec3 diffuse, inout vec3 specular, in int i, in vec3 lightDir)
{
	float theta = dot(lightDir, normalize(-light_list[i].direction));
	float epsilon   = light_list[i].cutOff - light_list[i].outerCutOff;
 	float intensity = clamp((theta - light_list[i].outerCutOff) / epsilon, 0.0, 1.0);  

	diffuse *= intensity;
	specular *= intensity;
}

//	Return Ambient vec3
vec3 getAmbient( in float strength, in int i)
{
	return strength * light_list[i].ambient * mat.ambient;
}

//	Return Specular vec3
vec3 getSpecular(in vec3 lightDir, in vec3 norm, in int i)
{
	vec3	viewDir = normalize(ViewPos - FragPos);
	float	spec = pow(max(dot(viewDir, reflect(-lightDir, norm)), 0.0), mat.shininess);
	return	spec * light_list[i].specular * mat.specular;
}

//	Return Diffuse vec3
vec3 getDiffuse(in vec3 lightDir, in vec3 norm, in int i)
{
	float diff = max(dot(norm, lightDir), 0.0);
	return diff * light_list[i].diffuse * light_list[i].power;
}

//	------------------------------------------

void main()
{/*
	vec3 norm ;

	if(has_normalMap)
	{
		//	transform the normal coordinate range from [0,1] to [-1,1]
		//	----------------------------------------------------------

		norm = normalize(texture(mat.normalMap.text, TexCoord).rgb * 2.0 - 1.0);
	}
	else
	{
		norm = normalize(Normal);
	}

	vec3 result = {0,0,0};

	//	Light Loop
	if(is_affected_by_light)
	{
		for(int i = 0 ; i < LightNumber && i < MAX_LIGHTS; i++)
		{
			if(light_list[i].power <= 0) continue;

			vec3 ligthWorldPosition = light_list[i].position.xyz;

			vec3 lightDir;

		
			// ------------------------ Directional Light	------------------------ //

			if(light_list[i].lightType == 1)
			{
				lightDir = normalize(-light_list[i].direction);
			}
			else
			{
				lightDir = normalize(ligthWorldPosition - FragPos); 
			}



			//	Ambient
			vec3 ambient = getAmbient( 0.5,i);

			//	Diffuse
			vec3 diffuse =  getDiffuse(lightDir, norm, i);

			//	Specular
			vec3 specular = getSpecular(lightDir, norm, i);



			//	PointLight
			if(light_list[i].lightType == 0)
			{
				SetPointLight(ambient, diffuse, specular, i, ligthWorldPosition);
			}
			//	SpotLight
			else if(light_list[i].lightType == 2)
			{
				setSpotLight(diffuse, specular,i,lightDir);
			}

			//	Check if Material has specular textures
			//	---------------------------------------

			if(has_specularMap)
			{
				specular = (texture(mat.specularMap.text, getTextCoord(mat.specularMap)) * vec4(specular,1.0)).xyz;
			}

		
			result += ambient + diffuse + specular;
		}
	}
	else
	{
		result = vec3(1.0, 1.0, 1.0);
	}

	
	//	Check if Material has emissive textures
	//	---------------------------------------

	vec4 emissive;
	if(has_emissiveMap)
	{
		emissive = texture(mat.emissiveMap.text, getTextCoord(mat.emissiveMap)) + vec4(mat.emissive,1.0);
	}
	else
	{
		emissive = vec4(mat.emissive,1.0);;
	}

	
	//	Check if Material has diffuse textures
	//	--------------------------------------

	vec4 diffuse;
	if(has_diffuseMap)
	{
		diffuse = texture(mat.diffuseMap.text, getTextCoord(mat.diffuseMap)) * vec4(mat.diffuse,1.0);
	}
	else
	{
		diffuse = vec4(mat.diffuse,1.0);
	}

	FragColor = diffuse * (vec4(result,1.0) + emissive);
	
	if(has_maskMap)
	{
		FragColor.a =texture(mat.maskMap.text, getTextCoord(mat.maskMap)).r;
	}

	FragColor = FragColor * ParticleColor;*/
	//FragColor = vec4(texture(mat.diffuseMap, TexCoord) * ParticleColor);
	FragColor =  texture(mat.diffuseMap.text, TexCoord) * ParticleColor;
}