#ifndef GERERRESERVATION_H
#define GERERRESERVATION_H

#include <ArduinoJson.h>                                 // Inclusion de la bibliothèque pour la gestion de JSON
#include <string>                                        // Inclusion de la bibliothèque pour les chaînes de caractères
#include <queue>                                         // Inclusion de la bibliothèque pour les queues (file d'attente)
#include <../lib/Infrastructure/drivers/DriverL298N.h>   // Inclusion des drivers pour la gestion du moteur L298N
#include <../lib/Infrastructure/drivers/DriverIR.h>      // Inclusion du driver pour les capteurs infrarouges
#include <../lib/Infrastructure/drivers/DriverBoutton.h> // Inclusion du driver pour les boutons
#include <../Domaine/Entities/Ascenseur.h>               // Inclusion de la classe représentant l'ascenseur

// Déclaration de la classe GererReservation
class GererReservation
{
private:
    // Référence vers l'entité Ascenseur (modèle)
    Ascenseur &_ascenseur;

    // Références vers les drivers de l'ascenseur (moteur, capteurs IR et boutons)
    DriverL298N &_driverL298n;
    DriverIR &_driverIR_E0;
    DriverIR &_driverIR_E1;
    DriverBoutton &_driverBoutton_Monter;
    DriverBoutton &_driverBoutton_Descendre;

    // Structure représentant une réservation
    struct Reservation
    {
        std::string id;       // Identifiant de la réservation
        int etageDepart;      // Étage de départ
        int etageDestination; // Étage de destination

        // Constructeur de la structure de réservation
        Reservation(const std::string &_id, int _depart, int _destination)
            : id(_id), etageDepart(_depart), etageDestination(_destination) {}
    };

    // Queue pour gérer les réservations
    std::queue<Reservation> _fileReservations;

    // Pointeur vers la réservation actuelle en cours
    Reservation *_reservationActuelle;

public:
    // Constructeur de la classe GererReservation
    GererReservation(Ascenseur &ascenseur, DriverL298N &driverL298n,
                     DriverIR &driverIR_E0, DriverIR &driverIR_E1,
                     DriverBoutton &driverBoutton_Monter,
                     DriverBoutton &driverBoutton_Descendre)
        : _ascenseur(ascenseur), _driverL298n(driverL298n),
          _driverIR_E0(driverIR_E0), _driverIR_E1(driverIR_E1),
          _driverBoutton_Monter(driverBoutton_Monter),
          _driverBoutton_Descendre(driverBoutton_Descendre),
          _reservationActuelle(nullptr) {}

    // Destructeur de la classe, supprime la réservation actuelle si elle existe
    ~GererReservation()
    {
        delete _reservationActuelle;
    }

    // Méthode pour ajouter une réservation à la file
    bool ajouterReservation(const String &jsonStr)
    {
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, jsonStr); // Désérialisation du JSON

        if (error)
        { // Si erreur de désérialisation
            Serial.println("Erreur JSON invalide");
            return false;
        }

        // Extraction des informations de la réservation depuis le JSON
        std::string id = doc["id"].as<const char *>();
        int etageDepart = doc["etage actuel"].as<int>();
        int etageDestination = doc["direction"].as<int>();

        // Validation des étages (valeurs possibles : 0 ou 1)
        if (etageDepart < 0 || etageDepart > 1 ||
            etageDestination < 0 || etageDestination > 1)
        {
            Serial.println("Étages invalides");
            return false;
        }

        // Ajout de la réservation dans la file
        _fileReservations.emplace(id, etageDepart, etageDestination);
        Serial.println("Nouvelle réservation ajoutée");
        return true;
    }

    // Méthode de mise à jour des événements (états des capteurs)
    void miseAjourEvenement()
    {
        // Mise à jour des états des capteurs IR (étage 0)
        if (_driverIR_E0._irsensor.getEtat() == ASCENSEUR_ETAGE_0_DETECTE)
        {
            _ascenseur.events.surIR_E0.etatPre = _ascenseur.events.surIR_E0.etat;
            _ascenseur.events.surIR_E0.etat = ASCENSEUR_ETAGE_0_DETECTE;
        }
        else if (_driverIR_E0._irsensor.getEtat() == ASCENSEUR_ETAGE_0_NON_DETECTE)
        {
            _ascenseur.events.surIR_E0.etatPre = _ascenseur.events.surIR_E0.etat;
            _ascenseur.events.surIR_E0.etat = ASCENSEUR_ETAGE_0_NON_DETECTE;
        }

        // Mise à jour des états des capteurs IR (étage 1)
        if (_driverIR_E1._irsensor.getEtat() == ASCENSEUR_ETAGE_1_DETECTE)
        {
            _ascenseur.events.surIR_E1.etatPre = _ascenseur.events.surIR_E1.etat;
            _ascenseur.events.surIR_E1.etat = ASCENSEUR_ETAGE_1_DETECTE;
        }
        else if (_driverIR_E1._irsensor.getEtat() == ASCENSEUR_ETAGE_1_NON_DETECTE)
        {
            _ascenseur.events.surIR_E1.etatPre = _ascenseur.events.surIR_E1.etat;
            _ascenseur.events.surIR_E1.etat = ASCENSEUR_ETAGE_1_NON_DETECTE;
        }
    }

    // Machine à états (FSM - Finite State Machine) pour gérer les états de l'ascenseur
    void fsm()
    {
        switch (_ascenseur.states)
        {
        case _ascenseur.systemStates::INIT:
            // Si l'ascenseur est à l'étage 0 ou 1, transition vers l'état correspondant
            if (_ascenseur.events.surIR_E0.etat == ASCENSEUR_ETAGE_0_DETECTE)
            {
                _ascenseur.statesPre = _ascenseur.systemStates::INIT;
                _ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_0;
                _ascenseur.actions.moteur.etat = ARRERTER_MOTEUR;
            }
            else if (_ascenseur.events.surIR_E1.etat == ASCENSEUR_ETAGE_1_DETECTE)
            {
                _ascenseur.statesPre = _ascenseur.systemStates::INIT;
                _ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_1;
                _ascenseur.actions.moteur.etat = ARRERTER_MOTEUR;
            }
            break;

        case _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_0:
            // Si une réservation existe, commencer à monter si la destination est l'étage 1
            if (!_fileReservations.empty() && !_reservationActuelle)
            {
                _reservationActuelle = new Reservation(_fileReservations.front());
                _fileReservations.pop();

                if (_reservationActuelle->etageDestination > 0)
                {
                    _ascenseur.actions.moteur.etat = DEMARRER_MOTEUR;
                    _ascenseur.actions.moteur.sense = MONTER_ASCENSEUR;
                    _ascenseur.statesPre = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_0;
                    _ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_MONTANT;
                }
            }
            break;

        case _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_1:
            // Si une réservation existe, commencer à descendre si la destination est l'étage 0
            if (!_fileReservations.empty() && !_reservationActuelle)
            {
                _reservationActuelle = new Reservation(_fileReservations.front());
                _fileReservations.pop();

                if (_reservationActuelle->etageDestination < 1)
                {
                    _ascenseur.actions.moteur.etat = DEMARRER_MOTEUR;
                    _ascenseur.actions.moteur.sense = DESCENDRE_ASCENSEUR;
                    _ascenseur.statesPre = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_1;
                    _ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_DESENDANT;
                }
            }
            break;

        case _ascenseur.systemStates::ASCENSEUR_EST_MONTANT:
            // Si l'ascenseur atteint l'étage 1, l'arrêter et supprimer la réservation
            if (_ascenseur.events.surIR_E1.etat == ASCENSEUR_ETAGE_1_DETECTE)
            {
                _ascenseur.actions.moteur.etat = ARRERTER_MOTEUR;
                _ascenseur.statesPre = _ascenseur.systemStates::ASCENSEUR_EST_MONTANT;
                _ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_1;
                delete _reservationActuelle;
                _reservationActuelle = nullptr;
            }
            break;

        case _ascenseur.systemStates::ASCENSEUR_EST_DESENDANT:
            // Si l'ascenseur atteint l'étage 0, l'arrêter et supprimer la réservation
            if (_ascenseur.events.surIR_E0.etat == ASCENSEUR_ETAGE_0_DETECTE)
            {
                _ascenseur.actions.moteur.etat = ARRERTER_MOTEUR;
                _ascenseur.statesPre = _ascenseur.systemStates::ASCENSEUR_EST_DESENDANT;
                _ascenseur.states = _ascenseur.systemStates::ASCENSEUR_EST_EN_ETAGE_0;
                delete _reservationActuelle;
                _reservationActuelle = nullptr;
            }
            break;
        }
    }

    // Méthode pour mettre à jour les actions du moteur (arrêt, montée, descente)
    void miseAjourActions()
    {
        if (_ascenseur.actions.moteur.etat == ARRERTER_MOTEUR)
        {
            _driverL298n.arreter(); // Arrêt du moteur
            Serial.println("Moteur arrêté");
        }
        else if (_ascenseur.actions.moteur.etat == DEMARRER_MOTEUR)
        {
            if (_ascenseur.actions.moteur.sense == DESCENDRE_ASCENSEUR)
            {
                _driverL298n.decendreAscenseur(); // Descente de l'ascenseur
                Serial.println("Descente ascenseur");
            }
            else if (_ascenseur.actions.moteur.sense == MONTER_ASCENSEUR)
            {
                _driverL298n.monterAscenseur(); // Montée de l'ascenseur
                Serial.println("Montée ascenseur");
            }
        }
    }
};

#endif
