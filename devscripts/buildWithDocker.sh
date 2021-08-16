#!/bin/bash
ls -la
cd klog/
qmake KLog.pro -spec linux-g++ CONFIG+=release && make -j$(nproc)
