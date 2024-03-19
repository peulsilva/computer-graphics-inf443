#version 330 core


// Inputs coming from the vertex shader
in struct fragment_data
{
    vec3 position; // position in the world space
    vec3 normal;   // normal in the world space
    vec3 color;    // current color on the fragment
    vec2 uv;       // current uv-texture on the fragment

} fragment;

// Output of the fragment shader - output color
layout(location=0) out vec4 FragColor;

// View matrix
uniform mat4 view;

struct material_structure
{
	vec3 color;  // Uniform color of the object
};
uniform material_structure material;

// Ambiant uniform controled from the GUI
uniform float ambiant;
uniform float diffus;
uniform float specular;
uniform float s_exp;
uniform float d_max_fog;
uniform float d_max_light; 
uniform vec3 light_color;
uniform vec3 fog_color;
uniform vec3 light_position;



// void main()
// {
// 	// vec3 current_color;
// 	// current_color = ambiant * material.color * light_color;
// 	// FragColor = vec4(current_color, 1.0); 	// Note: the last alpha component is not used here

//     vec3 pos = fragment.position;
//     vec3 normal = normalize(fragment.normal);
//     vec3 light_dir = normalize(light_position - pos); // u_l

//     vec3 current_color = (ambiant + diffus * max(dot(normal, light_dir), 0)) * material.color * light_color;

//     FragColor =  vec4(current_color, 1.0);
// }

// fog
// void main(){
//     mat3 O = transpose(mat3(view)); // get the orientation matrix
//     vec3 last_col = vec3(view * vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
//     vec3 camera_position = -O * last_col;

//     vec3 pos = fragment.position;
//     vec3 normal = normalize(fragment.normal);
//     vec3 light_dir = normalize(light_position - pos); // u_l


//     vec3 current_color = (ambiant + diffus * max(dot(normal, light_dir), 0)) * material.color * light_color;
    
//     vec3 light_reflection_dir = reflect(-light_dir, normal); // u_r
//     vec3 point_of_view_dir = normalize(camera_position);

//     current_color+= specular * pow(max(dot(light_reflection_dir, point_of_view_dir),0 ), s_exp) * light_color;

//     float attenuation = min(1,length(camera_position)/d_max_fog);
//     current_color = (1 - attenuation) * current_color + attenuation * fog_color;

//     FragColor = vec4(current_color, 1.0);
// }

// light attenuation
void main(){
    mat3 O = transpose(mat3(view)); // get the orientation matrix
    vec3 last_col = vec3(view * vec4(0.0, 0.0, 0.0, 1.0)); // get the last column
    vec3 camera_position = -O * last_col;

    vec3 pos = fragment.position;
    vec3 normal = normalize(fragment.normal);
    vec3 light_dir = normalize(light_position - pos); // u_l


    vec3 current_color = (ambiant + diffus * max(dot(normal, light_dir), 0)) * material.color * light_color;
    
    vec3 light_reflection_dir = reflect(-light_dir, normal); // u_r
    vec3 point_of_view_dir = normalize(camera_position);

    current_color+= specular * pow(max(dot(light_reflection_dir, point_of_view_dir),0 ), s_exp) * light_color;

    float fog_attenuation = min(1,length(camera_position)/d_max_fog);

    float distance_attenuation = min(1, length(light_position - pos)/d_max_light);

    current_color = (1 - distance_attenuation) * current_color;
    current_color = (1 - fog_attenuation) * current_color + fog_attenuation * fog_color;

    FragColor = vec4(current_color, 1.0f);
}