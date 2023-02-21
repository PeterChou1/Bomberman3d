#pragma once

/**
 * \brief Component containing all things related to display output
 */
struct Display
{
	double* zbuffer;
	double aspect_ratio;
	int resolution;
};
