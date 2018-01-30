#ifndef __SAVE_DEPTH_HPP__
#define __SAVE_DEPTH_HPP__

#define NOMINMAX

#include <iomanip>
#include <signal.h>
#include <iostream>
#include <limits>
#include <thread>

#include <opencv2/opencv.hpp>
#include <sl/Camera.hpp>

const std::string helpString = "[d] Save Depth, [n] Change Depth format, [p] Save Point Cloud, [m] Change Point Cloud format, [q] Quit";
const std::string prefixPointCloud = "Cloud_"; // Default PointCloud output file prefix
const std::string prefixDepth = "/"; // Default Depth image output file prefix
const std::string prefixLeft = "/";
const std::string prefixRight = "/";
const std::string logfile = "/home/nvidia/Desktop/ZEDlog.txt";//The path of the logfile
const std::string path = "/media/nvidia/zed/img/"; //The path to save the images
const std::string pathleft = "/left/";
const std::string pathright = "/right/";
const std::string pathdepth = "/depth/";
const int interval = 1; //The time interval of saving images: unit is second

std::string getPointCloudFormatName(sl::POINT_CLOUD_FORMAT f);
std::string getDepthFormatName(sl::DEPTH_FORMAT f);

void savePointCloud(sl::Camera& zed, std::string filename);
void saveDepth(sl::Camera& zed, std::string filename);
void saveSbSImage(sl::Camera& zed, std::string filename);
void saveLeftImage(sl::Camera& zed, std::string filename);
void saveRightImage(sl::Camera& zed, std::string filename);
void processKeyEvent(sl::Camera& zed, char &key);

#endif
