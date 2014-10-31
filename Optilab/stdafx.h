#define QT_NO_OPENGL
#include <QtWidgets>

#include <chrono>
#include <thread>

#define NOMINMAX
#include <Windows.h>
#include <atlstr.h>

#define WIN32_LEAN_AND_MEAN
#include <windowsx.h>
#include <commctrl.h>
#include <tchar.h>

#include <opencv2\opencv.hpp>

#include "DSCAMAPI.h"