#ifndef KLOG_GLOBAL_H
#define KLOG_GLOBAL_H

extern bool g_callsignCheck;
#if __cplusplus >= 201309L
    // use modern deprecation feature
#define KLOG_DEPRECATED [[deprecated]]
#else
    //... no idea how to handle this, but this seems overly noisy?
#define KLOG_DEPRECATED
#warning "KLOG_DEPRECATED is not defined yet!"
#endif

#endif
