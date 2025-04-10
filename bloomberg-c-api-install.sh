#!/bin/bash
# Script to automate installation of Bloomberg C API for Linux
# Usage: ./install_bloomberg_c_api.sh [version]

set -e

# Default version if not specified
DEFAULT_VERSION="3.19.1"
VERSION=${1:-$DEFAULT_VERSION}

# Directories
INSTALL_DIR="/opt/bloomberg/blpapi"
TEMP_DIR="/tmp/blpapi_install"
REPO_URL="https://blpapi.bloomberg.com/repository/releases/python/simple/"

# Ensure script is run as root
if [ "$EUID" -ne 0 ]; then
  echo "Please run as root or with sudo"
  exit 1
fi

# Create necessary directories
mkdir -p "$TEMP_DIR"
mkdir -p "$INSTALL_DIR/include"
mkdir -p "$INSTALL_DIR/lib"

echo "Installing Bloomberg C API version $VERSION..."

# Get Python wheel package first (easiest way to download Bloomberg API)
echo "Downloading Bloomberg API Python wheel..."
cd "$TEMP_DIR"
if ! command -v pip &> /dev/null; then
    echo "Installing pip..."
    apt-get update && apt-get install -y python3-pip
fi

# Download the wheel file
pip download --no-deps --index-url=$REPO_URL "blpapi==$VERSION" -d "$TEMP_DIR"

# Extract the wheel file (which is essentially a zip file)
WHEEL_FILE=$(find "$TEMP_DIR" -name "blpapi-*.whl" | head -1)
if [ -z "$WHEEL_FILE" ]; then
    echo "Failed to download Bloomberg API wheel file"
    exit 1
fi

echo "Extracting Bloomberg API files from $WHEEL_FILE..."
unzip -o "$WHEEL_FILE" -d "$TEMP_DIR/extracted"

# Install C development dependencies
echo "Installing C development dependencies..."
apt-get update && apt-get install -y \
    build-essential \
    pkg-config \
    libxml2-dev

# Copy C header files to installation directory
echo "Installing Bloomberg API C headers..."
# C headers typically have a .h extension
find "$TEMP_DIR/extracted/blpapi" -name "*.h" -exec cp {} "$INSTALL_DIR/include/" \;

# Copy shared libraries to installation directory
echo "Installing Bloomberg API C libraries..."
find "$TEMP_DIR/extracted/blpapi" -name "*.so*" -exec cp -P {} "$INSTALL_DIR/lib/" \;

# Create symlinks for compatibility
ln -sf "$INSTALL_DIR/lib/libblpapi3_64.so" "/usr/lib/libblpapi3_64.so"
ln -sf "$INSTALL_DIR/lib/libblpapi3_64.so" "/usr/lib/libblpapi.so"

# Update the library cache
ldconfig

# Setup environment variables in profile.d
cat > /etc/profile.d/bloomberg.sh << EOF
export BLPAPI_ROOT="$INSTALL_DIR"
export LD_LIBRARY_PATH="\$LD_LIBRARY_PATH:$INSTALL_DIR/lib"
export C_INCLUDE_PATH="\$C_INCLUDE_PATH:$INSTALL_DIR/include"
export LIBRARY_PATH="\$LIBRARY_PATH:$INSTALL_DIR/lib"
EOF

chmod +x /etc/profile.d/bloomberg.sh
source /etc/profile.d/bloomberg.sh

# Add pkg-config support
cat > /usr/lib/pkgconfig/blpapi.pc << EOF
prefix=$INSTALL_DIR
exec_prefix=\${prefix}
libdir=\${prefix}/lib
includedir=\${prefix}/include

Name: blpapi
Description: Bloomberg C API
Version: $VERSION
Libs: -L\${libdir} -lblpapi3_64
Cflags: -I\${includedir}
EOF

# Create a simple C example to verify installation
mkdir -p "$INSTALL_DIR/examples/c"
cat > "$INSTALL_DIR/examples/c/hello_bloomberg.c" << 'EOF'
#include <stdio.h>
#include <stdlib.h>
#include <blpapi_session.h>
#include <blpapi_sessionoptions.h>

int main(void) {
    printf("Testing Bloomberg C API...\n");
    
    /* Initialize session options */
    blpapi_SessionOptions_t *sessionOptions = blpapi_SessionOptions_create();
    if (!sessionOptions) {
        fprintf(stderr, "Failed to create session options\n");
        return 1;
    }
    
    /* Set server host and port - these are example values */
    blpapi_SessionOptions_setServerHost(sessionOptions, "localhost");
    blpapi_SessionOptions_setServerPort(sessionOptions, 8194);
    
    printf("Bloomberg C API session options initialized successfully\n");
    
    /* Clean up */
    blpapi_SessionOptions_destroy(sessionOptions);
    
    printf("Bloomberg C API test complete\n");
    return 0;
}
EOF

# Create a makefile for the example
cat > "$INSTALL_DIR/examples/c/Makefile" << EOF
CC = gcc
CFLAGS = \$(shell pkg-config --cflags blpapi) -Wall -Wextra
LDFLAGS = \$(shell pkg-config --libs blpapi)

all: hello_bloomberg

hello_bloomberg: hello_bloomberg.c
	\$(CC) \$(CFLAGS) -o hello_bloomberg hello_bloomberg.c \$(LDFLAGS)

clean:
	rm -f hello_bloomberg

.PHONY: all clean
EOF

echo "Bloomberg C API $VERSION has been installed to $INSTALL_DIR"
echo ""
echo "For C development:"
echo "- Include path: -I$INSTALL_DIR/include"
echo "- Library path: -L$INSTALL_DIR/lib -lblpapi3_64"
echo "- Or use pkg-config: pkg-config --cflags --libs blpapi"
echo ""
echo "A simple example has been created in $INSTALL_DIR/examples/c"
echo "To compile it: cd $INSTALL_DIR/examples/c && make"
echo ""
echo "To use the Bloomberg API in your C application, include:"
echo "#include <blpapi_session.h>"
echo "#include <blpapi_sessionoptions.h>"
echo "And other relevant headers as needed"

# Clean up
echo "Cleaning up temporary files..."
rm -rf "$TEMP_DIR"

echo "Installation complete!"
