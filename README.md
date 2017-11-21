
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

