# ACA_Lab2
# Implementing Branch Predictors.
Advanced Computer Architecture Lab 2 source and necessary files are Included.

Here 1-bit, 2-bit and (2,2) bit branch predictors are implemented.

In addition to those predictors, (n,2) bit branch predictor is also implemented to reduce the mispredictions.

##How to build and run the source.

First build the code using cmake.

Then an Executable is created in cmake_build/Debug folder with the name "BPAnalysis"

Now we can run the program by executing the following command.

```
./cmake_build/Debug/BPAnalysis -i <Path to your trace file> -<# prediction algorithm number>b
```
Algorithm Numbers :
    1-bit Predictor - 1
    2-bit Predictor - 2
    (2,2)-bit Predictor - 3
    Custom predictor - 4
        Custom predictor is implelemented in a way that we can run (n,2) bit predictors. But default is (7,2) bit predictor.
        But if you need to run a different algorithm of please change the variable ```correlated_bits``` to a desired number.
