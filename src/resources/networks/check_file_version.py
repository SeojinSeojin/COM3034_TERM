import tensorflow as tf

# Path to your .pb file
pb_file_path = 'final_cnn_20_ck.pb'

# Load the graph from the .pb file
with tf.compat.v1.gfile.GFile(pb_file_path, 'rb') as f:
    graph_def = tf.compat.v1.GraphDef()
    graph_def.ParseFromString(f.read())

# Retrieve the TensorFlow version
tf_version = graph_def.versions.producer
print('TensorFlow version:', tf_version)
