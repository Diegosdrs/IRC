#!/bin/bash

# Usage: ./dcc_receive.sh ip port fichier
# Exemple: ./dcc_receive.sh 192.168.1.100 8888 fichier_recu.txt

if [ $# -ne 3 ]; then
    echo "Usage: $0 ip port fichier"
    echo "Exemple: $0 192.168.1.100 8888 fichier_recu.txt"
    exit 1
fi

IP=$1
PORT=$2
FICHIER=$3

# Vérifier que le port est un nombre valide
if ! [[ "$PORT" =~ ^[0-9]+$ ]]; then
    echo "Erreur: Le port doit être un nombre."
    exit 1
fi

# Vérifier que le port est dans une plage valide
if [ "$PORT" -lt 1 ] || [ "$PORT" -gt 65535 ]; then
    echo "Erreur: Le port doit être entre 1 et 65535."
    exit 1
fi

# Vérifier si le fichier existe déjà
if [ -f "$FICHIER" ]; then
    read -p "Le fichier $FICHIER existe déjà. Voulez-vous l'écraser? (o/n) " REPONSE
    if [ "$REPONSE" != "o" ] && [ "$REPONSE" != "O" ]; then
        echo "Opération annulée."
        exit 0
    fi
fi

echo "Préparation de la réception du fichier depuis $IP:$PORT..."
echo "Le fichier sera enregistré sous: $FICHIER"
echo "Appuyez sur Ctrl+C pour annuler la réception."

# Exécuter nc pour recevoir le fichier
nc $IP $PORT > "$FICHIER"

# Vérifier si la connexion a réussi
if [ $? -eq 0 ]; then
    echo "Transfert terminé avec succès."
    echo "Fichier reçu: $FICHIER"
    echo "Taille: $(wc -c < "$FICHIER") octets"
else
    echo "Erreur lors du transfert. Veuillez vérifier l'adresse IP et le port."
    # Supprimer le fichier partiellement téléchargé s'il est vide
    if [ -f "$FICHIER" ] && [ ! -s "$FICHIER" ]; then
        rm "$FICHIER"
        echo "Fichier vide supprimé."
    fi
fi 