#include "../include/Combat.h"
#include "../include/UI.h"
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

    // Afficher l'état initial du combat sans message d'état du combat
    afficher();
    std::cout << "\nAppuyez sur Entrée pour commencer le combat...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

    // Boucle de combat
    while (!estTermine()) {
        auto tourMessages = effectuerTour();
        
        // Pause à la fin du tour complet pour que le joueur puisse suivre
        if (!estTermine()) {
            afficher();
            std::cout << "\n=== Fin du tour ===";
            std::cout << "\nAppuyez sur Entrée pour le prochain tour...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
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
    
    // Afficher l'état final du combat
    afficher();
    
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

    // Le joueur (entraineur 1) attaque en premier
    std::string attackMessage = pokemon1->getNom() + " de " + _entraineur1->getNom() + 
                          " utilise " + pokemon1->getNomAttaque() + "!";
    tourMessages.push_back(attackMessage);
    
    // Remplacer le dernier message du log au lieu d'ajouter continuellement
    if (!_messages.empty()) {
        _messages.push_back(attackMessage);
        
        // Limiter le nombre de messages dans le journal pour éviter l'accumulation
        if (_messages.size() > 10) {
            _messages.erase(_messages.begin(), _messages.begin() + (_messages.size() - 10));
        }
    } else {
        _messages.push_back(attackMessage);
    }
    
    // Afficher l'intention d'attaque et attendre la confirmation
    afficher();
    std::cout << "\nAppuyez sur Entrée pour effectuer l'attaque...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    
    // Exécuter l'attaque du Pokemon 1
    int degats = pokemon1->attaquer(*pokemon2);
    std::string damageMessage = pokemon2->getNom() + " perd " + std::to_string(degats) + " PV!";
    tourMessages.push_back(damageMessage);
    _messages.push_back(damageMessage);
    
    // Afficher le résultat de l'attaque
    afficher();
    
    // Vérifier si pokemon2 est KO
    if (pokemon2->getHp() <= 0) {
        std::string koMessage = pokemon2->getNom() + " de " + _entraineur2->getNom() + " est KO!";
        tourMessages.push_back(koMessage);
        _messages.push_back(koMessage);
        
        // Afficher l'état après le KO
        afficher();
        std::cout << "\nAppuyez sur Entrée pour continuer...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        // Trouver le prochain Pokemon disponible
        _indexPokemon2 = trouverPokemonDisponible(_entraineur2, _indexPokemon2 + 1);
        
        // Vérifier si tous les Pokemon de l'entraineur 2 sont KO
        if (_indexPokemon2 == -1) {
            _combatTermine = true;
            return tourMessages;
        } else {
            std::string newPokemonMessage = _entraineur2->getNom() + " envoie " + 
                                  _entraineur2->getPokemon(_indexPokemon2)->getNom() + "!";
            tourMessages.push_back(newPokemonMessage);
            _messages.push_back(newPokemonMessage);
            
            // Afficher l'état après le changement de Pokemon
            afficher();
            std::cout << "\nAppuyez sur Entrée pour continuer...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
    } else {
        // Si le Pokemon 2 n'est pas KO, attendre confirmation pour continuer
        std::cout << "\nAppuyez sur Entrée pour continuer...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    // Si le combat n'est pas terminé, l'adversaire (entraineur 2) attaque
    if (!_combatTermine) {
        // Récupérer le nouveau Pokemon si nécessaire
        pokemon2 = _entraineur2->getPokemon(_indexPokemon2); 
        
        std::string attackMessage2 = pokemon2->getNom() + " de " + _entraineur2->getNom() + 
                              " utilise " + pokemon2->getNomAttaque() + "!";
        tourMessages.push_back(attackMessage2);
        _messages.push_back(attackMessage2);
        
        // Afficher l'intention d'attaque de l'adversaire et attendre la confirmation
        afficher();
        std::cout << "\nAppuyez sur Entrée pour voir l'attaque adverse...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        
        // Si l'entraineur 2 est un Maitre, utiliser son bonus
        Maitre* maitre = dynamic_cast<Maitre*>(_entraineur2);
        int degats;
        
        if (maitre) {
            degats = maitre->attaquerAvecBonus(*pokemon2, *pokemon1);
            std::string bonusMessage = "Bonus de Maitre Pokemon appliqué!";
            tourMessages.push_back(bonusMessage);
            _messages.push_back(bonusMessage);
            afficher();
            std::cout << "\nAppuyez sur Entrée pour continuer...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        } else {
            degats = pokemon2->attaquer(*pokemon1);
        }
        
        std::string damageMessage2 = pokemon1->getNom() + " perd " + std::to_string(degats) + " PV!";
        tourMessages.push_back(damageMessage2);
        _messages.push_back(damageMessage2);
        
        // Afficher le résultat de l'attaque de l'adversaire
        afficher();
        
        // Vérifier si pokemon1 est KO
        if (pokemon1->getHp() <= 0) {
            std::string koMessage = pokemon1->getNom() + " de " + _entraineur1->getNom() + " est KO!";
            tourMessages.push_back(koMessage);
            _messages.push_back(koMessage);
            
            // Afficher l'état après le KO
            afficher();
            std::cout << "\nAppuyez sur Entrée pour continuer...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            
            // Trouver le prochain Pokemon disponible
            _indexPokemon1 = trouverPokemonDisponible(_entraineur1, _indexPokemon1 + 1);
            
            // Vérifier si tous les Pokemon de l'entraineur 1 sont KO
            if (_indexPokemon1 == -1) {
                _combatTermine = true;
                return tourMessages;
            } else {
                std::string newPokemonMessage = _entraineur1->getNom() + " envoie " + 
                                      _entraineur1->getPokemon(_indexPokemon1)->getNom() + "!";
                tourMessages.push_back(newPokemonMessage);
                _messages.push_back(newPokemonMessage);
                
                // Afficher l'état après le changement de Pokemon
                afficher();
                std::cout << "\nAppuyez sur Entrée pour continuer...";
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            }
        } else {
            // Si le Pokemon 1 n'est pas KO, attendre confirmation pour continuer
            std::cout << "\nAppuyez sur Entrée pour continuer...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
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
    // Keep the method for backward compatibility, but we don't use it anymore
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
    const int width = 100; // Increased from 80 to provide more space
    UI::clearScreen();
    
    // Get current active Pokemon
    Pokemon* pokemon1 = _entraineur1->getPokemon(_indexPokemon1);
    Pokemon* pokemon2 = _entraineur2->getPokemon(_indexPokemon2);
    
    if (!pokemon1 || !pokemon2) {
        std::cout << "Combat non initialisé ou terminé." << std::endl;
        return;
    }

    // Header with battle title
    std::cout << "+" << std::string(width, '-') << "+" << std::endl;
    std::string battleTitle = "COMBAT POKEMON: " + _entraineur1->getNom() + " VS " + _entraineur2->getNom();
    int titlePadding = (width - battleTitle.length()) / 2;
    std::cout << "|" << std::string(titlePadding, ' ') << battleTitle 
              << std::string(width - battleTitle.length() - titlePadding, ' ') << "|" << std::endl;
    std::cout << "+" << std::string(width, '-') << "+" << std::endl;
    
    // Pokemon HP bars
    std::string p1Display = pokemon1->getNom() + " (" + std::to_string(pokemon1->getHp()) + 
                           "/" + std::to_string(pokemon1->getMaxHp()) + " PV)";
    std::string p2Display = pokemon2->getNom() + " (" + std::to_string(pokemon2->getHp()) + 
                           "/" + std::to_string(pokemon2->getMaxHp()) + " PV)";
    
    // Left side - Player's Pokemon, Right side - Opponent's Pokemon
    int halfWidth = width / 2 - 2; // Account for the "| " at start and " |" at end
    std::cout << "| " << std::left << std::setw(halfWidth) << _entraineur1->getNom()
              << " | " << std::left << std::setw(halfWidth) << _entraineur2->getNom() << " |" << std::endl;
    
    // HP displays
    std::cout << "| " << std::left << std::setw(halfWidth) << p1Display 
              << " | " << std::left << std::setw(halfWidth) << p2Display << " |" << std::endl;
    
    // Visual HP bars with better spacing
    int barWidth = width / 4;
    
    // Player Pokemon HP bar
    int p1HPWidth = static_cast<int>(static_cast<double>(pokemon1->getHp()) / pokemon1->getMaxHp() * barWidth);
    std::cout << "| [" << std::string(p1HPWidth, '=') << std::string(barWidth - p1HPWidth, ' ') << "]";
    
    // Fix spacing between bars by calculating exact needed space
    int spaceBetweenBars = halfWidth - barWidth - 1;
    std::cout << std::string(spaceBetweenBars, ' ');
    
    // Separator
    std::cout << " | ";
    
    // Opponent Pokemon HP bar
    int p2HPWidth = static_cast<int>(static_cast<double>(pokemon2->getHp()) / pokemon2->getMaxHp() * barWidth);
    std::cout << "[" << std::string(p2HPWidth, '=') << std::string(barWidth - p2HPWidth, ' ') << "]";
    
    // Fix remaining space calculation
    int remainingSpace = halfWidth - barWidth - 1;
    std::cout << std::string(remainingSpace, ' ') << " |" << std::endl;
    
    // Separator line
    std::cout << "|" << std::string(width, '-') << "|" << std::endl;
    
    // Pokemon details
    std::string p1Types = "";
    for (const auto& type : pokemon1->getTypes()) {
        if (!p1Types.empty()) p1Types += ", ";
        p1Types += Pokemon::typeToString(type);
    }
    
    std::string p2Types = "";
    for (const auto& type : pokemon2->getTypes()) {
        if (!p2Types.empty()) p2Types += ", ";
        p2Types += Pokemon::typeToString(type);
    }
    
    // Fix types padding to ensure alignment
    std::cout << "| Type(s): " << std::left << std::setw(halfWidth - 9) << p1Types 
              << " | Type(s): " << std::left << std::setw(halfWidth - 9) << p2Types << " |" << std::endl;
              
    // Fix attacks padding to ensure alignment
    std::cout << "| Attaque: " << std::left << std::setw(halfWidth - 10) << pokemon1->getNomAttaque() 
              << " | Attaque: " << std::left << std::setw(halfWidth - 10) << pokemon2->getNomAttaque() << " |" << std::endl;
              
    // Separator line
    std::cout << "|" << std::string(width, '-') << "|" << std::endl;
    
    // Fix team overview padding
    int teamTitlePadding1 = halfWidth - 12 - _entraineur1->getNom().length();
    int teamTitlePadding2 = halfWidth - 12 - _entraineur2->getNom().length();
    
    // Ensure non-negative padding
    if (teamTitlePadding1 < 0) teamTitlePadding1 = 0;
    if (teamTitlePadding2 < 0) teamTitlePadding2 = 0;
    
    std::cout << "| Équipe de " << _entraineur1->getNom() << ":" 
              << std::string(teamTitlePadding1, ' ')
              << " | Équipe de " << _entraineur2->getNom() << ":" 
              << std::string(teamTitlePadding2, ' ') << " |" << std::endl;
    
    // Display Pokemon in two columns, each trainer's team on one side
    for (int i = 0; i < 6; ++i) {
        std::stringstream team1Row, team2Row;
        
        // Trainer 1's Pokemon
        Pokemon* p1 = _entraineur1->getPokemon(i);
        if (p1) {
            // Highlight active Pokemon
            if (i == _indexPokemon1) team1Row << "[*] ";
            else team1Row << "[" << i+1 << "] ";
            
            team1Row << std::left << std::setw(10) << p1->getNom();
            if (p1->getHp() <= 0) team1Row << " (KO)";
            else team1Row << " (" << p1->getHp() << "/" << p1->getMaxHp() << ")";
        }
        
        // Trainer 2's Pokemon
        Pokemon* p2 = _entraineur2->getPokemon(i);
        if (p2) {
            // Highlight active Pokemon
            if (i == _indexPokemon2) team2Row << "[*] ";
            else team2Row << "[" << i+1 << "] ";
            
            team2Row << std::left << std::setw(10) << p2->getNom();
            if (p2->getHp() <= 0) team2Row << " (KO)";
            else team2Row << " (" << p2->getHp() << "/" << p2->getMaxHp() << ")";
        }
        
        // Output the row with proper padding
        std::cout << "| " << std::left << std::setw(halfWidth) << team1Row.str() 
                  << " | " << std::left << std::setw(halfWidth) << team2Row.str() << " |" << std::endl;
    }
    
    // Separator line
    std::cout << "|" << std::string(width, '-') << "|" << std::endl;
    
    // Combat log section with proper width
    std::cout << "| " << std::left << std::setw(width-2) << "JOURNAL DE COMBAT" << " |" << std::endl;
    std::cout << "|" << std::string(width, '-') << "|" << std::endl;
    
    // Display only the most recent messages (up to 5)
    size_t messageCount = _messages.size();
    size_t start = (messageCount > 5) ? messageCount - 5 : 0;
    size_t displayedCount = 0;
    
    for (size_t i = start; i < messageCount; ++i) {
        // Skip empty messages
        if (_messages[i].empty()) continue;
        
        // Skip état du combat messages
        if (_messages[i].find("Etat du combat") != std::string::npos) continue;
        
        // Split long messages if needed
        std::string msg = _messages[i];
        while (msg.length() > width-4) {
            std::string part = msg.substr(0, width-4);
            std::cout << "| " << part << " |" << std::endl;
            msg = msg.substr(width-4);
            displayedCount++;
        }
        
        std::cout << "| " << std::left << std::setw(width-2) << msg << " |" << std::endl;
        displayedCount++;
    }
    
    // Fill remaining lines to ensure consistent display
    for (size_t i = displayedCount; i < 5; ++i) {
        std::cout << "| " << std::string(width-2, ' ') << " |" << std::endl;
    }
    
    // Bottom border
    std::cout << "+" << std::string(width, '-') << "+" << std::endl;
}

