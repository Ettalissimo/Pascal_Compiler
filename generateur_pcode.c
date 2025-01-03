#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define TABLEINDEX 100
#define TAILLEMEM 100
#define TAILLECODE 100

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
    INTO_ERR, DOWNTO_ERR, DDOT_ERR, DD_ERR,
    ND_ERR, ID_NUM_PO_ERR,INST_PCODE_ERR,
    //type Errors
    TYPE_ERR, INT_ERR, BOOL_ERR, REAL_ERR,
    CHAR_ERR, STRING_ERR  
} CODES_ERR;

FILE *fichier,*FICH_SORTIE;
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

typedef struct
{
    CODES_LEX CODE;
    char NOM[20];
} TSym_Cour;

TSym_Cour SYM_COUR;

typedef struct{
    char NOM[20];
    CODES_LEX CLASSE;
    int ADRESSE;
}SYMBOLE;

SYMBOLE TABLESYM[TABLEINDEX];

int OFFSET = -1;
int IND_DER_SYM_ACC = 0;
int PC = 0;
int SP = -1;
int index_Mots = 0;
int choice;
int IND_BZE;
int LABEL_BRN;
int AVANT_BEGIN = 1;

int MEM[TAILLEMEM];

typedef enum {
    ADD, SUB, MUL, DIV, EQL, NEQ, GTR,
    LSS, GEQ, LEQ, PRN, INN, INT, LDI, LDA, LDV,
    STO, BRN, BZE, HLT
} MNEMONIQUES;

typedef struct{
    MNEMONIQUES MNE;
    int SUITE;
}INSTRUCTION;

INSTRUCTION PCODE[TAILLECODE];
TSym_Cour sym,sym2,sym3;

void Lire_Car() {
    Car_Cour = fgetc(fichier);
}

void lire_nombre() {
    char nombre[11];
    int indice = 0;
    nombre[indice++] = Car_Cour;
    Lire_Car();
    while (isdigit(Car_Cour)) {
        nombre[indice++] = Car_Cour;
        Lire_Car();
    }
    nombre[indice] = '\0';
    SYM_COUR.CODE = NUM_TOKEN;
    strcpy(SYM_COUR.NOM, nombre);
}


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
    else if (STRCASECMP(mot, "for") == 0){
        SYM_COUR.CODE = FOR_TOKEN;
    }
    else if (STRCASECMP(mot, "case") == 0){
        SYM_COUR.CODE = CASE_TOKEN;
    }
    else if (STRCASECMP(mot, "of") == 0){
        SYM_COUR.CODE = OF_TOKEN;

    }else if (STRCASECMP(mot, "integer") == 0){  // Recognize int keyword
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

void Erreur(CODES_ERR code){
    printf("Erreur: %d\n", code);
    printf("Current Token: %d\n", SYM_COUR.CODE);
    printf("Current Lexeme: %s\n", SYM_COUR.NOM);
    exit(EXIT_FAILURE);
}

int RechercherSym(char mot[20]){
    for(int i = 0;i<index_Mots;i++){
        if(!STRCASECMP(TABLESYM[i].NOM,mot)){
            return i;
        }
    }
    return -1;
}

void AJOUTER(){
    TABLESYM[IND_DER_SYM_ACC].ADRESSE = ++OFFSET;
    TABLESYM[IND_DER_SYM_ACC].CLASSE = ID_TOKEN;
    strcpy(TABLESYM[IND_DER_SYM_ACC].NOM,sym.NOM);
    index_Mots++;
}

void Codage_Lex(char mot[20]) {
    int indice_token = -1;

    indice_token = RechercherSym(mot);

    if (indice_token != -1) {
        if ((AVANT_BEGIN == 1) && (indice_token > 10)) {
            Erreur(DD_ERR);
        } else {
            IND_DER_SYM_ACC = indice_token;
        }
    } else {
        if (AVANT_BEGIN == 1) {
            IND_DER_SYM_ACC = index_Mots;
            AJOUTER();
        } else {
            Erreur(ND_ERR);
        }
    }
}


void Test_Symbole(CODES_LEX cl, CODES_ERR COD_ERR){
    if (SYM_COUR.CODE == cl){
        Sym_Suiv();
    }
    else{
        Erreur(COD_ERR);
    }
}



void GENERER2(MNEMONIQUES M, int A) {
    if (PC == TAILLECODE) {
        printf("ERROR: PC is equal to TAILLECODE.\n");
    } else {
        PC = PC + 1;
        PCODE[PC].MNE = M;
        PCODE[PC].SUITE = A;
    }
}

void GENERER1(MNEMONIQUES M) {
    if (PC == TAILLECODE) {
        printf("ERROR: PC is equal to TAILLECODE.\n");
    }else {
        GENERER2(M,-1);
    }
}





void BLOCK(){
    OFFSET = -1;
    CONSTS();
    VARS();
    PCODE[0].MNE = INT;
    PCODE[0].SUITE = OFFSET+1;
    INSTS();
}

void PROGRAM(){
    Test_Symbole(PROGRAM_TOKEN, PROGRAM_ERR);
    Test_Symbole(ID_TOKEN, ID_ERR);
    Test_Symbole(PV_TOKEN, PV_ERR);
    BLOCK();
    GENERER1(HLT);
    Test_Symbole(PT_TOKEN, PT_ERR);
}

void AFFEC(){
    sym = SYM_COUR;
    Test_Symbole(ID_TOKEN, ID_ERR);
    Codage_Lex(sym.NOM);
    GENERER2(LDA,TABLESYM[IND_DER_SYM_ACC].ADRESSE);
    Test_Symbole(AFF_TOKEN, AFF_ERR);
    EXPR();
    GENERER1(STO);
}

void EXPR(){
    TERM();
    while ((SYM_COUR.CODE==PLUS_TOKEN) || (SYM_COUR.CODE==MOINS_TOKEN)){
        sym = SYM_COUR;
        Sym_Suiv();
        TERM();
        if(sym.CODE==PLUS_TOKEN) GENERER1(ADD);
        else GENERER1(SUB);
    }
}

void TERM(){
    FACT();
    while ((SYM_COUR.CODE==MULT_TOKEN) || (SYM_COUR.CODE==DIV_TOKEN)){
        sym = SYM_COUR;
        Sym_Suiv();
        FACT();
        if(sym.CODE==MULT_TOKEN) GENERER1(MUL);
        else GENERER1(DIV);
    }
}

void FACT(){
    switch (SYM_COUR.CODE) {
        case ID_TOKEN:
            Codage_Lex(SYM_COUR.NOM);
            GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
            GENERER1(LDV);
            Sym_Suiv();
            break;

        case NUM_TOKEN:
            GENERER2(LDI, atoi(SYM_COUR.NOM));
            Sym_Suiv();
            break;

        case PO_TOKEN:
            {
                Sym_Suiv();
                EXPR();
                Test_Symbole(PF_TOKEN, PF_ERR);
            };
            break;
        default:
            Erreur(ID_NUM_PO_ERR);
            break;
    }
}

void ECRIRE(){
    Test_Symbole(WRITE_TOKEN, WRITE_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    EXPR();
    GENERER1(PRN);
    while (SYM_COUR.CODE==VIR_TOKEN){
        Sym_Suiv();
        EXPR();
        GENERER1(PRN);
    }
    Test_Symbole(PF_TOKEN, PF_ERR);
}

void LIRE(){
    Test_Symbole(READ_TOKEN, READ_ERR);
    Test_Symbole(PO_TOKEN, PO_ERR);
    sym = SYM_COUR;
    Test_Symbole(ID_TOKEN, ID_ERR);
    Codage_Lex(sym.NOM);
    GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
    GENERER1(INN);
    while (SYM_COUR.CODE==VIR_TOKEN){
        Sym_Suiv();
        sym = SYM_COUR;
        Test_Symbole(ID_TOKEN, ID_ERR);
        Codage_Lex(sym.NOM);
        GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
        GENERER1(INN);
    }
    Test_Symbole(PF_TOKEN, PF_ERR);
}



void RELOP(){
    switch (SYM_COUR.CODE){
    case EG_TOKEN:
        Test_Symbole(EG_TOKEN, EG_ERR);
        choice = 1;
        break;
    case DIFF_TOKEN:
        Test_Symbole(DIFF_TOKEN, DIFF_ERR);
        choice = 2;
        break;
    case INF_TOKEN:
        Test_Symbole(INF_TOKEN, INF_ERR);
        choice = 3;
        break;
    case SUP_TOKEN:
        Test_Symbole(SUP_TOKEN, SUP_ERR);
        choice = 4;
        break;
    case INFEG_TOKEN:
        Test_Symbole(INFEG_TOKEN, INFEG_ERR);
        choice = 5;
        break;
    case SUPEG_TOKEN:
        Test_Symbole(SUPEG_TOKEN, SUPEG_ERR);
        choice = 6;
        break;
    default:
        Erreur(ERREUR_ERR);
        break;
    }
}


void COND(){
    choice = 0;
    EXPR();
    RELOP();
    EXPR();
    switch(choice){
        case 1:
            GENERER1(EQL);
            break;
        case 2:
            GENERER1(NEQ);
            break;
        case 3:
            GENERER1(GTR);
            break;
        case 4:
            GENERER1(GEQ);
            break;
        case 5:
            GENERER1(LSS);
            break;
        case 6:
            GENERER1(LEQ);
            break;
    }
}



void SI(){
    Test_Symbole(IF_TOKEN, IF_ERR);
    COND();
    GENERER2(BZE,PC+1);
    IND_BZE = PC;
    Test_Symbole(THEN_TOKEN, THEN_ERR);
    INST();
    PCODE[IND_BZE].SUITE = PC+1;
}




void TANTQUE(){
    Test_Symbole(WHILE_TOKEN, WHILE_ERR);
    LABEL_BRN = PC+1;
    COND();
    GENERER1(BZE);
    IND_BZE = PC;
    Test_Symbole(DO_TOKEN, DO_ERR);
    INST();
    GENERER2(BRN, LABEL_BRN);
    PCODE[IND_BZE].SUITE=PC+1;
}


void CONSTS() {
    switch (SYM_COUR.CODE) {
        case CONST_TOKEN: {
            Sym_Suiv();
            sym = SYM_COUR;
            Test_Symbole(ID_TOKEN, ID_ERR);
            Codage_Lex(sym.NOM);
            GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
            Test_Symbole(EG_TOKEN, EG_ERR);
            sym = SYM_COUR;
            Test_Symbole(NUM_TOKEN, NUM_ERR);
            GENERER2(LDI, atoi(sym.NOM));
            GENERER1(STO);
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
                sym = SYM_COUR;
                Sym_Suiv();
                Codage_Lex(sym.NOM);
                GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
                Test_Symbole(EG_TOKEN, EG_ERR);
                sym = SYM_COUR;
                Test_Symbole(NUM_TOKEN, NUM_ERR);
                GENERER2(LDI, atoi(sym.NOM));
                GENERER1(STO);
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
            };
            break;
        }
        case VAR_TOKEN: break;
        case BEGIN_TOKEN: break;
        default: Erreur(CONST_VAR_BEGIN_ERR); break;
    }
}

void VARS(){
    switch (SYM_COUR.CODE){
    case VAR_TOKEN:
        Sym_Suiv();
        sym = SYM_COUR;
        Test_Symbole(ID_TOKEN, ID_ERR);
        Codage_Lex(sym.NOM);
        while (SYM_COUR.CODE == VIR_TOKEN){
            Sym_Suiv();
            sym = SYM_COUR;
            Test_Symbole(ID_TOKEN, ID_ERR);
            Codage_Lex(sym.NOM);
        }
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
        Test_Symbole(PV_TOKEN, PV_ERR);
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

void POUR(){
    Test_Symbole(FOR_TOKEN, FOR_ERR);
    sym = SYM_COUR;
    AFFEC();
    switch (SYM_COUR.CODE){
        case DOWNTO_TOKEN:
            sym3 = SYM_COUR;
            Test_Symbole(DOWNTO_TOKEN, DOWNTO_ERR);
            break;
        case INTO_TOKEN:
            sym3 = SYM_COUR;
            Test_Symbole(INTO_TOKEN, INTO_ERR);
            break;
        default:
            Erreur(ERREUR_ERR);
            break;
    }
    sym2 = SYM_COUR;
    Test_Symbole(NUM_TOKEN, NUM_ERR);
    Test_Symbole(DO_TOKEN, DO_ERR);
    LABEL_BRN = PC+1;
    Codage_Lex(sym.NOM);
    GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
    GENERER1(LDV);
    GENERER2(LDI, atoi(sym2.NOM));
    switch (sym3.CODE){
        case DOWNTO_TOKEN:
            GENERER1(GEQ);
            break;
        case INTO_TOKEN:
            GENERER1(LEQ);
            break;
        default:
            break;
    }
    IND_BZE = PC+1;
    GENERER1(BZE);
    INST();
    Codage_Lex(sym.NOM);
    GENERER2(LDA,TABLESYM[IND_DER_SYM_ACC].ADRESSE);
    GENERER2(LDA,TABLESYM[IND_DER_SYM_ACC].ADRESSE);
    GENERER1(LDV);
    GENERER2(LDI,1);
    switch (sym3.CODE){
        case DOWNTO_TOKEN:
            GENERER1(SUB);
            break;
        case INTO_TOKEN:
            GENERER1(ADD);
            break;
        default:
            break;
    }
    GENERER1(STO);
    GENERER2(BRN,LABEL_BRN);
    PCODE[IND_BZE].SUITE = PC+1;
}

void REPETER(){
    Test_Symbole(REPEAT_TOKEN, REPEAT_ERR);
    IND_BZE = PC+1;
    INST();
    Test_Symbole(UNTIL_TOKEN, UNTIL_ERR);
    COND();
    GENERER2(BZE,IND_BZE);
}

void CAS(){
    Test_Symbole(CASE_TOKEN, CASE_ERR);
    sym = SYM_COUR;
    Test_Symbole(ID_TOKEN,ID_ERR);
    Codage_Lex(sym.NOM);
    GENERER2(LDA, TABLESYM[IND_DER_SYM_ACC].ADRESSE);
    GENERER1(LDV);
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

const char* MNEString(MNEMONIQUES mne) {
    switch (mne) {
        case ADD:
            return "ADD";
        case SUB:
            return "SUB";
        case MUL:
            return "MUL";
        case DIV:
            return "DIV";
        case EQL:
            return "EQL";
        case NEQ:
            return "NEQ";
        case GTR:
            return "GTR";
        case LSS:
            return "LSS";
        case GEQ:
            return "GEQ";
        case LEQ:
            return "LEQ";
        case PRN:
            return "PRN";
        case INN:
            return "INN";
        case INT:
            return "INT";
        case LDI:
            return "LDI";
        case LDA:
            return "LDA";
        case LDV:
            return "LDV";
        case STO:
            return "STO";
        case BRN:
            return "BRN";
        case BZE:
            return "BZE";
        case HLT:
            return "HLT";
        default:
            return "UNKNOWN";
    }
}

void SaveInstToFile(INSTRUCTION INST) {
    switch (INST.MNE) {
        case LDA:
            fprintf(FICH_SORTIE, "%s\t%d\n", "LDA", INST.SUITE);
            break;
        case LDI:
            fprintf(FICH_SORTIE, "%s\t%d\n", "LDI", INST.SUITE);
            break;
        case INT:
            fprintf(FICH_SORTIE, "%s\t%d\n", "INT", INST.SUITE);
            break;
        case BZE:
            fprintf(FICH_SORTIE, "%s\t%d\n", "BZE", INST.SUITE);
            break;
        case BRN:
            fprintf(FICH_SORTIE, "%s\t%d\n", "BRN", INST.SUITE);
            break;
        case LDV:
            fprintf(FICH_SORTIE, "%s\n", "LDV");
            break;
        case ADD:
            fprintf(FICH_SORTIE, "%s\n", "ADD");
            break;
        case SUB:
            fprintf(FICH_SORTIE, "%s\n", "SUB");
            break;
        case MUL:
            fprintf(FICH_SORTIE, "%s\n", "MUL");
            break;
        case DIV:
            fprintf(FICH_SORTIE, "%s\n", "DIV");
            break;
        case LEQ:
            fprintf(FICH_SORTIE, "%s\n", "LEQ");
            break;
        case GEQ:
            fprintf(FICH_SORTIE, "%s\n", "GEQ");
            break;
        case NEQ:
            fprintf(FICH_SORTIE, "%s\n", "NEQ");
            break;
        case LSS:
            fprintf(FICH_SORTIE, "%s\n", "LSS");
            break;
        case GTR:
            fprintf(FICH_SORTIE, "%s\n", "GTR");
            break;
        case HLT:
            fprintf(FICH_SORTIE, "%s", "HLT");
            break;
        case STO:
            fprintf(FICH_SORTIE, "%s\n", "STO");
            break;
        case INN:
            fprintf(FICH_SORTIE, "%s\n", "INN");
            break;
        case PRN:
            fprintf(FICH_SORTIE, "%s\n", "PRN");
            break;
        default:
            Erreur(INST_PCODE_ERR);
            break;
    }
}


int main(){
    fichier = fopen("program2.txt", "r");
    FICH_SORTIE = fopen("fichierSortie.txt","w+");
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
        for(int i = 0;i<PC+1;i++){
            SaveInstToFile(PCODE[i]);
        }
        printf("PCODE généré avec succès!!!\n");
    }
    else{
        printf("PAS BRAVO: fin de programme erronée!!!!\n");
        printf("Current Token: %d\n", SYM_COUR.CODE);
        printf("Current Lexeme: %s\n", SYM_COUR.NOM);
        Sym_Suiv();
    }

    fclose(fichier);
    fclose(FICH_SORTIE);

    return 0;
}