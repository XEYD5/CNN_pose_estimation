import tensorflow as tf
import os

# Get the include directory
include_dir = tf.sysconfig.get_include()
print("Include directory:", include_dir)

# Get the library directory
lib_dir = tf.sysconfig.get_lib()
print("Library directory:", lib_dir)

# For TensorFlow 2.x, sometimes the library files are in a 'python' subdirectory
python_lib_dir = os.path.join(lib_dir, 'python')
if os.path.exists(python_lib_dir):
    lib_dir = python_lib_dir

print("Final Library directory:", lib_dir)
