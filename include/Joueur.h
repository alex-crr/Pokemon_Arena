#pragma once
#include <string>
#include <array>
#include <vector>
#include "Pokemon.h"
#include "Entraineur.h" 

class Joueur: public Entraineur {
private:
    int _nbVictoires;
    int _nbDefaites;
    int _nbBadges;
        
public:
    // Constructeur
    Joueur(const std::string& nom);
    Joueur(const std::string& nom, const std::array<Pokemon*, 6>& pokemons);
    
    // Destructeur
    virtual ~Joueur() override;
    
    // Getters
    int getNbVictoires() const;
    int getNbDefaites() const;
    int getNbBadges() const;
    
    // Setters
    void incrementVictoires();
    void incrementDefaites();
    void incrementBadges();
    
    static std::vector<Joueur*> chargerJoueurs(const std::string& nomFichier, const std::vector<Pokemon*>& pokemons);
    
    // Override 
    virtual void afficher() const override;

};