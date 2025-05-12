#include "../include/Pokemon.h"
#include "../include/Joueur.h"
#include "../include/Leader.h"
#include "../include/Maitre.h"
#include "../include/Combat.h"
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

// Fonction pour nettoyer la console
void clearScreen() {
    #ifdef _WIN32
        system("cls"); // Pour Windows
    #else
        system("clear"); // Pour Unix/Linux/MacOS
    #endif
}

// Fonction pour afficher le menu principal
void afficherMenu() {
    clearScreen();
    std::cout << "\n===== MENU PRINCIPAL =====" << std::endl;
    std::cout << "1. Gérer mes Pokemon" << std::endl;  // Option regroupée
    std::cout << "2. Afficher mes statistiques" << std::endl;
    std::cout << "3. Affronter un Leader de Gymnase" << std::endl;
    std::cout << "4. Affronter un Maitre Pokemon" << std::endl;
    std::cout << "5. Interagir avec les entraineurs vaincus" << std::endl;
    std::cout << "0. Quitter" << std::endl;
    std::cout << "Votre choix: ";
}

// Fonction pour afficher le menu des Leaders
void afficherMenuLeaders(const std::vector<Leader*>& leaders) {
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

// Fonction pour afficher le menu de gestion des Pokemon
void afficherMenuPokemon() {
    clearScreen();
    std::cout << "\n===== GESTION DE MES POKEMON =====" << std::endl;
    std::cout << "1. Afficher mes Pokemon" << std::endl;
    std::cout << "2. Soigner mes Pokemon" << std::endl;
    std::cout << "3. Interagir avec mes Pokemon" << std::endl;
    std::cout << "4. Changer l'ordre de mes Pokemon" << std::endl;
    std::cout << "0. Retour au menu principal" << std::endl;
    std::cout << "Votre choix: ";
}

// Fonction pour obtenir un choix valide de l'utilisateur
int getValidChoice(int min, int max) {
    int choice;
    while (!(std::cin >> choice) || choice < min || choice > max) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Choix invalide. Veuillez entrer un nombre entre " << min << " et " << max << ": ";
    }
    return choice;
}

// Fonction pour attendre que l'utilisateur appuie sur Entrée
void waitForEnter() {
    std::cout << "\nAppuyez sur Entree pour continuer...";
    
    // Vider le buffer d'entrée et attendre la touche Entrée
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Fonction pour afficher les messages de combat avec regroupement des actions liées
void afficherMessagesCombat(const std::vector<std::string>& messages, const std::string& titreEntete) {
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

// Fonction pour gérer un combat
Combat* demarrerCombat(Joueur* joueur, Entraineur* entraineur, const std::string& titreCombat) {
    Combat* combat = new Combat(joueur, entraineur);
    std::vector<std::string> messages = combat->demarrer();
    
    afficherMessagesCombat(messages, titreCombat);
    waitForEnter();
    
    return combat;
}

// Add these helper functions to work with the Entraineur's Pokemons array
void afficherPokemons(const Entraineur* entraineur) {
    for (int i = 0; i < 6; i++) {
        Pokemon* pokemon = entraineur->getPokemon(i);
        if (pokemon != nullptr) {
            std::cout << i + 1 << ". ";
            pokemon->afficher();
        }
    }
}

void soignerPokemons(Entraineur* entraineur) {
    for (int i = 0; i < 6; i++) {
        Pokemon* pokemon = entraineur->getPokemon(i);
        if (pokemon != nullptr) {
            pokemon->soigner();
        }
    }
}

void echangerPokemons(Entraineur* entraineur, int index1, int index2) {
    if (index1 >= 0 && index1 < 6 && index2 >= 0 && index2 < 6) {
        Pokemon* temp = entraineur->getPokemon(index1);
        entraineur->setPokemon(index1, entraineur->getPokemon(index2));
        entraineur->setPokemon(index2, temp);
    }
}

// Helper to count non-null Pokemon
int countPokemons(const Entraineur* entraineur) {
    int count = 0;
    for (int i = 0; i < 6; i++) {
        if (entraineur->getPokemon(i) != nullptr) {
            count++;
        }
    }
    return count;
}

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
        
        std::cout << "Bienvenue dans Pokemon Arena!" << std::endl;
        std::cout << "Vous jouez avec " << joueur->getNom() << "." << std::endl;
        waitForEnter();
        
        // Boucle principale du menu
        bool running = true;
        while (running) {
            afficherMenu();
            int choix = getValidChoice(0, 5);
            
            switch (choix) {
                case 0: // Quitter
                    running = false;
                    std::cout << "Merci d'avoir joué à Pokemon Arena!" << std::endl;
                    break;
                    
                case 1: { // Gérer mes Pokemon
                    bool menuPokemon = true;
                    while (menuPokemon) {
                        afficherMenuPokemon();
                        int choixPokemon = getValidChoice(0, 4);
                        
                        switch (choixPokemon) {
                            case 0: // Retour
                                menuPokemon = false;
                                break;
                                
                            case 1: { // Afficher mes Pokemon
                                clearScreen();
                                std::cout << "\n===== MES POKEMON =====" << std::endl;
                                afficherPokemons(joueur);
                                waitForEnter();
                                break;
                            }
                                
                            case 2: { // Soigner mes Pokemon
                                clearScreen();
                                std::cout << "\n===== SOIGNER MES POKEMON =====" << std::endl;
                                soignerPokemons(joueur);
                                std::cout << "Tous vos Pokemon ont été soignés!" << std::endl;
                                waitForEnter();
                                break;
                            }
                                
                            case 3: { // Interagir avec mes Pokemon
                                clearScreen();
                                std::cout << "\n===== INTERAGIR AVEC MES POKEMON =====" << std::endl;
                                int pokemonCount = countPokemons(joueur);
                                if (pokemonCount == 0) {
                                    std::cout << "Vous n'avez pas de Pokemon!" << std::endl;
                                } else {
                                    // Afficher les Pokemon et permettre de sélectionner
                                    afficherPokemons(joueur);
                                    std::cout << "\nChoisissez un Pokemon (1-" 
                                            << pokemonCount << ") ou 0 pour annuler: ";
                                    int choixPokemonIndex = getValidChoice(0, pokemonCount);
                                    
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
                                        clearScreen();
                                        std::cout << "Vous interagissez avec " << selectedPokemon->getNom() << "." << std::endl;
                                        std::cout << selectedPokemon->interagir() << std::endl;
                                    }
                                }
                                waitForEnter();
                                break;
                            }
                                
                            case 4: { // Changer l'ordre des Pokemon
                                clearScreen();
                                std::cout << "\n===== CHANGER L'ORDRE DE MES POKEMON =====" << std::endl;
                                int pokemonCount = countPokemons(joueur);
                                if (pokemonCount < 2) {
                                    std::cout << "Vous n'avez pas assez de Pokemon pour changer leur ordre!" << std::endl;
                                } else {
                                    afficherPokemons(joueur);
                                    std::cout << "\nChoisissez le premier Pokemon (1-" 
                                            << pokemonCount << "): ";
                                    int choixIndex1 = getValidChoice(1, pokemonCount);
                                    
                                    std::cout << "Choisissez le deuxième Pokemon (1-" 
                                            << pokemonCount << "): ";
                                    int choixIndex2 = getValidChoice(1, pokemonCount);
                                    
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
                                        echangerPokemons(joueur, index1, index2);
                                        std::cout << "Ordre des Pokemon modifié!" << std::endl;
                                    }
                                }
                                waitForEnter();
                                break;
                            }
                        }
                    }
                    break;
                }
                    
                case 2: { // Afficher mes statistiques
                    clearScreen();
                    std::cout << "\n===== MES STATISTIQUES =====" << std::endl;
                    std::cout << "Nom: " << joueur->getNom() << std::endl;
                    std::cout << "Badges: " << joueur->getNbBadges() << std::endl;
                    std::cout << "Victoires: " << joueur->getNbVictoires() << std::endl;
                    std::cout << "Défaites: " << joueur->getNbDefaites() << std::endl;
                    std::cout << "\n===== MES POKEMON =====" << std::endl;
                    afficherPokemons(joueur);
                    waitForEnter();
                    break;
                }
                    
                case 3: { // Affronter un Leader de Gymnase
                    bool menuLeaders = true;
                    while (menuLeaders) {
                        afficherMenuLeaders(leaders);
                        int choixLeader = getValidChoice(0, leaders.size());
                        
                        if (choixLeader == 0) {
                            menuLeaders = false;
                        } else {
                            Leader* selectedLeader = leaders[choixLeader - 1];
                            Combat* combat = demarrerCombat(joueur, selectedLeader, 
                                                       "COMBAT CONTRE " + selectedLeader->getNom());
                            delete combat;
                            waitForEnter();
                            menuLeaders = false; // Retour au menu principal après un combat
                        }
                    }
                    break;
                }
                    
                case 4: { // Affronter un Maitre Pokemon
                    clearScreen();
                    std::cout << "\n===== MAITRES POKEMON DISPONIBLES =====" << std::endl;
                    for (size_t i = 0; i < maitres.size(); ++i) {
                        std::cout << i + 1 << ". " << maitres[i]->getNom() << std::endl;
                    }
                    std::cout << "0. Retour" << std::endl;
                    std::cout << "Votre choix: ";
                    
                    int choixMaitre = getValidChoice(0, maitres.size());
                    if (choixMaitre > 0) {
                        Maitre* selectedMaitre = maitres[choixMaitre - 1];
                        Combat* combat = demarrerCombat(joueur, selectedMaitre, 
                                                   "COMBAT CONTRE " + selectedMaitre->getNom());
                        delete combat;
                    }
                    waitForEnter();
                    break;
                }
                    
                case 5: { // Interagir avec les entraineurs vaincus
                    clearScreen();
                    std::cout << "\n===== ENTRAINEURS VAINCUS =====" << std::endl;
                    std::vector<Entraineur*> vaincus;
                    
                    // Collecter tous les entraineurs vaincus
                    for (auto leader : leaders) {
                        if (leader->getEstVaincu()) { // Changed from estVaincu() to getEstVaincu()
                            vaincus.push_back(leader);
                        }
                    }
                    
                    for (auto maitre : maitres) {
                        if (maitre->getEstVaincu()) { // Changed from estVaincu() to getEstVaincu()
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
                        
                        int choixVaincu = getValidChoice(0, vaincus.size());
                        if (choixVaincu > 0) {
                            Entraineur* selectedVaincu = vaincus[choixVaincu - 1];
                            clearScreen();
                            std::cout << "\n===== INTERACTION AVEC " << selectedVaincu->getNom() << " =====" << std::endl;
                            std::cout << selectedVaincu->getNom() << " : \"Vous m'avez battu, mais je m'entraîne dur pour notre revanche!\"" << std::endl;
                            
                            // On pourrait ajouter d'autres interactions ici
                        }
                    }
                    waitForEnter();
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

