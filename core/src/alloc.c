
#include <alloc.h>

// NOTE: https://stackoverflow.com/questions/2989810/which-cross-platform-preprocessor-defines-win32-or-win32-or-win32
#if !defined(_WIN32) && (defined(__unix__) || defined(__unix) || (defined(__APPLE__) && defined(__MACH__)))

// Unix OS
#include <unistd.h>

#if !defined(_POSIX_VERSION)
#error "Posix version not defined";
#endif // _POSIX_VERSION

// Posix
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>

void *Nx_virtual_alloc(NxU64 size)
{
    void *mem = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (mem == MAP_FAILED)
    {
        fprintf(stderr, "[NEXIS] mmap failed: errno=%d (%s)\n", errno, strerror(errno));
        return NULL;
    }
    return mem;
}

void Nx_virtual_free(void *mem, NxU64 size)
{
    NxI32 result = munmap(mem, size);
    if (result == -1)
    {
        fprintf(stderr, "[NEXIS] munmap failed: errno=%d (%s\n)", errno, strerror(errno));
    }
}

#else
#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>

// Windows

void *Nx_virtual_alloc(NxU64 size)
{
    void *mem = VirtualAlloc(NULL, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);
    if (!mem)
    {
        fprintf(stderr, "[NEXIS] VirtualAlloc failed\n");
        return NULL;
    }
    return mem;
}

void Nx_virtual_free(void *mem, NxU64 size)
{
    BOOL result = VirtualFree(mem, 0, MEM_RELEASE);
    if (!result)
    {
        fprintf(stderr, "[NEXIS] VirtualFree failed\n");
    }
}

#endif // Windows
#endif
