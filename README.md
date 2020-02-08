# RobotVision
#### UCF CAP4453 Robot Vision with Dr. Niels Da Vitoria Lobo - Fall 2019

Respository contains my implementations of [Sobel](https://en.wikipedia.org/wiki/Sobel_operator) and [Canny](https://en.wikipedia.org/wiki/Canny_edge_detector) Edge Detection algorithms 

## Sobel

Compile `m_sobel.c` and run using:

```
$sobel.exe inputName.pgm (int) lowThresh (int)highThresh
```
Example usage:
```
$sobel.exe garb34.pgm 20 120
```

## Canny
Compile `m_canny.c` and run using:
```
$m_canny.exe inputFile.pgm out1.pgm out2.pgm out3.pgm (int)sigmaThreshold (int)percentThreshold
```

Example usage:
```
$m_canny.exe garb34.pgm out1.pgm out2.pgm out3.pgm 1 10
```

## Notes
   * `m_sobel.c` and `m_canny.c` are intended to run on a 256x256px grayscale image in .pgm format.  
   * Compiled and ran on windows, never tested on Ubuntu/Mac so when compiling or running YMMV

## Sobel And Canny Results
### Input Image
![Sobel Input](https://github.com/maslychm/RobotVision/raw/master/repository_resources/SobelInput.PNG)

### Sobel Final Output
![Sobel Output](https://github.com/maslychm/RobotVision/raw/master/repository_resources/SobelOutput.PNG)

### Canny Marking Candidates
![Canny Candidates](https://github.com/maslychm/RobotVision/raw/master/repository_resources/CannyCandidates.PNG)

### Canny Final Output
![Canny Output](https://github.com/maslychm/RobotVision/raw/master/repository_resources/CannyFinal.PNG)