#include "../include/Pokemon.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <vector>
#include <algorithm>

// Initialize static members
std::unordered_map<Type, std::vector<Type>> Pokemon::_faiblesses;
std::unordered_map<Type, std::vector<Type>> Pokemon::_resistances;
bool Pokemon::_typesInitialized = false;

/**
 * @brief Initialize the type effectiveness maps
 */
void Pokemon::initTypeEffectiveness() {
    if (_typesInitialized) return;
    
    // Initialize weaknesses
    _faiblesses = {
        {Type::FEU, {Type::EAU, Type::ROCHE, Type::SOL}},
        {Type::EAU, {Type::PLANTE, Type::ELECTRIK}},
        {Type::PLANTE, {Type::FEU, Type::GLACE, Type::POISON, Type::VOL, Type::INSECTE}},
        {Type::ELECTRIK, {Type::SOL}},
        {Type::GLACE, {Type::FEU, Type::COMBAT, Type::ROCHE, Type::ACIER}},
        {Type::COMBAT, {Type::VOL, Type::PSY, Type::FEE}},
        {Type::POISON, {Type::SOL, Type::PSY}},
        {Type::SOL, {Type::EAU, Type::PLANTE, Type::GLACE}},
        {Type::VOL, {Type::ELECTRIK, Type::GLACE, Type::ROCHE}},
        {Type::PSY, {Type::INSECTE, Type::SPECTRE, Type::TENEBRES}},
        {Type::INSECTE, {Type::FEU, Type::VOL, Type::ROCHE}},
        {Type::ROCHE, {Type::EAU, Type::PLANTE, Type::COMBAT, Type::SOL, Type::ACIER}},
        {Type::SPECTRE, {Type::SPECTRE, Type::TENEBRES}},
        {Type::DRAGON, {Type::GLACE, Type::DRAGON, Type::FEE}},
        {Type::TENEBRES, {Type::COMBAT, Type::INSECTE, Type::FEE}},
        {Type::ACIER, {Type::FEU, Type::COMBAT, Type::SOL}},
        {Type::FEE, {Type::POISON, Type::ACIER}},
        {Type::NORMAL, {Type::COMBAT}}
    };
    
    // Initialize resistances
    _resistances = {
        {Type::FEU, {Type::PLANTE, Type::GLACE, Type::INSECTE, Type::ACIER, Type::FEE}},
        {Type::EAU, {Type::FEU, Type::EAU, Type::GLACE, Type::ACIER}},
        {Type::PLANTE, {Type::EAU, Type::SOL, Type::ROCHE}},
        {Type::ELECTRIK, {Type::VOL, Type::ACIER, Type::ELECTRIK}},
        {Type::GLACE, {Type::GLACE}},
        {Type::COMBAT, {Type::ROCHE, Type::INSECTE, Type::TENEBRES}},
        {Type::POISON, {Type::PLANTE, Type::FEE, Type::COMBAT, Type::POISON, Type::INSECTE}},
        {Type::SOL, {Type::POISON, Type::ROCHE}},
        {Type::VOL, {Type::PLANTE, Type::COMBAT, Type::INSECTE}},
        {Type::PSY, {Type::COMBAT, Type::PSY}},
        {Type::INSECTE, {Type::PLANTE, Type::COMBAT, Type::SOL}},
        {Type::ROCHE, {Type::FEU, Type::VOL, Type::POISON, Type::NORMAL}},
        {Type::SPECTRE, {Type::POISON, Type::INSECTE}},
        {Type::DRAGON, {Type::FEU, Type::EAU, Type::ELECTRIK, Type::PLANTE}},
        {Type::TENEBRES, {Type::SPECTRE, Type::PSY, Type::TENEBRES}},
        {Type::ACIER, {Type::NORMAL, Type::PLANTE, Type::GLACE, Type::VOL, Type::PSY, Type::INSECTE, Type::ROCHE, Type::DRAGON, Type::ACIER, Type::FEE}},
        {Type::FEE, {Type::COMBAT, Type::INSECTE, Type::TENEBRES, Type::DRAGON}},
        {Type::NORMAL, {}}
    };
    
    _typesInitialized = true;
}

/**
 * @brief Get the multiplier between an attack type and a defense type
 * @param typeDefense The defending Pokémon's type
 * @param typeAttaque The attacking move's type
 * @return The damage multiplier (2.0 for weakness, 0.5 for resistance, 1.0 for neutral)
 */
double Pokemon::getMultiplicateur(Type typeDefense, Type typeAttaque) {
    if (!_typesInitialized) {
        initTypeEffectiveness();
    }
    
    // Check if the defense type is weak to the attack type
    auto& weakVector = _faiblesses[typeDefense];
    if (std::find(weakVector.begin(), weakVector.end(), typeAttaque) != weakVector.end()) {
        return MULTI_WEAK;
    }
    
    // Check if the defense type resists the attack type
    auto& resistVector = _resistances[typeDefense];
    if (std::find(resistVector.begin(), resistVector.end(), typeAttaque) != resistVector.end()) {
        return MULTI_RESIST;
    }
    
    // Otherwise, neutral effectiveness
    return MULTI_NORMAL;
}

/**
 * @brief Get all types that a given type is weak against
 * @param type The type to check weaknesses for
 * @return Vector of types that deal super effective damage
 */
std::vector<Type> Pokemon::getWeaknesses(Type type) {
    if (!_typesInitialized) {
        initTypeEffectiveness();
    }
    return _faiblesses[type];
}

/**
 * @brief Get all types that a given type resists
 * @param type The type to check resistances for
 * @return Vector of types that deal not very effective damage
 */
std::vector<Type> Pokemon::getResistances(Type type) {
    if (!_typesInitialized) {
        initTypeEffectiveness();
    }
    return _resistances[type];
}

/**
 * @brief Constructeur 
 * @param nom 
 * @param types Vector de types (1 ou 2)
 * @param hp Points de vie du Pokemon
 * @param nomAttaque 
 * @param degatsAttaque 
 */
Pokemon::Pokemon(const std::string& nom, const std::vector<Type>& types, int hp, const std::string& nomAttaque, int degatsAttaque)
    : _nom(nom), _types(types), _hp(hp), _maxHp(hp), _nomAttaque(nomAttaque), _degatsAttaque(degatsAttaque)
{
    // liste d'initialisation vue avant pour permettre le const. problème?
}

// Getters
std::string Pokemon::getNom() const 
{
    return _nom;
}

std::vector<Type> Pokemon::getTypes() const
{
    return _types;
}

int Pokemon::getHp() const
{
    return _hp;
}

int Pokemon::getMaxHp() const
{
    return _maxHp;
}

/**
 * @brief Obtenir le nom de l'attaque du Pokémon
 * @return Nom de l'attaque
 */
std::string Pokemon::getNomAttaque() const
{
    return _nomAttaque;
}

/**
 * @brief Obtenir les dégâts de l'attaque du Pokémon
 * @return Dégâts de l'attaque
 */
int Pokemon::getDegatsAttaque() const
{
    return _degatsAttaque;
}

/**
 * @brief Soigne le Pokémon (récupère des PV)
 */
void Pokemon::soigner()
{
    _hp = _maxHp;
}

/**
 * @brief Calculer le multiplicateur de type
 * @param typeAttaque Type de l'attaque
 * @return Multiplicateur de dégât (1.0 = neutre, 2.0 = force, 0.5 = faiblesse)
 */
double Pokemon::calculerMultiplicateurType(Type typeAttaque) const
{
    if (!_typesInitialized) {
        initTypeEffectiveness();
    }
    
    double totalMulti = 1.0;
    
    // Apply multiplier for each of the Pokémon's types
    for (const auto& type : _types) {
        totalMulti *= getMultiplicateur(type, typeAttaque);
    }
    
    return totalMulti;
}

/**
 * @brief Attaque un autre Pokémon
 * @param cible Pokémon cible
 * @return Dégâts infligés
 */
int Pokemon::attaquer(Pokemon &cible) const
{
    return cible.recevoirAttaque(_degatsAttaque, _types[0]); // Utilise le premier type comme type d'attaque A revoir pour infliger maximum de dégats
}

/**
 * @brief Reçoit une attaque
 * @param degats Dégât de base
 * @param typeAttaque type de l'attaque
 * @return Dégât infligés après force faiblesse
 */
int Pokemon::recevoirAttaque(int degats, Type typeAttaque)
{
    double multiplicateur = calculerMultiplicateurType(typeAttaque);
    int degatsFinal = static_cast<int>(degats * multiplicateur);
    _hp -= degatsFinal;
    if (_hp < 0)
        _hp = 0;

    return degatsFinal;

    // On tue le pokemon a la fin du tour
}

/**
 * @brief Affiche les informations du Pokémon
 */
void Pokemon::afficher() const
{
    std::cout << "Nom: " << _nom << std::endl;
    std::cout << "hp: " << _hp << "/" << _maxHp << std::endl;
}

/**
 * @brief Convertit un pokemon en une chaîne de caractères
 * @return string représentant le Pokémon
 */
std::string Pokemon::ToString()
{
    return _nom + " (" + std::to_string(_hp) + "/" + std::to_string(_maxHp) + " PV)";
}

/**
 * @brief Charge des pokémons depuis un fichier
 * @param nomFichier Nom du fichier
 * @return Vecteur de pointeurs vers des Pokémon
 */
std::vector<Pokemon*> Pokemon::chargerPokemons(const std::string &nomFichier)
{
    std::vector<Pokemon*> pokemons;
    std::ifstream fichier(nomFichier);
    
    if (!fichier.is_open()) {
        throw std::runtime_error("Impossible d'ouvrir le fichier: " + nomFichier);
    }
    
    std::string ligne;
    // Sauter l'en-tête
    std::getline(fichier, ligne);
    
    while (std::getline(fichier, ligne)) {
        std::istringstream ss(ligne);
        std::string nom, type1Str, type2Str, hpStr, nomAttaque, degatsAttaqueStr;
        
        // Format: Nom,Type 1,Type 2,Points de Vie,Attaque,Dégâts d'Attaque
        std::getline(ss, nom, ',');
        std::getline(ss, type1Str, ',');
        std::getline(ss, type2Str, ',');
        std::getline(ss, hpStr, ',');
        std::getline(ss, nomAttaque, ',');
        std::getline(ss, degatsAttaqueStr, ',');
        
        // Convertir les types en enum
        Type type1 = convertirStringEnType(type1Str);
        std::vector<Type> types = {type1};
        
        // Ajouter le second type si présent
        if (!type2Str.empty()) {
            Type type2 = convertirStringEnType(type2Str);
            types.push_back(type2);
        }
        
        // Convertir les valeurs numériques
        int hp = std::stoi(hpStr);
        int degatsAttaque = std::stoi(degatsAttaqueStr);
        
        // Créer le Pokemon et l'ajouter au vecteur
        Pokemon* pokemon = new Pokemon(nom, types, hp, nomAttaque, degatsAttaque);
        pokemons.push_back(pokemon);
    }
    
    return pokemons;
}

/**
 * @brief Convertit une chaîne en type Pokémon
 * @param typeStr Chaîne représentant le type
 * @return Enum Type correspondant
 */
Type Pokemon::convertirStringEnType(const std::string& typeStr)
{
    if (typeStr == "Feu") return Type::FEU;
    if (typeStr == "Eau") return Type::EAU;
    if (typeStr == "Plante") return Type::PLANTE;
    if (typeStr == "Électrik" || typeStr == "Electrik") return Type::ELECTRIK;
    if (typeStr == "Glace") return Type::GLACE;
    if (typeStr == "Combat") return Type::COMBAT;
    if (typeStr == "Poison") return Type::POISON;
    if (typeStr == "Sol") return Type::SOL;
    if (typeStr == "Vol") return Type::VOL;
    if (typeStr == "Psy") return Type::PSY;
    if (typeStr == "Insecte") return Type::INSECTE;
    if (typeStr == "Roche") return Type::ROCHE;
    if (typeStr == "Spectre") return Type::SPECTRE;
    if (typeStr == "Dragon") return Type::DRAGON;
    if (typeStr == "Ténèbres" || typeStr == "Tenebres") return Type::TENEBRES;
    if (typeStr == "Acier") return Type::ACIER;
    if (typeStr == "Fée" || typeStr == "Fee") return Type::FEE;
    return Type::NORMAL; // Par défaut
}

/**
 * @brief Convertit un Type en chaîne de caractères
 * @param type Type à convertir
 * @return String représentant le type
 */
std::string Pokemon::typeToString(Type type)
{
    switch(type) {
        case Type::FEU: return "Feu";
        case Type::EAU: return "Eau";
        case Type::PLANTE: return "Plante";
        case Type::ELECTRIK: return "Électrik";
        case Type::GLACE: return "Glace";
        case Type::COMBAT: return "Combat";
        case Type::POISON: return "Poison";
        case Type::SOL: return "Sol";
        case Type::VOL: return "Vol";
        case Type::PSY: return "Psy";
        case Type::INSECTE: return "Insecte";
        case Type::ROCHE: return "Roche";
        case Type::SPECTRE: return "Spectre";
        case Type::DRAGON: return "Dragon";
        case Type::TENEBRES: return "Ténèbres";
        case Type::ACIER: return "Acier";
        case Type::FEE: return "Fée";
        case Type::NORMAL: return "Normal";
        default: return "Inconnu";
    }
}

/**
 * @brief Implementation de la methode interagir pour Pokemon
 * @return Message unique pour chaque espece de Pokemon
 */
std::string Pokemon::interagir() const
{
    // Message unique selon l'espece de Pokemon
    if (_nom == "Pikachu") return _nom + " fait: Pika pika!";
    else if (_nom == "Salamèche" || _nom == "Reptincel" || _nom == "Dracaufeu") return _nom + " agite sa queue enflammee!";
    else if (_nom == "Carapuce" || _nom == "Carabaffe" || _nom == "Tortank") return _nom + " rentre dans sa carapace puis ressort!";
    else if (_nom == "Bulbizarre" || _nom == "Herbizarre" || _nom == "Florizarre") return _nom + " libere un doux parfum!";
    else if (_nom == "Mew") return _nom + " flotte autour de vous joyeusement!";
    else if (_nom == "Mewtwo") return _nom + " vous fixe intensement...";
    else if (_nom.find("Nidoran") != std::string::npos) return _nom + " agite ses oreilles!";
    else return _nom + " s'approche de vous amicalement!";
}

/**
 * @brief Obtient les informations détaillées du Pokémon
 * @return Chaîne avec toutes les informations détaillées du Pokémon
 */
std::string Pokemon::getDetailedInfo() const
{
    if (!_typesInitialized) {
        initTypeEffectiveness();
    }
    
    std::stringstream ss;
    
    ss << "====== " << _nom << " ======" << std::endl;
    ss << "Points de vie: " << _hp << "/" << _maxHp << std::endl;
    
    // Afficher les types
    ss << "Type(s): ";
    for (size_t i = 0; i < _types.size(); i++) {
        ss << typeToString(_types[i]);
        if (i < _types.size() - 1) ss << ", ";
    }
    ss << std::endl;
    
    // Afficher l'attaque et ses dégâts
    ss << "Attaque: " << _nomAttaque << " (" << _degatsAttaque << " degats)" << std::endl;
    
    // Afficher les faiblesses
    ss << "Faible contre: ";
    bool premierType = true;
    
    // Pour chaque type possible
    for (int t = 0; t < 18; t++) {
        Type testType = static_cast<Type>(t);
        if (calculerMultiplicateurType(testType) > 1.0) {
            if (!premierType) ss << ", ";
            ss << typeToString(testType);
            premierType = false;
        }
    }
    if (premierType) ss << "Aucun type";
    ss << std::endl;
    
    // Afficher les résistances
    ss << "Résiste à: ";
    premierType = true;
    
    // Pour chaque type possible
    for (int t = 0; t < 18; t++) {
        Type testType = static_cast<Type>(t);
        if (calculerMultiplicateurType(testType) < 1.0) {
            if (!premierType) ss << ", ";
            ss << typeToString(testType);
            premierType = false;
        }
    }
    if (premierType) ss << "Aucun type";
    ss << std::endl;
    
    return ss.str();
}