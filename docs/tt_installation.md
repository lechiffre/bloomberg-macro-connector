# BlpConn: TT Installation

2025-05-12

This note describe step by step the process to install BlpConn library in the TT server.
The TT Server has a CentOS 7, which has been discontinued. Besides that, it has a special
configuration. Therefore, support for it is only provided by Trading Technologies.

## Github Repository Connection

The access to the Github repository was set up using SSH keys. Doing that, the repository
was cloned:

    git clone git@github.com:jailop/bloomberg-geco-connector.git
    

## Worling directory

The local copy of the repository is located in `/home/axssuperuser/bloomberg-geco-connector`.

It is assumed that all the following commands are executed from that directory,
unless otherwise stated.
    
## Configuration

A zip file with the needed configuration files was copied in the TT server: `config.zip`.

    unzip config.zip
    
When that file was uncompressed, the following files were created:

    ./config.json
    ./credentials/9F8E5FD041B209F39399EDB2E4120F53.pk12
    ./credentials/rootCertificate.pk7

## Flatbuffer Bindings

The package `flatbuffers` is not available in the TT server nor in the repositories it has access to.
It was intented to build from source, in the folder `/home/axssuperuser/build/flatbuffers/`, but
that failed.

Therefore, the `flatbuffers` generated files in the development machine were included in the source
code repository, in order to avoid their generation in the TT server. It was only needed to update
the repoitory:

    git pull
    
## CMake command

The default `cmake` version installed in the TT server was 2.8. The building
script is for CMake 3.17. That package was installed in the TT server:

    sudo yum install cmake3
    
Therefore, any time that CMake needs to be invoked, the command is `cmake3` instead of `cmake`.

## GTest

It was not possible to setup appropiately the `gtest` library in the TT server. For that reason, building
testing files has been omitted in the TT server.

## BLPAPI library

The `blpapi` library was copied in the following address: `/usr/local/lib/libblpapi3_64.so`

## Environment variables

Because the TT server uses `devtoolset_9`, it is needed to pass as environment variables the location
of the C and C++ compilers. The following variables were exported, including one related to the location of
the BLPAPI library:

    export CXX=/opt/rh/devtoolset-9/root/bin/g++
    export CC=/opt/rh/devtoolset-9/root/bin/gcc
    export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH
    
## CMake configuration file

The configuration file was adapted to run properly in the TT server. Changes included:

- The location of the `blpapi` library.
- Commenting the section to build the test programs
- Adding the path for the JSON library (used to read the configuration file)

This adapted version of the configuration file has been uploaded to the repository.

## Building the C++ library and programs

In order to build the C++ library and the programs:

    mkdir build
    cd build
    cmake3 ..
    cmake3 --build .
    cmake3 --install .
    
These new files were generated:

    ./lib/libblpconn.a
    ./bin/cli
    ./bin/deserialize
    ./bin/preliminar
    ./bin/simple
    
## SWIG C++/Go Wrappers

Given that an updated version of SWIG is already installed in the TT server, it
was not need to do any additional step to generate the bindings for the Go
language:

    cd swig
    make
    
These new files were generated:

    ./lib/libblpconngo.a
    ./go/blpconngo.go
    ./docs/go-api.txt
    
## Generating Go binaries

To generate the Go example binaries, as additional step it is need just to
updated the module information, using `go mod tidy`:

    cd go
    go mod tidy
    sh install.sh
    
These new files were generated:

    ./go/bin/cli
    ./go/bin/simple
    
## Running examples

To run the examples, given that the configuration file is located in the
default directory: `/home/axssuperuser/bloomberg-geco-connector`, the binaries
should be run from there:

    ./bin/simple
    ./bin/cli
    ./bin/deserialize tests/fbbin/fb_000001.bin
    ./go/bin/simple
    ./go/bin/cli
