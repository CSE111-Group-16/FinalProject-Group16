#!/usr/bin/env bash

if [ $# -lt 2 ]; then
    echo "Usage: $0 directory command [args...]" >&2
    exit 1
fi

directory="$1"
command="$2"
shift 2

# Check if directory exists
if [ ! -d "$directory" ]; then
    echo "Directory $directory does not exist" >&2
    exit 1
fi

# Check if command is provided
if [ -z "$command" ]; then
    echo "Please provide a command" >&2
    exit 1
fi

# Create a random temporary file
temp_1=$(mktemp /tmp/command_output.XXXXXX)
temp_2=$(mktemp /tmp/command_output.XXXXXX)

# Run the command with given arguments
"$command" "$@" < "$directory/0.in" > "$temp_1" 2> "$temp_2"

# Compare stdout and stderr with corresponding files
if ! diff --text "$directory/1.out" "$temp_1" >/dev/null 2>&1; then
    diff --text "$directory/1.out" "$temp_1"
    rm "$temp_1"
    rm "$temp_2"
    exit 1
fi

if ! diff --text "$directory/2.out" "$temp_2" >/dev/null 2>&1; then
    diff --text "$directory/2.out" "$temp_2"
    rm "$temp_1"
    rm "$temp_2"
    exit 1
fi

rm "$temp_1"
rm "$temp_2"
exit 0
