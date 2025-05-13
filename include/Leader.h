#pragma once
#include <string>
#include <array>
#include <vector>
#include <fstream>
#include "Pokemon.h"
#include "Entraineur.h" 

class Leader: public Entraineur {
public:
    /**
     * @brief Représente différents gymnases Pokémon
     */
    enum class Gymnase { 
        ARGENTA,     
        AZURIA,     
        CARMIN,     
        CELADOPOLE, 
        PARMANIE,   
        SAFRANIA,   
        CRAMOISILE, 
        JADIELLE    
    }; 

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
    virtual ~Leader();
    
    // Getters
    Gymnase getGymnase() const;
    std::string getBadge() const;
    
    // Override Interagir
    virtual void afficher() const override;
    virtual std::string interagir() const override;

    //Gymnase
    static std::string GymToString(Gymnase gym);
    static Gymnase StringToGym(const std::string& gymnaseStr);

    // Loader
    static std::vector<Leader*> chargerLeaders(const std::string& nomFichier, const std::vector<Pokemon*>& pokemons);
};