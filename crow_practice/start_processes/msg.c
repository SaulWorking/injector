#include <windows.h>


// C does not have a custom string type
// strings are pointers

int main(void)
{
	//MessageBoxW uses unicode, which is more modern 
	MessageBoxW(
		NULL,	// no owner window (handle) required [hWnd]
		L"The in text", // wchar_t string literal [LPCWSTR]
		L"The title",	// [LPCWSTR]
		MB_YESNOCANCEL | MB_ICONEXCLAMATION // function flags & the (maybe) bitwise OR [UINT]
	);
	
	//MessageBoxEX has more parameters
	return EXIT_SUCCESS;
}
