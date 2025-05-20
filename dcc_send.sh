#!/bin/bash

# Usage: ./dcc_send.sh destinataire fichier
# Exemple: ./dcc_send.sh bob test.txt

if [ $# -ne 2 ]; then
    echo "Usage: $0 destinataire fichier"
    exit 1
fi

DEST=$1
FICHIER=$2
PORT=8888  # Port par défaut, peut être modifié

# Vérifier l'existence du fichier
if [ ! -f "$FICHIER" ]; then
    echo "Erreur: Le fichier $FICHIER n'existe pas."
    exit 1
fi

# Obtenir la taille du fichier
TAILLE=$(wc -c < "$FICHIER")

# Obtenir l'adresse IP locale
IP=$(hostname -I | cut -d' ' -f1)

echo "Préparation de l'envoi de $FICHIER à $DEST"
echo "IP: $IP, Port: $PORT, Taille: $TAILLE octets"

# Lancer netcat en arrière-plan
nc -l $PORT < "$FICHIER" &
NC_PID=$!

# Informer l'utilisateur
echo "Le serveur netcat est prêt. Envoyez la commande SEND au destinataire."
echo "Utilisez la commande suivante dans IRSSI:"
echo "/SEND $DEST $FICHIER $PORT $TAILLE"

# Attendre la fin du transfert
wait $NC_PID
echo "Transfert terminé ou interrompu." 