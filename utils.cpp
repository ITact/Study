#include "utils.h"
#include <math.h>



//ת��int-string		
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

//����arg��������С���ֵ֮��
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
