
#include <Nexis/core.h>

int LLVMFuzzerTestOneInput(void *data, long long size)
{
	NxBuffer buffer = {
		.data = data,
		.size = size,
	};

	NxSystem system;
	Nx_system_load(&system, &buffer);
	
	return 0;
}
