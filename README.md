
# Build and Run

Open a terminal in the sample directory and execute the following command:

    mkdir build
    cd build
    cmake ..
    make
    ./ZED_Data_Collection
    
# Parameters

In SaveDepth.hpp
```cpp
const std::string logfile = "/home/nvidia/aprint.txt";//The path of the logfile
const std::string path = "/home/rk/Amy/mycode/ZED_dataset_collection/img/"; //The path to save the images
const int interval = 1; //The time interval of saving images: unit is second
```

# Reference

https://github.com/stereolabs/zed-opencv

# How to set the programme start at boot up
```sh
sudo cp ./ZED  /etc/init.d/ZED
sudo chmod +x /etc/init.d/ZED
sudo update-rc.d ZED defaults
```
ZED is the name of the script, if you need to save the data in SD card etc., you need to mount it before running the programme. 

in ZED
```sh
mount -t ntfs /dev/mmcblk1p1 /media/nvidia/ #The SD card to store data
/home/nvidia/amy/mycode/ZedDataCollection-master/build/ZED_Data_Collection #the place of the programme
```
