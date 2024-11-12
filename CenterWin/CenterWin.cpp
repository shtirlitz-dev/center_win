// CenterWin.cpp : Defines the entry point for the application.
//

#include "framework.h"
#include "CenterWin.h"


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	HWND hwnd = GetForegroundWindow();
	if (!hwnd)
		return 0;


	// Get the monitor that the window is currently on
	HMONITOR hMonitor = MonitorFromWindow(hwnd, MONITOR_DEFAULTTONEAREST);
	if (!hMonitor)
		return 0;

	// Get monitor information
	MONITORINFO monitorInfo = { sizeof(MONITORINFO) };
	if (!GetMonitorInfo(hMonitor, &monitorInfo))
		return 0;

	auto wrkRect = monitorInfo.rcWork;
	// Calculate the target width and height (80% of monitor's work area)

	LONG style = GetWindowLong(hwnd, GWL_STYLE);
	bool isSizable = (style & WS_SIZEBOX) != 0;

	int left, top, width, height;

	if (isSizable) {
		if (IsZoomed(hwnd))
			ShowWindow(hwnd, SW_RESTORE);

		const int ratio = 95; // % of monitor width and height
		int monitorWidth = wrkRect.right - wrkRect.left;
		int monitorHeight = wrkRect.bottom - wrkRect.top;
		width = MulDiv(monitorWidth, ratio, 100);
		height = MulDiv(monitorHeight, ratio, 100);
		left = wrkRect.left + (monitorWidth - width) / 2;
		top = wrkRect.top + (monitorHeight - height) / 2;
	}
	else {
		RECT r;
		GetWindowRect(hwnd, &r);
		if (r.left >= wrkRect.left
			&& r.top >= wrkRect.top
			&& r.right <= wrkRect.right
			&& r.bottom <= wrkRect.bottom)
			return 0; // inside monitor rect
		left = r.left;
		top = r.top;
		width = r.right - r.left;
		height = r.bottom - r.top;
		if (left + width > wrkRect.right)
			left = wrkRect.right - width;
		if (top + height > wrkRect.bottom)
			top = wrkRect.bottom - height;
		if (left < wrkRect.left)
			left = wrkRect.left;
		if (top < wrkRect.top)
			top = wrkRect.top;
	}

	SetWindowPos(hwnd, nullptr, left, top, width, height, SWP_NOZORDER | SWP_NOACTIVATE);
	return 0;
}


