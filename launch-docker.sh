#! /bin/bash

# Perform host detection and act appropriately
if [ "$(uname -s)" == "Darwin" ]
then
    echo "Running on Mac"
    open -a XQuartz
    XVAR=$(ifconfig en0 | grep inet | awk '$1=="inet" {print $2}')
    xhost +
elif [ "$(uname -s)" == "Linux" ]
then
    echo "Running on Linux"
    XVAR=$DISPLAY
else
    echo "Not Mac or Linux, exiting."
    exit 1
fi

# Generate configuration file for use in Docker
touch config.dat
echo "Hostname:mysql-server" > config.dat
echo "Port:3306" >> config.dat
echo "Password:root" >> config.dat
echo "Database:pixspace" >> config.dat
echo "Username:root" >> config.dat

# Create a network just for pixspace
docker network create --subnet 172.20.0.0/16 --ip-range 172.20.240.0/20 pixspace-network

# Handle mysql container and wait 10 seconds after creation to populate
docker rm -f mysql-server
docker run -d --rm -e MYSQL_ROOT_PASSWORD=root --name mysql-server --network=pixspace-network --ip="172.20.240.3" mysql:5.7
sleep 10
docker exec -i mysql-server mysql -uroot -proot < db_scripts/mysql_db_create.sql

# Create pixspace containers
docker run -d --rm -e DISPLAY=$XVAR:0 --name pixspace-server -v $(PWD)/config.dat:/config.dat --network=pixspace-network --ip="172.20.240.2" pixspace:1.0 "/pixspace/build/Server/pixspaceserver"
docker run -d --rm -e DISPLAY=$XVAR:0 --name pixspace-client --network=pixspace-network pixspace:1.0 "/pixspace/build/Client/pixspaceclient"