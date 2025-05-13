#include "../include/UI.h"
#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>

void UI::clearScreen() {
    #ifdef _WIN32
        system("cls"); // Pour Windows
    #else
        system("clear"); // Pour Unix/Linux/MacOS
    #endif
}

void UI::afficherMenu() {
    clearScreen();
    std::cout << "\n===== MENU PRINCIPAL =====" << std::endl;
    std::cout << "1. Gérer mes Pokemon" << std::endl;
    std::cout << "2. Afficher mes statistiques" << std::endl;
    std::cout << "3. Affronter un Leader de Gymnase" << std::endl;
    std::cout << "4. Affronter un Maitre Pokemon" << std::endl;
    std::cout << "5. Interagir avec les entraineurs vaincus" << std::endl;
    std::cout << "0. Quitter" << std::endl;
    std::cout << "Votre choix: ";
}

void UI::afficherMenuLeaders(const std::vector<Leader*>& leaders) {
    clearScreen();
    std::cout << "\n===== LEADERS DISPONIBLES =====" << std::endl;
    for (size_t i = 0; i < leaders.size(); ++i) {
        std::cout << i + 1 << ". " << leaders[i]->getNom() << " (Gymnase: "
                 << Entraineur::getGymName(leaders[i]->getGymnase())
                 << ", Badge: " << leaders[i]->getBadge() << ")" << std::endl;
    }
    std::cout << "0. Retour" << std::endl;
    std::cout << "Votre choix: ";
}

void UI::afficherMenuPokemon() {
    clearScreen();
    std::cout << "\n===== GESTION DE MES POKEMON =====" << std::endl;
    std::cout << "1. Afficher mes Pokemon" << std::endl;
    std::cout << "2. Soigner mes Pokemon" << std::endl;
    std::cout << "3. Interagir avec mes Pokemon" << std::endl;
    std::cout << "4. Changer l'ordre de mes Pokemon" << std::endl;
    std::cout << "0. Retour au menu principal" << std::endl;
    std::cout << "Votre choix: ";
}

int UI::getValidChoice(int min, int max) {
    int choice;
    while (!(std::cin >> choice) || choice < min || choice > max) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Choix invalide. Veuillez entrer un nombre entre " << min << " et " << max << ": ";
    }
    return choice;
}

void UI::waitForEnter() {
    std::cout << "\nAppuyez sur Entree pour continuer...";
    
    // Vider le buffer d'entrée et attendre la touche Entrée
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

void UI::afficherMessagesCombat(const std::vector<std::string>& messages, const std::string& titreEntete) {
    clearScreen();
    std::cout << "\n===== " << titreEntete << " =====" << std::endl;
    
    if (messages.empty()) return;
    
    // Ignorer les messages de titre dupliqués
    size_t i = 0;
    if (messages[0].find("DEBUT DU COMBAT") != std::string::npos || 
        messages[0].find("Etat du combat") != std::string::npos) {
        i = 1;
    }
    
    std::vector<std::string> groupedMessages;
    std::string currentGroup;
    
    // Parcourir les messages et les regrouper
    while (i < messages.size()) {
        currentGroup = messages[i++];
        
        // Regrouper les messages "VS" avec l'état du combat qui suit
        if (currentGroup.find(" VS ") != std::string::npos) {
            // Ajouter l'état du combat qui suit généralement
            while (i < messages.size() && 
                  (messages[i].find("Etat du combat") != std::string::npos ||
                   messages[i].find(":") != std::string::npos ||
                   messages[i].find("PV") != std::string::npos ||
                   messages[i].empty())) {
                
                // Si on trouve un nouvel état du combat, l'ajouter au groupe actuel
                if (messages[i].find("Etat du combat") != std::string::npos) {
                    currentGroup += "\n\n" + messages[i++];
                    continue;
                }
                
                currentGroup += "\n" + messages[i++];
            }
        }
        // Si c'est une attaque, ajouter les dégâts et les effets KO dans le même groupe
        else if (i < messages.size() && currentGroup.find(" utilise ") != std::string::npos) {
            while (i < messages.size() && 
                  (messages[i].find(" perd ") != std::string::npos || 
                   messages[i].find(" est KO") != std::string::npos)) {
                currentGroup += "\n" + messages[i++];
            }
        }
        // Si c'est l'état du combat, regrouper toutes les informations d'état
        else if (currentGroup.find("Etat du combat") != std::string::npos) {
            while (i < messages.size() && 
                  (messages[i].find(":") != std::string::npos || 
                   messages[i].find("PV") != std::string::npos || 
                   messages[i].empty())) {
                currentGroup += "\n" + messages[i++];
            }
        }
        // Regrouper les messages de victoire avec l'obtention de badge
        else if (i < messages.size() && currentGroup.find("remporte le combat") != std::string::npos) {
            while (i < messages.size() && 
                  (messages[i].find("obtient un badge") != std::string::npos || 
                   messages[i].find("badges") != std::string::npos)) {
                currentGroup += "\n" + messages[i++];
            }
        }
        
        groupedMessages.push_back(currentGroup);
    }
    
    // Afficher les groupes de messages avec pause entre chaque
    for (size_t j = 0; j < groupedMessages.size(); ++j) {
        std::cout << groupedMessages[j] << std::endl;
        
        // Attendre l'appui sur Entrée pour continuer, sauf pour le dernier groupe
        if (j < groupedMessages.size() - 1) {
            std::cout << "\nAppuyez sur Entrée pour la suite...";
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            if (std::cin.peek() != '\n') {
                std::cin.get();
            }
        }
    }
}

Combat* UI::demarrerCombat(Joueur* joueur, Entraineur* entraineur, const std::string& titreCombat) {
    // Créer un nouveau combat
    Combat* combat = new Combat(joueur, entraineur);
    
    // Lancer le combat - maintenant demarrer() gère tout l'affichage
    std::vector<std::string> messages = combat->demarrer();
    
    // Afficher seulement les messages finaux importants (résultat, badges)
    std::vector<std::string> resultMessages;
    for (const auto& msg : messages) {
        if (msg.find("remporte le combat") != std::string::npos ||
            msg.find("obtient un badge") != std::string::npos ||
            msg.find("Match nul") != std::string::npos) {
            resultMessages.push_back(msg);
        }
    }
    
    if (!resultMessages.empty()) {
        std::cout << "\n=== RÉSULTAT DU COMBAT ===" << std::endl;
        for (const auto& msg : resultMessages) {
            std::cout << msg << std::endl;
        }
    }
    
    waitForEnter();
    return combat;
}

void UI::afficherPokemons(const Entraineur* entraineur) {
    for (int i = 0; i < 6; i++) {
        Pokemon* pokemon = entraineur->getPokemon(i);
        if (pokemon != nullptr) {
            std::cout << i + 1 << ". ";
            pokemon->afficher();
        }
    }
}

void UI::soignerPokemons(Entraineur* entraineur) {
    for (int i = 0; i < 6; i++) {
        Pokemon* pokemon = entraineur->getPokemon(i);
        if (pokemon != nullptr) {
            pokemon->soigner();
        }
    }
}

void UI::echangerPokemons(Entraineur* entraineur, int index1, int index2) {
    if (index1 >= 0 && index1 < 6 && index2 >= 0 && index2 < 6) {
        Pokemon* temp = entraineur->getPokemon(index1);
        entraineur->setPokemon(index1, entraineur->getPokemon(index2));
        entraineur->setPokemon(index2, temp);
    }
}

int UI::countPokemons(const Entraineur* entraineur) {
    int count = 0;
    for (int i = 0; i < 6; i++) {
        if (entraineur->getPokemon(i) != nullptr) {
            count++;
        }
    }
    return count;
}