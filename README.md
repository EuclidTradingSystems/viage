## Dependencies

* cmake >= 3.14
* cpp 20 compiler
* qt6 Multimedia

## Changelog
* change paseword in settings, popup + close settings pane
* email, never necescary
* page 6 out
* Vertical allignement in calaulation document 
* reinit calculation evry time
* User management
* rent, change interest from 4% to 4.25%
* choose camera input
* better regexp handling files
* onboarding document: _ Espace entre contacts et partenaires
                       _ date de naissance avec heure pas necessaire
                       _ inversement de contact et parteniare
                       _ Exterieur : types de places
                       _ Equipement de proximite dans Exterieur
                       _ Bordure bien net des carres
* mail, les space between body and signature
* block download document button

## TODO

# Client side

* transmission du dossier au CA de Viager Swiss SA (en 2eme)
* scroll inertia
* login popup when changing screen
* Habitat completed
* corect text input color

++ timeout for connection (3h)
++ position Title in error header

# Server side

* new password mail: change subject, remove the second "equipe viage"

++ Excel documents for advisors 
++ Filter invert order of filtered item in report
++ Repport: inclusive filter (equivalent to the "contain" option)
++ get email from all users

## Start the service
cd Euclid/Viage5
dotnet euclid.dll --urls "http://*:6000;https://*:6001"
