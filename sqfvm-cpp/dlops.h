#pragma once
#include <string>

#if defined(__linux__)
#include <dlfcn.h>
#define DLOPS_LIB_OPEN(P) dlopen(P, RTLD_LAZY)
#define DLOPS_LIB_CLOSE(H) dlclose(H)
#define DLOPS_LIB_SYM(H, N) dlsym(H, N)
#define DLOPS_LIB_GETLASTERROR() dlerror()
#define DLOPS_LIB_HANDLE void*
#elif defined(_WIN32)
#include <Windows.h>
#define DLOPS_LIB_OPEN(P) LoadLibraryA(P)
#define DLOPS_LIB_CLOSE(H) FreeLibrary(H)
#define DLOPS_LIB_SYM(H, N) GetProcAddress(H, N)
#define DLOPS_LIB_GETLASTERROR() std::to_string(GetLastError())
#define DLOPS_LIB_HANDLE HMODULE
#else
#define DLOPS_LIB_OPEN(P)
#define DLOPS_LIB_CLOSE(H)
#define DLOPS_LIB_SYM(H, N)
#define DLOPS_LIB_GETLASTERROR()
#define DLOPS_LIB_HANDLE
#error UNSUPPORTED PLATFORM
#endif


class dlops
{
private:
	DLOPS_LIB_HANDLE handle;
	std::string mpath;
public:
	//Prevent copy
	dlops(const dlops&) = delete;
	void operator=(dlops const &x) = delete;


	dlops(std::string path)
	{
		handle = DLOPS_LIB_OPEN(path.c_str());
		if (0 == handle)
		{
			throw std::runtime_error(DLOPS_LIB_GETLASTERROR());
		}
		mpath = path;
	}
	~dlops()
	{
		close();
	}
	std::string path(void) { return mpath; }

	void* resolve(std::string name)
	{
		if (0 == handle)
			return 0;
		auto res = DLOPS_LIB_SYM(handle, name.c_str());
		if (0 == res)
		{
			throw std::runtime_error(DLOPS_LIB_GETLASTERROR());
		}
		else
		{
			return res;
		}
	}
	bool try_resolve(std::string name, void** outptr)
	{
		if (0 == handle)
			return 0;
		*outptr = DLOPS_LIB_SYM(handle, name.c_str());
		if (0 == *outptr)
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	void close(void)
	{
		if (0 == handle)
			return;
#if defined(__linux__)
		if (DLOPS_LIB_CLOSE(handle))
#elif defined(_WIN32)
		if (!DLOPS_LIB_CLOSE(handle))
#else
		if (DLOPS_LIB_CLOSE(handle))
#endif
		{
			throw std::runtime_error(DLOPS_LIB_GETLASTERROR());
		}
	}
};


#undef DLOPS_LIB_OPEN
#undef DLOPS_LIB_CLOSE
#undef DLOPS_LIB_SYM
#undef DLOPS_LIB_GETLASTERROR
#undef DLOPS_LIB_HANDLE