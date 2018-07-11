FROM ubuntu:xenial

RUN apt-get update -y && apt-get install -y qt5-default qttools5-dev build-essential libmysqlclient20 libmysqlclient-dev libqt5sql5-mysql libssl-dev mysql-client
RUN mkdir /pixspace

COPY ./PixelSpace.pro /pixspace/
COPY ./Client /pixspace/Client
COPY ./Server /pixspace/Server
COPY ./Shared /pixspace/Shared
COPY ./Test /pixspace/Test

RUN cd /pixspace/ && mkdir build && cd build && qmake ../PixelSpace.pro && make