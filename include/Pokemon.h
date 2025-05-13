#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <algorithm>
#include "Interagir.h"

enum class Type {
    FEU,     
    EAU,     
    PLANTE,  
    ELECTRIK,
    GLACE,   
    COMBAT,  
    POISON,
    SOL,     
    VOL,     
    PSY,     
    INSECTE, 
    ROCHE,   
    SPECTRE, 
    DRAGON,
    TENEBRES, 
    ACIER,    
    FEE,      
    NORMAL
};

class Pokemon : public Interagir {
private:
    // Maps for type effectiveness
    static std::unordered_map<Type, std::vector<Type>> _faiblesses;
    static std::unordered_map<Type, std::vector<Type>> _resistances;
    static bool _typesInitialized;
    
    // Constants for multipliers
    static constexpr double MULTI_WEAK = 2.0;
    static constexpr double MULTI_RESIST = 0.5;
    static constexpr double MULTI_NORMAL = 1.0;
    
    // Method to initialize the type maps
    static void initTypeEffectiveness();
    
protected:
    const std::string _nom;         
    std::vector<Type> _types; 
    int _hp;                  
    int _maxHp;               
    const std::string _nomAttaque;  
    int _degatsAttaque;        

public:
    Pokemon(const std::string& nom, const std::vector<Type>& types, int hp, const std::string& nomAttaque, int degatsAttaque);
    
    virtual ~Pokemon() = default;
    
    // Getters and setters
    std::string getNom() const;
    std::vector<Type> getTypes() const;
    int getHp() const;
    int getMaxHp() const;
    std::string getNomAttaque() const;
    int getDegatsAttaque() const;
    
    void soigner();
    
    double calculerMultiplicateurType(Type typeAttaque) const;
    
    int attaquer(Pokemon& cible) const;
    
    int recevoirAttaque(int degats, Type typeAttaque);
    
    std::string getDetailedInfo() const;
    
    std::string ToString();
    
    static std::vector<Pokemon*> chargerPokemons(const std::string& nomFichier);
    
    static Type stringToType(const std::string& typeStr);
    
    static std::string typeToString(Type type);
    
    static double getMultiplicateur(Type typeDefense, Type typeAttaque);
    
    static std::vector<Type> getWeaknesses(Type type);
    static std::vector<Type> getResistances(Type type);


    // Interagir
    virtual std::string interagir() const override;
    virtual void afficher() const;
};