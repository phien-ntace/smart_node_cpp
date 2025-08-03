#include <math.h>
#include "utility.h"

float convert_int_to_float(int x)
{
    int num_digits = 0;
    int temp_x = x;
    
    if (temp_x == 0) {
        num_digits = 1;
    } else {
        while (temp_x > 0) {
            temp_x /= 10;
            num_digits++;
        }
    }
    float divisor = pow(10, num_digits);
    return (float)x / divisor;
}