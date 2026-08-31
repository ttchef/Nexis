#pragma once

#include <types.hpp>

#include <raylib.h>

namespace ui {

struct Context {
	Context();
	~Context();

	void draw(Texture2D scene);	
};
	
} // namespace ui
