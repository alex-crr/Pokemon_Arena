#include "../include/Entraineur.h"
#include <iostream>

/**
 * @brief Constructeur d'Entraineur
 * @param nom Nom de l'entraineur
 */
Entraineur::Entraineur(const std::string& nom)
    : _nom(nom), _estVaincu(false)
{
    // Initialize pokemons array with nullptr
    for (int i = 0; i < 6; i++) {
        _pokemons[i] = nullptr;
    }
}

/**
 * @brief Constructeur d'Entraineur avec Pokémon
 * @param nom Nom de l'entraineur
 * @param pokemons Tableau des Pokémon de l'entraineur
 */
Entraineur::Entraineur(const std::string& nom, const std::array<Pokemon*, 6>& pokemons)
    : _nom(nom), _pokemons(pokemons), _estVaincu(false)
{
}

/**
 * @brief Destructeur d'Entraineur
 */
Entraineur::~Entraineur()
{
    // Clean up Pokemon pointers - assume Entraineur owns the Pokemon
    for (int i = 0; i < 6; i++) {
        if (_pokemons[i] != nullptr) {
            delete _pokemons[i];
            _pokemons[i] = nullptr;
        }
    }
}

/**
 * @brief Obtenir le nom de l'entraineur
 * @return Nom de l'entraineur
 */
std::string Entraineur::getNom() const
{
    return _nom;
}

/**
 * @brief Obtenir le tableau de Pokémon
 * @return Tableau des Pokémon de l'entraineur
 */
const std::array<Pokemon*, 6>& Entraineur::getPokemons() const
{
    return _pokemons;
}

/**
 * @brief Obtenir un Pokémon spécifique
 * @param index Position du Pokémon (0-5)
 * @return Pointeur vers le Pokémon ou nullptr
 */
Pokemon* Entraineur::getPokemon(int index) const
{
    if (index >= 0 && index < 6) {
        return _pokemons[index];
    }
    return nullptr;
}

/**
 * @brief Définir un Pokémon à une position spécifique
 * @param index Position du Pokémon (0-5)
 * @param pokemon Pointeur vers le Pokémon à assigner
 */
void Entraineur::setPokemon(int index, Pokemon* pokemon)
{
    if (index >= 0 && index < 6) {
        _pokemons[index] = pokemon;
    }
}

/**
 * @brief Affiche les informations de l'entraineur
 */
void Entraineur::afficher() const
{
    std::cout << "Entraineur: " << _nom << std::endl;
    std::cout << "Pokemon:" << std::endl;
    for (int i = 0; i < 6; i++) {
        if (_pokemons[i] != nullptr) {
            std::cout << "  Pokemon " << (i+1) << ": ";
            _pokemons[i]->afficher();
        }
    }
}

/**
 * @brief Vérifie si tous les Pokémon de l'entraineur sont KO
 * @return true si tous les Pokémon sont KO, false sinon
 */
bool Entraineur::estKO() const
{
    for (const auto& pokemon : _pokemons) {
        if (pokemon != nullptr && pokemon->getHp() > 0) {
            return false; // At least one Pokemon is not KO
        }
    }
    return true; // All Pokemon are KO or nullptr
}

/**
 * @brief Obtenir si l'entraineur a été vaincu
 * @return true si l'entraineur a été vaincu, false sinon
 */
bool Entraineur::getEstVaincu() const
{
    return _estVaincu;
}

/**
 * @brief Définir si l'entraineur a été vaincu
 * @param estVaincu Nouvel état de l'entraineur
 */
void Entraineur::setEstVaincu(bool estVaincu)
{
    _estVaincu = estVaincu;
}

/**
 * @brief Implementation de la methode interagir pour Entraineur
 * @return Message de l'entraineur s'il a été vaincu
 */
std::string Entraineur::interagir() const
{
    if (!_estVaincu) {
        return "Vous ne pouvez pas interagir avec " + _nom + " car vous ne l'avez pas encore vaincu.";
    }
    return _nom + " : \"Bravo pour ta victoire! Continue de t'entrainer pour devenir encore plus fort!\"";
}

std::string Entraineur::getGymName(Entraineur::Gymnase gym) {
    switch(gym) {
        case Gymnase::ARGENTA: return "Arène d'Argenta";
        case Gymnase::AZURIA: return "Arène d'Azuria";
        case Gymnase::CARMIN: return "Arène de Carmin sur Mer";
        case Gymnase::CELADOPOLE: return "Arène de Céladopole";
        case Gymnase::PARMANIE: return "Arène de Parmanie";
        case Gymnase::SAFRANIA: return "Arène de Safrania";
        case Gymnase::CRAMOISILE: return "Arène de Cramois'Île";
        case Gymnase::JADIELLE: return "Arène de Jadielle";
        default: return "Gymnase inconnu";
    }
}

/**
 * @brief Convertit une chaîne en enum Gymnase
 * @param gymnaseStr Chaîne représentant le gymnase
 * @return Enum Gymnase correspondant
 */
Entraineur::Gymnase Entraineur::convertirStringEnGymnase(const std::string& gymnaseStr)
{
    if (gymnaseStr == "Arène d'Argenta") return Gymnase::ARGENTA;
    if (gymnaseStr == "Arène d'Azuria") return Gymnase::AZURIA;
    if (gymnaseStr == "Arène de Carmin-sur-Mer") return Gymnase::CARMIN;
    if (gymnaseStr == "Arène de Céladopole") return Gymnase::CELADOPOLE;
    if (gymnaseStr == "Arène de Parmanie") return Gymnase::PARMANIE;
    if (gymnaseStr == "Arène de Safrania") return Gymnase::SAFRANIA;
    if (gymnaseStr == "Arène de Cramois'Île") return Gymnase::CRAMOISILE;
    return Gymnase::JADIELLE; // Par défaut
}
