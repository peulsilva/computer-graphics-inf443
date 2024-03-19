
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
    float terrain_length 
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

            float z = evaluate_terrain_height(x,y);

            vec3 new_pos = {x,y,z};
            
            if (is_valid_distance(new_pos, v, 1)){

                v.push_back({x,y,z});
                break;
            }

        }
        

    }

    return v;
}
