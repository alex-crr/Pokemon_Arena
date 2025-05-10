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
        std::cout << i + 1 << ". " << leaders[i]->getNom() << " (Gymnase: ";
        switch(leaders[i]->getGymnase()) {
            case Entraineur::Gymnase::ARGENTA: std::cout << "Arène d'Argenta"; break;
            case Entraineur::Gymnase::AZURIA: std::cout << "Arène d'Azuria"; break;
            case Entraineur::Gymnase::CARMIN: std::cout << "Arène de Carmin sur Mer"; break;
            case Entraineur::Gymnase::CELADOPOLE: std::cout << "Arène de Céladopole"; break;
            case Entraineur::Gymnase::PARMANIE: std::cout << "Arène de Parmanie"; break;
            case Entraineur::Gymnase::SAFRANIA: std::cout << "Arène de Safrania"; break;
            case Entraineur::Gymnase::CRAMOISILE: std::cout << "Arène de Cramois'Île"; break;
            case Entraineur::Gymnase::JADIELLE: std::cout << "Arène de Jadielle"; break;
        }
        std::cout << ", Badge: " << leaders[i]->getBadge() << ")" << std::endl;
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

int main() {
    try {
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
        
        // Stocker les combats effectués
        std::vector<Combat*> combatsEffectues;
        
        bool continuer = true;
        while (continuer) {
            afficherMenu();
            int choix = getValidChoice(0, 5);  // Ajusté pour le nouveau menu
            
            switch(choix) {
                case 1: // Gérer mes Pokemon
                {
                    bool retourMenuPrincipal = false;
                    while (!retourMenuPrincipal) {
                        afficherMenuPokemon();
                        int choixPokemon = getValidChoice(0, 4);
                        
                        switch(choixPokemon) {
                            case 1: // Afficher mes Pokemon
                            {
                                clearScreen();
                                std::cout << "\n===== MES POKEMON =====" << std::endl;
                                for (int i = 0; i < 6; ++i) {
                                    Pokemon* pokemon = joueur->getPokemon(i);
                                    if (pokemon) {
                                        std::cout << i + 1 << ".\n";
                                        std::cout << pokemon->getDetailedInfo() << std::endl;
                                        std::cout << "------------------------" << std::endl;
                                    }
                                }
                                waitForEnter();
                                break;
                            }
                                
                            case 2: // Soigner mes Pokemon
                            {
                                clearScreen();
                                std::cout << "\nSoigner tous mes Pokemon..." << std::endl;
                                for (int i = 0; i < 6; ++i) {
                                    Pokemon* pokemon = joueur->getPokemon(i);
                                    if (pokemon) {
                                        pokemon->soigner();
                                        std::cout << pokemon->getNom() << " a ete soigne! (" 
                                                 << pokemon->getHp() << "/" << pokemon->getMaxHp() << " PV)" << std::endl;
                                    }
                                }
                                waitForEnter();
                                break;
                            }
                            
                            case 3: // Interagir avec mes Pokemon
                            {
                                clearScreen();
                                std::cout << "\n===== INTERAGIR AVEC MES POKEMON =====" << std::endl;
                                
                                // Afficher la liste des Pokémon disponibles
                                std::cout << "Vos Pokemon disponibles:" << std::endl;
                                bool pokemonTrouves = false;
                                for (int i = 0; i < 6; ++i) {
                                    Pokemon* pokemon = joueur->getPokemon(i);
                                    if (pokemon) {
                                        pokemonTrouves = true;
                                        std::cout << i + 1 << ". " << pokemon->getNom() 
                                                  << " (PV: " << pokemon->getHp() << "/" << pokemon->getMaxHp() << ")" << std::endl;
                                    }
                                }
                                
                                if (!pokemonTrouves) {
                                    std::cout << "Vous n'avez pas de Pokemon disponible." << std::endl;
                                    waitForEnter();
                                    break;
                                }
                                
                                std::cout << "0. Retour" << std::endl;
                                std::cout << "Choisissez un Pokemon pour interagir: ";
                                int choixInteraction = getValidChoice(0, 6);
                                
                                if (choixInteraction > 0) {
                                    Pokemon* pokemon = joueur->getPokemon(choixInteraction - 1);
                                    if (pokemon) {
                                        clearScreen();
                                        std::cout << "\n===== INTERACTION AVEC " << pokemon->getNom() << " =====" << std::endl;
                                        std::cout << pokemon->interagir() << std::endl;
                                    } else {
                                        std::cout << "Pas de Pokemon à cette position." << std::endl;
                                    }
                                    waitForEnter();
                                }
                                break;
                            }
                            
                            case 4: // Changer l'ordre des Pokemon
                            {
                                clearScreen();
                                std::cout << "\n===== CHANGER L'ORDRE DE MES POKEMON =====" << std::endl;
                                
                                // Afficher la liste des Pokémon disponibles
                                std::cout << "Vos Pokemon actuels:" << std::endl;
                                bool pokemonTrouves = false;
                                for (int i = 0; i < 6; ++i) {
                                    Pokemon* pokemon = joueur->getPokemon(i);
                                    if (pokemon) {
                                        pokemonTrouves = true;
                                        std::cout << i + 1 << ". " << pokemon->getNom() 
                                                  << " (PV: " << pokemon->getHp() << "/" << pokemon->getMaxHp() << ")" << std::endl;
                                    } else {
                                        std::cout << i + 1 << ". [vide]" << std::endl;
                                    }
                                }
                                
                                if (!pokemonTrouves) {
                                    std::cout << "Vous n'avez pas de Pokemon disponible." << std::endl;
                                    waitForEnter();
                                    break;
                                }
                                
                                std::cout << "\nChoisissez le premier Pokemon à échanger (0 pour annuler): ";
                                int index1 = getValidChoice(0, 6);
                                
                                if (index1 == 0) {
                                    std::cout << "Échange annulé." << std::endl;
                                    waitForEnter();
                                    break;
                                }
                                
                                if (!joueur->getPokemon(index1 - 1)) {
                                    std::cout << "Emplacement vide sélectionné." << std::endl;
                                    waitForEnter();
                                    break;
                                }
                                
                                std::cout << "Choisissez le deuxième Pokemon à échanger (0 pour annuler): ";
                                int index2 = getValidChoice(0, 6);
                                
                                if (index2 == 0 || index1 == index2) {
                                    std::cout << "Échange annulé." << std::endl;
                                    waitForEnter();
                                    break;
                                }
                                
                                // Échanger les Pokémon
                                Pokemon* temp = joueur->getPokemon(index1 - 1);
                                joueur->setPokemon(index1 - 1, joueur->getPokemon(index2 - 1));
                                joueur->setPokemon(index2 - 1, temp);
                                
                                std::cout << "Échange effectué avec succès!" << std::endl;
                                waitForEnter();
                                break;
                            }
                                
                            case 0: // Retour au menu principal
                                retourMenuPrincipal = true;
                                break;
                        }
                    }
                    break;
                }
                    
                case 2: // Afficher mes statistiques
                {
                    clearScreen();
                    std::cout << "\n===== MES STATISTIQUES =====" << std::endl;
                    std::cout << "Joueur: " << joueur->getNom() << std::endl;
                    std::cout << "Victoires: " << joueur->getNbVictoires() << std::endl;
                    std::cout << "Defaites: " << joueur->getNbDefaites() << std::endl;
                    std::cout << "Badges: " << joueur->getNbBadges() << std::endl;
                    waitForEnter();
                    break;
                }
                    
                case 3: // Affronter un Leader de Gymnase
                {
                    bool retourMenu = false;
                    while (!retourMenu) {
                        afficherMenuLeaders(leaders);
                        int choixLeader = getValidChoice(0, leaders.size());
                        
                        if (choixLeader == 0) {
                            retourMenu = true;
                        } else {
                            Leader* leader = leaders[choixLeader - 1];
                            Combat* combat = demarrerCombat(joueur, leader, "DÉBUT DU COMBAT");
                            combatsEffectues.push_back(combat);
                            retourMenu = true;  // Retour au menu principal après un combat
                        }
                    }
                    break;
                }
                    
                case 4: // Affronter un Maitre Pokemon
                {
                    clearScreen();
                    if (joueur->getNbBadges() < 8) {
                        std::cout << "\nVous devez obtenir 8 badges pour affronter un Maitre Pokemon!" << std::endl;
                        std::cout << "Vous avez actuellement " << joueur->getNbBadges() << " badges." << std::endl;
                    } else {
                        // Sélectionner un Maître aléatoirement
                        int indexMaitre = std::rand() % maitres.size();
                        Maitre* maitre = maitres[indexMaitre];
                        
                        std::cout << "\nVous allez affronter le Maitre Pokemon " << maitre->getNom() << "!" << std::endl;
                        std::this_thread::sleep_for(std::chrono::seconds(2));
                        
                        Combat* combat = demarrerCombat(joueur, maitre, "COMBAT DE MAÎTRE POKÉMON");
                        combatsEffectues.push_back(combat);
                    }
                    waitForEnter();
                    break;
                }
                    
                case 5: // Interagir avec les entraineurs vaincus
                {
                    clearScreen();
                    std::cout << "\n===== ENTRAINEURS VAINCUS =====" << std::endl;
                    
                    // Afficher les leaders vaincus
                    std::vector<Entraineur*> entraineurs;
                    for (auto leader : leaders) {
                        if (leader->getEstVaincu()) {
                            entraineurs.push_back(leader);
                        }
                    }
                    
                    // Afficher les maîtres vaincus
                    for (auto maitre : maitres) {
                        if (maitre->getEstVaincu()) {
                            entraineurs.push_back(maitre);
                        }
                    }
                    
                    if (entraineurs.empty()) {
                        std::cout << "Vous n'avez vaincu aucun entraineur pour le moment." << std::endl;
                        waitForEnter();
                        break;
                    }
                    
                    // Afficher la liste des entraineurs vaincus
                    for (size_t i = 0; i < entraineurs.size(); ++i) {
                        std::string type = dynamic_cast<Maitre*>(entraineurs[i]) ? "Maitre" : "Leader";
                        std::cout << i + 1 << ". " << type << " " << entraineurs[i]->getNom() << std::endl;
                    }
                    
                    std::cout << "0. Retour" << std::endl;
                    std::cout << "Choisissez un entraineur pour interagir: ";
                    
                    int choixEntraineur = getValidChoice(0, entraineurs.size());
                    
                    if (choixEntraineur > 0) {
                        Entraineur* entraineurChoisi = entraineurs[choixEntraineur - 1];
                        clearScreen();
                        std::cout << "\n===== INTERACTION AVEC " << entraineurChoisi->getNom() << " =====" << std::endl;
                        std::cout << entraineurChoisi->interagir() << std::endl;
                        waitForEnter();
                    }
                    break;
                }
                
                case 0: // Quitter
                    clearScreen();
                    std::cout << "Merci d'avoir joue! A bientot!" << std::endl;
                    continuer = false;
                    break;
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
        
        for (auto combat : combatsEffectues) {
            delete combat;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "ERREUR: " << e.what() << std::endl;
    }
    
    return 0;
}

