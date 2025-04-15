# 🧠 Projet C - Traitement et Codage d'Images

> Réalisé dans le cadre du cours de Programmation C – Université Gustave Eiffel\
> \*\***Auteur** : Massinissa\
> \*\***Année** : 2024

---

## 📌 Objectif du projet

Ce projet vise à explorer le traitement d'images en niveaux de gris à travers des techniques avancées de programmation en C. Les principales fonctionnalités implémentées sont :

- **Segmentation d’image** : Partitionnement d’une image pour en extraire des régions significatives.
- **Construction d’un Quadtree** : Représentation hiérarchique de l’image pour optimiser son analyse.
- **Codage QTC (Quadtree Coding)** : Compression efficace des données de l’image.
- **Filtrage spatial** : Application de filtres moyenneurs et médians pour lisser ou améliorer les images.
- **Compression/Décompression** : Génération et décodage de fichiers compressés.

Ce projet a été conçu pour approfondir la maîtrise des structures de données complexes et des algorithmes de traitement d’image.

---

## 🗂️ Structure du projet

```
L3.2024.ProgC-Massinissa/
├── Prog/
│   ├── src/
│   │   └── main.c          # Programme principal
│   └── Makefile            # Compilation du programme
├── bib/
│   ├── include/            # Fichiers d’en-tête (.h)
│   ├── src/                # Fichiers source (.c)
│   ├── lib/                # Bibliothèque compilée (.so)
│   ├── Makefile            # Compilation de la bibliothèque
│   └── install.sh          # Script d’installation
├── Doxyfile                # Configuration pour Doxygen
└── README.md               
```

---

## 🚀 Installation et exécution

### Prérequis

- Compilateur C (gcc recommandé)
- Doxygen pour générer la documentation
- Système Unix/Linux (ou WSL pour Windows)

### Étapes

1. **Compiler la bibliothèque**

   ```bash
   cd bib/
   make
   ```

2. **Installer la bibliothèque (optionnel)**

   ```bash
   ./install.sh
   ```

3. **Compiler le programme principal**

   ```bash
   cd ../Prog/
   make
   ```

4. **Exécuter le programme**

   ```bash
   ./main
   ```

---

## 🧪 Fonctionnalités principales

- **Construction de Quadtree** : Décomposition récursive de l’image en quatre sous-régions.
- **Codage QTC** : Compression binaire compacte basée sur la structure du Quadtree.
- **Filtrage spatial** : Application de filtres (moyenneur, médian) pour réduire le bruit ou améliorer la qualité visuelle.
- **Compression/Décompression** : Sauvegarde et reconstitution d’images à partir de fichiers compressés.

Exemple d’utilisation :

```bash
./main input_image.pgm output_image.qtc
```

---

## 📚 Documentation

La documentation complète du code est générée avec **Doxygen**. Pour la créer :

```bash
doxygen Doxyfile
```

Le dossier `docs/` contiendra alors une version HTML consultable.

---