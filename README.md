
# PHP Opcode Tracer

Traces PHP execution at the VM opcode level based on Witcher's https://github.com/sefcom/Witcher/blob/master/docker/php7build.Dockerfile

## ⚠️ Important Limitations

- **JIT Compiler:** Does NOT work with PHP JIT. JIT bypasses the VM opcode executor, so opcodes won't be traced. Disable JIT with `opcache.jit=0`?

## Build
```bash
chmod +x build.sh
./build.sh
```

## Usage
```bash
export SYMPHP_TRACE=/tmp/trace.log
./sapi/cli/php script.php
cat /tmp/trace.log
```

## Note

Each PHP line becomes multiple opcodes, so you'll see duplicates:
```
script.php:5
script.php:5
script.php:5
script.php:7
script.php:7
```

**To remove consecutive duplicates:**
```bash
uniq /tmp/trace.log
```
