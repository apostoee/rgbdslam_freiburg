#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>


class ImageConverter
{
	ros::NodeHandle nh_;
	image_transport::ImageTransport it_;
	image_transport::Subscriber image_sub_;
	image_transport::Publisher image_pub_;
	image_transport::Subscriber depth_image_sub_;
	image_transport::Publisher depth_image_pub_;
  
	public:
		ImageConverter() : it_(nh_)
		{
			// Subscrive to input video feed and publish output video feed
			image_sub_ = it_.subscribe("/kinect/image", 1, 
			  &ImageConverter::imageCb, this);
			image_pub_ = it_.advertise("/image_converter/image", 1);
			
			depth_image_sub_ = it_.subscribe("/kinect/depth/image", 1, 
			  &ImageConverter::depthImageCb, this);
			depth_image_pub_ = it_.advertise("/image_converter/depth/image", 1);
		}

		~ImageConverter(){}

		void imageCb(const sensor_msgs::ImageConstPtr& msg)
		{
		cv_bridge::CvImagePtr cv_ptr;
		try
		{
		  cv_ptr = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
		}
		catch (cv_bridge::Exception& e)
		{
		  ROS_ERROR("cv_bridge exception: %s", e.what());
		  return;
		}


		// Output modified video stream
		image_pub_.publish(cv_ptr->toImageMsg());
		}
		
		void depthImageCb(const sensor_msgs::ImageConstPtr& msg)
		{
		cv_bridge::CvImagePtr cv_ptr;
		try
		{
		  cv_ptr = cv_bridge::toCvCopy(msg, "32FC1");
		}
		catch (cv_bridge::Exception& e)
		{
		  ROS_ERROR("cv_bridge exception: %s", e.what());
		  return;
		}


		// Output modified video stream
		depth_image_pub_.publish(cv_ptr->toImageMsg());
		}
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}
