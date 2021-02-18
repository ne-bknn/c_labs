#!/bin/bash

set -Eeuo pipefail

echo "" > "logs/$1_log.txt"

for f in $(ls test/cases); do
	echo "testcase: $f"
	./"$1" < "test/cases/$f" | tee -a logs/"$1_log.txt" || true
done
