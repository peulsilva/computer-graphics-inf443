
#include "models_textures.hpp"

using namespace cgp;
using namespace std;

mesh torus_with_texture()
{
    float a = 1.7f;
    float b = 0.6f;

    // Number of samples of the terrain is N x N
    int N = 50;

    mesh torus; // temporary terrain storage (CPU only)
    torus.position.resize(N*N);
    torus.uv.resize(N*N);

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // Compute the local surface function

            vec3 p = {
					(a + b*std::cos(2* Pi *u))*std::cos(2* Pi *v),
					(a + b*std::cos(2* Pi *u))*std::sin(2* Pi *v),
					b*std::sin(2* Pi *u)};
            

            // Store vertex coordinates
            torus.position[kv+N*ku] = p;
            torus.uv[kv+N*ku] = {
                v*10, u*4
                // (a + b*std::cos(2* Pi *u))*std::cos(2* Pi *v),
                // (a + b*std::cos(2* Pi *u))*std::sin(2* Pi *v)
            };
        }
    }


    // Generate triangle organization
    for(int ku=0; ku<N-1; ++ku)
    {
        for(int kv=0; kv<N-1; ++kv)
        {
            unsigned int idx = kv + N*ku;

            uint3 triangle_1 = {idx, idx+1, idx+1+N };
            uint3 triangle_2 = {idx, idx+1+N, idx+N };

            torus.connectivity.push_back(triangle_1);
            torus.connectivity.push_back(triangle_2);
        }
    }

    torus.fill_empty_field();
    return torus;
}


mesh cylinder_with_texture()
{
    float r = 1.0f;
    float h = 4.0f;

    // Number of samples of the terrain is N x N
    int N = 20;

    mesh cylinder; // temporary terrain storage (CPU only)
    cylinder.position.resize(N*N);
    cylinder.uv.resize(N*N);

    // Fill terrain geometry
    for(int ku=0; ku<N; ++ku)
    {
        for(int kv=0; kv<N; ++kv)
        {
            // Compute local parametric coordinates (u,v) \in [0,1]
            float u = ku/(N-1.0f);
            float v = kv/(N-1.0f);

            // Compute the local surface function
            vec3 p = {r*std::cos(2* Pi *u), r*std::sin(2* Pi *u), h*(v-0.5f)};
            vec2 uv = {u,v};

            // Store vertex coordinates
            cylinder.position[kv+N*ku] = p;
            cylinder.uv[kv+N*ku] = {u,v};
        }
    }

    // Generate triangle organization
    for(int ku=0; ku<N-1; ++ku)
    {
        for(int kv=0; kv<N-1; ++kv)
        {
            int idx = kv + N*ku;

            uint3 triangle_1 = {idx, idx+1+N, idx+1};
            uint3 triangle_2 = {idx, idx+N, idx+1+N};

            cylinder.connectivity.push_back(triangle_1);
            cylinder.connectivity.push_back(triangle_2);
        }
    }

	cylinder.fill_empty_field();
    return cylinder;
}


mesh disc_with_texture()
{
    float r = 1.0f;

	mesh disc;
    int N = 20;
    float len = 0.65f;
    vec2 canto = {0.15f, 0.15f};

	for (int k = 0; k < N; ++k)
	{
		float u = k/(N-1.0f);
		vec3 p = r * vec3(std::cos(2* Pi *u), std::sin(2* Pi *u), 0.0f);
		disc.position.push_back(p);
        vec2 uv; 
        if(u < 0.25) uv = {4*u, 0};
        else if(u < 0.5) uv = {1, 4*(u-0.25)};
        else if(u < 0.75) uv = {1-4*(u-0.5), 1};
        else uv = {0, 1-4*(u-0.75)};
        uv *= len;
        uv += canto;
        disc.uv.push_back(uv);
		
	}
	// middle point
    disc.position.push_back({0,0,0});
    disc.uv.push_back(vec2{0.5, 0.5}*len + canto); 

	for (int k = 0; k < N-1; ++k)
		disc.connectivity.push_back( uint3{ N, k, k+1});

	disc.fill_empty_field();
    return disc;
}



