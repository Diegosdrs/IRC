# Guide d'utilisation pour l'envoi de fichiers DCC

Ce guide vous explique comment envoyer et recevoir des fichiers via DCC (Direct Client-to-Client) en utilisant notre serveur IRC et le client IRSSI.

## Prérequis

- Serveur IRC compilé et en cours d'exécution
- Client IRSSI installé
- Netcat (nc) installé

## Envoi de fichier via DCC avec IRSSI

### Méthode 1: Commande DCC intégrée d'IRSSI

1. Lancez IRSSI et connectez-vous au serveur IRC:

```bash
irssi
/connect localhost 6667
/nick votre_pseudo
/user votre_user * * :Prénom Nom
/pass mot_de_passe
```

2. Rejoignez un canal (optionnel mais utile pour trouver des utilisateurs):

```bash
/join #canal
```

3. Pour envoyer un fichier à un autre utilisateur:

```bash
/dcc send destinataire /chemin/vers/fichier
```

4. Le destinataire recevra une notification et pourra accepter le transfert:

```bash
/dcc get expediteur
```

### Méthode 2: Avec notre commande SEND personnalisée

1. Connectez-vous au serveur comme précédemment
2. Exécutez la commande SEND avec les paramètres suivants:

```bash
/SEND destinataire fichier port taille_fichier
```

Exemple:

```bash
/SEND bob test.txt 8888 1024
```

3. Le destinataire peut accepter avec:

```bash
/ACCEPT ID_du_transfert
```

Ou refuser avec:

```bash
/DECLINE ID_du_transfert
```

## Envoi de fichier via Netcat (méthode alternative)

Netcat est un utilitaire réseau qui permet d'établir des connexions TCP/UDP simples. Il peut être utilisé pour transférer des fichiers manuellement:

### Côté émetteur

1. Préparez le serveur netcat pour envoyer un fichier:

```bash
# Remplacez 8888 par le port de votre choix
nc -l 8888 < fichier_a_envoyer.txt
```

2. Obtenez votre adresse IP:

```bash
ifconfig | grep "inet " | grep -v 127.0.0.1
```

3. Envoyez une demande de transfert à l'autre utilisateur via IRC:

```bash
/SEND destinataire fichier_a_envoyer.txt 8888 $(wc -c < fichier_a_envoyer.txt)
```

### Côté récepteur

1. Après avoir accepté la demande, le destinataire peut recevoir le fichier:

```bash
# Remplacez IP_EXPEDITEUR et PORT par les informations du transfert
nc IP_EXPEDITEUR PORT > fichier_recu.txt
```

## Script d'automatisation

Voici un script bash pour automatiser le processus d'envoi de fichier avec Netcat:

```bash
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
```

Enregistrez ce script sous le nom `dcc_send.sh`, rendez-le exécutable (`chmod +x dcc_send.sh`) et utilisez-le pour faciliter vos transferts de fichiers.

## Troubleshooting

- **Problèmes de pare-feu**: Assurez-vous que le port utilisé pour le transfert est ouvert dans votre pare-feu.
- **NAT/Routeur**: Si vous êtes derrière un routeur, vous devrez peut-être configurer le transfert de port.
- **Permissions**: Vérifiez que vous avez les droits en lecture sur le fichier à envoyer et en écriture dans le répertoire de destination.
- **Taille de fichier**: Pour les gros fichiers, le transfert peut prendre du temps. Soyez patient.
