# pull Ubunut image
FROM ubuntu:22.04

ENV DEBIAN_FRONTEND=noninteractive

# update and install OpenJDK
RUN apt-get update && apt install -y default-jdk default-jre wget

# set versions
ENV SPARK_VERSION=3.3.1
ENV HADOOP_VERSION=3.3.4

RUN mkdir -p /opt && cd /opt && wget -q http://archive.apache.org/dist/spark/spark-${SPARK_VERSION}/spark-${SPARK_VERSION}-bin-hadoop3.tgz && tar xf spark-${SPARK_VERSION}-bin-hadoop3.tgz && ln -s spark-${SPARK_VERSION}-bin-hadoop3 spark
RUN mkdir -p /opt && cd /opt && wget -q http://archive.apache.org/dist/hadoop/common/hadoop-${HADOOP_VERSION}/hadoop-${HADOOP_VERSION}.tar.gz && tar xf hadoop-${HADOOP_VERSION}.tar.gz && ln -s hadoop-${HADOOP_VERSION} hadoop

ENV PATH=$PATH:opt/spark/bin
