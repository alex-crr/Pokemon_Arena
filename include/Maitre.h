#pragma once
#include <string>
#include <array>
#include <vector>
#include "Pokemon.h"
#include "Entraineur.h" 

class Maitre: public Entraineur {
private:
    // Remove redundant _nom and _pokemons - already in base class
    static constexpr double BONUS_DEGATS = 1.25; // 25% de bonus de dégâts
        
public:
    // Constructeur
    Maitre(const std::string& nom);
    Maitre(const std::string& nom, const std::array<Pokemon*, 6>& pokemons);
    
    // Destructeur
    virtual ~Maitre() override;
    
    // Methods spécifiques
    int attaquerAvecBonus(Pokemon& pokemonAllié, Pokemon& cible) const;
    
    // Override
    virtual void afficher() const override;

    static std::vector<Maitre*> chargerMaitres(const std::string& nomFichier, const std::vector<Pokemon*>& pokemons);
};