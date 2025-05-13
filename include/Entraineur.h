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
    
    // Pokemon management methods
    void soignerPokemons();
    void echangerPokemons(int index1, int index2);
    int countPokemons() const; // Added count method
};