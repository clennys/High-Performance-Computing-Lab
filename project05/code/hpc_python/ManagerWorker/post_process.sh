#!/bin/bash

INPUT_FILE="$1"

OUTPUT_FILE="$2"

TEMP_FILE="mpi_temp.csv"

echo "processes,tasks,time" > "$TEMP_FILE"

total_tasks=0
processes=0
execution_time=0

while IFS= read -r line; do
    if [[ $line == MPI\ initialized\ with* ]]; then
        processes=$(echo "$line" | awk '{print $4}')
        total_tasks=0 
    elif [[ $line == Run\ took* ]]; then
        execution_time=$(echo "$line" | awk '{print $3}')
    elif [[ $line == Process* ]]; then
        tasks=$(echo "$line" | awk '{print $5}')
        if [[ $tasks =~ ^[0-9]+$ ]]; then
            total_tasks=$((total_tasks + tasks))
        else
            echo "Error: Invalid task count in line: $line" >&2
            tasks=0
        fi
    elif [[ $line == Done.* ]]; then
        echo "$processes,$total_tasks,$execution_time" >> "$TEMP_FILE"
    fi
done < "$INPUT_FILE"

# Sort the file: Total Tasks (descending) and Processes (ascending)
tail -n +2 "$TEMP_FILE" | sort -t, -k2,2nr -k1,1n > "$OUTPUT_FILE"

# Add the header to the final file
{ echo "Processes,Total Tasks,Execution Time"; cat "$OUTPUT_FILE"; } > temp.csv && mv temp.csv "$OUTPUT_FILE"

# Clean up temporary file
rm "$TEMP_FILE"

echo "Sorted CSV file created: $OUTPUT_FILE"
