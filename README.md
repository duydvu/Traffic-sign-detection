# Traffic sign detection
Traffic sign detection application using OpenCV and Tiny-dnn

## Prerequisites
These instructions are for Ubuntu users. If you're a Windows guy, you could google how to install these package on your machine.
- GCC 6.3.0: 
```
sudo add-apt-repository ppa:ubuntu-toolchain-r/test
sudo apt-get update
sudo apt-get install gcc-6 g++-6
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-6 60 --slave /usr/bin/g++ g++ /usr/bin/g++-6
```
- Boost package:
```
sudo apt-get install libboost-all-dev
```
- OpenCV: [Installation](https://docs.opencv.org/master/d7/d9f/tutorial_linux_install.html)

## Installation
Clone the repository
```
git clone https://github.com/duyisking/Traffic-sign-detection
```
Clone the Tiny-dnn library
```
git clone https://github.com/tiny-dnn/tiny-dnn
```
Then copy tiny-dnn folder into Traffic-sign-detection, cd to Traffic-sign-detection and run
```
cmake ./
make
```
After compilation, you can run
```
./main
```
to see the magic! (Maybe not)


