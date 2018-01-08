#include <IO.h>

/****************************************************************************************/
/*	NAME 	=									*/
/*	ARGS	=									*/
/*	RET	=									*/
/*	INFO	=									*/
/****************************************************************************************/

	IO::IO()
{
	
}

	IO::~IO()
{
	std::vector<std::string>::iterator	it;

	for(it = windows.begin(); it != windows.end(); ++it)
	{
		cv::destroyWindow((*it));
	}
}

void 	IO::create_new_window(std::string winName)
{
	windows.push_back(winName);
	posX.push_back(0);
	posY.push_back(0);
	cv::namedWindow(winName, cv::WINDOW_AUTOSIZE);
}

int	IO::window_set_position(int x, int y, std::string winName)
{
	for(unsigned int i = 0; i < windows.size(); ++i)
	{
		if(0 == windows[i].compare(winName))
		{	
			if((x >= 0) && (y >= 0))
			{
				posX[i] = x;
				posY[i] = y;
				return(1);
			}
			else
			{
				std::cerr << "Error - window_set_position | wrong coordinates." << std::endl;
				return(0);
			}
		}
	}
	std::cerr << "Error - window name " << winName << " was not created before." << std::endl;
	return(0);
}

/****************************************************************************************/
/*	NAME 	=	load_img							*/
/*	ARGS	=	- path = path of the image to be loaded				*/
/*			- dst = Matrix where we will store all pixels from the image	*/
/*	RET	=	- 1 = success							*/
/*			- 0 = failure							*/
/*	INFO	=	open an image and store each pixel into a matrix.		*/
/****************************************************************************************/

int	IO::load_img(const std::string path, cv::Mat& dst)
{
	dst = cv::imread(path, CV_LOAD_IMAGE_COLOR);
	if(dst.empty())
	{
		std::cerr << "Error - while loading " << path << "." << std::endl; 
		return(0);
	}
	return(1);
}

/****************************************************************************************/
/*	NAME 	=	draw_img							*/
/*	ARGS	=	- src = the image to be drawn					*/
/*			- winName = the window where we will draw the image		*/
/*	RET	=	- 1 = success							*/
/*			- 0 = failure							*/
/*	INFO	=	search the window name corresponding and draw into it the image.*/
/****************************************************************************************/

int	IO::draw_img(cv::Mat& src, std::string winName)
{
	std::vector<std::string>::iterator	it;

	if(src.empty())
	{
		std::cerr << "Error - image is empty." << std::endl; 
		return(0);
	}
	for(unsigned int i = 0; i < windows.size(); ++i)
	{
		if(0 == windows[i].compare(winName))
		{	
			cv::moveWindow(windows[i], posX[i], posY[i]); 
			cv::imshow(windows[i], src);
			return(1);
		}
	}
	std::cerr << "Error - window name " << winName << " was not created before." << std::endl; 
	return(0);
}

/****************************************************************************************/
/*	NAME 	=									*/
/*	ARGS	=									*/
/*	RET	=									*/
/*	INFO	=									*/
/****************************************************************************************/

int	IO::add_trackbar(std::string name, std::string winName, int* index, int max, void (*func)(int, void*))
{
	std::vector<std::string>::iterator	it;

	for(it = windows.begin(); it != windows.end(); ++it)
	{
		if(0 == (*it).compare(winName))
		{	
			cv::createTrackbar(name, winName, index, max, func);
			return(1);
		}
	}
	std::cerr << "Error - window name " << winName << " was not created before." << std::endl; 
	return(0);
} 














