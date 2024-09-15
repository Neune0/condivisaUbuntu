#include "../hdr/hud.h"

void printVite(GameData *GameData)
{
    int vite = GameData->gameInfo.vite;
    Schermo *schermo = &(GameData->schermo);
    char rana[RANA_H][RANA_W] = {
        {'\\', '.', '/'},
        {'/', '-', '\\'}};
    // pulizia sezione vite
    clearScreenArea(schermo, VITEROWSTART, VITESTARTCOL, VITE_W, RANA_H);

    // stampa sezione vite
    int row = 0, col = 0;
    for (int n_vite = 0; n_vite < vite; n_vite++)
    {
        for (int rel_row = 0; rel_row < RANA_H; rel_row++)
        {
            row = VITEROWSTART + rel_row;
            for (int rel_col = 0; rel_col < RANA_W; rel_col++)
            {
                col = VITESTARTCOL + rel_col + (n_vite * (RANA_W + 1));
                setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, rana[rel_row][rel_col], 0, RANA_COL, true);
            }
        }
    }
    return;
}

void printInitTempo(GameData *gameData)
{
    Schermo *schermo = &(gameData->schermo);
    for (int row = TEMPOROWSTART; row < TEMPOROWSTART + TEMPO_H; row++)
    {
        for (int col = TEMPOSTARTCOL; col < TEMPOSTARTCOL + TEMPO_W; col++)
        {
            setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, ' ', 0, TEMPO_COL, true);
        }
    }
    return;
}

void printTempo(GameData *gameData)
{
    Schermo *schermo = &(gameData->schermo);
    int secondi = gameData->gameInfo.secondi_di_gioco;
    int maxSeconds = 0; // nuermo massimo di secondi per manche a seconda del livello
    switch (gameData->gameInfo.livello)
    {
    case 1:
        maxSeconds = TEMPOLVL1;
        break;
    case 2:
        maxSeconds = TEMPOLVL2;
        break;
    case 3:
        maxSeconds = TEMPOLVL3;
        break;
    default:
        break;
    }

    int tempoInHudPassato = ((double)(TEMPO_W) / (double)maxSeconds) * (double)secondi;
    if (secondi <= maxSeconds)
    {
        for (int row = TEMPOROWSTART; row < TEMPOROWSTART + TEMPO_H; row++)
        {
            for (int col = TEMPOSTARTCOL; col < TEMPOSTARTCOL + tempoInHudPassato; col++)
            {
                setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, ' ', 0, SFONDO_COL, true);
            }
        }
    }
    return;
}

void printDefaultHudSup(GameData *gameData)
{
    // print default per livello e manche
    char defLvlManche[HUDSUP_H][LVLMANCHEDEF_W] = {
        {'@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', ' ', ' ', ' ', ' ', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', ' ', ' ', ' ', ' ', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
        {'@', '@', '@', '@', ' ', ' ', '@', '@', ' ', ' ', '@', '@', '@', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', '@'}};
    Schermo *schermo = &(gameData->schermo);
    int row = 0, col = 0, color;
    for (int rel_row = 0; rel_row < HUDSUP_H; rel_row++)
    {
        row = rel_row + HUDSUPLETTERROWSTART;
        for (int rel_col = 0; rel_col < LVLMANCHEDEF_W; rel_col++)
        {
            col = rel_col + HUDLETTERSUPCOLSX;
            color = defLvlManche[rel_row][rel_col] == '@' ? LVL_COL : SFONDO_COL;
            setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, ' ', 0, color, true);
        }
    }
    // print default per score
    char defScore[HUDSUP_H][SCOREDEF_W] = {
        {'@', '@', '@', '@', ' ', '@', '@', '@', '@', ' ', '@', '@', '@', '@', ' ', '@', '@', '@', ' ', ' ', '@', '@', '@', '@', ' ', ' '},
        {'@', ' ', ' ', ' ', ' ', '@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', ' ', '@', ' ', ' ', '@', ' ', ' ', ' ', ' ', '@'},
        {'@', '@', '@', '@', ' ', '@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', '@', '@', ' ', ' ', '@', '@', '@', ' ', ' ', ' '},
        {' ', ' ', ' ', '@', ' ', '@', ' ', ' ', ' ', ' ', '@', ' ', ' ', '@', ' ', '@', ' ', '@', ' ', ' ', '@', ' ', ' ', ' ', ' ', '@'},
        {'@', '@', '@', '@', ' ', '@', '@', '@', '@', ' ', '@', '@', '@', '@', ' ', '@', ' ', ' ', '@', ' ', '@', '@', '@', '@', ' ', ' '}};
    row = 0, col = 0;
    for (int rel_row = 0; rel_row < HUDSUP_H; rel_row++)
    {
        row = rel_row + HUDSUPLETTERROWSTART;
        for (int rel_col = 0; rel_col < SCOREDEF_W; rel_col++)
        {
            col = rel_col + SCOREDEFCOLSTART;
            color = defScore[rel_row][rel_col] == '@' ? LVL_COL : SFONDO_COL;
            setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, ' ', 0, color, true);
        }
    }
    return;
}

void printLvl(GameData *gameData)
{
    char(*digits)[CIFRA_H][CIFRA_W] = getDigits();
    int level = gameData->gameInfo.livello;
    char(*digit)[CIFRA_W] = digits[level]; // Seleziona la cifra corrispondente

    Schermo *schermo = &(gameData->schermo);

    clearScreenArea(schermo, HUDSUPLETTERROWSTART, LVLSTARTCOL, CIFRA_W, CIFRA_H);

    printDigit(schermo, digit, HUDSUPLETTERROWSTART, LVLSTARTCOL, LVL_COL); // print del livello
    return;
}

void printManche(GameData *gameData)
{
    Schermo *schermo = &(gameData->schermo);
    int manche = gameData->gameInfo.manche;
    char(*digits)[CIFRA_H][CIFRA_W] = getDigits();
    char(*digit)[CIFRA_W] = digits[manche]; // Seleziona la cifra corrispondente

    // Pulizia preliminare
    clearScreenArea(schermo, HUDSUPLETTERROWSTART, MANCHESTARTCOL, CIFRA_W, CIFRA_H);

    printDigit(schermo, digit, HUDSUPLETTERROWSTART, MANCHESTARTCOL, LVL_COL);

    return;
}

void printScore(GameData *gameData)
{
    Schermo *schermo = &(gameData->schermo);
    char(*digits)[CIFRA_H][CIFRA_W] = getDigits();
    int centinaia = (gameData->gameInfo.punteggio / 100) % 9;
    int decine = (gameData->gameInfo.punteggio / 10) % 9;
    int unita = gameData->gameInfo.punteggio % 9;

    // Pulizia dello score
    clearScreenArea(schermo, HUDSUPLETTERROWSTART, SCORESTARTCOL, SCORE_W, CIFRA_H);

    // stampa dello score
    printDigit(schermo, digits[centinaia], HUDSUPLETTERROWSTART, CENTSTARTCOL, LVL_COL);
    printDigit(schermo, digits[decine], HUDSUPLETTERROWSTART, DECSTARTCOL, LVL_COL);
    printDigit(schermo, digits[unita], HUDSUPLETTERROWSTART, UNITSTARTCOL, LVL_COL);
    return;
}

void aggiornaHud(GameData *gameData)
{
    if (gameData->gameInfo.livelloIsChanged)
    {
        printLvl(gameData);
        gameData->gameInfo.livelloIsChanged = false;
    }
    if (gameData->gameInfo.mancheIsChanged)
    {
        printManche(gameData);
        gameData->gameInfo.mancheIsChanged = false;
    }
    if (gameData->gameInfo.punteggioIsChanged)
    {
        printScore(gameData);
        gameData->gameInfo.punteggioIsChanged = false;
    }
    if (gameData->gameInfo.tempoIsChanged)
    {
        printTempo(gameData);
        gameData->gameInfo.tempoIsChanged = false;
    }
    if (gameData->gameInfo.viteIsChanged)
    {
        printVite(gameData);
        gameData->gameInfo.viteIsChanged = false;
    }
    return;
}

void printViteMinus()
{

    clear(); // pulisco schermo
    refresh();

    // stampa box
    stampaBox();

    //stampaLogoMenu(STARTROWLOGOMENU, STARTCOLLOGOMENU);


    char titoloMatrix[6][46] = {
        " _____                   _____                ",
        "|  ___| __ ___   __ _   |  ___| __ ___   __ _ ",
        "| |_ | '__/ _ \\ / _` |  | |_ | '__/ _ \\ / _` |",
        "|  _|| | | (_) | (_| |  |  _|| | | (_) | (_| |",
        "|_|  |_|  \\___/ \\__,  | |_|  |_|  \\___/ \\__, |",
        "                |___/                   |___/ "};

    for (int row = 4; row < 4 + 6; row++)
    {
        for (int col = 20; col < 20 + 46; col++)
        {
            attron(COLOR_PAIR(RANA_COL));
            mvaddch(row, col, titoloMatrix[row - 4][col - 20]);
            attroff(COLOR_PAIR(RANA_COL));
        }
    }

    // char defViteMinus[HUDVITEMENO_H][HUDVITEMENO_W] = {
    //     {'@', '@', ' ', ' ', '@', '@', ' ', '@', '@', ' ', '@', '@', '@', '@', '@', '@', ' ', '@', '@', '@', '@', '@', '@', ' ', ' ', ' ', ' '},
    //     {'@', '@', ' ', ' ', '@', '@', ' ', '@', '@', ' ', '@', '@', '@', '@', '@', '@', ' ', '@', '@', '@', '@', '@', '@', ' ', ' ', ' ', ' '},
    //     {'@', '@', ' ', ' ', '@', '@', ' ', ' ', ' ', ' ', ' ', ' ', '@', '@', ' ', ' ', ' ', '@', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    //     {'@', '@', ' ', ' ', '@', '@', ' ', ' ', ' ', ' ', ' ', ' ', '@', '@', ' ', ' ', ' ', '@', '@', '@', '@', ' ', ' ', ' ', '@', '@', '@'},
    //     {'@', '@', ' ', ' ', '@', '@', ' ', '@', '@', ' ', ' ', ' ', '@', '@', ' ', ' ', ' ', '@', '@', '@', '@', ' ', ' ', ' ', '@', '@', '@'},
    //     {'@', '@', ' ', ' ', '@', '@', ' ', '@', '@', ' ', ' ', ' ', '@', '@', ' ', ' ', ' ', '@', '@', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '},
    //     {' ', '@', '@', '@', '@', ' ', ' ', '@', '@', ' ', ' ', ' ', '@', '@', ' ', ' ', ' ', '@', '@', '@', '@', '@', '@', ' ', ' ', ' ', ' '},
    //     {' ', '@', '@', '@', '@', ' ', ' ', '@', '@', ' ', ' ', ' ', '@', '@', ' ', ' ', ' ', '@', '@', '@', '@', '@', '@', ' ', ' ', ' ', ' '}};

    nodelay(stdscr, FALSE); // riabilita l'input bloccante

    // // print di vite--
    // int row = 0, col = 0;
    // for (int rel_row = 0; rel_row < HUDVITEMENO_H; rel_row++)
    // {
    //     row = rel_row + VITEMENOROWSTART;
    //     for (int rel_col = 0; rel_col < HUDVITEMENO_W; rel_col++)
    //     {
    //         col = rel_col + VITEMENOCOLSTART;

    //         if (defViteMinus[rel_row][rel_col] == '@')
    //         {
    //             attron(COLOR_PAIR(LAVA_COL));
    //             mvaddch(row, col, ' ');
    //             attroff(COLOR_PAIR(LAVA_COL));
    //         }
    //     }
    // }
    // stampa la scritta premi un tasto per continuare
    mvprintw(31, 36, "Premi un tasto per continuare...");
    getch();
    refresh();
    nodelay(stdscr, TRUE); // riabilita l'input non bloccante
    clear();
    refresh();
}

void printDigit(Schermo *schermo, char digit[CIFRA_H][CIFRA_W], int startRow, int startCol, int color)
{
    for (int rel_row = 0; rel_row < CIFRA_H; rel_row++)
    {
        int row = rel_row + startRow;
        for (int rel_col = 0; rel_col < CIFRA_W; rel_col++)
        {
            int col = rel_col + startCol;
            int cellColor = (digit[rel_row][rel_col] == '@') ? color : SFONDO_COL;
            setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, ' ', 0, cellColor, true);
        }
    }
}

void clearScreenArea(Schermo *schermo, int startRow, int startCol, int width, int height)
{
    for (int rel_row = 0; rel_row < height; rel_row++)
    {
        int row = rel_row + startRow;
        for (int rel_col = 0; rel_col < width; rel_col++)
        {
            int col = rel_col + startCol;
            setScreenCell(&(schermo->screenMatrix[row][col]), HUD_OBJ, ' ', 0, SFONDO_COL, true);
        }
    }
}

char (*getDigits())[CIFRA_H][CIFRA_W]
{
    static char cifre[NUM_CIFRE][CIFRA_H][CIFRA_W] = {
        {// Zero
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', '@'},
         {'@', ' ', ' ', '@'},
         {'@', ' ', ' ', '@'},
         {'@', '@', '@', '@'}},
        {// Uno
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'}},
        {// Due
         {'@', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', ' '},
         {'@', '@', '@', '@'}},
        {// Tre
         {'@', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {' ', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {'@', '@', '@', '@'}},
        {// Quattro
         {'@', ' ', ' ', '@'},
         {'@', ' ', ' ', '@'},
         {'@', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'}},
        {// Cinque
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', ' '},
         {'@', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {'@', '@', '@', '@'}},
        {// Sei
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', ' '},
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', '@'},
         {'@', '@', '@', '@'}},
        {// Sette
         {'@', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'},
         {' ', ' ', ' ', '@'}},
        {// Otto
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', '@'},
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', '@'},
         {'@', '@', '@', '@'}},
        {// Nove
         {'@', '@', '@', '@'},
         {'@', ' ', ' ', '@'},
         {'@', '@', '@', '@'},
         {' ', ' ', ' ', '@'},
         {'@', '@', '@', '@'}}};

    return cifre;
}