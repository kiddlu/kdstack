enum {
	os_linux = 1,
	os_aix,
	os_freebsd,
	os_hpux,
	os_mac,
	os_netbsd,
	os_openbsd,
	os_solaris,
	os_windows,
	os_android,
	os_dragonfly,

	os_nr,
};

#if defined(__ANDROID__)
#include "os-android.h"
#elif defined(__linux__)
#include "os-linux.h"
#elif defined(__FreeBSD__)
#include "os-freebsd.h"
#elif defined(__OpenBSD__)
#include "os-openbsd.h"
#elif defined(__NetBSD__)
#include "os-netbsd.h"
#elif defined(__sun__)
#include "os-solaris.h"
#elif defined(__APPLE__)
#include "os-mac.h"
#elif defined(_AIX)
#include "os-aix.h"
#elif defined(__hpux)
#include "os-hpux.h"
#elif defined(WIN32)
#include "os-windows.h"
#elif defined (__DragonFly__)
#include "os-dragonfly.h"
#else
#error "unsupported os"
#endif
