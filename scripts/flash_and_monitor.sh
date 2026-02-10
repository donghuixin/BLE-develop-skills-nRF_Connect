#!/bin/bash

# Check if a build directory is provided
if [ -z "$1" ]; then
    BUILD_DIR="build"
else
    BUILD_DIR="$1"
fi

echo "Flashing from $BUILD_DIR..."
west flash -d $BUILD_DIR

if [ $? -eq 0 ]; then
    echo "Flash successful. Starting monitor..."
    # Attempt to open serial monitor. 
    # Note: 'west espressif monitor' is specific, standard west doesn't have a generic monitor.
    # We'll try to find a connected JLink/COM port or use a generic tool if available.
    # For NCS, RTT is common.
    
    echo "Starting JLink RTT Viewer or Serial Monitor..."
    # Placeholder for simple cat or screen if device is known, strictly speaking 'west' doesn't have a generic 'monitor' command for all boards.
    # But usually 'west build -t run' can flash and run.
    # Let's try to use JLinkRTTClient if available for RTT
    
    if command -v JLinkRTTClient &> /dev/null; then
        JLinkRTTClient
    else
        echo "JLinkRTTClient not found. Please open your preferred serial monitor."
    fi
else
    echo "Flash failed."
fi
