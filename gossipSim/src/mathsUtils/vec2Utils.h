#pragma once
#include <Daedalus.h>

namespace mathsUtils
{
	float vec2_dot_product(const daedalusCore::maths::vec2& first, const daedalusCore::maths::vec2& second);

	float vec2_magnitude(const daedalusCore::maths::vec2& vec);

	float angle_of_vec2(const daedalusCore::maths::vec2& vec);
}
