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
    
    if (leaders.empty()) {
        std::cout << "\n===== LEADERS DISPONIBLES =====" << std::endl;
        std::cout << "Aucun Leader disponible pour le moment." << std::endl;
        return;
    }
    
    std::cout << "\n===== LEADERS DISPONIBLES =====" << std::endl;
    for (size_t i = 0; i < leaders.size(); ++i) {
        std::cout << i + 1 << ". " << leaders[i]->getNom() << " (Gymnase: "
                 << Leader::GymToString(leaders[i]->getGymnase())
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

void UI::afficherPokemons(const Entraineur* entraineur) {
    for (int i = 0; i < 6; i++) {
        Pokemon* pokemon = entraineur->getPokemon(i);
        if (pokemon != nullptr) {
            std::cout << i + 1 << ". ";
            pokemon->afficher();
        }
    }
}

// Remove other unused methods (afficherMessagesCombat, demarrerCombat, soignerPokemons, echangerPokemons)
// Keep only countPokemons until it's moved to Entraineur