"""
Resizes every image in a directory (including subdirectories) 
to half of its width and height.

Args:
    folder_path (str): Path to the folder containing images.
"""

import os
import cv2

def resize_images_in_folder(folder_path):
    for root, _, files in os.walk(folder_path):
        for file in files:
            if file.endswith((".png")): 
                file_path = os.path.join(root, file)
                image = cv2.imread(file_path)
                if image is None:
                    print(f"Could not open image: {file_path}")
                    continue
                height, width = image.shape[:2]
                new_width, new_height = width // 2, height // 2
                resized_image = cv2.resize(image, (new_width, new_height))
                cv2.imwrite(file_path, resized_image)
                print(f"{file_path}")

# Folder path
#folder_path = "/home/jorge/Desktop/dataset_ready/aruco_failed"
folder_path = ""
resize_images_in_folder(folder_path)
