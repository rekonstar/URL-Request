#pragma once
#include <Windows.h>
#include <TlHelp32.h>
#include <vector>

#define dirbase_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x500C, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define read_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x501C, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define base_code CTL_CODE(FILE_DEVICE_UNKNOWN, 0x502C, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)

typedef struct _r {
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
	BOOLEAN write;
} r, * pr;

typedef struct _ba {
	INT32 process_id;
	ULONGLONG* address;
} ba, * pba;

typedef struct _dtb {
	INT32 process_id;
	bool* operation;
} dtb, * dtbl;

extern HANDLE driver_handle;
extern INT32 process_id;
extern uintptr_t virtualaddy;

class Kernel
{
public:
	bool initalize_driver();
	void read_phys(PVOID address, PVOID buffer, DWORD size);
	uintptr_t find_base();
	bool find_dirbase();
	INT32 find_proc(LPCTSTR process_name);
};
inline Kernel* kernel;

template <typename T>
T read(uint64_t address)
{
	T buffer{ };
	kernel->read_phys((PVOID)address, &buffer, sizeof(T));

	return buffer;
}