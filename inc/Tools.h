#ifndef 		TOOLS_H_
	#define 	TOOLS_H_

	#include <opencv2/opencv.hpp>

	#include <iostream>
	#include <vector>

	class Tools
	{
		private:
					Tools();
		virtual 		~Tools();
	
		public:
			static int	prodVect(cv::Vec3i, cv::Vec3i, cv::Vec3i&);
			static int	prodScal(cv::Vec3i, cv::Vec3i, float&);
	};

#endif 			/* TOOLS_H_ */
