#include "../include/Combat.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <sstream>
#include <iomanip>

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
    throw std::logic_error("La méthode interagir() n'est pas implémentée.");
}

void Combat::afficher() const
{
    const int width = 78;
    
    // Get current active Pokemon
    Pokemon* pokemon1 = _entraineur1->getPokemon(_indexPokemon1);
    Pokemon* pokemon2 = _entraineur2->getPokemon(_indexPokemon2);
    
    if (!pokemon1 || !pokemon2) {
        std::cout << "Combat non initialisé ou terminé." << std::endl;
        return;
    }
    
    // Header with trainer info and active Pokemon
    std::cout << "+" << std::string(width, '-') << "+" << std::endl;
    
    std::string trainer1Info = "Trainer: " + _entraineur1->getNom();
    std::string trainer2Info = "Trainer: " + _entraineur2->getNom();
    std::cout << "| " << std::left << std::setw(40) << trainer1Info 
              << std::left << std::setw(36) << trainer2Info << " |" << std::endl;
              
    std::stringstream pokemon1Info;
    pokemon1Info << "Active: " << std::left << std::setw(10) << pokemon1->getNom() 
                << " (HP: " << pokemon1->getHp() << "/" << pokemon1->getMaxHp() << ")";
    
    std::stringstream pokemon2Info;
    pokemon2Info << std::left << std::setw(15) << pokemon2->getNom()
                << " (HP: " << pokemon2->getHp() << "/" << pokemon2->getMaxHp() << ")";
    
    std::cout << "| " << std::left << std::setw(40) << pokemon1Info.str()
              << std::left << std::setw(36) << pokemon2Info.str() << " |" << std::endl;
    
    std::cout << "| " << std::string(width-2, ' ') << " |" << std::endl;
    
    // Trainer 1's team
    std::cout << "| " << _entraineur1->getNom() << "'s Team:" << std::string(width - 15 - _entraineur1->getNom().length(), ' ') << " |" << std::endl;
    
    // Display Pokemon in two rows of 3
    for (int i = 0; i < 6; i += 3) {
        std::stringstream teamRow;
        for (int j = i; j < i+3 && j < 6; ++j) {
            Pokemon* p = _entraineur1->getPokemon(j);
            if (p) {
                teamRow << "  [" << j+1 << "] " << std::left << std::setw(10) << p->getNom();
                if (p->getHp() <= 0) teamRow << " (KO)";
                teamRow << "  ";
            }
        }
        std::cout << "| " << std::left << std::setw(width-2) << teamRow.str() << " |" << std::endl;
    }
    
    std::cout << "| " << std::string(width-2, ' ') << " |" << std::endl;
    
    // Trainer 2's team
    std::cout << "| " << _entraineur2->getNom() << "'s Team:" << std::string(width - 15 - _entraineur2->getNom().length(), ' ') << " |" << std::endl;
    
    for (int i = 0; i < 6; i += 3) {
        std::stringstream teamRow;
        for (int j = i; j < i+3 && j < 6; ++j) {
            Pokemon* p = _entraineur2->getPokemon(j);
            if (p) {
                teamRow << "  [" << j+1 << "] " << std::left << std::setw(10) << p->getNom();
                if (p->getHp() <= 0) teamRow << " (KO)";
                teamRow << "  ";
            }
        }
        std::cout << "| " << std::left << std::setw(width-2) << teamRow.str() << " |" << std::endl;
    }
    
    std::cout << "| " << std::string(width-2, ' ') << " |" << std::endl;
    std::cout << "+" << std::string(width, '-') << "+" << std::endl;
    
    // Display last few combat messages
    if (!_messages.empty()) {
        std::cout << "| " << std::left << std::setw(width-2) << "Last combat actions:" << " |" << std::endl;
        
        // Display the last 3 messages or fewer if not available
        size_t start = _messages.size() > 3 ? _messages.size() - 3 : 0;
        for (size_t i = start; i < _messages.size(); ++i) {
            // Split long messages if needed
            std::string msg = _messages[i];
            while (msg.length() > width-4) {
                std::string part = msg.substr(0, width-4);
                std::cout << "| " << part << " |" << std::endl;
                msg = msg.substr(width-4);
            }
            std::cout << "| " << std::left << std::setw(width-2) << msg << " |" << std::endl;
        }
    }
    
    std::cout << "+" << std::string(width, '-') << "+" << std::endl;
}

