#include <Algorithms.h>

int	Algorithms::saturation(cv::Mat& src, cv::Mat& dst, int saturation_index)
{
	if(src.empty())
	{
		std::cerr << "error - saturation | image is empty." << std::endl;
		return(0);
	}
	dst = src.clone();
	int	x = 0;
	int	y = 0;
	int	medium = 0;
	int	saturation = 0;

	for(y = 0; y < dst.rows; y++)
	{
		for(x = 0; x < dst.cols; x++)
		{
			medium = (dst.at<cv::Vec3b>(y, x)[0] +  dst.at<cv::Vec3b>(y, x)[1] + dst.at<cv::Vec3b>(y, x)[2]) / 3;
			saturation = std::abs(dst.at<cv::Vec3b>(y, x)[0] - medium) 
					+ std::abs(dst.at<cv::Vec3b>(y, x)[1] - medium)
					+ std::abs(dst.at<cv::Vec3b>(y, x)[2] - medium);
			if(saturation > saturation_index)
			{
				dst.at<cv::Vec3b>(y, x)[0] = 128;
				dst.at<cv::Vec3b>(y, x)[1] = 128;
				dst.at<cv::Vec3b>(y, x)[2] = 128;
			}
		}
	}
	return(1);
}

int	Algorithms::adaptive_threshold(cv::Mat& src, cv::Mat& dst, int threshold_index)
{
	int	sizeX;
	int	sizeY;
	int	A;
	int	B;
	int	C;
	float 	median;
	cv::Mat	tmp;

	if(src.empty())
	{
		std::cerr << "error - adaptive_threshold | image is empty." << std::endl;
		return(0);
	}
	dst = src.clone();
	tmp = src.clone();
	cv::cvtColor(src, dst, CV_BGR2GRAY);
	cv::cvtColor(src, tmp, CV_BGR2GRAY);
	sizeX = dst.rows;
	sizeY = dst.cols;
	for(int x = 0; x < sizeX; ++x)
	{
		for(int y = 0; y < sizeY; ++y)
		{
			if((x > 0) && (y > 0))
			{
				A = tmp.at<unsigned char>(x, y);	// pixel courant
				B = tmp.at<unsigned char>(x - 1, y);	// pixel en haut
				C = tmp.at<unsigned char>(x, y - 1);	// pixel a gauche
	      			median = (A + B + C) / 3;
	      			if(median < threshold_index)
					dst.at<unsigned char>(x, y) = 0;
				else
					dst.at<unsigned char>(x, y) = 255;
			}
		}
	}
	return(1);
}

int	Algorithms::morphology(cv::Mat& src, cv::Mat& dst, int morphology_index, int threshold_index)
{
	if(src.empty())
	{
		std::cerr << "error - morphology | image is empty." << std::endl;
		return(0);
	}
	dst = src.clone();
	int 	morph_size = morphology_index;
	cv::Mat element = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(2 * morph_size + 1, 2 * morph_size + 1), cv::Point(morph_size, morph_size));
	if(127 > threshold_index)
		cv::morphologyEx(dst, dst, cv::MORPH_CLOSE, element, cv::Point(3, 3), 1);
	else	
		cv::morphologyEx(dst, dst, cv::MORPH_OPEN, element, cv::Point(3, 3), 1);
	return(1);
}

int	Algorithms::group(cv::Mat& src, cv::Mat& dst)
{
	if(src.empty())
	{
		std::cerr << "error - group | image is empty." << std::endl;
		return(0);
	}
	dst = src.clone();
	std::vector<std::vector<cv::Point> >	contours;
	std::vector<cv::Vec4i> 			hierarchy;
	std::vector<cv::Vec3f> 			vecCircles;               
	std::vector<cv::Vec3f>::iterator 	itrCircles;

	cv::findContours(dst, contours, hierarchy, CV_RETR_TREE,  CV_CHAIN_APPROX_SIMPLE, cv::Point(0, 0));

	std::vector<std::vector<cv::Point> >	contours_poly(contours.size());
     	std::vector<cv::Rect> 			boundRect(contours.size());
     	std::vector<cv::Point2f>		center(contours.size());
     	std::vector<float>			radius(contours.size());

     	for(unsigned int i = 0; i < contours.size(); i++)
     	{ 
        	cv::approxPolyDP(cv::Mat(contours[i]), contours_poly[i], 3, true);
         	boundRect[i] = boundingRect(cv::Mat(contours_poly[i]));
	}
	
	cv::Mat drawing = cv::Mat::zeros(dst.size(), CV_8UC3);
     	for(unsigned int i = 0; i < contours.size(); i++)
     	{
         	cv::Scalar color = cv::Scalar(255, 0, 255);
        	cv::drawContours(drawing, contours_poly, i, color, 1, 8, std::vector<cv::Vec4i>(), 0, cv::Point());
		cv::rectangle(drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);          
	}
	dst = drawing.clone();
	return(1);
}

int	Algorithms::hough(cv::Mat& in, cv::Mat& src, cv::Mat& dst, int hough_index)
{
	if(src.empty())
	{
		std::cerr << "error - hough | image is empty." << std::endl;
		return(0);
	}
	dst = src.clone();
	Canny(src, dst, 50, 200, 3);
	std::vector<cv::Vec2f> lines;
  	HoughLines(dst, lines, 1, CV_PI/180, 100, 0, 0 );
	std::vector<cv::Vec4i>	l;
  	for(size_t i = 0; i < lines.size(); i++)
  	{
     		float rho = lines[i][0];
		float theta = lines[i][1];
     		cv::Point pt1, pt2;
    		double a = cos(theta), b = sin(theta);
     		double x0 = a*rho, y0 = b*rho;
     		pt1.x = cvRound(x0 + 1000*(-b));
     		pt1.y = cvRound(y0 + 1000*(a));
     		pt2.x = cvRound(x0 - 1000*(-b));
     		pt2.y = cvRound(y0 - 1000*(a));
		cv::Vec4i	tmp = cv::Vec4i(pt1.x, pt1.y, pt2.x, pt2.y);
		l.push_back(tmp);     		
	//line(in, pt1, pt2, cv::Scalar(0,0,255), 1, CV_AA);
  	}
	ransac(l, in);
	return(1);
}

static int	make_line(cv::Vec4i line, cv::Vec3i& out)
{
	cv::Vec3i	a;
	cv::Vec3i	b;

	a = cv::Vec3i(line[0], line[1], 1);
	b = cv::Vec3i(line[2], line[3], 1);
	Tools::prodVect(a, b, out);
	return(1);
}

int	Algorithms::ransac(std::vector<cv::Vec4i> lines, cv::Mat& in)
{
	cv::Vec4i		l1;
	cv::Vec4i		l2;
	cv::Vec4i		li;
	cv::Vec3i		line1;
	cv::Vec3i		line2;
	cv::Vec3i		linei;
	cv::Vec3i		x;
	std::vector<cv::Vec4i>	group;
	float			prodScalar;

	l1 = lines[rand() % lines.size() - 1];
	l2 = lines[rand() % lines.size() - 1];
	line(in, cv::Point(l1[0], l1[1]), cv::Point(l1[2], l1[3]), cv::Scalar(0,255,0), 2, CV_AA);
	line(in, cv::Point(l2[0], l2[1]), cv::Point(l2[2], l2[3]), cv::Scalar(255,0,0), 2, CV_AA);
	make_line(l1, line1);
	make_line(l2, line2);	
	// intersection x = line1 x line2
	Tools::prodVect(line1, line2, x);
	if(x[2] > 1)
	{
		x[0] = x[0] / x[2];
		x[1] = x[1] / x[2];
		x[2] = 1;
		cv::circle(in, cv::Point(x[0], x[1]), 5, cv::Scalar(0,0,255), -1, 8, 1);
		for(size_t i = 0; i < lines.size(); ++i)
		{
			li = lines[i];
			make_line(li, linei);
			Tools::prodScal(linei, x, prodScalar);
			if(std::abs(prodScalar) < 9000)	
				group.push_back(lines[i]);
			
		}			
	}
	else
	{
		for(size_t i = 0; i < lines.size(); ++i)
		{
			li = lines[i];
			make_line(li, linei);
			Tools::prodScal(linei, x, prodScalar);
			if(std::abs(prodScalar) < 0.1)
				group.push_back(lines[i]);
		}
	}
	for(size_t i = 0; i < group.size(); ++i)
	{
		cv::Vec4i l = group[i];
    		line(in, cv::Point(l[0], l[1]), cv::Point(l[2], l[3]), cv::Scalar(0,0,255), 0.5, CV_AA);
	}
	return(1);
}

