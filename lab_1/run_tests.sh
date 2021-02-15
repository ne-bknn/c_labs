#!/bin/bash

for file in test/cases/*; do
	cat "$file";
	./task.out < "$file"
	echo "========================"
done
