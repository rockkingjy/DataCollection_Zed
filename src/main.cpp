///////////////////////////////////////////////////////////////////////////
//
// Copyright (c) 2017, STEREOLABS.
//
// All rights reserved.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
///////////////////////////////////////////////////////////////////////////

/***********************************************************************************************
 ** This sample demonstrates how to use the ZED SDK with OpenCV. 					  	      **
 ** Depth and images are captured with the ZED SDK, converted to OpenCV format and displayed. **
 ***********************************************************************************************/

 // ZED includes
#include <sl/Camera.hpp>

// OpenCV includes
#include <opencv2/opencv.hpp>

// Sample includes
#include <SaveDepth.hpp>

//timer
#include <iostream>
#include <cstdio>
#include <ctime>

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>

using namespace sl;

cv::Mat slMat2cvMat(Mat& input);
void printHelp();


int main(int argc, char **argv) {
    int ret;//return for system commands
    //logfile remove old one and create new one;
    std::ifstream fin(logfile.c_str());
    if (fin) {
        std::cout << "deleting file..." << std::endl;
        ret = system("rm /home/nvidia/Desktop/ZEDlog.txt");
    }
    std::ofstream filestream(logfile.c_str());
    filestream<<"Start running ZED programme..."<<std::endl;

    //Waiting for boot up
    std::clock_t start;
    double duration = 0;

    start = std::clock();
    while(duration < 30){
    	duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
    }
    filestream << "Duration: "<< duration <<std::endl;

    //mount the sd card
    ret = system("mkdir /media/nvidia/zed/");
    ret = system("umount /dev/mmcblk1p1");
    ret = system("mount /dev/mmcblk1p1 /media/nvidia/zed/");
    while (ret != 0) {
        ret = system("mount /dev/mmcblk1p1 /media/nvidia/zed/");
        filestream<<"SD card mount failed."<<std::endl;
    }
    filestream<<"SD card mounted to /media/nvidia/zed/"<<std::endl;

    //imgfile for store the images
    DIR* dir = opendir(path.c_str());
    if (dir)
    {
    /* Directory exists. */
        filestream<<"Folder Img/ already exists."<<std::endl;
        closedir(dir);
    } else if (ENOENT == errno)
    {
    /* Directory does not exist. */
        filestream<<"Folder Img/ does not exist."<<std::endl;
        mkdir(path.c_str(), 0777);
        filestream<<"Folder Img/ created."<<std::endl;
    } else {
    /* opendir() failed for some other reason. */
        filestream<<"Error in Folder Img/."<<std::endl;
    }

    // Create a ZED camera object
    Camera zed;
    InitParameters init_params;
    init_params.camera_resolution = RESOLUTION_HD1080;
    init_params.depth_mode = DEPTH_MODE_PERFORMANCE;
    init_params.coordinate_units = UNIT_METER;

    // Open the camera
    filestream<<"Opening camera..."<<std::endl;
    ERROR_CODE err = zed.open(init_params);
    if (err != SUCCESS) {
        printf("%s\n", errorCode2str(err).c_str());
        filestream<<errorCode2str(err).c_str()<<std::endl;
        zed.close();
        err = zed.open(init_params);
        //return 1; // Quit if an error occurred
    }
    filestream<<"Camera open succeed!"<<std::endl;	

    // Set runtime parameters after opening the camera
    RuntimeParameters runtime_parameters;
    runtime_parameters.sensing_mode = SENSING_MODE_STANDARD;

    // Prepare new image size to retrieve half-resolution images
    Resolution image_size = zed.getResolution();
    int new_width = image_size.width;// / 2;
    int new_height = image_size.height;// / 2;

    // To share data between sl::Mat and cv::Mat, use slMat2cvMat()
    // Only the headers and pointer to the sl::Mat are copied, not the data itself
    Mat image_zed(new_width, new_height, MAT_TYPE_8U_C4);
    cv::Mat image_ocv = slMat2cvMat(image_zed);
    Mat depth_image_zed(new_width, new_height, MAT_TYPE_8U_C4);
    cv::Mat depth_image_ocv = slMat2cvMat(depth_image_zed);
    Mat point_cloud;

    // Loop until 'q' is pressed
    char key = ' ';
    int count_file = 0;
    int count_save = 0;

    // Check whether the sub folder is exist. 
    std::string filefolder = path + std::to_string(count_file);
    while(std::ifstream(filefolder)) {
        std::cout<<"Folder "<<filefolder<<" exists."<<std::endl;
	filestream<<"Folder"<<filefolder<<" exists."<<std::endl;
        count_file += 1;
        filefolder = path + std::to_string(count_file);
    }            
    mkdir(filefolder.c_str(), 0777);
    std::cout<<"Folder "<<filefolder<<" created."<<std::endl;
    filestream<<"Folder"<<filefolder<<" created."<<std::endl;

    while (key != 'q') {
        if (zed.grab(runtime_parameters) == SUCCESS) {
            start = std::clock();
            while(duration < interval){
                duration = ( std::clock() - start ) / (double) CLOCKS_PER_SEC;
            }
            std::cout << "Duration: "<< duration <<std::endl;
    	    //filestream << "Duration: "<< duration <<std::endl;
	        duration = 0;
            //save images
            saveLeftImage(zed, filefolder.c_str() + prefixLeft + std::to_string(count_save) + std::string(".png"));
            saveDepth(zed, filefolder.c_str() + prefixDepth + std::to_string(count_save));           
    	    filestream << "Image "<< count_save << " save into folder: "<< filefolder <<std::endl;
            count_save++;
        } else {
	    	filestream<<"ZED grab fail!"<<std::endl;
	    	zed.close();
	    	err = zed.open(init_params);
    	    if (err != SUCCESS) {
        	    printf("%s\n", errorCode2str(err).c_str());
        	    filestream<<errorCode2str(err).c_str()<<std::endl;
        	    zed.close();
        	    err = zed.open(init_params);
    	    }
	    }
    }
    zed.close();
    return 0;
}

/**
* Conversion function between sl::Mat and cv::Mat
**/
cv::Mat slMat2cvMat(Mat& input) {
    // Mapping between MAT_TYPE and CV_TYPE
    int cv_type = -1;
    switch (input.getDataType()) {
        case MAT_TYPE_32F_C1: cv_type = CV_32FC1; break;
        case MAT_TYPE_32F_C2: cv_type = CV_32FC2; break;
        case MAT_TYPE_32F_C3: cv_type = CV_32FC3; break;
        case MAT_TYPE_32F_C4: cv_type = CV_32FC4; break;
        case MAT_TYPE_8U_C1: cv_type = CV_8UC1; break;
        case MAT_TYPE_8U_C2: cv_type = CV_8UC2; break;
        case MAT_TYPE_8U_C3: cv_type = CV_8UC3; break;
        case MAT_TYPE_8U_C4: cv_type = CV_8UC4; break;
        default: break;
    }

    // Since cv::Mat data requires a uchar* pointer, we get the uchar1 pointer from sl::Mat (getPtr<T>())
    // cv::Mat and sl::Mat will share a single memory structure
    return cv::Mat(input.getHeight(), input.getWidth(), cv_type, input.getPtr<sl::uchar1>(MEM_CPU));
}

