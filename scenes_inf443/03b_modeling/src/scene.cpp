#include "scene.hpp"

#include "terrain.hpp"
#include "tree.hpp"

using namespace cgp;




void scene_structure::initialize()
{
	camera_control.initialize(inputs, window); // Give access to the inputs and window global state to the camera controler
	camera_control.set_rotation_axis_z();
	camera_control.look_at({ 15.0f,6.0f,6.0f }, {0,0,0});
	

	// General information
	display_info();

	global_frame.initialize_data_on_gpu(mesh_primitive_frame());

	int N_terrain_samples = 100;
	float terrain_length = 20;

	terrain_mesh = create_terrain_mesh(N_terrain_samples, terrain_length);
	// mesh terrain_mesh = create_naive_terrain(terrain_length);
	terrain.initialize_data_on_gpu(terrain_mesh);
	terrain.texture.load_and_initialize_texture_2d_on_gpu(
		project::path + "assets/texture_grass.jpg",
		GL_REPEAT,
		GL_REPEAT
	);

	update_terrain(
		terrain_mesh,
		terrain,
		parameters
	);

	// terrain.material.color = { 0.6f,0.85f,0.5f };
	// terrain.material.phong.specular = 0.0f; // non-specular terrain material

	mesh tree_mesh = create_tree();
	tree.initialize_data_on_gpu(tree_mesh);

	int n_trees = 100;
	tree_position = generate_positions_on_terrain(n_trees, terrain_length, parameters);	
	
	// terrain.initialize_data_on_gpu(tree);
	
}



void scene_structure::display_frame()
{
	// Set the light to the current position of the camera
	environment.light = camera_control.camera_model.position();
	
	if (gui.display_frame)
		draw(global_frame, environment);

	for (auto& position : tree_position){
		tree.model.translation = position;
		draw(tree, environment);

		if (gui.display_wireframe){
			draw_wireframe(tree, environment);
		}
	}

	draw(terrain, environment);

	if (gui.display_wireframe){
		draw_wireframe(terrain, environment);
	}

}


void scene_structure::display_gui()
{
	ImGui::Checkbox("Frame", &gui.display_frame);
	ImGui::Checkbox("Wireframe", &gui.display_wireframe);

	bool update = false;
	update |= ImGui::SliderFloat("Persistance", &parameters.persistency, 0.f, 1.f);
	update |= ImGui::SliderFloat("Frequency gain", &parameters.frequency_gain, 1.f, 2.5f);
	update |= ImGui::SliderInt("Octave", &parameters.octave, 1, 8);
	update |= ImGui::SliderFloat("Height", &parameters.terrain_height, 0.f, 1.5f);

	if (update){// if any slider has been changed - then update the terrain
		update_terrain(terrain_mesh, terrain, parameters);
		
	}
}

void scene_structure::mouse_move_event()
{
	if (!inputs.keyboard.shift)
		camera_control.action_mouse_move(environment.camera_view);
}
void scene_structure::mouse_click_event()
{
	camera_control.action_mouse_click(environment.camera_view);
}
void scene_structure::keyboard_event()
{
	camera_control.action_keyboard(environment.camera_view);
}
void scene_structure::idle_frame()
{
	camera_control.idle_frame(environment.camera_view);
}

void scene_structure::display_info()
{
	std::cout << "\nCAMERA CONTROL:" << std::endl;
	std::cout << "-----------------------------------------------" << std::endl;
	std::cout << camera_control.doc_usage() << std::endl;
	std::cout << "-----------------------------------------------\n" << std::endl;
}
