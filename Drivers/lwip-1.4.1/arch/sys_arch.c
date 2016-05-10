/* Most of the functions defined in sys.h must be implemented in the
 * architecture-dependent file sys_arch.c */

#include "sys.h"
#include "arch/cc.h"
#include "Timer.h"

u32_t sys_jiffies(void) {

    return lwip_time;
}



u32_t sys_now(void)
{
	return lwip_time;
}



