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
    
    // Pokemon-display functions
    static void afficherPokemons(const Entraineur* entraineur);
};
