#ifndef 		ALGORITHMS_H_
	#define 	ALGORITHMS_H_

	#include <opencv2/opencv.hpp>

	#include <iostream>
	#include <vector>

	#include <Tools.h>

	class Algorithms
	{
		private:
					Algorithms();
		virtual 		~Algorithms();
	
		public:
			static int	saturation(cv::Mat&, cv::Mat&, int);
			static int	adaptive_threshold(cv::Mat&, cv::Mat&, int);
			static int	morphology(cv::Mat&, cv::Mat&, int, int);
			static int	group(cv::Mat&, cv::Mat&);
			static int	hough(cv::Mat&, cv::Mat&, cv::Mat&, int);
			static int	ransac(std::vector<cv::Vec4i>, cv::Mat&);
	};

#endif 			/* ALGORITHMS_H_ */
