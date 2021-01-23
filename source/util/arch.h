#pragma once
#ifndef ARCH_H
#define ARCH_H

// Set Platform
#if defined(_WIN32)
#define PLATFORM_WINDOWS
#elif defined(__linux__)
#define PLATFORM_LINUX
#elif (defined(__APPLE__) && defined(__MACH__))
#define PLATFORM_MACOS
#else
#error "Unsupported OS"
#endif

#endif