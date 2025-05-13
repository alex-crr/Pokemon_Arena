#pragma once

#include <vector>
#include <string>
#include "../include/Pokemon.h"
#include "../include/Joueur.h"
#include "../include/Leader.h"
#include "../include/Maitre.h"
#include "../include/Combat.h"

class UI {
public:
    // Menu-related functions
    static void clearScreen();
    static void afficherMenu();
    static void afficherMenuLeaders(const std::vector<Leader*>& leaders);
    static void afficherMenuPokemon();
    
    // User input functions
    static int getValidChoice(int min, int max);
    static void waitForEnter();
    
    // Combat-related functions
    static void afficherMessagesCombat(const std::vector<std::string>& messages, const std::string& titreEntete);
    static Combat* demarrerCombat(Joueur* joueur, Entraineur* entraineur);
    
    // Pokemon-related functions
    static void afficherPokemons(const Entraineur* entraineur);
    static void soignerPokemons(Entraineur* entraineur);
    static void echangerPokemons(Entraineur* entraineur, int index1, int index2);
    static int countPokemons(const Entraineur* entraineur);
};
