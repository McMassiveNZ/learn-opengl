#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <chrono>
#include <thread>
#include <array>
#include <cinttypes>
#include <span>
#include <algorithm>
#include <functional>
#include <queue>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#include <Windowsx.h>
#include <hidusage.h>
#endif

#pragma warning(push)
#pragma warning(disable : 4201 )
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#pragma warning(pop)