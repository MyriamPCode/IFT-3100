//Shader pour plusieurs points d'éclairage

#version 330

out vec4 FragColor;

// attributs interpolés à partir des valeurs en sortie du shader de sommets
in vec3 surface_position;
in vec3 surface_normal;

// couleurs de réflexion du matériau
uniform vec3 color_ambient;
uniform vec3 color_diffuse;
uniform vec3 color_specular;

// facteur de brillance spéculaire du matériau
uniform float brightness;

uniform float ambientActive;
uniform float directionnalActive;
uniform float pointActive;
uniform float spotActive;

struct AmbientLight {
  vec3 color;
  float strength;
};

struct DirectionnalLight {
  vec3 direction;
  
  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

struct PointLight {    
    vec3 position;
    
    float constant;
    float linear;
    float quadratic;  

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct SpotLight {
  vec3 position;
  vec3 direction;
  float cutoff;

  float constant;
  float linear;
  float quadratic;

  vec3 ambient;
  vec3 diffuse;
  vec3 specular;
};

vec3 CalcDirectionnalLight(DirectionnalLight light, vec3 surface_normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 surface_normal, vec3 surface_position, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 surface_normal, vec3 surface_position, vec3 viewDir);
vec3 CalcAmbientLight(AmbientLight light);

uniform AmbientLight ambLight;
uniform DirectionnalLight dirLight;
uniform PointLight pointLight;
uniform SpotLight spotLight;

void main()
{
  vec3 result = vec3(0.0);
  vec3 norm = normalize(surface_normal);
  vec3 viewDir = normalize(-surface_position);
  if (ambientActive == 1.0f){
    result += CalcAmbientLight(ambLight);
  }
  if (directionnalActive == 1.0f){
    result += CalcDirectionnalLight(dirLight, norm, viewDir);
  }
  if (pointActive == 1.0f){
    result += CalcPointLight(pointLight, norm, surface_position, viewDir);
  }
  if (spotActive == 1.0f){
    result += CalcSpotLight(spotLight, norm, surface_position, viewDir);
  }
  //result += CalcAreaLight();
  
  FragColor = vec4(result, 1.0);
}

vec3 CalcAmbientLight(AmbientLight light){
  float intensity = 0.05f;
  vec3 ambient = ambLight.color * color_ambient * intensity;
  return (ambient);
}

vec3 CalcDirectionnalLight(DirectionnalLight light, vec3 surface_normal, vec3 viewDir)
{
  vec3 lightDir = normalize(-light.direction);
  float diff = max(dot(surface_normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, surface_normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), brightness);
  vec3 ambient = light.ambient * color_diffuse;
  vec3 diffuse = light.diffuse * diff * color_diffuse;
  vec3 specular = light.specular * spec * color_specular;
  return (ambient + diffuse + specular);
}

vec3 CalcPointLight(PointLight light, vec3 surface_normal, vec3 surface_position, vec3 viewDir)
{
  vec3 lightDir = normalize(light.position - surface_position);
  float diff = max(dot(surface_normal, lightDir), 0.0);
  vec3 reflectDir = reflect(-lightDir, surface_normal);
  float spec = pow(max(dot(viewDir, reflectDir), 0.0), brightness);
  float distance = length(light.position - surface_position);
  float attenuation = 1.0 / (light.constant + light.linear * distance + 
  			     light.quadratic * (distance * distance));    
  vec3 ambient  = light.ambient  * color_diffuse;
  vec3 diffuse  = light.diffuse  * diff * color_diffuse;
  vec3 specular = light.specular * spec * color_specular;
  ambient  *= attenuation;
  diffuse  *= attenuation;
  specular *= attenuation;
  return (ambient + diffuse + specular);
}

vec3 CalcSpotLight(SpotLight light, vec3 surface_normal, vec3 surface_position, vec3 viewDir)
{
  vec3 lightDir = normalize(light.position - surface_position);
  float theta = dot(lightDir, normalize(-light.direction));
    
  float diff = max(dot(surface_normal, lightDir), 0.0);
  if (theta < light.cutoff) 
  {       
    vec3 ambient = light.ambient * color_diffuse;
        
    // diffuse 
    vec3 diffuse = light.diffuse * diff * color_diffuse;  
        
    // specular
    vec3 reflectDir = reflect(-lightDir, surface_normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), brightness);
    vec3 specular = light.specular * spec * color_specular;  
        
    // attenuation
    float distance    = length(light.position - surface_position);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;   
            
    return (ambient + diffuse + specular);
  } else return light.ambient * color_diffuse;
}