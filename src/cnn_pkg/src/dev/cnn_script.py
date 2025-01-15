import cv2
import tensorflow as tf
import numpy as np
from PIL import Image

class FramePredictor:
    def __init__(self, model_path):
        tf.config.set_visible_devices([], 'GPU')
        self.model = tf.keras.models.load_model(model_path)
        self.class_labels = ['Forward', 'Left', 'Right']

    def preprocess_image(self, frame):
        img = Image.fromarray(frame)
        img = img.resize((200, 200))
        img_array = np.array(img) / 255.0
        img_array = np.expand_dims(img_array, axis=0)
        return img_array

    def predict(self, frame, obj_detected):
        if obj_detected:
            input_data = self.preprocess_image(frame)
            predictions = self.model.predict(input_data)
            predicted_index = np.argmax(predictions[0])
            predicted_letter = self.class_labels[predicted_index]
            return predicted_letter
        return "No drone detected"

class ObjectDetector:
    def __init__(self, prototxt_path, model_path, confidence_threshold=0.5):#0.8):
        self.net = cv2.dnn.readNetFromCaffe(prototxt_path, model_path)
        self.class_names = ["background", "aeroplane", "bicycle", "bird", "boat", "bottle", "bus",
                            "car", "cat", "chair", "cow", "diningtable", "dog", "horse", "motorbike",
                            "person", "pottedplant", "sheep", "sofa", "train", "tvmonitor"]
        self.confidence_threshold = confidence_threshold

    def detect_objects(self, frame):
        input_blob = cv2.dnn.blobFromImage(frame, 0.007843, (300, 300), 127.5, False)
        self.net.setInput(input_blob, "data")
        detection = self.net.forward("detection_out")
        detection_mat = detection[0, 0, :, :]
        
        for i in range(detection_mat.shape[0]):
            confidence = detection_mat[i, 2]

            if confidence > self.confidence_threshold:
                object_class = int(detection_mat[i, 1])
                class_name = self.class_names[object_class]

                if class_name in ["aeroplane"]:#, "person"]:
                    obj_detected = True
                    x_left_bottom = int(detection_mat[i, 3] * frame.shape[1])
                    y_left_bottom = int(detection_mat[i, 4] * frame.shape[0])
                    x_right_top = int(detection_mat[i, 5] * frame.shape[1])
                    y_right_top = int(detection_mat[i, 6] * frame.shape[0])
                    
                    class_name = "Distance"
                    cv2.rectangle(frame, (x_left_bottom, y_left_bottom), (x_right_top, y_right_top), (0, 255, 0), 2)
                    label = f"{class_name}: {confidence:.2f}"
                    label_size, base_line = cv2.getTextSize(label, cv2.FONT_HERSHEY_SIMPLEX, 0.5, 1)
                    top = max(y_left_bottom, label_size[1])
                    cv2.rectangle(frame, (x_left_bottom, top - label_size[1]), (x_left_bottom + label_size[0], top + base_line), (255, 255, 255), cv2.FILLED)
                    cv2.putText(frame, label, (x_left_bottom, top), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 0, 0))
                    return frame, True

        return frame, False

def main():
    model_path = '/home/jorge/capture_ws/src/cnn_pkg/src/dev/my_model.h5'
    predictor = FramePredictor(model_path)
    prototxt_path = "/home/jorge/capture_ws/src/cnn_pkg/src/dev/MobileNetSSD_deploy.prototxt"
    model_path = "/home/jorge/capture_ws/src/cnn_pkg/src/dev/MobileNetSSD_deploy.caffemodel"
    detector = ObjectDetector(prototxt_path, model_path)

    cap = cv2.VideoCapture(0)
    while True:
        ret, frame = cap.read()
        if not ret:
            break
        #frame2, obj_detected = detector.detect_objects(frame)
        prediction = predictor.predict(frame, True)#obj_detected)
        cv2.putText(frame, f'Output: {prediction}', (10, 30), cv2.FONT_HERSHEY_SIMPLEX, 1, (255, 0, 0), 2, cv2.LINE_AA)
        cv2.imshow('Frame', frame)
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    cap.release()
    cv2.destroyAllWindows()

if __name__ == "__main__":
    main()
