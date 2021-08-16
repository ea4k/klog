#!/bin/bash
ls -la
cd /Work/klog/
qmake KLog.pro -spec linux-g++ CONFIG+=release && make -j$(nproc)
