FROM pla/spark_base:latest
ADD worker.sh /root
CMD ["/root/worker.sh"]