typedef unsigned int uint;


inline int clamp(int min, int max, int val)
{
	if (val < min)
	{
		return min;
	}
	if (val > max)
	{
		return max;
	}
	return val;
}