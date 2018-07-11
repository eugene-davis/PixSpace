#!/bin/bash  

docker build -f server.Dockerfile -t pixspace-server:1.0 .
docker build -f client.Dockerfile -t pixspace-client:1.0 .