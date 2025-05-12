#pragma once
#include <string>
#include <array>
#include <vector>
#include "Pokemon.h"
#include "Entraineur.h" 

class Leader: public Entraineur {
private:
    Gymnase _gymnase; 
    std::string _badge;
        
public:
    // Constructeur
    Leader(const std::string& nom, Gymnase gymnase);
    Leader(const std::string& nom, Gymnase gymnase, const std::array<Pokemon*, 6>& pokemons);
    Leader(const std::string& nom, Gymnase gymnase, const std::string& badge);
    Leader(const std::string& nom, Gymnase gymnase, const std::string& badge, const std::array<Pokemon*, 6>& pokemons);
    
    // Destructeur
    virtual ~Leader() override;
    
    // Getters
    Gymnase getGymnase() const;
    std::string getBadge() const;
    
    // Override
    virtual void afficher() const override;
    
    static std::vector<Leader*> chargerLeaders(const std::string& nomFichier, const std::vector<Pokemon*>& pokemons);
};