#!/bin/bash

if [[ "$1" == *"--stable"* ]]; then
    echo "COMPILING CZECHTINA"
    cc -O1 -w stable/main.c -o czechtina
    # Your code for handling the "--stable" argument goes here
    echo "Installing Stable version"
    # move the binary to /usr/local/bin
    sudo mv czechtina /usr/local/bin
    exit 0  # Exit with a non-zero exit code
elif [[ "$1" == *"--unstable"* ]]; then
    echo "COMPILING CZECHTINA"
    cc -O1 -w current/main.c -o czechtina
else
    echo "Usage: $0 [--stable] [--unstable]"
    exit 1  # Exit with a non-zero exit code
fi

echo "Building unstable version"
./czechtina

echo "Do you want to install the unstable version? (y/n)"
read -r install_unstable
if [[ "$install_unstable" == "y" ]]; then
    echo "moving unstable version to /usr/local/bin"
    sudo mv czechtina /usr/local/bin
fi
