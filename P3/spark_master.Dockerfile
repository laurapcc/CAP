FROM pla/spark_base:latest
ADD master.sh /root
CMD ["/root/mashter.sh"]