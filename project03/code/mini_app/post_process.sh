#!/bin/bash

logfile="$1"
output="pde.csv"

if [[ ! -f "$logfile" ]]; then
    echo "Log file not found!"
    exit 1
fi

echo "threads,resolution,time" > "$output"

while read -r line; do
    if [[ "$line" == threads* ]]; then
        threads=$(echo "$line" | awk '{print $3}')
    fi
    if [[ "$line" == mesh* ]]; then
        resolution=$(echo "$line" | awk '{print $3 "x" $5}')
    fi

    if [[ "$line" == simulation* ]]; then
        time=$(echo "$line" | awk '{print $3}')
    fi

    if [[ "$line" == Goodbye* ]]; then
        echo "$threads,$resolution,$time" >> "$output"
    fi
done < "$logfile"

echo "Data extracted and saved to $output"

