FROM ubuntu:xenial as builder

RUN apt-get update -y && apt-get install -y qt5-default qttools5-dev build-essential
RUN mkdir /pixspace

COPY ./Client-Only.pro /pixspace/
COPY ./Client /pixspace/Client
COPY ./Shared /pixspace/Shared

RUN cd /pixspace/ && mkdir build && cd build && qmake ../Client-Only.pro && make

FROM ubuntu:xenial

RUN apt-get update -y && apt-get install -y qt5-default
RUN mkdir /pixspace/
COPY --from=builder /pixspace/build/Client/ /pixspace/Client
COPY --from=builder /pixspace/build/Shared/ /pixspace/Shared

ENTRYPOINT [ "/pixspace/Client/pixspaceclient" ]