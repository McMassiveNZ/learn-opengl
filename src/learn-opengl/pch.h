#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <format>
#include <vector>
#include <chrono>
#include <thread>
#include <array>
#include <cinttypes>

#ifdef _WIN32
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#ifndef NOMINMAX
#define NOMINMAX
#endif
#include <Windows.h>
#endif