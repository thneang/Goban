#include <IO.h>
#include <Algorithms.h>

#include <time.h>

#include <opencv2/opencv.hpp>

void	on_morphology(int, void*);
void	on_threshold(int, void*);

cv::Mat in;
cv::Mat	src;
cv::Mat	sat;
cv::Mat	tre;
cv::Mat	mor;
cv::Mat	grp;
cv::Mat	hou;
IO	context;

int	saturation_index = 100;
int	threshold_index = 100;
int	morphology_index = 3;
int	hough_index = 0;

void	on_saturation(int, void*)
{
	Algorithms::saturation(src, sat, saturation_index);
	context.draw_img(sat, "1- saturation");
	//on_threshold(0, NULL);
}

void	on_threshold(int, void*)
{
	Algorithms::adaptive_threshold(sat, tre, threshold_index);
	context.draw_img(tre, "2- threshold");
	//on_morphology(0, NULL);
}

void	on_morphology(int, void*)
{
	if(morphology_index > 2)
	{
		Algorithms::morphology(tre, mor, morphology_index, threshold_index);
		context.draw_img(mor, "3- morphology");
	}
	else
	{
		mor = tre.clone();
		context.draw_img(mor, "3- morphology");
	}
	Algorithms::group(mor, grp);
	context.draw_img(grp, "4- group");
}


void	on_hough(int, void*)
{
	in = src.clone();
	Algorithms::hough(in, tre, hou, hough_index);
	// if lines are too thin use
	// Algorithms::hough(in, in, hou, hough_index);
	context.draw_img(in, "5- hough");
}

int	main(int ac, char** av)
{
	srand(time(NULL));
	int	gap = 100;

	IO::load_img("BDD/image10.jpg", src);
	
	in = src.clone();

	context.create_new_window("1- saturation");
	context.create_new_window("2- threshold");
	context.window_set_position(src.cols + gap, 0, "2- threshold");
	context.create_new_window("3- morphology");
	context.window_set_position(src.cols * 2 + gap, 0, "3- morphology");
	context.create_new_window("4- group");
	context.window_set_position(0, src.rows + gap, "4- group");
	context.create_new_window("5- hough");
	context.window_set_position(src.cols + gap, src.rows + gap, "5- hough");

	context.add_trackbar("saturation", "1- saturation", &saturation_index, 255, on_saturation);
	context.add_trackbar("threshold", "2- threshold", &threshold_index, 255, on_threshold);
	context.add_trackbar("morphology", "3- morphology", &morphology_index, 6, on_morphology);
	context.add_trackbar("intersections", "5- hough", &hough_index, 255, on_hough);

	Algorithms::saturation(src, sat, saturation_index);
	Algorithms::adaptive_threshold(sat, tre, threshold_index);
	Algorithms::adaptive_threshold(sat, mor, threshold_index);
	Algorithms::morphology(tre, mor, morphology_index, threshold_index);
	Algorithms::group(mor, grp);

	context.draw_img(src, "1- saturation");
	context.draw_img(src, "2- threshold");
	context.draw_img(src, "3- morphology");
	context.draw_img(src, "4- group");
	context.draw_img(src, "5- hough");
	
	cv::waitKey(0);
	return(0);
}
