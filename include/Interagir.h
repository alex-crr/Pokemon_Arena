#pragma once
#include <string>

/**
 * @brief Interface permettant aux entites du jeu d'etre interagies
 */
class Interagir {
public:
    virtual ~Interagir() = default;
    
    virtual std::string interagir() const = 0;
    virtual void afficher() const = 0;
};
