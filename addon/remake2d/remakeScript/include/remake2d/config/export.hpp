#ifndef REMAKE2D_CONFIG_EXPORT_
#define REMAKE2D_CONFIG_EXPORT_

// RMK_SCRIPT_API    : exported symbol (needed on Windows, harmless elsewhere)
// RMK_SCRIPT_HIDDEN : symbol kept private to the library (GCC/Clang only)
#if defined(REMAKE2D_SCRIPT_SHARED)
  #if defined(_WIN32) || defined(__CYGWIN__)
    #if defined(REMAKE2D_SCRIPT_BUILD)
      #define RMK_SCRIPT_API __declspec(dllexport)
    #else
      #define RMK_SCRIPT_API __declspec(dllimport)
    #endif
    #define RMK_SCRIPT_HIDDEN
  #elif defined(__GNUC__) || defined(__clang__)
    #define RMK_SCRIPT_API    __attribute__((visibility("default")))
    #define RMK_SCRIPT_HIDDEN __attribute__((visibility("hidden")))
  #else
    #define RMK_SCRIPT_API
    #define RMK_SCRIPT_HIDDEN
  #endif
#else
  #define RMK_SCRIPT_API
  #define RMK_SCRIPT_HIDDEN
#endif

#endif