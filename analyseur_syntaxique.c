#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#ifdef _WIN32
#define STRCASECMP stricmp
#else
#define STRCASECMP strcasecmp
#endif

typedef enum {
    ID_TOKEN, PROGRAM_TOKEN, CONST_TOKEN, VAR_TOKEN,
    BEGIN_TOKEN, END_TOKEN, IF_TOKEN, THEN_TOKEN,
    WHILE_TOKEN, DO_TOKEN, READ_TOKEN, WRITE_TOKEN,
    PV_TOKEN, PT_TOKEN, PLUS_TOKEN, MOINS_TOKEN,
    MULT_TOKEN, DIV_TOKEN, VIR_TOKEN, AFF_TOKEN,
    INF_TOKEN, INFEG_TOKEN, SUP_TOKEN, SUPEG_TOKEN,
    DIFF_TOKEN, PO_TOKEN, PF_TOKEN, EOF_TOKEN,
    NUM_TOKEN, ERREUR_TOKEN, FIN_TOKEN, EG_TOKEN,
    REPEAT_TOKEN, UNTIL_TOKEN, FOR_TOKEN, ELSE_TOKEN,
    CASE_TOKEN, OF_TOKEN, INTO_TOKEN, DOWNTO_TOKEN,
    DDOT_TOKEN,
    // Added new type tokens
    INT_TOKEN, BOOL_TOKEN, REAL_TOKEN,
    CHAR_TOKEN, STRING_TOKEN  
} CODES_LEX;


typedef enum {
    ID_ERR, PROGRAM_ERR, CONST_ERR, VAR_ERR,
    BEGIN_ERR, END_ERR, IF_ERR, THEN_ERR,
    WHILE_ERR, DO_ERR, READ_ERR, WRITE_ERR,
    PV_ERR, PT_ERR, PLUS_ERR, MOINS_ERR,
    MULT_ERR, DIV_ERR, VIR_ERR, AFF_ERR,
    INF_ERR, INFEG_ERR, SUP_ERR, SUPEG_ERR,
    DIFF_ERR, PO_ERR, PF_ERR, EOF_ERR,
    NUM_ERR, ERREUR_ERR, FIN_ERR, EG_ERR,
    CONST_VAR_BEGIN_ERR, VAR_BEGIN_ERR, REPEAT_ERR, UNTIL_ERR,
    FOR_ERR, ELSE_ERR, CASE_ERR, OF_ERR,
    INTO_ERR, DOWNTO_ERR, DDOT_ERR,
    //type Errors
    TYPE_ERR, INT_ERR, BOOL_ERR, REAL_ERR,
    CHAR_ERR, STRING_ERR  
} CODES_ERR;

typedef struct{
    CODES_LEX CODE;
    char NOM[20];
} TSym_Cour;

TSym_Cour SYM_COUR;

FILE *fichier;

char Car_Cour;

void PROGRAM();
void BLOCK();
void CONSTS();
void VARS();
void INSTS();
void INST();
void AFFEC();
void SI();
void REPETER();
void POUR();
void TANTQUE();
void ECRIRE();
void LIRE();
void COND();
void RELOP();
void EXPR();
void ADDOP();
void TERM();
void MULOP();
void FACT();
void CAS();

void Lire_Car();
void Erreur(CODES_ERR code);
void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR);
void Sym_Suiv();
void lire_mot();
void lire_nombre();


void lire_mot(){
    char mot[20];
    int indice = 0;
    mot[indice++] = Car_Cour;
    Lire_Car();
    while (isalpha(Car_Cour) || isdigit(Car_Cour)){
        mot[indice++] = Car_Cour;
        Lire_Car();
    }
    mot[indice] = '\0';
    if (STRCASECMP(mot, "program") == 0){
        SYM_COUR.CODE = PROGRAM_TOKEN;
    }
    else if (STRCASECMP(mot, "const") == 0){
        SYM_COUR.CODE = CONST_TOKEN;
    }
    else if (STRCASECMP(mot, "var") == 0){
        SYM_COUR.CODE = VAR_TOKEN;
    }
    else if (STRCASECMP(mot, "begin") == 0){
        SYM_COUR.CODE = BEGIN_TOKEN;
    }
    else if (STRCASECMP(mot, "end") == 0){
        SYM_COUR.CODE = END_TOKEN;
    }
    else if (STRCASECMP(mot, "if") == 0){
        SYM_COUR.CODE = IF_TOKEN;
    }
    else if (STRCASECMP(mot, "then") == 0){
        SYM_COUR.CODE = THEN_TOKEN;
    }
    else if (STRCASECMP(mot, "while") == 0){
        SYM_COUR.CODE = WHILE_TOKEN;
    }
    else if (STRCASECMP(mot, "do") == 0){
        SYM_COUR.CODE = DO_TOKEN;
    }
    else if (STRCASECMP(mot, "read") == 0){
        SYM_COUR.CODE = READ_TOKEN;
    }
    else if (STRCASECMP(mot, "write") == 0){
        SYM_COUR.CODE = WRITE_TOKEN;
    }
    else if (STRCASECMP(mot, "else") == 0){
        SYM_COUR.CODE = ELSE_TOKEN;
    }
    else if (STRCASECMP(mot, "repeat") == 0){
        SYM_COUR.CODE = REPEAT_TOKEN;
    }
    else if (STRCASECMP(mot, "until") == 0){
        SYM_COUR.CODE = UNTIL_TOKEN;
    }
    else if (stricmp(mot, "downto") == 0) {
        SYM_COUR.CODE = DOWNTO_TOKEN;
    }
    else if (stricmp(mot, "to") == 0) {
        SYM_COUR.CODE = INTO_TOKEN;
    }
    else if (STRCASECMP(mot, "for") == 0){
        SYM_COUR.CODE = FOR_TOKEN;
    }
    else if (STRCASECMP(mot, "case") == 0){
        SYM_COUR.CODE = CASE_TOKEN;
    }
    else if (STRCASECMP(mot, "of") == 0){
        SYM_COUR.CODE = OF_TOKEN;
    }
    else if (STRCASECMP(mot, "integer") == 0){  // Recognize int keyword
        SYM_COUR.CODE = INT_TOKEN;
    }
    else if (STRCASECMP(mot, "boolean") == 0){  // Recognize bool keyword
        SYM_COUR.CODE = BOOL_TOKEN;
    }
    else if (STRCASECMP(mot, "real") == 0){  // Recognize float keyword
        SYM_COUR.CODE = REAL_TOKEN;
    }
    else if (STRCASECMP(mot, "char") == 0){  // Recognize char keyword
        SYM_COUR.CODE = CHAR_TOKEN;
    }
    else if (STRCASECMP(mot, "string") == 0){  // Recognize string keyword
        SYM_COUR.CODE = STRING_TOKEN;
    }
    else{
        SYM_COUR.CODE = ID_TOKEN;
    }
    strcpy(SYM_COUR.NOM, mot);
}
/*
void lire_nombre(){
    char nombre[11];
    int indice = 0;
    nombre[indice++] = Car_Cour;
    Lire_Car();
    while (isdigit(Car_Cour)){
        nombre[indice++] = Car_Cour;
        Lire_Car();
    }
    nombre[indice] = '\0';
    SYM_COUR.CODE = NUM_TOKEN;
    strcpy(SYM_COUR.NOM, nombre);
}
*/
void lire_nombre(){
    char nombre[20];
    int indice = 0;
    nombre[indice++] = Car_Cour;
    Lire_Car();
    while (isdigit(Car_Cour) || Car_Cour == '.') {
        nombre[indice++] = Car_Cour;
        Lire_Car();
    }
    nombre[indice] = '\0';

    // Check if the literal contains a period to determine if it's a float or int
    if (strchr(nombre, '.') != NULL) {
        SYM_COUR.CODE = REAL_TOKEN;
    } else {
        SYM_COUR.CODE = INT_TOKEN;
    }

    strcpy(SYM_COUR.NOM, nombre);
}

void Skip_Comments() {
    if (Car_Cour == '/') {
        Lire_Car(); 
        if (Car_Cour == '/') { 
            while (Car_Cour != '\n' && Car_Cour != EOF) {
                Lire_Car();
            }
        } else if (Car_Cour == '*') { 
            Lire_Car(); 
            while (!(Car_Cour == '*' && fgetc(fichier) == '/') && Car_Cour != EOF) {
                Lire_Car();
            }
            Lire_Car(); 
        } else {
            Car_Cour = '/';
        }
    }
}

void Sym_Suiv(){
    while (Car_Cour == ' ' || Car_Cour == '\n' || Car_Cour == '\t' || Car_Cour == '/' || Car_Cour == '{') {
        if (Car_Cour == '{') { 
            while (Car_Cour != '}' && Car_Cour != EOF) {
                Lire_Car(); 
            }
            Lire_Car(); 
        }
        Skip_Comments(); 
        Lire_Car(); 
    }
    if (isalpha(Car_Cour)){
        lire_mot();
    }
    else if (isdigit(Car_Cour)){
        lire_nombre();
    }
    else{
        char s[2];
        s[0] = Car_Cour;
        switch (Car_Cour){
        case ';':
            s[1] = '\0';
            SYM_COUR.CODE = PV_TOKEN;
            Lire_Car();
            break;

        case '+':
            s[1] = '\0';
            SYM_COUR.CODE = PLUS_TOKEN;
            Lire_Car();
            break;

        case '-':
            s[1] = '\0';
            SYM_COUR.CODE = MOINS_TOKEN;
            Lire_Car();
            break;

        case '*':
            s[1] = '\0';
            SYM_COUR.CODE = MULT_TOKEN;
            Lire_Car();
            break;

        case '/':
            s[1] = '\0';
            SYM_COUR.CODE = DIV_TOKEN;
            Lire_Car();
            break;

        case ',':
            s[1] = '\0';
            SYM_COUR.CODE = VIR_TOKEN;
            Lire_Car();
            break;

        case ':':
            Lire_Car();
            if (Car_Cour == '='){
                s[1] = '=';
                s[2] = '\0';
                SYM_COUR.CODE = AFF_TOKEN;
                Lire_Car();
            }
            else{
                s[1] = '\0';
                SYM_COUR.CODE = DDOT_TOKEN;
            }
            break;

        case '<':
            Lire_Car();
            if (Car_Cour == '='){
                s[1] = '=';
                s[2] = '\0';
                SYM_COUR.CODE = INFEG_TOKEN;
                Lire_Car();
            }
            else if (Car_Cour == '>'){
                s[1] = '=';
                s[2] = '\0';
                SYM_COUR.CODE = DIFF_TOKEN;
                Lire_Car();
            }
            else{
                s[1] = '\0';
                SYM_COUR.CODE = INF_TOKEN;
            }
            break;

        case '>':
            Lire_Car();
            if (Car_Cour == '='){
                s[1] = '=';
                s[2] = '\0';
                SYM_COUR.CODE = SUPEG_TOKEN;
                Lire_Car();
            }
            else{
                s[1] = '\0';
                SYM_COUR.CODE = SUP_TOKEN;
            }
            break;

        case '(':
            s[1] = '\0';
            SYM_COUR.CODE = PO_TOKEN;
            Lire_Car();
            break;
        case '=':
            s[1] = '\0';
            SYM_COUR.CODE = EG_TOKEN;
            Lire_Car();
            break;

        case ')':
            s[1] = '\0';
            SYM_COUR.CODE = PF_TOKEN;
            Lire_Car();
            break;

        case '.':
            s[1] = '\0';
            SYM_COUR.CODE = PT_TOKEN;
            Lire_Car();
            break;

        case EOF:
            s[1] = '\0';
            SYM_COUR.CODE = EOF_TOKEN;
            break;

        default:
            s[1] = '\0';
            SYM_COUR.CODE = ERREUR_TOKEN;
            Lire_Car();
        }
        strcpy(SYM_COUR.NOM,s);
    }
    printf("Symbol: %s\n", SYM_COUR.NOM);
}

void Lire_Car(){
    Car_Cour = fgetc(fichier);
}

void Erreur(CODES_ERR code){
    printf("Erreur: %d\n", code);
    printf("Current Token: %d\n", SYM_COUR.CODE);
    printf("Current Lexeme: %s\n", SYM_COUR.NOM);
    exit(EXIT_FAILURE);
}

void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR){
    if (SYM_COUR.CODE == cl){
        Sym_Suiv();
    }
    else{
        Erreur(COD_ERR);
    }
}

void PROGRAM(){
    Test_Symbole(PROGRAM_TOKEN, PROGRAM_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(PV_TOKEN, PV_ERR);
    BLOCK();
    Test_Symbole(PT_TOKEN, PT_ERR);
}

void BLOCK(){
    CONSTS();
    VARS();
    INSTS();
}
/*
void CONSTS() {
    switch (SYM_COUR.CODE) {
    case CONST_TOKEN:
        Sym_Suiv();
        Test_Symbole(ID_TOKEN, ID_ERR);
        Test_Symbole(EG_TOKEN, EG_ERR);

        // Vérifie si le type est un entier ou un nombre à virgule flottante
        if (SYM_COUR.CODE == INT_TOKEN || SYM_COUR.CODE == REAL_TOKEN) {
            Sym_Suiv();
        } else {
            Erreur(NUM_ERR); // Erreur si le type n'est pas spécifié correctement
        }

        Test_Symbole(PV_TOKEN, PV_ERR);

        while (SYM_COUR.CODE == ID_TOKEN) {
            Sym_Suiv();
            Test_Symbole(EG_TOKEN, EG_ERR);

            // Vérifie à nouveau le type pour chaque identifiant
            if (SYM_COUR.CODE == INT_TOKEN || SYM_COUR.CODE == REAL_TOKEN) {
                Sym_Suiv();
            } else {
                Erreur(NUM_ERR); // Erreur si le type n'est pas spécifié correctement
            }

            Test_Symbole(PV_TOKEN, PV_ERR);
        }
        break;
    case VAR_TOKEN:
        break;
    case BEGIN_TOKEN:
        break;
    default:
        Erreur(CONST_VAR_BEGIN_ERR);
        break;
    }
}*/

void CONSTS() {
    switch (SYM_COUR.CODE) {
    case CONST_TOKEN:
        Sym_Suiv();
        Test_Symbole(ID_TOKEN, ID_ERR);
        Test_Symbole(EG_TOKEN, EG_ERR);

        // Vérifie le type de la constante
        switch (SYM_COUR.CODE) {
        case INT_TOKEN:
        case REAL_TOKEN:
        case CHAR_TOKEN:
        case STRING_TOKEN:
        case BOOL_TOKEN:
            Sym_Suiv();
            break;
        default:
            Erreur(NUM_ERR); // Erreur si le type n'est pas reconnu
            break;
        }

        Test_Symbole(PV_TOKEN, PV_ERR);

        while (SYM_COUR.CODE == ID_TOKEN) {
            Sym_Suiv();
            Test_Symbole(EG_TOKEN, EG_ERR);

            // Vérifie le type de chaque identifiant
            switch (SYM_COUR.CODE) {
            case INT_TOKEN:
            case REAL_TOKEN:
            case CHAR_TOKEN:
            case STRING_TOKEN:
            case BOOL_TOKEN:
                Sym_Suiv();
                break;
            default:
                Erreur(NUM_ERR); // Erreur si le type n'est pas reconnu
                break;
            }

            Test_Symbole(PV_TOKEN, PV_ERR);
        }
        break;
    case VAR_TOKEN:
        break;
    case BEGIN_TOKEN:
        break;
    default:
        Erreur(CONST_VAR_BEGIN_ERR);
        break;
    }
}


void VARS() {
    switch (SYM_COUR.CODE) {
    case VAR_TOKEN:
        Sym_Suiv();
        do {
            // Attend un identifiant pour chaque variable
            Test_Symbole(ID_TOKEN, ID_ERR);

            // Vérifie s'il y a une virgule pour déclarer une autre variable du même type
            while (SYM_COUR.CODE == VIR_TOKEN) {
                Sym_Suiv();
                Test_Symbole(ID_TOKEN, ID_ERR);
            }

            // Attend un deux-points pour la déclaration de type
            Test_Symbole(DDOT_TOKEN, DDOT_ERR);

            // Gère le type de chaque variable
            switch (SYM_COUR.CODE) {
            case INT_TOKEN:
            case BOOL_TOKEN:
            case REAL_TOKEN:
            case CHAR_TOKEN:                
            case STRING_TOKEN:
                    Sym_Suiv();
                    break;
                default:
                    Erreur(ERREUR_ERR);  // Erreur si le type n'est pas valide
                    break;
            }

            // Attend un point-virgule pour terminer la déclaration de variable
            Test_Symbole(PV_TOKEN, PV_ERR);

        } while (SYM_COUR.CODE == ID_TOKEN);

        break;
    case BEGIN_TOKEN:
        break;
    default:
        Erreur(VAR_BEGIN_ERR);
        break;
    }
}

void INSTS(){
    if (SYM_COUR.CODE == BEGIN_TOKEN){
        Sym_Suiv();
        INST();
        while (SYM_COUR.CODE == PV_TOKEN){
            Sym_Suiv();
            INST();
        }
        if (SYM_COUR.CODE == END_TOKEN){
            Sym_Suiv();
        }
        else{
            Erreur(END_ERR);
        }
    }
    else{
        Erreur(BEGIN_ERR);
    }
}

void INST(){
    switch (SYM_COUR.CODE){
    case BEGIN_TOKEN:
        INSTS();
        break;
    case ID_TOKEN:
        AFFEC();
        break;
    case IF_TOKEN:
        SI();
        break;
    case WHILE_TOKEN:
        TANTQUE();
        break;
    case WRITE_TOKEN:
        ECRIRE();
        break;
    case READ_TOKEN:
        LIRE();
        break;
    case REPEAT_TOKEN:
        REPETER();
        break;
    case FOR_TOKEN:
        POUR();
        break;
    case CASE_TOKEN:
        CAS();
        break;
    default:
        break;
    }
}

void AFFEC(){
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(AFF_TOKEN, AFF_ERR);
    EXPR();
}

void SI(){
    Test_Symbole(IF_TOKEN, IF_ERR);
    COND();
    Test_Symbole(THEN_TOKEN, THEN_ERR);
    INST();
    if (SYM_COUR.CODE == ELSE_TOKEN){
        INST();
    }
}

void TANTQUE(){
    Test_Symbole(WHILE_TOKEN, WHILE_ERR);
    COND();
    Test_Symbole(DO_TOKEN, DO_ERR);
    INST();
}

void ECRIRE(){
    Test_Symbole(WRITE_TOKEN, WRITE_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    EXPR();
    while (SYM_COUR.CODE == VIR_TOKEN){
        Sym_Suiv();
        EXPR();
    }
    Test_Symbole(PF_TOKEN, PF_ERR);
}

void LIRE(){
    Test_Symbole(READ_TOKEN, READ_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    while (SYM_COUR.CODE == VIR_TOKEN){
        Sym_Suiv();
        Test_Symbole(ID_TOKEN, ID_ERR);
    }
    Test_Symbole(PF_TOKEN, PF_ERR);
}

void COND(){
    EXPR();
    RELOP();
    EXPR();
}

void EXPR(){
    TERM();
    while (SYM_COUR.CODE == PLUS_TOKEN || SYM_COUR.CODE == MOINS_TOKEN){
        ADDOP();
        TERM();
    }
}

void TERM(){
    FACT();
    while (SYM_COUR.CODE == MULT_TOKEN || SYM_COUR.CODE == DIV_TOKEN){
        MULOP();
        FACT();
    }
}

void FACT(){
    switch (SYM_COUR.CODE){
    case ID_TOKEN:
        Test_Symbole(ID_TOKEN, ID_ERR);
        break;
    case NUM_TOKEN:
        Test_Symbole(NUM_TOKEN, NUM_ERR);
        break;
    case PO_TOKEN:
        Test_Symbole(PO_TOKEN, PO_ERR);
        EXPR();
        Test_Symbole(PF_TOKEN, PF_ERR);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void RELOP(){
    switch (SYM_COUR.CODE){
    case EG_TOKEN:
        Test_Symbole(EG_TOKEN, EG_ERR);
        break;
    case DIFF_TOKEN:
        Test_Symbole(DIFF_TOKEN, DIFF_ERR);
        break;
    case INF_TOKEN:
        Test_Symbole(INF_TOKEN, INF_ERR);
        break;
    case SUP_TOKEN:
        Test_Symbole(SUP_TOKEN, SUP_ERR);
        break;
    case INFEG_TOKEN:
        Test_Symbole(INFEG_TOKEN, INFEG_ERR);
        break;
    case SUPEG_TOKEN:
        Test_Symbole(SUPEG_TOKEN, SUPEG_ERR);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void ADDOP(){
    switch (SYM_COUR.CODE){
    case PLUS_TOKEN:
        Test_Symbole(PLUS_TOKEN, PLUS_ERR);
        break;
    case MOINS_TOKEN:
        Test_Symbole(MOINS_TOKEN, MOINS_ERR);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void MULOP(){
    switch (SYM_COUR.CODE){
    case MULT_TOKEN:
        Test_Symbole(MULT_TOKEN, MULT_ERR);
        break;
    case DIV_TOKEN:
        Test_Symbole(DIV_TOKEN, DIV_ERR);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}

void POUR(){
    Test_Symbole(FOR_TOKEN, FOR_ERR);
    AFFEC();

    switch (SYM_COUR.CODE){
    case DOWNTO_TOKEN:
        Test_Symbole(DOWNTO_TOKEN, DOWNTO_ERR);
        break;
    case INTO_TOKEN:
        Test_Symbole(INTO_TOKEN, INTO_ERR);
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }

    Test_Symbole(INT_TOKEN, INT_ERR);
    Test_Symbole(DO_TOKEN, DO_ERR);
    INST();

}


void REPETER(){
    Test_Symbole(REPEAT_TOKEN, REPEAT_ERR);
    INST();
    Test_Symbole(UNTIL_TOKEN, UNTIL_ERR);
    COND();
}

void CAS(){
    Test_Symbole(CASE_TOKEN, CASE_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(OF_TOKEN, OF_TOKEN);
    Test_Symbole(NUM_TOKEN, NUM_ERR);
    Test_Symbole(DDOT_TOKEN, DDOT_ERR);
    INST();
    while (SYM_COUR.CODE == NUM_TOKEN){
        Sym_Suiv();
        Test_Symbole(DDOT_TOKEN, DDOT_ERR);
        INST();
    }
    if (SYM_COUR.CODE == ELSE_TOKEN) {
        Sym_Suiv();
        INST();
    }
    
    Test_Symbole(END_TOKEN, END_ERR);
}



int main(){
    fichier = fopen("program2.txt", "r");
    if (fichier == NULL){
        perror("Erreur lors de l'ouverture du fichier");
        return 1;
    }
    Lire_Car();
    Sym_Suiv();
    PROGRAM();
    printf("Program execution completed.\n");
    if (SYM_COUR.CODE == EOF_TOKEN){
        printf("BRAVO: le programme est correcte!!!\n");
    }
    else{
        printf("PAS BRAVO: fin de programme erronée!!!!\n");
        printf("Current Token: %d\n", SYM_COUR.CODE);
        printf("Current Lexeme: %s\n", SYM_COUR.NOM);
        Sym_Suiv();
    }

    fclose(fichier);

    return 0;
}