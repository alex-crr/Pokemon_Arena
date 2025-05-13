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

/**
 * @brief Soigne tous les Pokémon de l'entraineur
 */
void Entraineur::soignerPokemons()
{
    for (int i = 0; i < 6; i++) {
        if (_pokemons[i] != nullptr) {
            _pokemons[i]->soigner();
        }
    }
}

/**
 * @brief Échange la position de deux Pokémon dans l'équipe
 * @param index1 Position du premier Pokémon
 * @param index2 Position du deuxième Pokémon
 */
void Entraineur::echangerPokemons(int index1, int index2)
{
    if (index1 >= 0 && index1 < 6 && index2 >= 0 && index2 < 6) {
        Pokemon* temp = _pokemons[index1];
        _pokemons[index1] = _pokemons[index2];
        _pokemons[index2] = temp;
    }
}

/**
 * @brief Compte le nombre de Pokémon non-nuls dans l'équipe
 * @return Nombre de Pokémon
 */
int Entraineur::countPokemons() const
{
    int count = 0;
    for (int i = 0; i < 6; i++) {
        if (_pokemons[i] != nullptr) {
            count++;
        }
    }
    return count;
}
