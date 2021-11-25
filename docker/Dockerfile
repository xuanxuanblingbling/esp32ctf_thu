FROM ubuntu:16.04

RUN sed -i "s/http:\/\/archive.ubuntu.com/http:\/\/mirrors.tuna.tsinghua.edu.cn/g" /etc/apt/sources.list && \
    apt-get update 

RUN apt-get install -y software-properties-common && \
    apt-add-repository ppa:mosquitto-dev/mosquitto-ppa && \
    apt-get install -y mosquitto 
    
CMD ["/bin/sh","-c","service mosquitto start; while true ; do sleep 10; done"]

# docker build -t mqtt:v1 .
# docker run -p 1883:1883 -d mqtt:v1 