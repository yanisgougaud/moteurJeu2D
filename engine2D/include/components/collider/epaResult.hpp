#pragma once

#include "../../math/Vector2D.hpp"


class EPAResult
{
    bool isValid;
    Vector2D normal;
    float penetrationDepth;

public:

    EPAResult(
        bool isValid,
        const Vector2D& normal,
        float penetrationDepth
    )
        : isValid( isValid ),
          normal( normal ),
          penetrationDepth( penetrationDepth )
    {
    }

    bool getIsValid() const {
        return isValid;
    }

    const Vector2D& getNormal() const {
        return normal;
    }

    float getPenetrationDepth() const {
        return penetrationDepth;
    }
    
};
