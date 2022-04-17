#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/opencv.hpp"
#include <iostream>
#include <stdio.h>
#include <time.h>

using namespace cv;
using namespace std;

#define DETEKSIORANGE 0

int main(int argc, char** argv)
{

    VideoCapture cap(1); 

    //Check camera available
    if(!cap.isOpened()) 
    {
        printf("Error opening camera!\n");
        return EXIT_FAILURE;
    }
  
    char key;
    int frameCount=0;
    int detectionCount=0;
    int rx = 0;
    int ry = 0;
    float totalWaktu;
    
    ///HSV Variable
	// int iLowH = 0;
    // int iHighH = 23;
    // int iLowS = 96; 
    // int iHighS = 159;
    // int iLowV = 120;
    // int iHighV = 255;
    // int erodehsv = 14;
    // int dilatehsv = 35;

    // int iLowH = 0;
    // int iHighH = 45;
    // int iLowS = 165; 
    // int iHighS = 255;
    // int iLowV = 165;
    // int iHighV = 255;
    // int erodehsv = 10;
    // int dilatehsv = 40;

    int iLowH = 5;
    int iHighH = 11;
    int iLowS = 66; 
    int iHighS = 255;
    int iLowV = 82;
    int iHighV = 255;
    int erodehsv = 10;
    int dilatehsv = 40;
    
    // Loads an image
    Mat src, circleDetect, orangeDetect;

    //Set serial comunication port
    // char serialPortFilename[] = "/dev/ttyACM0";

    while(1)
    {

        // FILE *serPort = fopen(serialPortFilename, "w");
        clock_t begin = clock();
        cap.read(src);

        frameCount++;
        circleDetect = src.clone();
        orangeDetect = src.clone();

        //Grayscale source image
        Mat gray;
        cvtColor(src, gray, COLOR_BGR2GRAY);
        Mat dst;
        equalizeHist(gray, dst);
        // medianBlur(gray, gray, 5);

        //HSV source image
        Mat imgHSV;
        cvtColor(src, imgHSV, COLOR_BGR2HSV);
        createTrackbar("LowH", "HSV", &iLowH, 179);	//Hue (0 - 179)
		createTrackbar("HighH", "HSV", &iHighH, 279);

		createTrackbar("LowS", "HSV", &iLowS, 255);	//Saturation (0 - 255)
		createTrackbar("HighS", "HSV", &iHighS, 255);

		createTrackbar("LowV", "HSV", &iLowV, 255);	//Value (0 - 255)
		createTrackbar("HighV", "HSV", &iHighV, 255);

		createTrackbar("erode", "HSV", &erodehsv, 255);	//Value (0 - 255)
		createTrackbar("dilate", "HSV", &dilatehsv, 255);
		inRange(imgHSV, Scalar(iLowH, iLowS, iLowV), Scalar(iHighH, iHighS, iHighV), imgHSV); //Threshold the image

		//morphological opening (remove small objects from the foreground)
		erode(imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(erodehsv, erodehsv)) );
		dilate( imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(dilatehsv, dilatehsv)) ); 

		//morphological closing (fill small holes in the foreground)
		dilate( imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(dilatehsv, dilatehsv)) ); 
		erode(imgHSV, imgHSV, getStructuringElement(MORPH_ELLIPSE, Size(erodehsv, erodehsv)) );

        //Circle detection
        vector<Vec3f> circles;
        GaussianBlur( gray, gray, Size( 9, 9 ), 0, 0);
        HoughCircles(imgHSV, circles, HOUGH_GRADIENT, 2,
                     imgHSV.rows/8,  // change this value to detect circles with different distances to each other
                     100, 50, 10, 400 // change the last two parameters
                // (min_radius & max_radius) to detect larger circles
        );

        //Draw border to circle detected
        for( size_t i = 0; i < circles.size(); i++ )
        {
            Vec3i c = circles[0];
            Point center = Point(c[0], c[1]);
            int radius = c[2];

            //Draw center point and border
            circle( circleDetect, center, 1, Scalar(0,100,100), 3, LINE_AA);
            circle( circleDetect, center, radius, Scalar(255,0,255), 3, LINE_AA);

            //Circle coordinates
            rx = c[0];
            ry = c[1];
            detectionCount++;
            break;
        }

        Mat dsts;
        cvtColor(dst, dsts, COLOR_GRAY2BGR);
        cvtColor(gray, gray, COLOR_GRAY2BGR);
        //Show image
        imshow("gray", gray);
        imshow("equalized", dst);
        imshow("detectedCircles", circleDetect);
        imshow("dsts", dsts);
        imshow("HSV", imgHSV);
        // imshow("Source", src);
        
        //Clock counter
        clock_t end = clock();
        float waktu = (float)(end-begin)*2/CLOCKS_PER_SEC;
        totalWaktu = totalWaktu + waktu;

        //Print detection data
        printf("Jumlah frame:%d  \t", frameCount);
        printf("Jumlah deteksi:%d \t", detectionCount);
        printf("x =%d\t", rx);
        printf("y =%d\t", ry);
        printf("t = %0.3f\n", waktu);

        //Reset ball coordinates
        rx = 0;
        ry = 0;
        
        //create a new String ROS message.
		//Message definition in this link http://docs.ros.org/api/std_msgs/html/msg/String.html
		// std_msgs::String msg;

		// //create a string for the data
		// std::stringstream ss;
		// sprintf(ss,"%d",rx);
		// //assign the string data to ROS message data field
		// msg.data = ss.str();

		char writeBuffer[50];

		key = cvWaitKey(1);
		if(key == 'x')
		{
			break;
		}
		else if(key == 'a')
		{
            printf("Snap stored!\n");
            // 
   //          // writeBuffer = 'a';
   //          char data = 'a';
   //          sprintf(writeBuffer,"%c", data);
			
			// //print the content of the message in the terminal
			// ROS_INFO("[Talker] I published %s\n", msg.data.c_str());

			// //Publish the message
			// chatter_publisher.publish(msg);
   //     		ros::spinOnce();
   //          // fprintf( serPort, "a");
        }

		// if(totalWaktu > 60)
		// {
		// 	break;
		// }

        // char writeBuffer[50];
        // 
        // fprintf(serPort, "x");
        // sprintf(writeBuffer,"%d",rx);
        // fprintf(serPort, "%s", writeBuffer);
        // fprintf(serPort, "y");
        // sprintf(writeBuffer,"%d",ry);
        // fprintf(serPort, "%s", writeBuffer);

        // fclose(serPort);
        // loop_rate.sleep();
    }
    float detectionPercentage = (float)frameCount;
    detectionPercentage = (float)detectionCount*100/detectionPercentage;
    printf("\n\n");
    printf("total frame = %d\n", frameCount);
    printf("total time = %f\n", totalWaktu);
    printf("fps = %f\n", (float)frameCount/totalWaktu);
    printf("detection percentage = %f%\n", detectionPercentage);
    return 0;
}