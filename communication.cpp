#include "C:\Users\rootste\source\repos\demon_services\dependencies\communication\communication.hpp"

HANDLE driver_handle = INVALID_HANDLE_VALUE;
INT32 process_id = 0;
uintptr_t virtualaddy = 0;

bool Kernel::initalize_driver()
{
	driver_handle = CreateFileW(L"\\\\.\\\XenonWinShitx86", GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, 0, OPEN_EXISTING, 0, 0);

	if (!driver_handle || (driver_handle == INVALID_HANDLE_VALUE))
	{
		return false;
	}

	return true;
}

void Kernel::read_phys(PVOID address, PVOID buffer, DWORD size)
{
	_r args = { 0 };
	args.address = (ULONGLONG)address;
	args.buffer = (ULONGLONG)buffer;
	args.size = size;
	args.process_id = process_id;
	args.write = FALSE;

	DeviceIoControl(driver_handle, read_code, &args, sizeof(args), nullptr, 0, 0, 0);
}

uintptr_t Kernel::find_base()
{
	uintptr_t image_address = { NULL };
	_ba args = { NULL };
	args.process_id = process_id;
	args.address = (ULONGLONG*)&image_address;

	DeviceIoControl(driver_handle, base_code, &args, sizeof(args), nullptr, 0, 0, 0);

	return image_address;
}

bool Kernel::find_dirbase()
{
	bool ret = false;
	_dtb args = { 0 };
	args.process_id = process_id;
	args.operation = (bool*)&ret;

	DeviceIoControl(driver_handle, dirbase_code, &args, sizeof(args), nullptr, 0, 0, 0);

	return ret;
}

INT32 Kernel::find_proc(LPCTSTR process_name)
{
	PROCESSENTRY32 pt;
	HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);

	if (Process32First(hsnap, &pt))
	{
		do
		{
			if (!lstrcmpi(pt.szExeFile, process_name))
			{
				CloseHandle(hsnap);
				process_id = pt.th32ProcessID;
				return pt.th32ProcessID;
			}
		} while (Process32Next(hsnap, &pt));
	}

	CloseHandle(hsnap);

	return { NULL };
}