#include <Tools.h>

int	Tools::prodVect(cv::Vec3i a, cv::Vec3i b, cv::Vec3i& out)
{
	out[0] = a[1] * b[2] - a[2] * b[1];	//	a0	b0
	out[1] = a[2] * b[0] - a[0] * b[2];	//	a1	b1
	out[2] = a[0] * b[1] - a[1] * b[0];	//	a2	b2
						//	a0	b0
						//	a1	b1
	return(1);
}

int	Tools::prodScal(cv::Vec3i a, cv::Vec3i b, float& out)
{
	out = a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
	return(1);
}
