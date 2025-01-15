import os
import shutil
import cv2
import random

def get_folder(c):
    """
    Determines the folder path based on a given character c (R, L, F).
    Images are stored with the following probabilities:
      - 70% in 'training/' + c/
      - 15% in 'validation/' + c/
      - 15% in 'testing/'
    
    Args:
        c (str): A character representing the classification (R, L, F).

    Returns:
        str: The folder path where the file should be stored.
    """
    probability = random.random()  # Random float between 0 and 1
    if probability < 0.7:
        return f"training/{c}/"
    elif probability < 0.85:
        return f"validation/{c}/"
    else:
        return "testing/"

# Define paths
input_folder = "/home/jorge/Desktop/dataset_raw/aruco_failed/"  # Replace with your input folder path
output_folder = "/home/jorge/Desktop/dataset_ready/"  # Replace with your output folder path

# Classification keys and labels
classification_keys = {'r': 'R', 'l': 'L', 'f': 'F'}
history = []  # Stack to keep track of classified images

# Get sorted list of files
image_files = [f for f in sorted(os.listdir(input_folder)) if f.endswith(".png")]
current_index = 0

while current_index < len(image_files):
    filename = image_files[current_index]
    input_path = os.path.join(input_folder, filename)

    # Display the image
    image = cv2.imread(input_path)
    cv2.imshow("Image", image)
    print(f"Classifying image: {filename}")

    # Wait for key press
    key = cv2.waitKey(0)
    char = chr(key).lower()

    if char in classification_keys:
        label = classification_keys[char]
        old_name, ext = os.path.splitext(filename)

        # Remove the part '_N_0' from the filename (or any part matching '_N_x')
        name = old_name.split('_N_')[0]  # Keep everything before '_N_'
        new_filename = f"img_{name}_{label}{ext}"  # Append the label and the extension
        folder = "aruco_failed/"
        output_path = os.path.join(output_folder, folder)

        # Copy the file to the output folder
        shutil.copy(input_path, os.path.join(output_path, new_filename))
        print(f"Image classified as '{label}' and saved as {new_filename}")

        # Add to history and move to the next image
        history.append((current_index, output_path, new_filename))
        current_index += 1
    elif char == 'x':  # Go back one image
        if history:
            last_index, last_folder, last_file = history.pop()
            current_index = last_index
            # Remove the last copied file
            os.remove(os.path.join(last_folder, last_file))
            print(f"Removed classification for {last_file}. Going back to reclassify.")
        else:
            print("No previous image to go back to.")
    else:
        print("Invalid key! Use R, L, F, or X.")

# Cleanupf
cv2.destroyAllWindows()
