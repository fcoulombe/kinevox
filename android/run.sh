#!/bin/sh
make @ProjectName@ -j8 && cd android/ && ant debug && cd .. && adb -d install -r android/bin/@ProjectName@-debug.apk && adb logcat -c && adb logcat 
