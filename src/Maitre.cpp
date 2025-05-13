#include "../include/Maitre.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

/**
 * @brief Constructeur de Maître
 * @param nom Nom du maître
 */
Maitre::Maitre(const std::string& nom)
    : Entraineur(nom)
{
}

/**
 * @brief Constructeur de Maître avec Pokémon
 * @param nom Nom du maître
 * @param pokemons Tableau des Pokémon du maître
 */
Maitre::Maitre(const std::string& nom, const std::array<Pokemon*, 6>& pokemons)
    : Entraineur(nom, pokemons)
{
}

/**
 * @brief Destructeur de Maître
 */
Maitre::~Maitre()
{
}

/**
 * @brief Attaque avec bonus de dégâts de 25%
 * @param pokemonAllie Pokémon qui attaque
 * @param cible Pokémon cible de l'attaque
 * @return Dégâts infligés avec le bonus
 */
int Maitre::attaquerAvecBonus(Pokemon& pokemonAllie, Pokemon& cible) const
{
    // Get attack type and damage
    Type attackType = pokemonAllie.getTypes()[0];
    int baseDamage = pokemonAllie.getDegatsAttaque();
    
    // Apply Master bonus to base damage (25% increase)
    int bonusDamage = static_cast<int>(baseDamage * BONUS_DEGATS);
    
    // Apply the boosted damage directly to the target
    int actualDamage = cible.recevoirAttaque(bonusDamage, attackType);
    
    return actualDamage;
}

/**
 * @brief Affiche les informations du maître
 */
void Maitre::afficher() const
{
    std::cout << "Maître Pokémon: " << getNom() << std::endl;
    std::cout << "Bonus de dégâts: 25%" << std::endl;
    std::cout << "Pokemon:" << std::endl;
    for (int i = 0; i < 6; i++) {
        if (getPokemon(i) != nullptr) {
            std::cout << "  Pokemon " << (i+1) << ": ";
            getPokemon(i)->afficher();
        }
    }
}

/**
 * @brief Charge des maîtres depuis un fichier CSV
 * @param nomFichier Chemin du fichier
 * @param pokemons Liste des pokémons disponibles pour l'association
 * @return Vecteur des maîtres chargés
 */
std::vector<Maitre*> Maitre::chargerMaitres(const std::string& nomFichier, const std::vector<Pokemon*>& pokemons)
{
    std::vector<Maitre*> maitres;
    std::ifstream fichier(nomFichier);
    
    if (!fichier.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier: " + nomFichier);
    }
    
    std::string ligne;
    // Sauter l'en-tête
    std::getline(fichier, ligne);
    
    while (std::getline(fichier, ligne)) {
        std::istringstream ss(ligne);
        std::string nom;
        std::array<std::string, 6> nomPokemons;
        
        // Format: Nom,Pokemon1,Pokemon2,Pokemon3,Pokemon4,Pokemon5,Pokemon6
        std::getline(ss, nom, ',');
        for (int i = 0; i < 6; i++) {
            std::getline(ss, nomPokemons[i], ',');
        }
        
        // Créer le maître
        Maitre* maitre = new Maitre(nom);
        
        // Associer les pokémons
        for (int i = 0; i < 6; i++) {
            if (!nomPokemons[i].empty()) {
                // Trouver le pokémon par nom
                auto it = std::find_if(pokemons.begin(), pokemons.end(), 
                    [&nomPokemons, i](Pokemon* p) { return p->getNom() == nomPokemons[i]; });
                
                if (it != pokemons.end()) {
                    // Create a copy of the Pokemon instead of using the original
                    Pokemon* pokemonCopy = new Pokemon(**it);
                    maitre->setPokemon(i, pokemonCopy);
                }
            }
        }
        
        maitres.push_back(maitre);
    }
    
    return maitres;
}
