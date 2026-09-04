
#pragma once

#include <types.hpp>
#include <camera.hpp>
#include <ui/context.hpp>
#include <window.hpp>
#include <app_state.hpp>
#include <app_context.hpp>
#include <project.hpp>
#include <asset_manager.hpp>

#include <Nexis/core.h>

struct GridShader
{
	Shader handle;
	i32 camera_pos_loc;	
};

class App
{
private:
	AppState state;
	GridShader grid_shader;	

	std::vector<ProjectHeader> projects{};

	Window window;
	ui::Context ui;
	SceneCamera camera{};
	asset::Manager asset_manager{};
	Project project;

	NxSystem system;
	NxRenderer renderer;

public:
	App();
	~App();

	bool should_close();
	void update();
	void draw();
	AppContext make_context();
};
