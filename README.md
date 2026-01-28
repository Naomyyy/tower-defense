# Monsters Attack 

Monsters Attack est un jeu de stratégie en temps réel appartenant au genre Tower Defense. L’objectif principal du joueur est de défendre sa base contre des vagues successives d’ennemis qui progressent le long d’un chemin
fixe.

## Compilation et Installation

Bien que le projet soit compatible multiplateforme, l’utilisation d’un environnement Linux est recommandée pour une gestion simplifiée des dépendances.

### Instructions pour Linux (Recommandé)
L’installation de la SFML se fait via le gestionnaire de paquets: 
```bash
`sudo apt install libsfml-dev`
```
 Ensuite, exécutez les commandes suivantes dans votre terminal :
```bash
mkdir build
cd build
cmake ..
make
```
### Instructions pour Windows
Sur Windows, il est nécessaire de télécharger les binaires de la SFML sur le site officiel. La procédure de compilation via le terminal (CMD ou PowerShell) est la suivante :
```bash
mkdir build
cd build
cmake ..
cmake --build . --config Release
```

### Exécution
Une fois la compilation terminée, un exécutable nommé tower_defense est généré dans le dossier build. Pour lancer le jeu, il suffit d’exécuter la commande suivante :
```bash
./tower_defense
