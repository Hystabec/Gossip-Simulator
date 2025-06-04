#include "arrow2D.h"

#include "../mathsUtils/vec2Utils.h"

const float arrowHeadSize = 0.125f;

void renderArrow(const daedalusCore::maths::vec2& start, const daedalusCore::maths::vec2& end, const daedalusCore::maths::vec4& colour)
{
	static daedalusCore::shr_ptr<daedalusCore::graphics::Texture2D> arrowHeadTexture = daedalusCore::graphics::Texture2D::create("resources/arrowHead.png");
	daedalusCore::graphics::primatives2D::RotatedQuadProperties ArrowHeadProps;
	ArrowHeadProps.size = { arrowHeadSize };
	ArrowHeadProps.texture = arrowHeadTexture;
	ArrowHeadProps.colour = colour;

	daedalusCore::maths::vec2 diffrence = start - end;
	auto normalDiff = mathsUtils::vec2_normalize(diffrence);

	ArrowHeadProps.position = { end.x + (normalDiff.x * (arrowHeadSize/2)), end.y + (normalDiff.y * (arrowHeadSize/2)), -0.1f };

	ArrowHeadProps.rotation = mathsUtils::angle_of_vec2(diffrence) - daedalusCore::maths::degrees_to_radians(90);

	daedalusCore::graphics::Renderer2D::drawRotatedQuad(ArrowHeadProps);

	daedalusCore::maths::vec2 centrePoint = (start + end) / 2;

	daedalusCore::graphics::Renderer2D::drawRotatedQuad(
		{
			{ centrePoint.x, centrePoint.y, -0.1f },
			{ ((float)abs(mathsUtils::vec2_magnitude(diffrence) - (arrowHeadSize * 1.75f))) , arrowHeadSize/2},
			mathsUtils::angle_of_vec2(diffrence),
			colour
		});
}