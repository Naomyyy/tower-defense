# 🏰 Monsters Attack

**Monsters Attack** est un jeu de type **Tower Defense** développé en **C++**, en utilisant les principes de la **Programmation Orientée Objet (POO)**.  
L’objectif du jeu est d’empêcher les monstres d’atteindre la fin du chemin en plaçant stratégiquement différentes tours sur la carte.

## 🎮 Gameplay

- Le joueur doit défendre la carte contre des vagues d’ennemis.
- Chaque tour possède des caractéristiques spécifiques (vitesse, dégâts, type d’attaque).
- Les tours **ne peuvent pas être placées sur le chemin des ennemis**. Elles peuvent uniquement être placées sur les blocs bleus indiqués sur la carte.
- Le jeu contient des menus de navigation : menu de démarrage et de choix de la difficulté, menu de pause (en appuyant sur la touche **Esc**) et menu de fin de partie (victoire ou défaite).

## 🛠️ Technologies utilisées

- **C++**
- **Programmation Orientée Objet**
- **CMake** (système de compilation)
- **SFML** (bibliothèque graphique)

## ⚙️ Compilation et exécution

### Prérequis

Avant de compiler le projet, assurez-vous d’avoir installé :

- Un compilateur compatible avec **C++17** ou supérieur  
  (par exemple `g++` ou `clang++`)
- **CMake** (version 3.10 ou supérieure)
- **SFML**

### Compilation du projet

Depuis le répertoire racine du projet :


mkdir build

cd build

cmake ..

make


Après la compilation, lancez le jeu depuis le dossier build avec :

./tower_defense

## Nettoyage du projet (optionnel)

Pour supprimer les fichiers de compilation :

make clean

rm -rf build
