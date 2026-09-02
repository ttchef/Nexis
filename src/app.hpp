
#pragma once

#include <types.hpp>
#include <particle.hpp>
#include <camera.hpp>
#include <ui/context.hpp>
#include <window.hpp>
#include <app_state.hpp>
#include <app_context.hpp>
#include <project.hpp>

#include <vector>

struct GridShader
{
	Shader handle;
	i32 camera_pos_loc;	
};

struct App
{
	AppState state;
	GridShader grid_shader;	

	std::vector<Project> projects{};

	Window window;
	ui::Context ui;

	// Current particle system.
	// There is only one system in a project/entire life of the program
	particle::System system{};
	SceneCamera camera{};

	App();
	~App();

	bool should_close();
	void update();
	void draw();
	AppContext make_context();
};
