#!/bin/bash

logfile="$1"
output="powermethod.csv"

if [[ ! -f "$logfile" ]]; then
    echo "Log file not found!"
    exit 1
fi

echo "threads,resolution,iterations,eigenvalue,time" > "$output"

grep "^###" "$logfile" | sed 's/^### //' >> "$output"

echo "Data extracted and saved to $output"

