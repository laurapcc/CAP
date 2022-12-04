spark = SparkSession.builder.appName('spark').master('k8s://https://spark://spark-master:7077') \
.config("spark.driver.host", "http://192.168.49.2:30707/") \
.getOrCreate()