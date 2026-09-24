#pragma once

#include "../../math/Vector2D.hpp"

#include <vector>

class GJKResult {

    bool hasCollision;
    std::vector<Vector2D> simplex;

public:

    GJKResult( bool hasCollision = false,
        std::vector<Vector2D> simplex = {} 
    ) : 
       hasCollision( hasCollision ),
       simplex( simplex ) 
       {
       }

    bool getHasCollision() const {
        return hasCollision;
    }
};

