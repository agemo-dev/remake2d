#ifndef REMAKE2D_CONFIG_EXPORT_
#define REMAKE2D_CONFIG_EXPORT_

#if defined(REMAKE2D_SCRIPT_SHARED)
  #if defined(_WIN32) || defined(__CYGWIN__)
    #if defined(REMAKE2D_SCRIPT_BUILD)
      #define RMK_SCRIPT_API __declspec(dllexport)
    #else
      #define RMK_SCRIPT_API __declspec(dllimport)
    #endif
  #elif defined(__GNUC__) || defined(__clang__)
    #define RMK_SCRIPT_API __attribute__((visibility("default")))
  #else
    #define RMK_SCRIPT_API
  #endif
#else
  #define RMK_SCRIPT_API
#endif

#endif