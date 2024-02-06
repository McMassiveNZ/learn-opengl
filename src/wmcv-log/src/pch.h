#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <vector>
#include <cassert>
#include <cstdarg>
#include <cinttypes>
#include <array>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#endif
