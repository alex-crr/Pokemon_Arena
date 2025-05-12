#pragma once
#include <string>
#include <array>
#include "Pokemon.h"
#include "Interagir.h"

class Entraineur : public Interagir {
protected: 
    const std::string _nom;
    std::array<Pokemon*, 6> _pokemons; 
    bool _estVaincu;  // Ajout d'un attribut pour savoir si l'entraineur a été vaincu

public:
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

    // Constructeur
    Entraineur(const std::string& nom);
    Entraineur(const std::string& nom, const std::array<Pokemon*, 6>& pokemons);
    
    // Destructeur
    virtual ~Entraineur();
    
    // Getters
    std::string getNom() const;
    const std::array<Pokemon*, 6>& getPokemons() const;
    Pokemon* getPokemon(int index) const;
    bool getEstVaincu() const;
    
    // Setters
    void setPokemon(int index, Pokemon* pokemon);
    void setEstVaincu(bool estVaincu);
    
    // Other methods
    virtual void afficher() const;
    virtual bool estKO() const;
    
    // Implementation de l'interface Interagir
    virtual std::string interagir() const override;

    /**
     * Returns the full name of a gym based on its enum value
     * @param gym The Gymnase enum value
     * @return The full display name of the gym
     */
    static std::string getGymName(Gymnase gym);
    
    /**
     * Convertit une chaîne en enum Gymnase
     * @param gymnaseStr Chaîne représentant le gymnase
     * @return Enum Gymnase correspondant
     */
    static Gymnase convertirStringEnGymnase(const std::string& gymnaseStr);
};