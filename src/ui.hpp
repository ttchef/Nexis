#pragma once

#include <clay.h>
#include <cstdint>

namespace ui {
	struct Context {
		Clay_Arena arena;

		Context(float width, float height);
		Clay_RenderCommandArray compute_layout();
	};
} // namespace ui
