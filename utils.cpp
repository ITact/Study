#include "utils.h"
#include <math.h>



//转换int-string		
string itos(int arg)
{
    ostringstream buffer;
	
    buffer << arg;	
	
    return buffer.str();		
}


//float-string		
string ftos(float arg)
{
    ostringstream buffer;
	
    buffer << arg;	
	
    return buffer.str();		
}

//控制arg在两个最小最大值之间
void Clamp(double &arg, double min, double max)
{
	if (arg < min)
	{
		arg = min;
	}

	if (arg > max)
	{
		arg = max;
	}
}
