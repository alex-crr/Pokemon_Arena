#include "../include/Joueur.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

/**
 * @brief Constructeur de Joueur
 * @param nom Nom du joueur
 */
Joueur::Joueur(const std::string& nom)
    : Entraineur(nom), _nbVictoires(0), _nbDefaites(0), _nbBadges(0)
{
}

/**
 * @brief Constructeur de Joueur avec Pokémon
 * @param nom Nom du joueur
 * @param pokemons Tableau des Pokémon du joueur
 */
Joueur::Joueur(const std::string& nom, const std::array<Pokemon*, 6>& pokemons)
    : Entraineur(nom, pokemons), _nbVictoires(0), _nbDefaites(0), _nbBadges(0)
{
}

/**
 * @brief Destructeur de Joueur
 */
Joueur::~Joueur()
{
}

/**
 * @brief Obtenir le nombre de victoires du joueur
 * @return Nombre de victoires
 */
int Joueur::getNbVictoires() const
{
    return _nbVictoires;
}

/**
 * @brief Obtenir le nombre de défaites du joueur
 * @return Nombre de défaites
 */
int Joueur::getNbDefaites() const
{
    return _nbDefaites;
}

/**
 * @brief Obtenir le nombre de badges du joueur
 * @return Nombre de badges
 */
int Joueur::getNbBadges() const
{
    return _nbBadges;
}

/**
 * @brief Incrémente le nombre de victoires
 */
void Joueur::incrementVictoires()
{
    _nbVictoires++;
}

/**
 * @brief Incrémente le nombre de défaites
 */
void Joueur::incrementDefaites()
{
    _nbDefaites++;
}

/**
 * @brief Incrémente le nombre de badges
 */
void Joueur::incrementBadges()
{
    _nbBadges++;
}

/**
 * @brief Affiche les informations du joueur
 */
void Joueur::afficher() const
{
    std::cout << "Joueur: " << getNom() << std::endl;
    std::cout << "Victoires: " << _nbVictoires << " | Defaites: " << _nbDefaites << " | Badges: " << _nbBadges << std::endl;
    std::cout << "Pokemon:" << std::endl;
    for (int i = 0; i < 6; i++) {
        if (getPokemon(i) != nullptr) {
            std::cout << "  Pokemon " << (i+1) << ": ";
            getPokemon(i)->afficher();
        }
    }
}

/**
 * @brief Charge des joueurs depuis un fichier CSV
 * @param nomFichier Chemin du fichier
 * @param pokemons Liste des pokémons disponibles pour l'association
 * @return Vecteur des joueurs chargés
 */
std::vector<Joueur*> Joueur::chargerJoueurs(const std::string& nomFichier, const std::vector<Pokemon*>& pokemons)
{
    std::vector<Joueur*> joueurs;
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
        
        // Créer le joueur
        Joueur* joueur = new Joueur(nom);
        
        // Associer les pokémons
        for (int i = 0; i < 6; i++) {
            if (!nomPokemons[i].empty()) {
                // Trouver le pokémon par nom
                auto it = std::find_if(pokemons.begin(), pokemons.end(), 
                    [&nomPokemons, i](Pokemon* p) { return p->getNom() == nomPokemons[i]; });
                
                if (it != pokemons.end()) {
                    joueur->setPokemon(i, *it);
                }
            }
        }
        
        joueurs.push_back(joueur);
    }
    
    return joueurs;
}
