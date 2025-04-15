#!/bin/bash

echo "Compilation de la bibliothèque..."

echo "Installation des fichiers..."
sudo cp libqtc.so /usr/local/lib/ || { echo "Erreur : Impossible de copier la bibliothèque."; exit 1; }
sudo cp include/qtc.h /usr/local/include/ || { echo "Erreur : Impossible de copier l'en-tête."; exit 1; }

echo "Mise à jour du cache des bibliothèques..."
ldconfig || { echo "Erreur : Mise à jour du cache échouée."; exit 1; }

echo "Installation terminée avec succès !"
