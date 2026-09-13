#pragma once

#include <iostream>
#include <mutex>
#include <stdexcept>
#include <utility>

class Graphics {
public:

    Graphics( const Graphics& ) = delete;
    Graphics& operator=( const Graphics& ) = delete;

    static Graphics& getInstance() {
        static Graphics instance;
        return instance;
    }

    // Récupéré tous les éléments affichable
    // Trier ceux hors limite de l'affichage à l'écran

private:
    Graphics() { }

    ~Graphics() = default;
};

