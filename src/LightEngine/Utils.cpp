#include "Utils.h"
#include "Rendering.h"


namespace Utils
{
    Vector2f PointIntersecBySegmentAndCircle(const Segment& p_segment, const Circle& p_circle)
    {

        Vector2f AB = p_segment.b - p_segment.a; // vector de la distance et direction du segment
        Vector2f dirAB = AB.Normalized(); //direction normalisée

        Vector2f AC = p_circle.position - p_segment.a; // vector de la distance et direction du début du segment jusqu'au centre du cercle
        float distanceAD = Vector2f::Dot(AC, dirAB); //distance entre le cercle est le point D

        Vector2f point = p_circle.position + distanceAD; 
        return point;
    }






}
