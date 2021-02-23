#!/bin/bash

set -Eeuo pipefail

for f in $(ls test/cases); do
	echo "testcase: $f"
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes \
	         --verbose --log-file="logs/valgrind-out-$f.txt" ./task.out < "test/cases/$f" || true

	if grep -q 'no leaks are possible' < "logs/valgrind-out-$f.txt"; then
		mv "logs/valgrind-out-$f.txt" "logs/valgrind-out-$f-clean.txt"
	fi
done
