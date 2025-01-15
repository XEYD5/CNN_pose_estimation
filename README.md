# CNN Pose Estimation for Drones

## Project Overview
# CNN Pose Estimation for Drones

## Project Overview
This project focuses on drone pose estimation using a Convolutional Neural Network (CNN). The system is designed to estimate the pose of a drone in real-time, even while in flight.
Future work developments include improving result resolution and to incorporate on-the-go learning capabilities in future iterations

## Features
- **Real-Time Pose Estimation**: Utilizes a CNN to provide real-time pose estimation of general plain-like drones.
- **Robust to Distortions**: Tested with various levels of blur, darkness, and quality reduction to ensure robustness.
- **Open for Progress**: The project is set up for future improvements and additional features.

## Project Structure
- **src/**: Contains the source code for the project.
- **datasets/**: Includes various datasets used for training and testing the CNN under different distortions.
- **models/**: Pre-trained models and scripts for training new models.

## Installation
To set up the project, clone this repository and initialize the ROS workspace:

```bash
git clone https://github.com/XEYD5/CNN_pose_estimation.git
cd CNN_pose_estimation
catkin_make
```

## Usage
1. Launch the ROS workspace:
   ```bash
   source devel/setup.bash
   roslaunch <package_name> <launch_file>.launch
   ```
2. Run the CNN for pose estimation:
   ```bash
   python src/cnn_pkg/src/run_pose_estimation.py
   ```

## Results
The CNN has been tested extensively, showing robustness to various distortions.
   - 98.29% testing images
   - 96.61% aruco_failed images
   - even severe quality reduction doesnt affect the model
   - 0, 20, 50 % rises in error rate at 60, 70, 80 darkness
   - 0, 5, 10 % rises in error rate at level 7, 8, 9 blur



  lvl    lvl    lvl    er %
| dark | blur | qual | rslt |
|  10  |  10  |  10  |   1  |
|  90  |  10  |  10  |  59  |
|  10  |  90  |  10  |  16  |
|  90  |  90  |  10  |  59  |
|  10  |  10  |  90  |   2  |
|  90  |  10  |  90  |  59  |
|  10  |  90  |  90  |  17  |
|  90  |  90  |  90  |  59  |

## Future Work
- **Improving Estimations**: Current pose estimations are very basic, just to prove the concept.
- **On-the-Go Learning**: Incorporating live learning to adapt the CNN during flight.
- **Additional Features**: Enhancing the system with more robust algorithms and extending to other drone models.

## Contact
For any questions or suggestions, please contact Jorge Pinto Ferreira at jjb.ferreira@campus.fct.unl.pt .
