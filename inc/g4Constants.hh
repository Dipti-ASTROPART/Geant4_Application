#ifndef     g4Constants_HH
#define     g4Constants_HH

#define RESET       "\033[0m"
#define BOLD        "\033[1m"
#define ITALIC      "\033[3m"

#define COLOR_RED       "\x1b[31m"
#define COLOR_GREEN     "\x1b[32m"
#define COLOR_YELLOW    "\x1b[33m"
#define COLOR_BLUE      "\x1b[34m"
#define COLOR_BG_BLUE   "\x1b[44m"
#define COLOR_MAGENTA   "\x1b[35m"
#define COLOR_CYAN      "\x1b[36m"
#define COLOR_RESET     "\x1b[0m"

#define COLOR_OUTPUT    COLOR_YELLOW
#define COLOR_INPUT     COLOR_CYAN
#define COLOR_ERROR     COLOR_RED
#define COLOR_WARNING   COLOR_BLUE
#define COLOR_ADVICE    COLOR_GREEN

// Define the build date using preprocessor directives
#define BUILD_DATE __DATE__

#endif
