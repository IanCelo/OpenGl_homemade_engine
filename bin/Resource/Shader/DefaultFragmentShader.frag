#version 450 core

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

struct Map
{
	bool exist;
	sampler2D text;
	vec3 offset;
	vec3 tiling;
};

struct Bump
{
	Map   map;
	float multiplier;
};

struct Material
{
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	vec3 emissive;

	float shininess;
	
	Map diffuseMap;
	Map specularMap;
	Map emissiveMap;
	Map maskMap;
	Bump normalMap;
};

struct Light
{
	bool enabled;

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

#define MAX_LIGHTS 64
uniform Light light_list[MAX_LIGHTS];

uniform int LightNumber;
uniform Material mat;
uniform vec3 ViewPos;
uniform bool showNormal;

in vec3 Normal;
in vec2 TexCoord;
in vec3 FragPos;


/*----------------------------------------------------------------------------------------*/

//	Get text coord
//	--------------

vec2 getTextCoord(in vec2 texCoord, in Map in_map)
{
	return texCoord * in_map.tiling.xy + in_map.offset.xy;
}

/*----------------------------------------------------------------------------------------*/

//	Set PointLight
//	--------------

void SetPointLight(inout vec3	ambient, inout vec3 diffuse, inout vec3 specular, in int i, in vec3 ligthWorldPosition)
{
	float dist    = length(ligthWorldPosition -FragPos);
	float attenuation = 1.0 / (light_list[i].attenuation.x + light_list[i].attenuation.y * dist + light_list[i].attenuation.z * (dist * dist));
	
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;
}

/*----------------------------------------------------------------------------------------*/

//	Set SpotLight
//	-------------

void setSpotLight(inout vec3 diffuse, inout vec3 specular, in int i, in vec3 lightDir)
{
	float theta = dot(lightDir, normalize(-light_list[i].direction));
	float epsilon   = light_list[i].cutOff - light_list[i].outerCutOff;
 	float intensity = clamp((theta - light_list[i].outerCutOff) / epsilon, 0.0, 1.0);  

	diffuse *= intensity;
	specular *= intensity;
}

/*----------------------------------------------------------------------------------------*/

//	Return Ambient vec3
//	-------------------
vec3 getAmbient( in float strength, in int i)
{
	return strength * light_list[i].ambient * mat.ambient;
}

/*----------------------------------------------------------------------------------------*/

//	Return Specular vec3
//	--------------------
vec3 getSpecular(in vec3 lightDir, in vec3 norm, in vec3 viewDir, in int i)
{
	vec3 color = light_list[i].specular * mat.specular;
	if(mat.specularMap.exist)
	{
		color *= texture(mat.specularMap.text, getTextCoord(TexCoord, mat.specularMap)).rgb;
	}

    vec3 halfwayDir = normalize(lightDir + viewDir);
	float	spec = pow(max(dot(norm, halfwayDir), 0.0), mat.shininess);
	return	spec * color;
}

/*----------------------------------------------------------------------------------------*/

//	Return Diffuse vec3
//	-------------------
vec3 getDiffuse(in vec3 lightDir, in vec3 norm, in int i)
{
	float diff = max(dot(lightDir,norm), 0.1);
	return diff * light_list[i].diffuse * light_list[i].power;
}

/*----------------------------------------------------------------------------------------*/

//	Loop on lights and apply them
//	-----------------------------
vec3 getLightChanges(in vec2 texCoord, in vec3 norm, in vec3 color)
{
	vec3 result = {0.0,0.0,0.0};
	
	vec3 viewDir = normalize(ViewPos - FragPos);

	//	Light Loop
	for(int i = 0 ; i < LightNumber && i < MAX_LIGHTS; i++)
	{
		if (!light_list[i].enabled)
			continue;

		vec3 ligthWorldPosition = light_list[i].position;

		vec3 lightDir;

		//	Directionnal Light
		if(light_list[i].lightType == 1)
		{
			lightDir = normalize(ligthWorldPosition);
		}
		else
		{
			lightDir = normalize(ligthWorldPosition -FragPos); 
		}

		
		
		vec3 ambient  = getAmbient( 1.0,i);
		vec3 diffuse  = getDiffuse(lightDir, norm, i);
		vec3 specular = getSpecular(lightDir, norm, viewDir, i);
		
		//	Point Light
		if(light_list[i].lightType == 0)
		{
			SetPointLight(ambient, diffuse, specular, i, ligthWorldPosition);
		}
		//	Spot Light
		else if(light_list[i].lightType == 2)
		{
			setSpotLight(diffuse, specular,i,lightDir);
		}

		//	Check if Material has specular textures
		//	---------------------------------------

		result += ambient + diffuse + specular;
	}

	return result * color;
}

/*----------------------------------------------------------------------------------------*/

void main()
{	
	vec3 norm = Normal;
	
	//	Check if Material has diffuse textures
	//	--------------------------------------

	vec3 color = mat.diffuse;
	if(mat.diffuseMap.exist)
	{
		color = texture(mat.diffuseMap.text, getTextCoord(TexCoord, mat.diffuseMap)).rgb * mat.diffuse;
	}


	vec3 result =  getLightChanges(TexCoord, norm, color);
	
	//	Check if Material has emissive textures
	//	---------------------------------------

	vec4 emissive = vec4(mat.emissive,1.0);
	if(mat.emissiveMap.exist)
	{
		emissive = texture(mat.emissiveMap.text, getTextCoord(TexCoord, mat.emissiveMap)) + vec4(mat.emissive,1.0);
	}

	if (showNormal)
	{
		FragColor = vec4(norm, 1.0);
	}
	else
	{
	    FragColor = vec4(result,1.0) + emissive;
	}
	
	if(mat.maskMap.exist)
	{
		FragColor.a = texture(mat.maskMap.text, getTextCoord(TexCoord, mat.maskMap)).r;
	}

	
    //	Check whether fragment output is higher than threshold,
	//	If so output as brightness color
	//	--------------------------------

    float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));

    if(brightness > 1.0)
	{
        BrightColor = vec4(FragColor.rgb, 1.0);
	}
    else
	{
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}