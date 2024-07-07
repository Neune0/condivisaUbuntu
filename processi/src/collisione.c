#include "../hdr/collisione.h"

Collisione detectCollisione(GameData *gameData)
{
    Collisione collisione;
    collisione.tipoCollisione = NO_COLLISIONE;
    Schermo *schermo = &(gameData->schermo); // lo schermo di gioco

    // switch sul carattere in pipe
    switch (gameData->pipeData.type)
    {
    // rana
    case 'X':
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
                    return collisione;
                    break;
                case TANA_OPEN_OBJ:
                    collisione.tipoCollisione = RANA_TANA_APERTA;
                    collisione.id_oggetto_attivo = gameData->pipeData.id;
                    collisione.oggetto_attivo = RANA_OBJ;
                    collisione.oggetto_passivo = TANA_OPEN_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    return collisione;
                    break;
                case TANA_CLOSE_OBJ:
                    collisione.tipoCollisione = RANA_TANA_CHIUSA;
                    collisione.id_oggetto_attivo = gameData->pipeData.id;
                    collisione.oggetto_attivo = RANA_OBJ;
                    collisione.oggetto_passivo = TANA_CLOSE_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    return collisione;
                    break;
                case COCCODRILLO_BUONO_OBJ:
                    collisione.tipoCollisione = RANA_COCCODRILLO_BUONO;
                    collisione.id_oggetto_attivo = gameData->pipeData.id;
                    collisione.oggetto_attivo = RANA_OBJ;
                    collisione.id_oggetto_passivo = schermo->screenMatrix[row][col].id;
                    collisione.oggetto_passivo = COCCODRILLO_BUONO_OBJ;
                    return collisione;
                    break;
                default:
                    break;
                }
            }
        }
        break;
    // proiettile rana
    case 'P':
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
                    break;
                default:
                    break;
                }
            }
        }

        break;
    // tronco sull if controllo che sia cattivo
    case 'C':
    case 'c':
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
                        break;
                    default:
                        break;
                    }
                }
            }
        }

        break;
    default:
        break;
    }

    return collisione;
}

void handleCollisione(GameData *gameData, Collisione collisione)
{
    if (collisione.tipoCollisione == PROIETTILE_COCCODRILLO_CATTIVO)
    {
        mvprintw(31, 106, "collisione p_c");
    }

    switch (collisione.tipoCollisione)
    {
    case RANA_FIUME:
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
        aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
        break;
    case PROIETTILE_COCCODRILLO_CATTIVO:
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
    case RANA_COCCODRILLO_BUONO:
        if(collisione.oggetto_attivo==RANA_OBJ){
            //reset di tutti i coccodrilli a rana_on false
            for(int i=0;i<MAXNCOCCODRILLI;i++){
                gameData->controlloCoccodrilli[i].rana_on=false;
            }
            // imposta a true rana_on del coccodrillo coinvolto nella collisione
            gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].rana_on=true;

            // stampo il coccodrillo buono
            if(gameData->controlloCoccodrilli[collisione.id_oggetto_passivo].direction==1){
                aggiornaOggettoNew(gameData,gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo],gameData->oldPos.coccodrilli,S_COCCODRILLO_DX);
            }
            else{
                 aggiornaOggettoNew(gameData,gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo],gameData->oldPos.coccodrilli,S_COCCODRILLO_SX);
            }

            // stampo sopra il coccodrillo buono la rana
             aggiornaOggettoNew(gameData,gameData->oldPos.coccodrilli[collisione.id_oggetto_passivo],&(gameData->oldPos.rana),S_RANA);

        }else{
            // l'oggetto attivo e il coccodrillo buono
            //reset di tutti i coccodrilli a rana_on false
            for(int i=0;i<MAXNCOCCODRILLI;i++){
                gameData->controlloCoccodrilli[i].rana_on=false;
            }
            // imposta a true rana_on del coccodrillo coinvolto nella collisione
            gameData->controlloCoccodrilli[collisione.id_oggetto_attivo].rana_on=true;

            // stampo il coccodrillo buono
            if(gameData->controlloCoccodrilli[gameData->pipeData.id].direction==1){
                 aggiornaOggetto(gameData,gameData->oldPos.coccodrilli,S_COCCODRILLO_DX);
            }
            else{
                aggiornaOggetto(gameData,gameData->oldPos.coccodrilli,S_COCCODRILLO_SX);
            }
            
            // stampo sopra il coccodrillo buono la rana
            aggiornaOggetto(gameData, &(gameData->oldPos.rana), S_RANA);
           

            
        }
        break;
    default:
        break;
    }
    return;
}