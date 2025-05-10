#include "../include/Leader.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>

/**
 * @brief Constructeur de Leader
 * @param nom Nom du leader
 * @param gymnase Gymnase associé au leader
 */
Leader::Leader(const std::string& nom, Gymnase gymnase)
    : Entraineur(nom), _gymnase(gymnase)
{
}

/**
 * @brief Constructeur de Leader avec Pokémon
 * @param nom Nom du leader
 * @param gymnase Gymnase associé au leader
 * @param pokemons Tableau des Pokémon du leader
 */
Leader::Leader(const std::string& nom, Gymnase gymnase, const std::array<Pokemon*, 6>& pokemons)
    : Entraineur(nom, pokemons), _gymnase(gymnase)
{
}

/**
 * @brief Constructeur de Leader avec badge
 * @param nom Nom du leader
 * @param gymnase Gymnase associé au leader
 * @param badge Badge attribué par le leader
 */
Leader::Leader(const std::string& nom, Gymnase gymnase, const std::string& badge)
    : Entraineur(nom), _gymnase(gymnase), _badge(badge)
{
}

/**
 * @brief Constructeur de Leader avec Pokémon et badge
 * @param nom Nom du leader
 * @param gymnase Gymnase associé au leader
 * @param badge Badge attribué par le leader
 * @param pokemons Tableau des Pokémon du leader
 */
Leader::Leader(const std::string& nom, Gymnase gymnase, const std::string& badge, const std::array<Pokemon*, 6>& pokemons)
    : Entraineur(nom, pokemons), _gymnase(gymnase), _badge(badge)
{
}

/**
 * @brief Destructeur de Leader
 */
Leader::~Leader()
{
}

/**
 * @brief Obtenir le gymnase associé au leader
 * @return Gymnase du leader
 */
Entraineur::Gymnase Leader::getGymnase() const
{
    return _gymnase;
}

/**
 * @brief Obtenir le badge attribué par le leader
 * @return Nom du badge
 */
std::string Leader::getBadge() const
{
    return _badge;
}

/**
 * @brief Affiche les informations du leader
 */
void Leader::afficher() const
{
    std::cout << "Leader: " << getNom() << std::endl;
    std::cout << "Gymnase: ";
    switch(_gymnase) {
        case Gymnase::G1: std::cout << "G1"; break;
        case Gymnase::G2: std::cout << "G2"; break;
        case Gymnase::G3: std::cout << "G3"; break;
        case Gymnase::G4: std::cout << "G4"; break;
        case Gymnase::G5: std::cout << "G5"; break;
        case Gymnase::G6: std::cout << "G6"; break;
        case Gymnase::G7: std::cout << "G7"; break;
        case Gymnase::G8: std::cout << "G8"; break;
    }
    std::cout << std::endl;
    std::cout << "Badge: " << _badge << std::endl;
    
    std::cout << "Pokemon:" << std::endl;
    for (int i = 0; i < 6; i++) {
        if (getPokemon(i) != nullptr) {
            std::cout << "  Pokemon " << (i+1) << ": ";
            getPokemon(i)->afficher();
        }
    }
}

/**
 * @brief Convertit une chaîne en enum Gymnase
 * @param gymnaseStr Chaîne représentant le gymnase
 * @return Enum Gymnase correspondant
 */
Entraineur::Gymnase Leader::convertirStringEnGymnase(const std::string& gymnaseStr)
{
    if (gymnaseStr == "G1" || gymnaseStr == "Arène d'Argenta") return Gymnase::G1;
    if (gymnaseStr == "G2" || gymnaseStr == "Arène d'Azuria") return Gymnase::G2;
    if (gymnaseStr == "G3" || gymnaseStr == "Arène de Carmin") return Gymnase::G3;
    if (gymnaseStr == "G4" || gymnaseStr == "Arène de Céladopole") return Gymnase::G4;
    if (gymnaseStr == "G5" || gymnaseStr == "Arène de Parmanie") return Gymnase::G5;
    if (gymnaseStr == "G6" || gymnaseStr == "Arène de Safrania") return Gymnase::G6;
    if (gymnaseStr == "G7" || gymnaseStr == "Arène de Cramois'Île") return Gymnase::G7;
    return Gymnase::G8; // Par défaut
}

/**
 * @brief Charge des leaders depuis un fichier CSV
 * @param nomFichier Chemin du fichier
 * @param pokemons Liste des pokémons disponibles pour l'association
 * @return Vecteur des leaders chargés
 */
std::vector<Leader*> Leader::chargerLeaders(const std::string& nomFichier, const std::vector<Pokemon*>& pokemons)
{
    std::vector<Leader*> leaders;
    std::ifstream fichier(nomFichier);
    
    if (!fichier.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier: " + nomFichier);
    }
    
    std::string ligne;
    // Sauter l'en-tête
    std::getline(fichier, ligne);
    
    while (std::getline(fichier, ligne)) {
        std::istringstream ss(ligne);
        std::string nom, gymnaseStr, badge;
        std::array<std::string, 6> nomPokemons;
        
        // Format: Nom,Gymnase,Medaille,Pokemon1,Pokemon2,Pokemon3,Pokemon4,Pokemon5,Pokemon6
        std::getline(ss, nom, ',');
        std::getline(ss, gymnaseStr, ',');
        std::getline(ss, badge, ',');
        for (int i = 0; i < 6; i++) {
            std::getline(ss, nomPokemons[i], ',');
        }
        
        // Créer le leader
        Gymnase gymnase = convertirStringEnGymnase(gymnaseStr);
        Leader* leader = new Leader(nom, gymnase, badge);
        
        // Associer les pokémons
        for (int i = 0; i < 6; i++) {
            if (!nomPokemons[i].empty()) {
                // Trouver le pokémon par nom
                auto it = std::find_if(pokemons.begin(), pokemons.end(), 
                    [&nomPokemons, i](Pokemon* p) { return p->getNom() == nomPokemons[i]; });
                
                if (it != pokemons.end()) {
                    leader->setPokemon(i, *it);
                }
            }
        }
        
        leaders.push_back(leader);
    }
    
    return leaders;
}
