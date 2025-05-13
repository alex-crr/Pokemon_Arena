#pragma once
#include <string>

/**
 * @brief Interface permettant aux entites du jeu d'etre interagies
 */
class Interagir {
public:
    virtual ~Interagir() = default;
    
    /**
     * @brief Methode d'interaction avec l'entite
     * @return Message resultant de l'interaction
     */
    virtual std::string interagir() const = 0;
    virtual void afficher() const = 0;
};
