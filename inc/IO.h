#ifndef 		IO_H_
	#define 	IO_H_

	#include <opencv2/opencv.hpp>

	#include <string>
	#include <vector>
	#include <iostream>

	class IO
	{
		private:
			std::vector<std::string>	windows;
			std::vector<int>		posX;
			std::vector<int>		posY;

		public:
						IO();
			virtual 		~IO();
			void			create_new_window(std::string);
			int			window_set_position(int, int, std::string);
			static int		load_img(const std::string, cv::Mat&);
			int			draw_img(cv::Mat&, std::string);
			int			add_trackbar(std::string, std::string, int*, int, void (*)(int, void*));
	};

#endif 			/* IO_H_ */
