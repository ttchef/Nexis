
#include <Nexis/core.h>

NxI32 LLVMFuzzerTestOneInput(const NxU8 *data, NxUsize size)
{
	NxBuffer buffer = {
		.data = (void *)data,
		.size = size,
	};

	NxSystem system;
	Nx_system_load(&system, &buffer);

	Nx_buffer_free(&buffer);
	
	return 0;
}
