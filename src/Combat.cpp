#include "../include/Combat.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>

Combat::Combat(Entraineur* entraineur1, Entraineur* entraineur2)
    : _entraineur1(entraineur1), _entraineur2(entraineur2), _indexPokemon1(0), _indexPokemon2(0), _combatTermine(false)
{
}

std::vector<std::string> Combat::demarrer()
{
    _messages.clear();
    _messages.push_back("=== DEBUT DU COMBAT ===");
    _messages.push_back(_entraineur1->getNom() + " VS " + _entraineur2->getNom());

    // Trouver le premier Pokemon disponible pour chaque entraineur
    _indexPokemon1 = trouverPokemonDisponible(_entraineur1, 0);
    _indexPokemon2 = trouverPokemonDisponible(_entraineur2, 0);

    // Verifier que les deux entraineurs ont au moins un Pokemon
    if (_indexPokemon1 == -1 || _indexPokemon2 == -1) {
        _messages.push_back("Un des entraineurs n'a pas de Pokemon disponible!");
        _combatTermine = true;
        return _messages;
    }

    // Premier affichage de l'état du combat
    _messages.push_back(getEtatCombat());

    // Boucle de combat avec goto
    debut_tour:
    if (!estTermine()) {
        auto tourMessages = effectuerTour();
        _messages.insert(_messages.end(), tourMessages.begin(), tourMessages.end());
        
        if (!estTermine()) {
            goto debut_tour;
        }
    }

    // Annoncer le vainqueur
    Entraineur* vainqueur = getVainqueur();
    if (vainqueur) {
        _messages.push_back(vainqueur->getNom() + " remporte le combat!");
        
        // Marquer l'adversaire comme vaincu s'il s'agit de l'entraineur 2
        if (vainqueur == _entraineur1) {
            _entraineur2->setEstVaincu(true);
            
            // Si le joueur a gagné
            Joueur* joueurVainqueur = dynamic_cast<Joueur*>(vainqueur);
            if (joueurVainqueur) {
                joueurVainqueur->incrementVictoires();
                
                // Si l'adversaire était un Leader, donner un badge
                if (dynamic_cast<Leader*>(_entraineur2)) {
                    joueurVainqueur->incrementBadges();
                    _messages.push_back(joueurVainqueur->getNom() + " obtient un badge! Total: " 
                              + std::to_string(joueurVainqueur->getNbBadges()) + " badges.");
                }
            }
        } else {
            // Si le joueur a perdu
            Joueur* joueur = dynamic_cast<Joueur*>(_entraineur1);
            if (joueur) {
                joueur->incrementDefaites();
            }
        }
    } else {
        _messages.push_back("Match nul!");
    }

    _messages.push_back("=== FIN DU COMBAT ===");
    return _messages;
}

std::vector<std::string> Combat::effectuerTour()
{
    std::vector<std::string> tourMessages;
    Pokemon* pokemon1 = _entraineur1->getPokemon(_indexPokemon1);
    Pokemon* pokemon2 = _entraineur2->getPokemon(_indexPokemon2);

    if (!pokemon1 || !pokemon2) {
        _combatTermine = true;
        return tourMessages;
    }

    // Le joueur attaque en premier
    tourMessages.push_back(pokemon1->getNom() + " de " + _entraineur1->getNom() + 
                          " utilise " + pokemon1->getNomAttaque() + "!");
    
    // Attaque du Pokemon 1
    int degats = pokemon1->attaquer(*pokemon2);
    tourMessages.push_back(pokemon2->getNom() + " perd " + std::to_string(degats) + " PV!");
    
    // Vérifier si pokemon2 est KO
    if (pokemon2->getHp() <= 0) {
        tourMessages.push_back(pokemon2->getNom() + " de " + _entraineur2->getNom() + " est KO!");
        
        // Trouver le prochain Pokemon disponible
        _indexPokemon2 = trouverPokemonDisponible(_entraineur2, _indexPokemon2 + 1);
        
        // Vérifier si tous les Pokemon de l'entraineur 2 sont KO
        if (_indexPokemon2 == -1) {
            _combatTermine = true;
            return tourMessages;
        } else {
            tourMessages.push_back(_entraineur2->getNom() + " envoie " + 
                                  _entraineur2->getPokemon(_indexPokemon2)->getNom() + "!");
        }
    }
    
    // Si le Pokemon 2 n'est pas KO, il contre-attaque
    if (!_combatTermine) {
        // Récupérer le nouveau Pokemon si nécessaire
        pokemon2 = _entraineur2->getPokemon(_indexPokemon2); 
        
        tourMessages.push_back(pokemon2->getNom() + " de " + _entraineur2->getNom() + 
                              " utilise " + pokemon2->getNomAttaque() + "!");
        
        // Si l'entraineur 2 est un Maitre, utiliser son bonus
        Maitre* maitre = dynamic_cast<Maitre*>(_entraineur2);
        int degats;
        
        if (maitre) {
            degats = maitre->attaquerAvecBonus(*pokemon2, *pokemon1);
            tourMessages.push_back("Bonus de Maitre Pokemon applique!");
        } else {
            degats = pokemon2->attaquer(*pokemon1);
        }
        
        tourMessages.push_back(pokemon1->getNom() + " perd " + std::to_string(degats) + " PV!");
        
        // Vérifier si pokemon1 est KO
        if (pokemon1->getHp() <= 0) {
            tourMessages.push_back(pokemon1->getNom() + " de " + _entraineur1->getNom() + " est KO!");
            
            // Trouver le prochain Pokemon disponible
            _indexPokemon1 = trouverPokemonDisponible(_entraineur1, _indexPokemon1 + 1);
            
            // Vérifier si tous les Pokemon de l'entraineur 1 sont KO
            if (_indexPokemon1 == -1) {
                _combatTermine = true;
                return tourMessages;
            } else {
                tourMessages.push_back(_entraineur1->getNom() + " envoie " + 
                                      _entraineur1->getPokemon(_indexPokemon1)->getNom() + "!");
            }
        }
    }
    
    // Ajouter l'état actuel du combat si non terminé
    if (!_combatTermine) {
        tourMessages.push_back(getEtatCombat());
    }
    
    return tourMessages;
}

bool Combat::estTermine() const
{
    return _combatTermine || _entraineur1->estKO() || _entraineur2->estKO();
}

Entraineur* Combat::getVainqueur() const
{
    if (!estTermine()) {
        return nullptr; // Le combat n'est pas terminé
    }
    
    if (_entraineur1->estKO()) {
        return _entraineur2;
    } else if (_entraineur2->estKO()) {
        return _entraineur1;
    }
    
    return nullptr; // Match nul
}

int Combat::trouverPokemonDisponible(const Entraineur* entraineur, int indexCourant) const
{
    for (int i = indexCourant; i < 6; ++i) {
        Pokemon* pokemon = entraineur->getPokemon(i);
        if (pokemon && pokemon->getHp() > 0) {
            return i;
        }
    }
    return -1; // Aucun Pokemon disponible
}

std::string Combat::getEtatCombat() const
{
    Pokemon* pokemon1 = _entraineur1->getPokemon(_indexPokemon1);
    Pokemon* pokemon2 = _entraineur2->getPokemon(_indexPokemon2);
    
    std::stringstream ss;
    ss << "--- Etat du combat ---\n"
       << _entraineur1->getNom() << ": " << pokemon1->getNom() << " (" 
       << pokemon1->getHp() << "/" << pokemon1->getMaxHp() << " PV)\n"
       << _entraineur2->getNom() << ": " << pokemon2->getNom() << " (" 
       << pokemon2->getHp() << "/" << pokemon2->getMaxHp() << " PV)";
    
    return ss.str();
}

std::string Combat::interagir() const
{
    // L'interaction avec un combat permet de revoir les derniers messages
    std::stringstream ss;
    ss << "Résumé du dernier combat entre " << _entraineur1->getNom() 
       << " et " << _entraineur2->getNom() << ":\n";
    
    if (_messages.empty()) {
        ss << "Pas d'informations disponibles sur ce combat.";
    } else {
        // Afficher les 5 derniers messages ou moins s'il y en a moins
        size_t start = _messages.size() > 5 ? _messages.size() - 5 : 0;
        for (size_t i = start; i < _messages.size(); ++i) {
            ss << "- " << _messages[i] << "\n";
        }
    }
    
    return ss.str();
}

