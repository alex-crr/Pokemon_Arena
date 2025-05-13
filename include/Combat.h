#pragma once
#include "Entraineur.h"
#include "Joueur.h"
#include "Leader.h"
#include "Maitre.h"
#include "Interagir.h"
#include <string>
#include <vector>

class Combat : public Interagir {
private:
    Entraineur* _entraineur1; // Joueur
    Entraineur* _entraineur2; // Adversaire (Leader ou Maitre)
    int _indexPokemon1; // Index du Pokemon actif pour l'entraineur 1
    int _indexPokemon2; // Index du Pokemon actif pour l'entraineur 2
    bool _combatTermine;
    std::vector<std::string> _messages; // Stocke les messages du combat

public:
    // Constructeur
    Combat(Entraineur* entraineur1, Entraineur* entraineur2);

    // Lancer le combat
    std::vector<std::string> demarrer();

    // Effectuer un tour de combat et retourner les messages
    std::vector<std::string> effectuerTour();

    // Vérifier si le combat est terminé
    bool estTermine() const;

    // Obtenir le vainqueur
    Entraineur* getVainqueur() const;

    // Obtenir le prochain Pokemon disponible
    int trouverPokemonDisponible(const Entraineur* entraineur, int indexCourant) const;

    // Obtenir l'état actuel du combat
    std::string getEtatCombat() const;
    
    // Implementation de l'interface Interagir
    virtual std::string interagir() const override;

    virtual void afficher() const override;
};
