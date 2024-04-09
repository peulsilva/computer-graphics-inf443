#pragma once

#include "cgp/cgp.hpp"

using namespace cgp;

float evaluate_terrain_height(float x, float y);

/** Compute a terrain mesh 
	The (x,y) coordinates of the terrain are set in [-length/2, length/2].
	The z coordinates of the vertices are computed using evaluate_terrain_height(x,y).
	The vertices are sampled along a regular grid structure in (x,y) directions. 
	The total number of vertices is N*N (N along each direction x/y) 	*/
class perlin_noise_parameters {
	public:

	float persistency = 1.0f;
	float frequency_gain = 1.2f;
	int octave = 6;
	float terrain_height = 0.5f;

	// perlin_noise_parameters(float persistency, float frequency_gain, int octave, float terrain_height){
	// 	this->frequency_gain = frequency_gain;
	// 	this->persistency = persistency;
	// 	this->octave = octave;
	// 	this-> terrain_height = terrain_height;
	// }

};
cgp::mesh create_terrain_mesh(int N, float length);
mesh create_naive_terrain(float terrain_length);
std::vector<cgp::vec3> generate_positions_on_terrain(int N, float terrain_length, perlin_noise_parameters& parameters);
void update_terrain(mesh& terrain, mesh_drawable& terrain_visual, perlin_noise_parameters parameters);


