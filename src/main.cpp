#include "../include/Pokemon.h"
#include "../include/Joueur.h"
#include "../include/Leader.h"
#include "../include/Maitre.h"
#include "../include/Combat.h"
#include "../include/UI.h"
#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <thread>
#include <limits>
#include <locale>    
#include <clocale>   

// Removed helper functions that are now in UI.cpp

int main() {
    try {
        try
        {
            std::locale::global(std::locale("fr_FR.UTF-8"));
        }
        catch (const std::runtime_error& e)
        {
            setlocale(LC_ALL, "fr_FR.UTF-8");
        }
        UI::clearScreen();
        
        UI::clearScreen();
        
        // Initialiser le générateur de nombres aléatoires
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        
        // 1. Charger les Pokemon
        std::vector<Pokemon*> pokemons = Pokemon::chargerPokemons("data/pokemon.csv");
        
        // 2. Charger les Joueurs, Leaders et Maitres
        std::vector<Joueur*> joueurs = Joueur::chargerJoueurs("data/joueur.csv", pokemons);
        std::vector<Leader*> leaders = Leader::chargerLeaders("data/leaders.csv", pokemons);
        std::vector<Maitre*> maitres = Maitre::chargerMaitres("data/maitres.csv", pokemons);
        
        // Vérifier que les données ont été chargées correctement
        if (joueurs.empty() || leaders.empty() || maitres.empty()) {
            throw std::runtime_error("Erreur de chargement des donnees!");
        }
        
        // Utiliser le premier joueur comme joueur principal
        Joueur* joueur = joueurs[0];
        
        UI::clearScreen();
        
        std::cout << "Bienvenue dans Pokemon Arena!" << std::endl;
        std::cout << "Vous jouez avec " << joueur->getNom() << "." << std::endl;
        UI::waitForEnter();
        
        // Boucle principale du menu
        bool running = true;
        while (running) {
            UI::afficherMenu();
            int choix = UI::getValidChoice(0, 5);
            
            switch (choix) {
                case 0: // Quitter
                    running = false;
                    std::cout << "Merci d'avoir joué à Pokemon Arena!" << std::endl;
                    break;
                    
                case 1: { // Gérer mes Pokemon
                    bool menuPokemon = true;
                    while (menuPokemon) {
                        UI::afficherMenuPokemon();
                        int choixPokemon = UI::getValidChoice(0, 4);
                        
                        switch (choixPokemon) {
                            case 0: // Retour
                                menuPokemon = false;
                                break;
                                
                            case 1: { // Afficher mes Pokemon
                                UI::clearScreen();
                                std::cout << "\n===== MES POKEMON =====" << std::endl;
                                UI::afficherPokemons(joueur);
                                UI::waitForEnter();
                                break;
                            }
                                
                            case 2: { // Soigner mes Pokemon
                                UI::clearScreen();
                                std::cout << "\n===== SOIGNER MES POKEMON =====" << std::endl;
                                joueur->soignerPokemons(); // Use the method from Entraineur
                                std::cout << "Tous vos Pokemon ont été soignés!" << std::endl;
                                UI::waitForEnter();
                                break;
                            }
                                
                            case 3: { // Interagir avec mes Pokemon
                                UI::clearScreen();
                                std::cout << "\n===== INTERAGIR AVEC MES POKEMON =====" << std::endl;
                                int pokemonCount = joueur->countPokemons(); // Use Entraineur's method
                                if (pokemonCount == 0) {
                                    std::cout << "Vous n'avez pas de Pokemon!" << std::endl;
                                } else {
                                    // Afficher les Pokemon et permettre de sélectionner
                                    UI::afficherPokemons(joueur);
                                    std::cout << "\nChoisissez un Pokemon (1-" 
                                            << pokemonCount << ") ou 0 pour annuler: ";
                                    int choixPokemonIndex = UI::getValidChoice(0, pokemonCount);
                                    
                                    if (choixPokemonIndex > 0) {
                                        // Adjust for correct indexing
                                        int actualIndex = 0;
                                        int currentCount = 0;
                                        for (int i = 0; i < 6; i++) {
                                            if (joueur->getPokemon(i) != nullptr) {
                                                currentCount++;
                                                if (currentCount == choixPokemonIndex) {
                                                    actualIndex = i;
                                                    break;
                                                }
                                            }
                                        }
                                        
                                        Pokemon* selectedPokemon = joueur->getPokemon(actualIndex);
                                        UI::clearScreen();
                                        std::cout << "Vous interagissez avec " << selectedPokemon->getNom() << "." << std::endl;
                                        std::cout << selectedPokemon->interagir() << std::endl;
                                    }
                                }
                                UI::waitForEnter();
                                break;
                            }
                                
                            case 4: { // Changer l'ordre des Pokemon
                                UI::clearScreen();
                                std::cout << "\n===== CHANGER L'ORDRE DE MES POKEMON =====" << std::endl;
                                int pokemonCount = joueur->countPokemons();
                                if (pokemonCount < 2) {
                                    std::cout << "Vous n'avez pas assez de Pokemon pour changer leur ordre!" << std::endl;
                                } else {
                                    UI::afficherPokemons(joueur);
                                    std::cout << "\nChoisissez le premier Pokemon (1-" 
                                            << pokemonCount << "): ";
                                    int choixIndex1 = UI::getValidChoice(1, pokemonCount);
                                    
                                    std::cout << "Choisissez le deuxième Pokemon (1-" 
                                            << pokemonCount << "): ";
                                    int choixIndex2 = UI::getValidChoice(1, pokemonCount);
                                    
                                    // Convert display indexes to actual array indexes
                                    int index1 = -1, index2 = -1;
                                    int currentCount = 0;
                                    
                                    for (int i = 0; i < 6; i++) {
                                        if (joueur->getPokemon(i) != nullptr) {
                                            currentCount++;
                                            if (currentCount == choixIndex1) index1 = i;
                                            if (currentCount == choixIndex2) index2 = i;
                                        }
                                    }
                                    
                                    if (index1 != -1 && index2 != -1 && index1 != index2) {
                                        joueur->echangerPokemons(index1, index2); // Use the method from Entraineur
                                        std::cout << "Ordre des Pokemon modifié!" << std::endl;
                                    }
                                }
                                UI::waitForEnter();
                                break;
                            }
                        }
                    }
                    break;
                }
                    
                case 2: { // Afficher mes statistiques
                    UI::clearScreen();
                    std::cout << "\n===== MES STATISTIQUES =====" << std::endl;
                    std::cout << "Nom: " << joueur->getNom() << std::endl;
                    std::cout << "Badges: " << joueur->getNbBadges() << std::endl;
                    std::cout << "Victoires: " << joueur->getNbVictoires() << std::endl;
                    std::cout << "Défaites: " << joueur->getNbDefaites() << std::endl;
                    std::cout << "\n===== MES POKEMON =====" << std::endl;
                    UI::afficherPokemons(joueur);
                    UI::waitForEnter();
                    break;
                }
                    
                case 3: { // Affronter un Leader de Gymnase
                    bool menuLeaders = true;
                    while (menuLeaders) {
                        // Filter out defeated leaders
                        std::vector<Leader*> availableLeaders;
                        for (auto leader : leaders) {
                            if (!leader->getEstVaincu()) {
                                availableLeaders.push_back(leader);
                            }
                        }
                        
                        // Check if there are any undefeated leaders
                        if (availableLeaders.empty()) {
                            UI::clearScreen();
                            std::cout << "\n===== LEADERS DE GYMNASE =====" << std::endl;
                            std::cout << "Vous avez déjà vaincu tous les Leaders de Gymnase!" << std::endl;
                            UI::waitForEnter();
                            menuLeaders = false;
                        } else {
                            // Show menu with available leaders only
                            UI::afficherMenuLeaders(availableLeaders);
                            int choixLeader = UI::getValidChoice(0, availableLeaders.size());
                            
                            if (choixLeader == 0) {
                                menuLeaders = false;
                            } else {
                                Leader* selectedLeader = availableLeaders[choixLeader - 1];
                                // Create and manage the combat directly without UI::demarrerCombat
                                Combat* combat = new Combat(joueur, selectedLeader);
                                combat->demarrer();
                                combat->afficher(); // This shows the final state
                                
                                delete combat;
                                UI::waitForEnter();
                                menuLeaders = false; // Return to main menu after a battle
                            }
                        }
                    }
                    break;
                }
                    
                case 4: { // Affronter un Maitre Pokemon
                    UI::clearScreen();
                    std::cout << "\n===== MAITRES POKEMON DISPONIBLES =====" << std::endl;
                    for (size_t i = 0; i < maitres.size(); ++i) {
                        std::cout << i + 1 << ". " << maitres[i]->getNom() << std::endl;
                    }
                    std::cout << "0. Retour" << std::endl;
                    std::cout << "Votre choix: ";
                    
                    int choixMaitre = UI::getValidChoice(0, maitres.size());
                    if (choixMaitre > 0) {
                        Maitre* selectedMaitre = maitres[choixMaitre - 1];
                        // Create and manage the combat directly
                        Combat* combat = new Combat(joueur, selectedMaitre);
                        combat->demarrer();
                        combat->afficher();
                        
                        delete combat;
                    }
                    UI::waitForEnter();
                    break;
                }
                    
                case 5: { // Interagir avec les entraineurs vaincus
                    UI::clearScreen();
                    std::cout << "\n===== ENTRAINEURS VAINCUS =====" << std::endl;
                    std::vector<Entraineur*> vaincus;
                    
                    // Collecter tous les entraineurs vaincus
                    for (auto leader : leaders) {
                        if (leader->getEstVaincu()) { // Use getEstVaincu() instead of estVaincu()
                            vaincus.push_back(leader);
                        }
                    }
                    
                    for (auto maitre : maitres) {
                        if (maitre->getEstVaincu()) { // Use getEstVaincu() instead of estVaincu()
                            vaincus.push_back(maitre);
                        }
                    }
                    
                    if (vaincus.empty()) {
                        std::cout << "Vous n'avez vaincu aucun entraineur pour l'instant." << std::endl;
                    } else {
                        for (size_t i = 0; i < vaincus.size(); ++i) {
                            std::cout << i + 1 << ". " << vaincus[i]->getNom() << std::endl;
                        }
                        std::cout << "0. Retour" << std::endl;
                        std::cout << "Choisissez un entraineur avec qui interagir: ";
                        
                        int choixVaincu = UI::getValidChoice(0, vaincus.size());
                        if (choixVaincu > 0) {
                            Entraineur* selectedVaincu = vaincus[choixVaincu - 1];
                            UI::clearScreen();
                            std::cout << "\n===== INTERACTION AVEC " << selectedVaincu->getNom() << " =====" << std::endl;
                            std::cout << selectedVaincu->interagir() << std::endl;
                        }
                    }
                    UI::waitForEnter();
                    break;
                }
            }
        }
        
        // Libération de la mémoire
        for (auto pokemon : pokemons) {
            delete pokemon;
        }
        
        for (auto joueur : joueurs) {
            delete joueur;
        }
        
        for (auto leader : leaders) {
            delete leader;
        }
        
        for (auto maitre : maitres) {
            delete maitre;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "ERREUR: " << e.what() << std::endl;
    }
    
    return 0;
}

