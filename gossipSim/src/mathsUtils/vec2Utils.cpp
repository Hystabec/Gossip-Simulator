#include "vec2Utils.h"

float mathsUtils::vec2_dot_product(const daedalusCore::maths::vec2& first, const daedalusCore::maths::vec2& second)
{
    return ((first.x * second.x) + (first.y * second.y));;
}

float mathsUtils::vec2_magnitude(const daedalusCore::maths::vec2& vec)
{
    return sqrtf((vec.x * vec.x) + (vec.y * vec.y));
}

float mathsUtils::angle_of_vec2(const daedalusCore::maths::vec2& vec)
{
    return atan2(vec.y, vec.x);
}