# RobotVision
## UCF CAP4453 Robot Vision - Fall 2019

### Assignment 1: Sobel and Canny algorithms

`m_sobel.c` and `m_canny.c` are intended to run on a 256x256px grayscale image in .pgm format

#### For Sobel

Compile `m_sobel.c` and run using:

```
$sobel.exe inputName.pgm (int) lowThresh (int)highThresh

Example:

$sobel.exe garb34.pgm 20 120
```

#### For Canny

Compile `m_canny.c` and run using:

```
$m_canny.exe inputFile.pgm out1.pgm out2.pgm out3.pgm (int)sigmaThreshold (int)percentThreshold

Example:

$m_canny.exe garb34.pgm out1.pgm out2.pgm out3.pgm 1 10
```