
#include <ui.hpp>
#include <cstdint>
#include <iostream>

void HandleClayErrors(Clay_ErrorData errorData) {
    std::cout << "[CLAY ERROR]" << errorData.errorText.chars << std::endl;
}

ui::Context::Context(float width, float height) {
	std::uint64_t totalMemorySize = Clay_MinMemorySize();
    this->arena = Clay_CreateArenaWithCapacityAndMemory(totalMemorySize, malloc(totalMemorySize));
    Clay_Initialize(arena, { .width = width, .height = height }, (Clay_ErrorHandler) { HandleClayErrors });
}

Clay_RenderCommandArray ui::Context::compute_layout() {
	Clay_BeginLayout();

	return Clay_EndLayout();
}
