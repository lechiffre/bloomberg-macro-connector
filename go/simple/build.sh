#!/bin/sh

export CGO_CXXFLAGS="-I../include -I../blpapi3_64/blpapi_cpp_3.25.3.1/include"
export CGO_LDFLAGS="-L../../lib -L../../blpapi3_64/blpapi_cpp_3.25.3.1/Linux/ -L.. -lblpapi3_64 -lblpconn -lblpconngo"
go build
