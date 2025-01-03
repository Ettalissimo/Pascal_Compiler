#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
    DDOT_TOKEN,PROGRAM_ERR,ID_ERR,PV_ERR,PT_ERR,CONST_VAR_BEGIN_ERR,NUM_ERR,EG_ERR,
    DDOT_ERR,ERREUR_ERR,VAR_BEGIN_ERR,BEGIN_ERR,END_ERR,AFF_ERR,IF_ERR,THEN_ERR,WHILE_ERR,DO_ERR,
    WRITE_ERR,PO_ERR,PF_ERR, CASE_ERR,
    UNTIL_ERR,REPEAT_ERR,INTO_ERR,DOWNTO_ERR,FOR_ERR,DIV_ERR,MULT_ERR,MOINS_ERR,PLUS_ERR,SUPEG_ERR,
    INFEG_ERR,SUP_ERR,INF_ERR,DIFF_ERR,READ_ERR,
    // Added new type tokens
    INT_TOKEN, BOOL_TOKEN, REAL_TOKEN,
    CHAR_TOKEN, STRING_TOKEN  
} CODES_LEX;

typedef enum {
    NO_ERROR,
    UNDECLARED_ID_ERROR,
    TYPE_MISMATCH_ERROR,
    OTHER_ERROR
} SEMANTIC_ERROR;

typedef struct{
    CODES_LEX CODE;
    char NOM[20];
} TSym_Cour;

TSym_Cour SYM_COUR;

FILE *fichier;

char Car_Cour;

SEMANTIC_ERROR semantic_error = NO_ERROR;

void Sym_Suiv();
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
    if (strcasecmp(mot, "program") == 0){
        SYM_COUR.CODE = PROGRAM_TOKEN;
    }
    else if (strcasecmp(mot, "const") == 0){
        SYM_COUR.CODE = CONST_TOKEN;
    }
    else if (strcasecmp(mot, "var") == 0){
        SYM_COUR.CODE = VAR_TOKEN;
    }
    else if (strcasecmp(mot, "begin") == 0){
        SYM_COUR.CODE = BEGIN_TOKEN;
    }
    else if (strcasecmp(mot, "end") == 0){
        SYM_COUR.CODE = END_TOKEN;
    }
    else if (strcasecmp(mot, "if") == 0){
        SYM_COUR.CODE = IF_TOKEN;
    }
    else if (strcasecmp(mot, "then") == 0){
        SYM_COUR.CODE = THEN_TOKEN;
    }
    else if (strcasecmp(mot, "while") == 0){
        SYM_COUR.CODE = WHILE_TOKEN;
    }
    else if (strcasecmp(mot, "do") == 0){
        SYM_COUR.CODE = DO_TOKEN;
    }
    else if (strcasecmp(mot, "read") == 0){
        SYM_COUR.CODE = READ_TOKEN;
    }
    else if (strcasecmp(mot, "write") == 0){
        SYM_COUR.CODE = WRITE_TOKEN;
    }
    else if (strcasecmp(mot, "else") == 0){
        SYM_COUR.CODE = ELSE_TOKEN;
    }
    else if (strcasecmp(mot, "repeat") == 0){
        SYM_COUR.CODE = REPEAT_TOKEN;
    }
    else if (strcasecmp(mot, "until") == 0){
        SYM_COUR.CODE = UNTIL_TOKEN;
    }
    else if (strcasecmp(mot, "for") == 0){
        SYM_COUR.CODE = FOR_TOKEN;
    }
    else if (strcasecmp(mot, "case") == 0){
        SYM_COUR.CODE = CASE_TOKEN;
    }
    else if (strcasecmp(mot, "of") == 0){
        SYM_COUR.CODE = OF_TOKEN;
    }
    else if (strcasecmp(mot, "integer") == 0){  // Recognize int keyword
        SYM_COUR.CODE = INT_TOKEN;
    }
    else if (strcasecmp(mot, "boolean") == 0){  // Recognize bool keyword
        SYM_COUR.CODE = BOOL_TOKEN;
    }
    else if (strcasecmp(mot, "real") == 0){  // Recognize float keyword
        SYM_COUR.CODE = REAL_TOKEN;
    }
    else if (strcasecmp(mot, "char") == 0){  // Recognize char keyword
        SYM_COUR.CODE = CHAR_TOKEN;
    }
    else if (strcasecmp(mot, "string") == 0){  // Recognize string keyword
        SYM_COUR.CODE = STRING_TOKEN;
    }
    else{
        SYM_COUR.CODE = ID_TOKEN;
    }
    strcpy(SYM_COUR.NOM, mot);
}

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

void Lire_Car(){
    Car_Cour = fgetc(fichier);
}

void Sym_Suiv() {
    while (Car_Cour == ' ' || Car_Cour == '\n' || Car_Cour == '\t') {
        Lire_Car();
    }
    if (isalpha(Car_Cour)) {
        lire_mot();
    } else if (isdigit(Car_Cour)) {
        lire_nombre();
    } else {
        switch (Car_Cour) {
            case ';':
                SYM_COUR.CODE = PV_TOKEN;
                Lire_Car();
                break;

            case '.':
                SYM_COUR.CODE = PT_TOKEN;
                Lire_Car();
                break;

            case '+':
                SYM_COUR.CODE = PLUS_TOKEN;
                Lire_Car();
                break;

            case '-':
                SYM_COUR.CODE = MOINS_TOKEN;
                Lire_Car();
                break;

            case '*':
                SYM_COUR.CODE = MULT_TOKEN;
                Lire_Car();
                break;

            case '/':
                SYM_COUR.CODE = DIV_TOKEN;
                Lire_Car();
                break;

            case ',':
                SYM_COUR.CODE = VIR_TOKEN;
                Lire_Car();
                break;

            case ':':
                Lire_Car();
                if (Car_Cour == '=') {
                    SYM_COUR.CODE = AFF_TOKEN;
                    Lire_Car();
                } else {
                    SYM_COUR.CODE = DDOT_TOKEN;
                }
                break;

            case '<':
                Lire_Car();
                if (Car_Cour == '=') {
                    SYM_COUR.CODE = INFEG_TOKEN;
                    Lire_Car();
                } else if (Car_Cour == '>') {
                    SYM_COUR.CODE = DIFF_TOKEN;
                    Lire_Car();
                } else {
                    SYM_COUR.CODE = INF_TOKEN;
                }
                break;

            case '>':
                Lire_Car();
                if (Car_Cour == '=') {
                    SYM_COUR.CODE = SUPEG_TOKEN;
                    Lire_Car();
                } else {
                    SYM_COUR.CODE = SUP_TOKEN;
                }
                break;

            case '(':
                SYM_COUR.CODE = PO_TOKEN;
                Lire_Car();
                break;
            case '=':
                SYM_COUR.CODE = EG_TOKEN;
                Lire_Car();
                break;

            case ')':
                SYM_COUR.CODE = PF_TOKEN;
                Lire_Car();
                break;

            case EOF:
                SYM_COUR.CODE = EOF_TOKEN;
                break;

            default:
                SYM_COUR.CODE = ERREUR_TOKEN;
                Lire_Car(); 
        }
    }
    
}

void Erreur(SEMANTIC_ERROR error_code){
    // Fonction pour afficher et gérer les erreurs sémantiques
    switch(error_code) {
        case UNDECLARED_ID_ERROR:
            printf("Erreur sémantique: Identifiant non déclaré\n");
            break;
        case TYPE_MISMATCH_ERROR:
            printf("Erreur sémantique: Incompatibilité de type\n");
            break;
        case OTHER_ERROR:
            printf("Erreur sémantique: Autre erreur\n");
            break;
        default:
            printf("Erreur sémantique inconnue\n");
    }
    exit(EXIT_FAILURE);
}

void Test_Symbole(CODES_LEX cl, SEMANTIC_ERROR error_code){
    if (SYM_COUR.CODE == cl){
        Sym_Suiv();
    }
    else{
        Erreur(error_code);
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
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(AFF_TOKEN, AFF_ERR);
    Test_Symbole(NUM_TOKEN, NUM_ERR);

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

    Test_Symbole(NUM_TOKEN, NUM_ERR);
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


int main() {
    fichier = fopen("program_type.txt", "r");
    if (fichier == NULL) {
        fprintf(stderr, "Impossible d'ouvrir le fichier source.txt\n");
        return EXIT_FAILURE;
    }

    
    Lire_Car();
    Sym_Suiv();
    PROGRAM();
    //analyse sémantique
    Erreur(ERREUR_ERR);
     printf("Program execution completed.\n");
    
    fclose(fichier);
    return EXIT_SUCCESS;
}
