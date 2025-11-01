#!/bin/bash

set -e

# Clone PHP 7.3.3
git clone https://github.com/php/php-src.git php-monitor
cd php-monitor
git checkout php-7.3.3

# Apply patch
git apply ../zend_monitor.patch

# Build
./buildconf --force
./configure --enable-debug
make -j$(nproc)

echo "Done! Test with:"
echo "  export SYMPHP_TRACE=/tmp/trace.log"
echo "  ./sapi/cli/php test.php"
