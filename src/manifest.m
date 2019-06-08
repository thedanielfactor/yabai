#include <ScriptingBridge/ScriptingBridge.h>
#include <Carbon/Carbon.h>
#include <Cocoa/Cocoa.h>
#include <IOKit/ps/IOPowerSources.h>
#include <IOKit/ps/IOPSKeys.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <dirent.h>
#include <stdbool.h>
#include <assert.h>
#include <fcntl.h>
#include <regex.h>
#include <execinfo.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <pthread.h>

#include "misc/macros.h"
#include "misc/log.h"
#include "misc/helpers.h"
#include "misc/sbuffer.h"
#define HASHTABLE_IMPLEMENTATION
#include "misc/hashtable.h"
#undef HASHTABLE_IMPLEMENTATION
#include "misc/socket.h"
#include "misc/socket.c"

#include "osax/sa.h"
#include "osax/sa_loader.c"
#include "osax/sa_payload.c"
#include "osax/sa.m"

#include "event.h"
#include "event_loop.h"
#include "event_tap.h"
#include "process.h"
#include "workspace.h"
#include "mouse.h"
#include "rule.h"
#include "message.h"
#include "display.h"
#include "space.h"
#include "view.h"
#include "border.h"
#include "window.h"
#include "application.h"
#include "process_manager.h"
#include "display_manager.h"
#include "space_manager.h"
#include "window_manager.h"
#include "bar.h"

#include "event.c"
#include "event_loop.c"
#include "event_tap.c"
#include "process.c"
#include "workspace.m"
#include "rule.c"
#include "message.c"
#include "display.c"
#include "space.c"
#include "view.c"
#include "border.c"
#include "window.c"
#include "application.c"
#include "process_manager.c"
#include "display_manager.c"
#include "space_manager.c"
#include "window_manager.c"
#include "bar.c"

#include "yabai.c"