FROM rikorose/gcc-cmake

RUN  mkdir /usr/src/myapp

COPY main.cpp /usr/src/myapp
COPY CMakeLists.txt /usr/src/myapp

WORKDIR /usr/src/myapp

RUN cmake .
RUN make

ENTRYPOINT [ "./connection" ]

