ps ax | grep -i scons | awk '{print $1}' | xargs kill -9
