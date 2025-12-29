export PATH=/opt/msvc/bin/x64:$PATH
CC="cl /Zi /FS" CXX="cl /Zi /FS" cmake .. -DCMAKE_BUILD_TYPE=Debug -DCMAKE_SYSTEM_NAME=Windows
make
