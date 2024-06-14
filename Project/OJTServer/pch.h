#pragma once
#define _WINSOCKAPI_
#include <InnoOJTCore.h>

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

#define INNO_CLIENT_FRAME_PER_SECOND 120.f
#define INN_FRAME_DELTA_TIME (1.f / INNO_CLIENT_FRAME_PER_SECOND)