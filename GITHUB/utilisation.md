Edited by: @Arthur

Git:
    -Pour crer un nouvelle version avec un nouveau fichier puis mettre à jour son cloud github, il faut ajouter les nouveaux fichiers au projet (git add File.extention), si vous n'avez pas de fichier à ajouter, pas besoin de git add
    -Ensuite il faut crer une mise à jour en lui donnant l'appelation la plus précise possible (git commit -m "ajout du fichier planif")
    -Enfin envoyer la mise à jour au cloud avec git push

Commentaire:
    Attention, si vous copier/supprimer avec windows un dossier/fichier, il ne sera pas supprimé pour GIT, il faudrat utiliser git rm
    Attention bis, avant de commencer à modifier les fichiers faites un -git pull

Résumé:
    EN UTILISANT GIT ASSURER VOUS D'ETRE DANS LE DOSSIER DU PROJET
    -git add FileName.extention
    -git add *
    -git rm FileName.extention
    -git rm -r DirectoryName
    -git commit -m "Commentaire sur l'update"
    -git push
    -git pull


1) Comment se déplacer dans le dossier du projet?
    #cd permet de se déplacer vers des dossiers
    cd ~/Document/github/projet-E2/
    
        Possibles erreurs : 
            -Utilisateur/OneDrive/Documents/ différent de /Utilisateur/Documents/
            -Le sens des "/" ou "\"

2) git add?
    #permet d'ajouter des fichiers (pas des dossiers vides) au projet
    -2 paramètres:
        -git add FileName.extention
            #ajoute FileName.extention
        -git add * 
            #ajoutes tous les fichiers à condition d'avoir fait l'étape 1

3) git rm?
    #permet de supprimer des fichiers (pas des dossiers vides) au projet
    -3 paramètres:
        -git rm FileName.extention
            #supprime FileName.extention
        -git rm * 
            #supprime tous les fichiers à condition d'avoir fait l'étape 1
        -git rm -r DirectoryName
            #supprime un dossier

4) git commit -m "Commentaire"
    #Permet de créer un nouvelle version, en local
    -git commit -m "ajout du fichier présentation"

5) git push?
    #permet d'envoyer la dernière version voir 4 sur GihHub
    -git push
6) git pull?
    #permet de récuperer ce qui se trouve sur le github
    -git pull
