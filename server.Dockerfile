FROM ubuntu:xenial as builder

RUN apt-get update -y && apt-get install -y qt5-default qttools5-dev build-essential libmysqlclient20 libmysqlclient-dev libqt5sql5-mysql libssl-dev
RUN mkdir /pixspace

COPY ./Server-Only.pro /pixspace/
COPY ./Server /pixspace/Server
COPY ./Shared /pixspace/Shared

RUN cd /pixspace/ && mkdir build && cd build && qmake ../Server-Only.pro && make

FROM ubuntu:xenial

RUN apt-get update -y && apt-get install -y qt5-default libmysqlclient20 libqt5sql5-mysql mysql-client
RUN mkdir /pixspace/
COPY --from=builder /pixspace/build/Server/ /pixspace/Server
COPY --from=builder /pixspace/build/Shared/ /pixspace/Shared

ENTRYPOINT [ "/pixspace/Server/pixspaceserver" ]