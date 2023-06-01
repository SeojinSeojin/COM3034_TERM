import sys

import tf2onnx.convert

# Path to your .pb file
pb_file_path = 'final_cnn_20_ck.pb/saved_model.pb'

# Path to save the ONNX file
onnx_file_path = 'final_cnn_20_ck.onnx'

# Convert .pb to ONNX
sys.argv = ['tf2onnx.convert', '--saved-model', pb_file_path, '--output', onnx_file_path, '--tag', 'serve']
tf2onnx.convert.main()