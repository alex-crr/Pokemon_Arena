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
     * @brief Represents different Pokémon gymnasiums
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
    
    // Override
    virtual void afficher() const override;
    virtual std::string interagir() const override;

    // Static methods for Gymnase enum
    /**
     * Converts a Gymnase enum value to its string representation
     * @param gym The Gymnase enum value
     * @return The full display name of the gym
     */
    static std::string GymToString(Gymnase gym);
    
    /**
     * Converts a string to a Gymnase enum value
     * @param gymnaseStr String representing the gymnasium
     * @return Corresponding Gymnase enum value
     */
    static Gymnase StringToGym(const std::string& gymnaseStr);

    // Static method to load leaders from file
    static std::vector<Leader*> chargerLeaders(const std::string& nomFichier, const std::vector<Pokemon*>& pokemons);
};