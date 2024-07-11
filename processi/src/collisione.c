#include "../hdr/collisione.h"

Collisione detectCollisione(GameData *gameData)
{
    Collisione collisione;
    collisione.tipoCollisione = NO_COLLISIONE;
    collisione.hit_point_y = 0;
    collisione.hit_point_x = 0;
    Schermo *schermo = &(gameData->schermo); // lo schermo di gioco

    // switch sul carattere in pipe
    switch (gameData->pipeData.type)
    {
    case 'X': // rana
    {
        // calcolo posizone assoluta della rana in potenza
        // ovvero sommo a abspos la posizione relativa in pipe
        RanaAbsPos ranaPos;
        ranaPos.x = gameData->ranaAbsPos.x + gameData->pipeData.x;
        ranaPos.y = gameData->ranaAbsPos.y + gameData->pipeData.y;

        // se il movimento della rana non è lecito non c'è aggiornamento ne collisione
        if (!isFrogMoveLecit(ranaPos.x, ranaPos.y))
        {
            return collisione;
        }

        // se la rana si è spostata lungo un coccodrillo devo modificare solo l'offset della rana
        if (gameData->pipeData.x != 0 && gameData->ranaAbsPos.on_coccodrillo)
        {

            int offset = gameData->ranaAbsPos.offset_on_coccodrillo + gameData->pipeData.x;
            // se il movimento lungo il coccodrillo è lecito ovvero non sfora:
            if (offset >= 0 && offset <= 9)
            {
                // aggiorno offset
                gameData->ranaAbsPos.offset_on_coccodrillo += gameData->pipeData.x;
            }
            else
            {
                // non c'è aggiornamento ne collisione
                return collisione;
            }
        }

        for (int row = ranaPos.y; row < ranaPos.y + RANA_H; row++)
        {
            for (int col = ranaPos.x; col < ranaPos.x + RANA_W; col++)
            {

                switch (schermo->screenMatrix[row][col].tipo)
                {
                case FIUME_OBJ:
                    collisione.tipoCollisione = RANA_FIUME;
                    collisione.id_oggetto_attivo = gameData->pipeData.id;
                    collisione.oggetto_attivo = RANA_OBJ;
                    collisione.oggetto_passivo = FIUME_OBJ;
                    collisione.id_oggetto_passivo = 0;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                    return collisione;
                    break;
                case LAVA_OBJ:
                    collisione.tipoCollisione = RANA_LAVA;
                    collisione.id_oggetto_attivo = gameData->pipeData.id;
                    collisione.oggetto_attivo = RANA_OBJ;
                    collisione.oggetto_passivo = LAVA_OBJ;
                    collisione.id_oggetto_passivo = 0;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                    return collisione;
                    break;
                case TANA_OPEN_OBJ:
                    collisione.tipoCollisione = RANA_TANA_APERTA;
                    collisione.id_oggetto_attivo = gameData->pipeData.id;
                    collisione.oggetto_attivo = RANA_OBJ;
                    collisione.oggetto_passivo = TANA_OPEN_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                    return collisione;
                    break;
                case TANA_CLOSE_OBJ:
                    collisione.tipoCollisione = RANA_TANA_CHIUSA;
                    collisione.id_oggetto_attivo = gameData->pipeData.id;
                    collisione.oggetto_attivo = RANA_OBJ;
                    collisione.oggetto_passivo = TANA_CLOSE_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                    return collisione;
                    break;
                case COCCODRILLO_BUONO_OBJ:
                    collisione.tipoCollisione = RANA_COCCODRILLO_BUONO;
                    collisione.id_oggetto_attivo = gameData->pipeData.id;
                    collisione.oggetto_attivo = RANA_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.oggetto_passivo = COCCODRILLO_BUONO_OBJ;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                    return collisione;
                    break;
                case PN_OBJ:
                    collisione.tipoCollisione = PROIETTILENEMICO_RANA;
                    collisione.id_oggetto_attivo = gameData->pipeData.id;
                    collisione.oggetto_attivo = RANA_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.oggetto_passivo = PN_OBJ;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                    return collisione;
                    break;
                case N_OBJ:
                    collisione.tipoCollisione = NEMICO_RANA;
                    collisione.id_oggetto_attivo = gameData->pipeData.id;
                    collisione.oggetto_attivo = RANA_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.oggetto_passivo = N_OBJ;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                    return collisione;
                case COCCODRILLO_CATTIVO_OBJ:
                     collisione.tipoCollisione = RANA_COCCODRILLO_CATTIVO;
                    collisione.id_oggetto_attivo = gameData->pipeData.id;
                    collisione.oggetto_attivo = RANA_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.oggetto_passivo = COCCODRILLO_CATTIVO_OBJ;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                default:
                    break;
                }
            }
        }
        break;
    }
    case 'P': // proiettile rana
    {
        PipeData proiettileData = gameData->pipeData;

        for (int row = proiettileData.y; row < proiettileData.y + PROIETTILE_H; row++)
        {
            for (int col = proiettileData.x; col < proiettileData.x + PROIETTILE_W; col++)
            {

                switch (schermo->screenMatrix[row][col].tipo)
                {
                case COCCODRILLO_CATTIVO_OBJ:
                    collisione.tipoCollisione = PROIETTILE_COCCODRILLO_CATTIVO;
                    collisione.id_oggetto_attivo = proiettileData.id;
                    collisione.oggetto_attivo = P_OBJ;
                    collisione.oggetto_passivo = COCCODRILLO_CATTIVO_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                    return collisione;
                    break;
                case N_OBJ:
                    collisione.tipoCollisione = PROIETTILE_NEMICO;
                    collisione.id_oggetto_attivo = proiettileData.id;
                    collisione.oggetto_attivo = P_OBJ;
                    collisione.oggetto_passivo = N_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                    return collisione;
                    break;
                case PN_OBJ:
                    collisione.tipoCollisione = PROIETTILE_PROIETTILENEMICO;
                    collisione.id_oggetto_attivo = proiettileData.id;
                    collisione.oggetto_attivo = P_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                    return collisione;
                    break;
                default:
                    break;
                }
            }
        }

        break;
    }
    case 'p': // proiettile nemico
    {
        PipeData proiettileData = gameData->pipeData;

        for (int row = proiettileData.y; row < proiettileData.y + PROIETTILE_H; row++)
        {
            for (int col = proiettileData.x; col < proiettileData.x + PROIETTILE_W; col++)
            {

                switch (schermo->screenMatrix[row][col].tipo)
                {

                case RANA_OBJ:
                    collisione.tipoCollisione = PROIETTILENEMICO_RANA;
                    collisione.id_oggetto_attivo = proiettileData.id;
                    collisione.oggetto_attivo = PN_OBJ;
                    collisione.oggetto_passivo = RANA_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                    return collisione;
                    break;
                case P_OBJ:
                    collisione.tipoCollisione = PROIETTILE_PROIETTILENEMICO;
                    collisione.id_oggetto_attivo = proiettileData.id;
                    collisione.oggetto_attivo = PN_OBJ;
                    collisione.oggetto_passivo = P_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;
                    return collisione;
                    break;
                default:
                    break;
                }
            }
        }

        break;
    }
    case 'C': // coccodrillo a destra
    case 'c': // coccodrillo a sinistra
    {
        PipeData coccodrilloData = gameData->pipeData;

        if (!(gameData->controlloCoccodrilli[coccodrilloData.id].is_buono))
        {
            for (int row = coccodrilloData.y; row < coccodrilloData.y + COCCODRILLO_H; row++)
            {
                for (int col = coccodrilloData.x; col < coccodrilloData.x + COCCODRILLO_W; col++)
                {

                    switch (schermo->screenMatrix[row][col].tipo)
                    {
                    case P_OBJ:
                        collisione.tipoCollisione = PROIETTILE_COCCODRILLO_CATTIVO;
                        collisione.id_oggetto_attivo = coccodrilloData.id;
                        collisione.oggetto_attivo = COCCODRILLO_CATTIVO_OBJ;
                        collisione.oggetto_passivo = P_OBJ;
                        collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                        collisione.hit_point_x = col;
                        collisione.hit_point_y = row;
                        return collisione;
                        break;
                    default:
                        break;
                    }
                }
            }
        }
        break;
    }
    case 'n': // pianta
    {
        PipeData nemicoData = gameData->pipeData;

        for (int row = nemicoData.y; row < nemicoData.y + PIANTA_H; row++)
        {
            for (int col = nemicoData.x; col < nemicoData.x + PIANTA_W; col++)
            {

                switch (schermo->screenMatrix[row][col].tipo)
                {
                case RANA_OBJ:
                    collisione.tipoCollisione = NEMICO_RANA;
                    collisione.id_oggetto_attivo = nemicoData.id;
                    collisione.oggetto_attivo = N_OBJ;
                    collisione.oggetto_passivo = RANA_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.hit_point_x = col;
                    collisione.hit_point_y = row;

                    return collisione;
                    break;
                default:
                    break;
                }
            }
        }

        break;
    }
    default:
    {
        break;
    }
    }

    return collisione;
}

void handleCollisione(GameData *gameData, Collisione collisione)
{
    switch (collisione.tipoCollisione)
    {
    case RANA_FIUME:
    {
        // stampo la rana sopra il fiume
        int newPosAbsRanaX = gameData->pipeData.x + gameData->ranaAbsPos.x;
        int newPosAbsRanaY = gameData->pipeData.y + gameData->ranaAbsPos.y;
        gameData->pipeData.x = newPosAbsRanaX;
        gameData->pipeData.y = newPosAbsRanaY;
        // normale aggiornamento
        aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
        gameData->ranaAbsPos.x = gameData->pipeData.x;
        gameData->ranaAbsPos.y = gameData->pipeData.y;
        stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
        refresh();                                     // Aggiorna la finestra
        usleep(500000);

        // riproduco suono plof
        // tolgo una vita alla rana
        gameData->gameInfo.vite--;
        // schermata vite--
        // faccio ripartire la rana
        resetRana(gameData);
        gameData->ranaAbsPos.on_coccodrillo = false;
        gameData->ranaAbsPos.id_coccodrillo = -1;
        aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
        break;
    }
    case RANA_LAVA:
    {
        // stampo la rana sopra la lava
        int newPosAbsRanaX = gameData->pipeData.x + gameData->ranaAbsPos.x;
        int newPosAbsRanaY = gameData->pipeData.y + gameData->ranaAbsPos.y;
        gameData->pipeData.x = newPosAbsRanaX;
        gameData->pipeData.y = newPosAbsRanaY;
        // normale aggiornamento
        aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
        gameData->ranaAbsPos.x = gameData->pipeData.x;
        gameData->ranaAbsPos.y = gameData->pipeData.y;
        stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
        refresh();                                     // Aggiorna la finestra
        usleep(500000);

        
        // tolgo una vita alla rana
        gameData->gameInfo.vite--;
        
        // faccio ripartire la rana
        resetRana(gameData);
        gameData->ranaAbsPos.on_coccodrillo = false;
        gameData->ranaAbsPos.id_coccodrillo = -1;
        aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
        break;
    }
    case RANA_TANA_APERTA:
    {   
        // stampo la rana sopra la tana aperta
        int newPosAbsRanaX = gameData->pipeData.x + gameData->ranaAbsPos.x;
        int newPosAbsRanaY = gameData->pipeData.y + gameData->ranaAbsPos.y;
        gameData->pipeData.x = newPosAbsRanaX;
        gameData->pipeData.y = newPosAbsRanaY;
        // normale aggiornamento
        aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
        gameData->ranaAbsPos.x = gameData->pipeData.x;
        gameData->ranaAbsPos.y = gameData->pipeData.y;
        stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
        refresh();                                     // Aggiorna la finestra
        usleep(500000);

        // faccio ripartire la rana
        resetRana(gameData);
        gameData->ranaAbsPos.on_coccodrillo = false;
        gameData->ranaAbsPos.id_coccodrillo = -1;
        aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);

        // stampo tana chiusa
        stampaTanaChiusa(gameData->tane[collisione.id_oggetto_passivo],gameData);
        
        break;
    }
    case RANA_TANA_CHIUSA: 
    {
         // stampo la rana sopra la tana chiusa
        int newPosAbsRanaX = gameData->pipeData.x + gameData->ranaAbsPos.x;
        int newPosAbsRanaY = gameData->pipeData.y + gameData->ranaAbsPos.y;
        gameData->pipeData.x = newPosAbsRanaX;
        gameData->pipeData.y = newPosAbsRanaY;
        // normale aggiornamento
        aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
        gameData->ranaAbsPos.x = gameData->pipeData.x;
        gameData->ranaAbsPos.y = gameData->pipeData.y;
        stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
        refresh();                                     // Aggiorna la finestra
        usleep(500000);

        // faccio ripartire la rana
        resetRana(gameData);
        gameData->ranaAbsPos.on_coccodrillo = false;
        gameData->ranaAbsPos.id_coccodrillo = -1;
        aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
        gameData->gameInfo.vite--;
        break;
    }
    case PROIETTILE_COCCODRILLO_CATTIVO:
    {
        // il proiettile muore e il coccodrillo diventa buono
        // l'oggetto attivo è il proiettile
        if (collisione.oggetto_attivo == P_OBJ)
        {
            uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_attivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
            gameData->contatori.contProiettili--;

            // il coccodrillo diventa buono
            gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].is_buono = true;

            // il coccodrillo esce da tutte le fasi in cui si trova
            gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].offset_deep = 0;
            gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].is_fase_pre_immersione = false;
            gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].is_fase_immersione = false;
            gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].is_going_deep = false;
            gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].is_going_up = false;
            gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].passi_in_immersione = 0;
            gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].passi_in_pre_immersione = 0;
            gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].passi_deep = 0;
        }
        else
        {
            // l'oggetto attivo è il tronco
            uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_passivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
            gameData->contatori.contProiettili--;

            // il coccodrillo diventa buono
            gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].is_buono = true;
            // il coccodrillo esce da tutte le fasi in cui si trova
            gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].offset_deep = 0;
            gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].is_fase_pre_immersione = false;
            gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].is_fase_immersione = false;
            gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].is_going_deep = false;
            gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].is_going_up = false;
            gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].passi_in_immersione = 0;
            gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].passi_in_pre_immersione = 0;
            gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].passi_deep = 0;
        }
        break;
    }
    case RANA_COCCODRILLO_BUONO:
    {
        if (collisione.oggetto_attivo == RANA_OBJ)
        {
            // stampo il coccodrillo buono
            if (gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].direction == 1)
            {
                aggiornaOggettoNew(gameData, gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo], gameData->oldPos.coccodrilli, S_COCCODRILLO_DX);
            }
            else
            {
                aggiornaOggettoNew(gameData, gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo], gameData->oldPos.coccodrilli, S_COCCODRILLO_SX);
            }

            // stampo sopra il coccodrillo buono la rana
            // aggiorno abs pos rana
            gameData->ranaAbsPos.x = collisione.hit_point_x;
            gameData->ranaAbsPos.y = collisione.hit_point_y;
            gameData->ranaAbsPos.on_coccodrillo = true;
            gameData->ranaAbsPos.id_coccodrillo = collisione.id_oggetto_passivo;
            // aggiorno offset rana su coccodrillo
            gameData->ranaAbsPos.offset_on_coccodrillo = collisione.hit_point_x - gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo].x;
            // stampo e aggiorno rana
            PipeData ranaData;
            ranaData.x = gameData->ranaAbsPos.x;
            ranaData.type = 'X';
            ranaData.y = gameData->ranaAbsPos.y;
            ranaData.id = 0;
            aggiornaOggettoNew(gameData, ranaData, &(gameData->oldPos.rana), S_RANA);
        }
        else
        {
            // l'oggetto attivo e il coccodrillo buono
            // stampo il coccodrillo buono
            if (gameData->controlloCoccodrilli[gameData->pipeData.id].direction == 1)
            {
                aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_DX);
            }
            else
            {
                aggiornaOggetto(gameData, gameData->oldPos.coccodrilli, S_COCCODRILLO_SX);
            }
            // aggiorno abs pos rana
            gameData->ranaAbsPos.x = collisione.hit_point_x;
            gameData->ranaAbsPos.y = collisione.hit_point_y;
            gameData->ranaAbsPos.on_coccodrillo = true;
            gameData->ranaAbsPos.id_coccodrillo = collisione.id_oggetto_attivo;
            // aggiorno offset rana su coccodrillo
            gameData->ranaAbsPos.offset_on_coccodrillo = collisione.hit_point_x - gameData->oldPos.coccodrilli[collisione.id_oggetto_attivo].x;
            // stampo e aggiorno rana
            PipeData ranaData;
            ranaData.x = gameData->ranaAbsPos.x;
            ranaData.type = 'X';
            ranaData.y = gameData->ranaAbsPos.y;
            ranaData.id = 0;
            // stampo sopra il coccodrillo buono la rana
            aggiornaOggettoNew(gameData, ranaData, &(gameData->oldPos.rana), S_RANA);
        }
        break;
    }
    case PROIETTILE_NEMICO:
    {
        if (collisione.oggetto_attivo == P_OBJ)
        {
            // uccisione del proiettile amico che ha colpito il nemico
            uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_attivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
            gameData->contatori.contProiettili--;

            // uccisione del nemico (pianta)
            uccidiNemico(gameData->pids.pidNemici, collisione.id_oggetto_passivo);
            // cancella oggetto nemico
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.nemici[collisione.id_oggetto_passivo], gameData->oldPos.nemici, S_PIANTA);
            gameData->contatori.contNemici--;
        }

        break;
    }
    case PROIETTILENEMICO_RANA:
    {
        if (collisione.oggetto_attivo == RANA_OBJ)
        {
            // l'oggetto attivo è la rana

            // uccido la rana
            // stampo la rana sopra il fiume
            int newPosAbsRanaX = gameData->pipeData.x + gameData->ranaAbsPos.x;
            int newPosAbsRanaY = gameData->pipeData.y + gameData->ranaAbsPos.y;
            gameData->pipeData.x = newPosAbsRanaX;
            gameData->pipeData.y = newPosAbsRanaY;
            // normale aggiornamento
            aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
            gameData->ranaAbsPos.x = gameData->pipeData.x;
            gameData->ranaAbsPos.y = gameData->pipeData.y;
            stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
            refresh();                                     // Aggiorna la finestra
            usleep(500000);

            // riproduco suono plof
            // tolgo una vita alla rana
            gameData->gameInfo.vite--;
            // schermata vite--
            // faccio ripartire la rana
            resetRana(gameData);
            gameData->ranaAbsPos.on_coccodrillo = false;
            gameData->ranaAbsPos.id_coccodrillo = -1;
            aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);

            // uccido il proiettile nemico
            // uccisione del proiettile amico che ha colpito il nemico
            uccidiProiettileNemico(gameData->pids.pidProiettiliNemici, collisione.id_oggetto_passivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.proiettiliNemici[collisione.id_oggetto_passivo], gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
            gameData->contatori.contProiettiliN--;
        }
        else
        {
            // l'oggetto attivo è il proiettile nemico
            // uccisione del proiettile nemico
            uccidiProiettileNemico(gameData->pids.pidProiettiliNemici, collisione.id_oggetto_attivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.proiettiliNemici[collisione.id_oggetto_attivo], gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
            gameData->contatori.contProiettiliN--;

            // uccido la rana
            gameData->gameInfo.vite--;
            // schermata vite--
            // faccio ripartire la rana
            resetRana(gameData);
            gameData->ranaAbsPos.on_coccodrillo = false;
            gameData->ranaAbsPos.id_coccodrillo = -1;
            inizializzaPosRana(&(gameData->ranaAbsPos));

            gameData->pipeData.id = 0;
            gameData->pipeData.x = gameData->ranaAbsPos.x;
            gameData->pipeData.y = gameData->ranaAbsPos.y;
            gameData->pipeData.type = 'X';
            aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
        }
        break;
    }
    case PROIETTILE_PROIETTILENEMICO:
    {
        if (collisione.oggetto_attivo == P_OBJ)
        {
            // il proiettile amivo è l'oggetto attivo
            // uccido il proiettile amico

            uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_attivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggetto(gameData, gameData->oldPos.proiettili, S_PROIETTILE);
            gameData->contatori.contProiettili--;

            // uccido il proiettile nemico
            uccidiProiettileNemico(gameData->pids.pidProiettiliNemici, collisione.id_oggetto_passivo); // uccide il processo proiettile
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.proiettiliNemici[collisione.id_oggetto_passivo], gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
            gameData->contatori.contProiettiliN--;
        }
        else
        {
            // il proiettile nemico è l'oggetto attivo
            // uccido il proiettile nemico
            uccidiProiettileNemico(gameData->pids.pidProiettiliNemici, collisione.id_oggetto_attivo); // uccide il processo proiettile
            // ucciso processo proiettile e impostato a zero il pid in array pid proiettili
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.proiettiliNemici[collisione.id_oggetto_attivo], gameData->oldPos.proiettiliNemici, S_PROIETTILE_NEMICO);
            gameData->contatori.contProiettiliN--;

            // uccido il proiettile amico
            uccidiProiettile(gameData->pids.pidProiettili, collisione.id_oggetto_passivo);
            cancellaOggettoDaMatrice(gameData, gameData->oldPos.proiettili[collisione.id_oggetto_passivo], gameData->oldPos.proiettili, S_PROIETTILE);
            gameData->contatori.contProiettili--;
        }
        break;
    }
    case NEMICO_RANA:
    {
        if (collisione.oggetto_attivo == RANA_OBJ)
        {
            // la rana è l'oggetto attivo
            // uccido la rana

            // tolgo una vita alla rana
            gameData->gameInfo.vite--;

            // faccio ripartire la rana
            resetRana(gameData);
            gameData->ranaAbsPos.on_coccodrillo = false;
            gameData->ranaAbsPos.id_coccodrillo = -1;
            aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);

            // reprint del nemico
            stampaSpriteInMatrice(&(gameData->oldPos.nemici[collisione.id_oggetto_passivo]), &(gameData->sprites[S_PIANTA]), gameData);
        }
        else
        {

            beep();
            // la pianta è l'oggetto attivo

            // uccido la pianta ma non la rana
            // uccisione del nemico (pianta)
            uccidiNemico(gameData->pids.pidNemici, collisione.id_oggetto_attivo);
        }
        break;
    }
    case RANA_COCCODRILLO_CATTIVO:
    {
        if(collisione.oggetto_attivo==RANA_OBJ){
            // stampo la rana sopra il coccodrillo
        int newPosAbsRanaX = gameData->pipeData.x + gameData->ranaAbsPos.x;
        int newPosAbsRanaY = gameData->pipeData.y + gameData->ranaAbsPos.y;
        gameData->pipeData.x = newPosAbsRanaX;
        gameData->pipeData.y = newPosAbsRanaY;
        // normale aggiornamento
        aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
        gameData->ranaAbsPos.x = gameData->pipeData.x;
        gameData->ranaAbsPos.y = gameData->pipeData.y;
        stampaMatrice(gameData->schermo.screenMatrix); // stampa a video solo celle della matrice dinamica modificate rispetto al ciclo precedente
        refresh();                                     // Aggiorna la finestra
        usleep(500000);

        
        // tolgo una vita alla rana
        gameData->gameInfo.vite--;
        // faccio ripartire la rana
        resetRana(gameData);
        gameData->ranaAbsPos.on_coccodrillo = false;
        gameData->ranaAbsPos.id_coccodrillo = -1;
        aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
        }
        break;
    }
    default:
    {
        break;
    }
    }
    return;
}