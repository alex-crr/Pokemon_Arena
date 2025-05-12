# Pokemon Arena

Un simulateur de combat Pokémon en C++ développé dans le cadre du cours de Programmation Orientée Objet.

## Fonctionnalités

- Système de combat au tour par tour
- Gestion des types de Pokémon avec leurs forces et faiblesses
- Différents types d'entraîneurs (Joueurs, Leaders, Maîtres)
- Interface utilisateur en console avec menus interactifs
- Système de progression avec badges à collecter
- Interaction avec les Pokémon et les entraîneurs

## Structure du projet

- `include/` - Fichiers d'en-tête (.h)
- `src/` - Fichiers source (.cpp)
- `data/` - Fichiers de données (.csv)
- `tests/` - Tests unitaires
- `docs/` - Documentation technique
- `tools/` - Outils utilitaires

## Compilation et exécution

### Prérequis

- Compilateur C++ compatible C++11 ou supérieur
- Make

### Compilation

```bash
make
```

### Exécution

```bash
make run
```

### Tests

```bash
make run-tests
```

## Diagramme UML

Vous pouvez générer le diagramme UML du projet avec l'outil fourni :

```bash
cd tools
python GenerateUML.py
```

Le diagramme UML sera généré au format PlantUML dans le dossier `docs/`.

## Concepts de POO utilisés

- **Héritage** : Hiérarchie de classes pour les Pokémon et les Entraîneurs
- **Polymorphisme** : Méthodes virtuelles pour les interactions et les attaques
- **Encapsulation** : Attributs privés/protégés avec getters et setters
- **Interfaces** : Classe abstraite Interagir pour unifier les comportements
- **Classes abstraites** : Pour définir des comportements génériques
- **Composition et agrégation** : Relations entre les objets du jeu

## Auteurs

- [Votre nom ici]
