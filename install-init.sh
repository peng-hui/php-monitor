#!/bin/bash

set -e

# Clone PHP 7.3.3
git clone --depth 1 --branch  PHP-7.3.3 https://github.com/php/php-src.git php-src
cd php-src
git checkout PHP-7.3.3

# Apply patch
git apply ../zend_monitor.patch

# Build
./buildconf --force
./configure 
make clean
make -j$(nproc)

echo "Done! Test with:"
echo "  export SYMPHP_TRACE=/tmp/trace.log"
echo "  ./sapi/cli/php test.php"
