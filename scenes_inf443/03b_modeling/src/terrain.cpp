
// #include "<iostream>"
// #include "<vector>"
#include "terrain.hpp"


using namespace cgp;
using namespace std;

// Evaluate 3D position of the terrain for any (x,y)
float evaluate_terrain_height(float x, float y)
{

    vector<vec2> p = { 
        vec2{-10., -10.}, 
        vec2{5.,5.}, 
        vec2{-3.,4.}, 
        vec2{6.,4.} 
    };
    vector<float> h = {3., -1.5, 1., 2.};
    vector<float> sigma= {10. , 3., 4., 4.};

    float z = 0;
    for (int i = 0; i < p.size(); i++){
        float d = norm(
            vec2(x,y) - p[i]
        );

        d = d/sigma[i];
        
        z+= h[i]* exp(-d*d);
    }

    return z;
}

mesh create_terrain_mesh(int N, float terrain_length)
{

    mesh terrain; // temporary terrain storage (CPU only)
    terrain.position.resize(N*N);

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // Compute the real coordinates (x,y) of the terrain in [-terrain_length/2, +terrain_length/2]
            float x = (u - 0.5f) * terrain_length;
            float y = (v - 0.5f) * terrain_length;

            // Compute the surface height function at the given sampled coordinate
            float z = evaluate_terrain_height(x,y);

            // Store vertex coordinates
            terrain.position[kv+N*ku] = {x,y,z};
            terrain.uv.push_back({10*u,10*v});
        }
    }

    // Generate triangle organization
    //  Parametric surface with uniform grid sampling: generate 2 triangles for each grid cell
    for(int ku=0; ku<N-1; ++ku)
    {
        for(int kv=0; kv<N-1; ++kv)
        {
            unsigned int idx = kv + N*ku; // current vertex offset

            uint3 triangle_1 = {idx, idx+1+N, idx+1};
            uint3 triangle_2 = {idx, idx+N, idx+1+N};

            terrain.connectivity.push_back(triangle_1);
            terrain.connectivity.push_back(triangle_2);
        }
    }



    // need to call this function to fill the other buffer with default values (normal, color, etc)
	terrain.fill_empty_field(); 

    return terrain;
}

mesh create_naive_terrain(float terrain_length){
    int const terrain_sample = 180;
	mesh terrain = mesh_primitive_grid(
        terrain_length/2.*vec3{-1,-1,0},
        terrain_length/2.*vec3{1,-1,0},
        terrain_length/2.*vec3{1,1,0},
        terrain_length/2.*vec3{-1,1,0},
        terrain_sample,
        terrain_sample
    );
	return terrain;
}

void update_terrain(mesh& terrain, mesh_drawable& terrain_visual, perlin_noise_parameters parameters){
	// Number of samples in each direction (assuming a square grid)
	int const N = std::sqrt(terrain.position.size());

	// Recompute the new vertices
	for (int ku = 0; ku < N; ++ku) {
		for (int kv = 0; kv < N; ++kv) {
			
			// Compute local parametric coordinates (u,v) \in [0,1]
            const float u = ku/(N-1.0f);
            const float v = kv/(N-1.0f);

			int const idx = ku*N+kv;

			// Compute the Perlin noise
			float const noise = noise_perlin({u, v}, parameters.octave, parameters.persistency, parameters.frequency_gain);

			// use the noise as height value
			terrain.position[idx].z = parameters.terrain_height*noise;

			// use also the noise as color value
			terrain.color[idx] = 0.3f*vec3(0,0.5f,0)+0.7f*noise*vec3(1,1,1);
		}
	}

	// Update the normal of the mesh structure
	terrain.normal_update();
	
	// Update step: Allows to update a mesh_drawable without creating a new one
	terrain_visual.vbo_position.update(terrain.position);
	terrain_visual.vbo_normal.update(terrain.normal);
	terrain_visual.vbo_color.update(terrain.color);
	
}



bool is_valid_distance(
    vec3& new_pos, 
    vector<vec3>& positions, 
    float min_distance    
){
    for (auto& p : positions){
        float d = pow(p.x - new_pos.x, 2) + pow(p.y - new_pos.y, 2);
        d = sqrt(d);

        if (d < min_distance){
            return false;
        }
    }

    return true;
}

vector<vec3> generate_positions_on_terrain(
    int N,
    float terrain_length,
    perlin_noise_parameters& parameters
){
    vector<vec3> v;

    for (int i =0; i < N; i++){
        while (true){
            float x = rand_uniform(
                -terrain_length/2.,
                terrain_length/2.
            ) ;
            float y = rand_uniform(
                -terrain_length/2.,
                terrain_length/2.
            );
    
            float z = noise_perlin({x,y}, parameters.octave, parameters.persistency, parameters.frequency_gain);
            z *= parameters.terrain_height;

            vec3 new_pos = {x,y,z};
            
            if (is_valid_distance(new_pos, v, 1)){

                v.push_back({x,y,z});
                break;
            }

        }
        

    }

    return v;
}
