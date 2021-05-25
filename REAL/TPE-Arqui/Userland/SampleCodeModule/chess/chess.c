// This is a personal academic project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <chess.h>
#include <unistd.h>

static char *figures[PIECES_AMOUNT]={"king","queen","bishop","knight","rook","pawn"};

static int board[SQUARES][SQUARES] ={
            {ROOK,KNIGHT,BISHOP,QUEEN,KING,BISHOP,KNIGHT,ROOK},
            {PAWN,PAWN,PAWN,PAWN,PAWN,PAWN,PAWN,PAWN},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {-PAWN,-PAWN,-PAWN,-PAWN,-PAWN,-PAWN,-PAWN,-PAWN},
            {-ROOK,-KNIGHT,-BISHOP,-QUEEN,-KING,-BISHOP,-KNIGHT,-ROOK}
        };

static char pieces[PIECES_AMOUNT][PIECES_SIZE][PIECES_SIZE] = {
    {
        {'/','/','/','/','/','/','/','/'},
        {'/','/','/','#','.','#','/','/'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','/','#','.','#','/','/'},
        {'/','/','/','#','#','#','/','/'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','#','.','#','.','#','/'}
    },
    {
        {'/','/','/','/','/','/','/','/'},
        {'/','#','.','#','.','#','.','#'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','/','#','#','#','/','/'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','#','.','#','.','#','/'}
    },
    {
        {'/','/','/','/','/','/','/','/'},
        {'/','/','/','#','.','#','/','/'},
        {'/','/','#','#','.','.','#','/'},
        {'/','/','#','.','#','.','#','/'},
        {'/','/','/','#','#','#','/','/'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','#','.','#','.','#','/'}
    },
    {
        {'/','/','/','/','/','/','/','/'},
        {'/','/','/','/','/','/','/','/'},
        {'/','/','#','.','.','#','/','/'},
        {'/','/','/','#','.','.','#','/'},
        {'/','/','#','.','.','#','/','/'},
        {'/','#','.','.','.','.','#','/'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','#','.','.','.','#','/'}
    },
    {
        {'/','/','/','/','/','/','/','/'},
        {'/','#','.','#','.','#','.','#'},
        {'/','#','.','#','.','#','.','#'},
        {'/','#','.','.','.','.','.','#'},
        {'/','#','#','.','.','.','#','#'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','#','.','.','.','#','/'}
    },
    {
        {'/','/','/','/','/','/','/','/'},
        {'/','/','/','/','/','/','/','/'},
        {'/','/','/','/','/','/','/','/'},
        {'/','/','/','#','.','#','/','/'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','/','#','.','#','/','/'},
        {'/','/','#','.','.','.','#','/'},
        {'/','/','#','.','.','.','#','/'}
    }
    };

static int secondsV[3]={0,0,0};
static int turn = 1;
static int oldGame = 0;
static char buffer[TOTAL_LINES_CHESS][CONSOLE_SIZE_X/BASE_CHAR_WIDTH-2] = {{0}};
static int rotate=0;
static int castleMove[2][3]={{0,0,0},{0,0,0}};
static int xyPassant[3]={-1,-1,-1};
static int lastEat =1;

static void loadColorsPieces(uint64_t pieceC[][PIECES_SIZE],int j);
static void console();
static void reDrawChessConsole(char buffer[][CONSOLE_SIZE_X/BASE_CHAR_WIDTH-2]);
static void endGame(int winner, int by);
static void drawBoard();
static int playAgain();
static void promotePiece(int x,int y);
static int rookValidMov(int xi,int yi,int xf, int yf);
static int kingValidMov(int xi,int yi,int xf, int yf);
static int bishopValidMov(int xi,int yi,int xf, int yf);
static int queenValidMov(int xi,int yi,int xf, int yf);
static int pawnValidMov(int xi,int yi,int xf, int yf);
static int knightValidMov(int xi,int yi,int xf, int yf);
static int validMovement(int xi,int yi,int xf, int yf,int perPlayer);
static void internTimer();
static void turnBoard();
static int detectCheck();
static int detectCheckMate();
static int validCastle(char *buf);
static int drownedKing();
static void drawGame();
static void drawFullConsole(int end);
static int fewPieces();
static int noMoreMoves();

void chess(){
    clearScreen(BACKGROUND);
    int play = playAgain();
    if (play==1 ){
        oldGame = 0;
        secondsV[0]=0;
        secondsV[1]=0;
        secondsV[2]=readSeconds();
        turn =1;
        rotate = 0;
        lastEat =turn;
        for (int i =0;i<2;i++){
            for (int j = 0;j<3;j++){
                castleMove[i][j]=0;
            }
        }
        for  (int i = 0;i  < TOTAL_LINES_CHESS;i++){
            for  (int j =0 ;j<CONSOLE_SIZE_X/BASE_CHAR_WIDTH-2;j++){
                buffer[i][j]=0;
            }
        }
        int newboard[SQUARES][SQUARES] = {
            {ROOK,KNIGHT,BISHOP,QUEEN,KING,BISHOP,KNIGHT,ROOK},
            {PAWN,PAWN,PAWN,PAWN,PAWN,PAWN,PAWN,PAWN},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {-PAWN,-PAWN,-PAWN,-PAWN,-PAWN,-PAWN,-PAWN,-PAWN},
            {-ROOK,-KNIGHT,-BISHOP,-QUEEN,-KING,-BISHOP,-KNIGHT,-ROOK}
        };

        for (int i = 0; i < SQUARES; i++){
            for (int j = 0; j < SQUARES; j++){
                board[i][j] = newboard[i][j];
            }
        }
    } else if (play == 2){
        myExit();
    }
    console();
}

static void turnBoard(){
    rotate= (rotate+1)%4; //si rotate es par van las letras arriba y los numeros de costado
}

static int playAgain(){
    drawString(SCREEN_WIDTH/2-3*BASE_CHAR_WIDTH*3,CONSOLE_LIMIT_Y+BASE_CHAR_HEIGHT*3*2,"CHESS",6,WHITE,BLACK,3,1);
    if (!oldGame){
        drawString(SCREEN_WIDTH/2-3*BASE_CHAR_WIDTH*6,CONSOLE_LIMIT_Y+BASE_CHAR_HEIGHT*3*2*2,"1- CONTINUE",12,SILVER,BLACK,3,1);
    } else {
        drawString(SCREEN_WIDTH/2-3*BASE_CHAR_WIDTH*6,CONSOLE_LIMIT_Y+BASE_CHAR_HEIGHT*3*2*2,"1- CONTINUE",12,WHITE,BLACK,3,1);
    }
    drawString(SCREEN_WIDTH/2-3*BASE_CHAR_WIDTH*6,CONSOLE_LIMIT_Y+BASE_CHAR_HEIGHT*3*2*3,"2- NEW GAME",12,WHITE,BLACK,3,1);
    drawString(SCREEN_WIDTH/2-3*BASE_CHAR_WIDTH*4,CONSOLE_LIMIT_Y+BASE_CHAR_HEIGHT*3*2*4,"3- EXIT",8,WHITE,BLACK,3,1);
    char buf[1];
    buf[0] = 0;
    int ans=0;
    while (1){
        if (readKeyboard(buf, 1)) {
            if (buf[0]=='1' && oldGame){
                ans=0;
                break;
            } else if(buf[0]=='2'){
                ans=1;
                break;
            }  else if (buf[0]=='3'){
                ans=2;
                break;
            }
        }
    }
    return ans;
}

static void drawBoard(){
    int x =SCREEN_WIDTH/2-BOARDDIM/2;
    int y= BOARDDIM/8;
    int movx = x;
    int movy = y;
    uint64_t colorPieces[PIECES_SIZE][PIECES_SIZE];
    for (int i = 0; i < SQUARES; movy+=SQUAREDIM, movx= x ,i++){
        for (int j = 0; j < SQUARES; movx+=SQUAREDIM,j++){
                if ((j+i+1)%2){
                    drawRect(movx,movy,SQUAREDIM,SQUAREDIM,SILVER);
                } else {
                    drawRect(movx,movy,SQUAREDIM,SQUAREDIM,DARK_GRAY);
                }
        }
    }
    int startT=0,endT,startL=0,endL,stepT,stepL;
    switch (rotate) {
    case 0:
        startT=startL=0;
        endL=endT=7;
        stepL=stepT=1;
        break;
    case 1:
        endL=startT=0;
        endT=startL=7;
        stepT=1;
        stepL=-1;
        break;
    case 2:
        startT=startL=7;
        endT=endL=0;
        stepT=stepL=-1;
        break;
    case 3:
        startL=endT=0;
        endL=startT=7;
        stepL=1;
        stepT=-1;
        break;
    }
    //indices cambiar squares con otro indice
    movx = x+SQUAREDIM/2-BASE_CHAR_WIDTH/2;
    movy = y - BASE_CHAR_HEIGHT*2;
    char indix[2][SQUARES+1] = {{"ABCDEFGH"},{"87654321"}};
    for (int i = 0,t = startT; i < SQUARES; i++,t+=stepT){
        drawString(movx,movy,indix[rotate%2]+t,1,WHITE,BACKGROUND,2,0);
        movx+=SQUAREDIM;
    }
    movx = x- BASE_CHAR_WIDTH*3;
    movy = y + SQUAREDIM/2-BASE_CHAR_HEIGHT/2;
    for (int i = 0,l =startL; i < SQUARES; i++,  l+=stepL){
        drawString(movx,movy,indix[(rotate+1)%2]+l,1,WHITE,BACKGROUND,2,0);
        movy+=SQUAREDIM;
    }
    movy=y;
    movx=x;
    if (rotate==0||rotate==2){
        for (int i = 0,t=startT; i < SQUARES; movy+=SQUAREDIM, movx= x ,i++,t+=stepT){
            for (int j = 0,l=startL; j < SQUARES; movx+=SQUAREDIM,j++, l+=stepL){
                if (board[t][l]!=0){
                    loadColorsPieces(colorPieces,board[t][l]);
                    drawMatrix(movx,movy,colorPieces[0],PIECES_SIZE,PIECES_SIZE,SQUAREDIM/PIECES_SIZE);
                }
            }
        }
    } else { 
        for (int i = 0,t=endT; i < SQUARES; movy+=SQUAREDIM, movx= x ,i++,t-=stepT){
            for (int j = 0,l=endL; j < SQUARES; movx+=SQUAREDIM,j++, l-=stepL){
                if (board[l][t]!=0){
                    loadColorsPieces(colorPieces,board[l][t]);
                    drawMatrix(movx,movy,colorPieces[0],PIECES_SIZE,PIECES_SIZE,SQUAREDIM/PIECES_SIZE);
                }
            }
        }
    }
}

static void loadColorsPieces(uint64_t pieceC[][PIECES_SIZE],int j){
    int playern = (j>0)? 1:0;
    int piece = (j>0)? j-1:-j-1;
    uint64_t color[2]= {WHITE,BLACK};
    for (int i = 0; i < PIECES_SIZE; i++){
        for (int k = 0; k < PIECES_SIZE; k++){
            if (pieces[piece][i][k]=='.') {
                pieceC[i][k] = color[playern];
            } else if (pieces[piece][i][k]=='#'){
                pieceC[i][k] = color[(playern+1)%2];
            } else {
                pieceC[i][k] = 0x01000000;
            }
        }
    }
}

static int kingValidMov(int xi,int yi,int xf, int yf){
    if ( iabs(xi-xf) > 1 || iabs(yi-yf) >1 ) return 0;
    return 1;
}

static int bishopValidMov(int xi,int yi,int xf, int yf){
    if (iabs(xi-xf)!=iabs(yi-yf) ) return 0;
    int stepx= iabs(xf-xi)/(xf-xi);
    int stepy= iabs(yf-yi)/(yf-yi);
    for (int i = 0,w=yi+stepy,j=xi+stepx; i < iabs(xi-xf)-1; i++,j+=stepx,w+=stepy) {
        if (board[w][j]!=0) return 0;
    }
    return 1;
}

static int rookValidMov(int xi,int yi,int xf, int yf){
    if ((xi-xf)*(yi-yf)!=0) return 0;
    if (xi-xf==0){
        int start=(yi < yf)?yi:yf;
        int end=(yi < yf)?yf:yi;
        for (int i = start+1; i < end; i++) {
            if (board[i][xi]!=0) return 0;
        }
    } else {
        int start=(xi < xf)?xi:xf;
        int end=(xi < xf)?xf:xi;
        for (int i = start+1; i < end; i++){
            if(board[yi][i]!=0) return 0;
        }
    }
    return 1;
}

static int queenValidMov(int xi,int yi,int xf, int yf){
    if ((xi-xf)==0 || (yf-yi)==0 ){
        return rookValidMov(xi,yi,xf,yf);
    }else if(iabs((xf-xi)/(yf-yi))==1){
        return bishopValidMov(xi,yi,xf,yf);
    }
    return 0;
}

static int knightValidMov(int xi,int yi,int xf, int yf){
    if (iabs(xi-xf)*iabs(yi-yf)!=2) return 0;
    return 1;
}

static int pawnValidMov(int xi,int yi,int xf, int yf){
    if (iabs(xi-xf)>1 || iabs(yi-yf)>2) return 0;
    if (iabs(xi-xf)==1 && xyPassant[0]!=-1 && xyPassant[0]==xf && xyPassant[1]==yf){
        return 1;
    }
    if (board[yf][xf]==0 && xi-xf!=0) return 0;
    if (board[yf][xf]!=0 && xi-xf==0) return 0;
    if (board[yi][xi]>0) {
        if (board[yf][xf]==0 && yf-yi==2 && yi==1){
            xyPassant[0]=xf;
            xyPassant[1]=2;
            xyPassant[2]=turn;
            return 1;
        }
        if (yf-yi!=1) return 0;
    } else if (board[yi][xi]< 0){
        if  (board[yf][xf]==0 && yf-yi==-2 && yi==6){
            xyPassant[0]=xf;
            xyPassant[1]=5;
            xyPassant[2]=turn;
            return 1;
        }
        if (yf-yi!=-1) return 0;
    }
    return 1;
}

static int validMovement(int xi,int yi,int xf, int yf,int perPlayer){
    if (xi < 0 || xi >7 || yi < 0 || yi >7|| xf < 0 || xf >7|| yf < 0 || yf >7) return 0; // error de tamano
    if (perPlayer==0){
        if (turn%2==0 && board[yi][xi]<0) return 0; //error de player
        if (turn%2==1 && board[yi][xi]>0) return 0;
    }
    if (perPlayer==1){
        if (board[yi][xi]>0) return 0;
    } else if (perPlayer==2){
        if (board[yi][xi]<0) return 0;
    }
    if (board[yi][xi]==0) return 0; // no se puede ejecutar este movimiento
    if (board[yi][xi] * board[yf][xf] > 0) return 0;
    switch (iabs(board[yi][xi]))
    {
    case 1:
        return kingValidMov(xi,yi,xf,yf);
    case 2:
        return queenValidMov(xi,yi,xf,yf);
    case 3:
        return bishopValidMov(xi,yi,xf,yf);
    case 4:
        return knightValidMov(xi,yi,xf,yf);
    case 5:
        return rookValidMov(xi,yi,xf,yf);
    case 6:
        return pawnValidMov(xi,yi,xf,yf);
    default:
        break;
    }
    return 0;
}

static void internTimer(){
    if (secondsV[2]!=readSeconds()){
        secondsV[(turn)%2]++;
        char toPrintTimer1[8];
        toPrintTimer1[0] = secondsV[(turn)%2]/3600%10+'0';
        toPrintTimer1[1] = ':';
        toPrintTimer1[2] = secondsV[(turn)%2]%3600/60/10%10+'0';
        toPrintTimer1[3] = secondsV[(turn)%2]%3600/60%10+'0';
        toPrintTimer1[4] = ':';
        toPrintTimer1[5] = secondsV[(turn)%2]%60/10%10+'0';
        toPrintTimer1[6] = secondsV[(turn)%2]%60%10+'0';
        char toPrintTimer2[8];
        toPrintTimer2[0] = secondsV[(turn+1)%2]/3600%10+'0';
        toPrintTimer2[1] = ':';
        toPrintTimer2[2] = secondsV[(turn+1)%2]%3600/60/10%10+'0';
        toPrintTimer2[3] = secondsV[(turn+1)%2]%3600/60%10+'0';
        toPrintTimer2[4] = ':';
        toPrintTimer2[5] = secondsV[(turn+1)%2]%60/10%10+'0';
        toPrintTimer2[6] = secondsV[(turn+1)%2]%60%10+'0';
        int movy = PLAYER_2_PLACE_Y+2*BASE_CHAR_HEIGHT;
        drawRect(PLAYER_12_PLACE_X,movy+(turn%2)*BOARDDIM/2,7*2*BASE_CHAR_WIDTH,2*BASE_CHAR_HEIGHT,BACKGROUND);
        drawString(PLAYER_12_PLACE_X,movy+(turn%2)*BOARDDIM/2,toPrintTimer1,7,WHITE,BLACK,2,1);
        drawRect(PLAYER_12_PLACE_X,movy+((turn+1)%2)*BOARDDIM/2,7*2*BASE_CHAR_WIDTH,2*BASE_CHAR_HEIGHT,BACKGROUND);
        drawString(PLAYER_12_PLACE_X,movy+((turn+1)%2)*BOARDDIM/2,toPrintTimer2,7,WHITE,BLACK,2,1);
        secondsV[2]=readSeconds();
    }
}

static int coorCheck(int x, int y){
    for (int i = 0; i < SQUARES; i++){
        for (int j = 0; j < SQUARES;j++){
            if (validMovement(j,i,x,y,(turn)%2+1)) return 1;
        }
    }
    return 0;
}

static int validCastle(char *buf){
    if (!strcmp("CASTLE S",buffer[turn])){
        if (turn%2){ //turno de blancas
            if (castleMove[turn%2][0]==0 && castleMove[turn%2][1]==0){
                if (board[7][5]==0 && board[7][6]==0){
                    if (!coorCheck(5,7) && !coorCheck(6,7) && !coorCheck(4,7)){
                        board[7][6]=board[7][4];
                        board[7][4]=0;
                        board[7][5]=board[7][7];
                        board[7][7]=0;
                        castleMove[turn%2][0]=1;
                        return 1;
                    }
                }
            } // castlemove == 0 el rey no se movio
        } else {
            if (castleMove[turn%2][0]==0 && castleMove[turn%2][1]==0){
                if (board[0][5]==0 && board[0][6]==0){
                    if (!coorCheck(5,0) && !coorCheck(6,0) && !coorCheck(4,0)){
                        board[0][6]=board[0][4];
                        board[0][4]=0;
                        board[0][5]=board[0][7];
                        board[0][7]=0;
                        castleMove[turn%2][0]=1;
                        return 1;
                    }
                }
            }
        }
    }else if(!strcmp("CASTLE L",buffer[turn])){
        if (turn%2){
            if (castleMove[turn%2][0]==0 && castleMove[turn%2][2]==0){
                if (board[7][2]==0 && board[7][1]==0 && board[7][3]==0){
                    if (!coorCheck(2,7) && !coorCheck(3,7) && !coorCheck(4,7)){
                        board[7][2]=board[7][4];
                        board[7][4]=0;
                        board[7][3]=board[7][0];
                        board[7][0]=0;
                        castleMove[turn%2][0]=1;
                        return 1;
                    }
                }
            }
        } else if (castleMove[turn%2][0]==0 && castleMove[turn%2][2]==0){
            if (board[0][3]==0 && board[0][2]==0 && board[0][1]==0){
                if (!coorCheck(3,0) && !coorCheck(2,0) && !coorCheck(1,0)){
                    board[0][2]=board[0][4];
                    board[0][4]=0;
                    board[0][3]=board[0][0];
                    board[0][0]=0;
                    castleMove[turn%2][0]=1;
                    return 1;
                }
            }
        }
    }
    return 0;
}

static int reverse(int y){
    switch (y){
    case 7:
        return 0;
    case 6:
        return 1;
    case 5:
        return 2;
    case 4:
        return 3;
    case 3:
        return 4;
    case 2:
        return 5;
    case 1:
        return 6;
    case 0:
        return 7;
    default:
        return 8;
        break;
    }
}

static void console(){
    drawGame();
    int movx = 7*BASE_CHAR_WIDTH+CONSOLE_LIMIT_X;
    int l=0;
    int validMove=0;
    int check=0;
    while (1){
        internTimer();
        if (secondsV[1]-secondsV[0]>60 || secondsV[1]>5400){
            endGame(2,1);
            break;
        } else if (secondsV[0]-secondsV[1]>60 || secondsV[0]>5400){
            endGame(1,1);
            break;
        }
        
        uint8_t bufferLength = 1;
        char buf[bufferLength];
        for (int i = 0; i < bufferLength; i++){
            buf[i] = 0;
        }
        if (readKeyboard(buf, bufferLength)) {
            buf[0]= toUpper(buf[0]);
            drawRect(CONSOLE_LIMIT_X,PROMOTE_LOGS,CONSOLE_SIZE_X,(9)*BASE_CHAR_HEIGHT,BLACK);
            if (buf[0]=='P') {
                oldGame =1;
                endGame(-1,0);
                break;
            }
            if (buf[0]=='Q'){
                drawFullConsole(0);
                reDrawChessConsole(buffer);
                continue;
            }
            if (buf[0]=='R'){
                turnBoard();
                drawBoard();
                continue;
            } else if (buf[0]=='\n' || l > MAX_LENGTH){
                l=0;
                int xi = buffer[turn][0]-'A';
                int yi = buffer[turn][1]-'0'-1;
                int xf =buffer[turn][6]-'A';
                int yf = buffer[turn][7]-'0'-1;
                yi = reverse(yi);
                yf = reverse(yf);
                if (validMovement(xi,yi,xf,yf,0) ){
                    int aux =board[yf][xf];
                    board[yf][xf]= board[yi][xi];
                    board[yi][xi] = 0;
                    if ( detectCheck()==(((turn+1)%2)+1)){
                        board[yi][xi]=board[yf][xf];
                        board[yf][xf]=aux;
                        reDrawChessConsole(buffer);
                        drawString(CONSOLE_LIMIT_X,MOVEMENT,"YOU CANNOT CHECK YOURSELF",26,WHITE,BLACK,1,0);
                    } else {
                        validMove=1;
                        if (xyPassant[2]==turn-1){
                            if (yf ==2 && xyPassant[1]==2) {
                                board[yf+1][xf] =0;
                            } else if(yf== 5 && xyPassant[1]==5){
                                board[yf-1][xf] = 0;
                            }
                            xyPassant[0]=xyPassant[1]=xyPassant[2]=-1;
                        }
                        if (aux){
                            lastEat=turn;
                        }
                        if(iabs(board[yf][xf])==KING){
                            castleMove[turn%2][0]=1;
                        }else if ((board[yf][xf])==ROOK){
                            if(xi==7){
                                castleMove[turn%2][2]=1;
                            } else{
                                castleMove[turn%2][2]=1;
                            }
                        }else if(board[yf][xf]==-ROOK){
                            if(xi==7){
                                castleMove[turn%2][1]=1;
                            } else{
                                castleMove[turn%2][1]=1;
                            }
                        }
                        if (aux){
                            drawString(CONSOLE_LIMIT_X,CONSOLE_LIMIT_Y+CONSOLE_SIZE_Y+8*BASE_CHAR_HEIGHT,figures[iabs(board[yf][xf])-1],strlen(figures[iabs(board[yf][xf])-1]),WHITE,BLACK,1,0);
                            drawString(CONSOLE_LIMIT_X+(strlen(figures[iabs(board[yf][xf])-1]))*BASE_CHAR_WIDTH,CONSOLE_LIMIT_Y+CONSOLE_SIZE_Y+8*BASE_CHAR_HEIGHT," eats ",6,WHITE,BLACK,1,0);     
                            drawString(CONSOLE_LIMIT_X+(strlen(figures[iabs(board[yf][xf])-1])+6)*BASE_CHAR_WIDTH,CONSOLE_LIMIT_Y+CONSOLE_SIZE_Y+8*BASE_CHAR_HEIGHT,figures[iabs(aux)-1],strlen(figures[iabs(aux)-1]),WHITE,BLACK,1,0);
                        }
                        if (iabs(aux)==KING) {
                            endGame((turn+1)%2+1,2);
                            break;
                        }
                        drawBoard();
                        if((board[yf][xf]==6 && yf==7) || (board[yf][xf] == -6 && yf==0)){
                            promotePiece(xf,yf);
                            drawBoard();
                        }
                        turn=(turn+1);
                        reDrawChessConsole(buffer);
                    }
                } else if (validCastle(buffer[turn])){
                    drawBoard();
                    turn=(turn+1)%TOTAL_LINES_CHESS;
                    reDrawChessConsole(buffer);
                    validMove=1;
                }else {
                    drawRect(CONSOLE_LIMIT_X,PROMOTE_LOGS,CONSOLE_SIZE_X,(8)*BASE_CHAR_HEIGHT,BLACK);
                    drawString(CONSOLE_LIMIT_X,MOVEMENT,"INVALID MOVEMENT",17,WHITE,BLACK,1,0);
                    for (int i=0;i<CONSOLE_SIZE_X/BASE_CHAR_WIDTH-2;i++) {
                        buffer[turn][i]=0;
                    }
                    reDrawChessConsole(buffer);
                }
            } else if (buf[0]=='\b'){
                if (l>0){
                    l--;
                    buffer[turn][l]=0;
                    drawRect(movx+l*BASE_CHAR_WIDTH,CONSOLE_LIMIT_Y+CONSOLE_SIZE_Y-BASE_CHAR_HEIGHT,BASE_CHAR_WIDTH,BASE_CHAR_HEIGHT,0);
                }
            }else {
                drawString(movx+l*BASE_CHAR_WIDTH,CONSOLE_LIMIT_Y+CONSOLE_SIZE_Y-BASE_CHAR_HEIGHT,buf,1,WHITE,BLACK,1,0);
                buffer[turn][l++] = buf[0];
            }
            if (iabs(lastEat-turn)>=50){
                endGame(0,4);
                break;
            }
            if (noMoreMoves()){
                endGame(0,3);
                break;
            }
            if  (detectCheckMate()) {
                endGame((detectCheck()-1)?1:2,2);
                break;
            }
            if (fewPieces()){
                endGame(0,2);
                break;
            }
            int drowned = drownedKing();
            if (drowned){
                endGame(0,1);
                break;
            }
            check =detectCheck();
            if (check==1){
                    drawString(CONSOLE_LIMIT_X,MOVEMENT+BASE_CHAR_HEIGHT,"PLAYER 1 IS ON CHECK",21,WHITE,BLACK,1,0);
                } else if(check==2){
                    drawString(CONSOLE_LIMIT_X,MOVEMENT+BASE_CHAR_HEIGHT,"PLAYER 2 IS ON CHECK",21,WHITE,BLACK,1,0);
            } else {
                drawRect(CONSOLE_LIMIT_X,PROMOTE_LOGS+9*BASE_CHAR_HEIGHT,CONSOLE_SIZE_X,BASE_CHAR_HEIGHT,BLACK);
            }
            if (validMove){
                validMove=0;
            }
        }
    }
} 

static void promotePiece(int x,int y){
    drawString(CONSOLE_LIMIT_X,PROMOTE_LOGS,"you can promote your",21,WHITE,1,1,1);
    drawString(CONSOLE_LIMIT_X,PROMOTE_LOGS+BASE_CHAR_HEIGHT,"pawn choose between:",21,WHITE,1,1,1);
    for (int i =1;i<PIECES_AMOUNT-1;i++){
        drawString(CONSOLE_LIMIT_X,PROMOTE_LOGS+BASE_CHAR_HEIGHT*(i+1),figures[i],strlen(figures[i]),WHITE,1,1,0);
        char pieceNumber[2];
        intToBase(i,10,pieceNumber);
        drawString(CONSOLE_LIMIT_X+BASE_CHAR_WIDTH*10,PROMOTE_LOGS+BASE_CHAR_HEIGHT*(i+1),pieceNumber,1,WHITE,1,1,0);
    }
    while(1){
        uint8_t bufferLength = 1;
        char buf[bufferLength];
        for (int i = 0; i < bufferLength; i++){
            buf[i] = 0;
        }
        int team = (board[y][x]>0)?1:-1;
        if (readKeyboard(buf, bufferLength)) {
            if (buf[0]-'0' < 5 &&  buf[0]-'0'>0){
                board[y][x] = (buf[0]-'0'+1)*team;
                break;
            }
        }
    }
    drawRect(CONSOLE_LIMIT_X,PROMOTE_LOGS,CONSOLE_SIZE_X,10*BASE_CHAR_HEIGHT,BLACK);
}

static void endGame(int winner, int by){
    int movx=0,movy=0;
    if (winner>=0){
        oldGame=0;
    }
    for (int i = 0; i < SCREEN_HEIGHT/SQUAREDIM; movy+=SQUAREDIM, movx= 0 ,i++){
        for (int j = 0; j < SCREEN_WIDTH/SQUAREDIM; movx+=SQUAREDIM,j++){
            if ((j+i)%2){
                drawRect(movx,movy,SQUAREDIM,SQUAREDIM,SILVER);
            } else {
                drawRect(movx,movy,SQUAREDIM,SQUAREDIM,DARK_GRAY);
            }
        }
    }
    if (winner>0){
        char winnerC = (char)winner+'0';
        drawString(SCREEN_WIDTH/2-7*4*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2-2*BASE_CHAR_HEIGHT,"THE WINNER IS",14,BACKGROUND,BLACK,4,1);
        drawString(SCREEN_WIDTH/2-4*4*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2+BASE_CHAR_HEIGHT*4-2*BASE_CHAR_HEIGHT,"PLAYER :",9,BACKGROUND,BLACK,4,1);
        drawString(SCREEN_WIDTH/2-4*4*BASE_CHAR_WIDTH+BASE_CHAR_WIDTH*4*9,SCREEN_HEIGHT/2+BASE_CHAR_HEIGHT*4-2*BASE_CHAR_HEIGHT,&winnerC,1,BACKGROUND,BLACK,4,1);
        switch (by)
        {
        case 1:
            drawString(SCREEN_WIDTH/2-4*4*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2-4*3*BASE_CHAR_HEIGHT,"TIMEOUT",8,BACKGROUND,BLACK,4,1);
            break;
        case 2:
            drawString(SCREEN_WIDTH/2-5*4*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2-4*3*BASE_CHAR_HEIGHT,"CHECK MATE",11,BACKGROUND,BLACK,4,1);
            break;
        default:
            break;
        }
    } else if (winner==0){
        drawString(SCREEN_WIDTH/2-2*4*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2-2*BASE_CHAR_HEIGHT,"DRAW",4,BACKGROUND,BLACK,4,1);
        switch (by)
        {
        case 1:
            drawString(SCREEN_WIDTH/2-5*3*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2+BASE_CHAR_HEIGHT*4*2-2*BASE_CHAR_HEIGHT,"STALEMATE",10,BACKGROUND,BLACK,3,1);
            break;
        case 2:
            drawString(SCREEN_WIDTH/2-11*3*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2+BASE_CHAR_HEIGHT*4*2-2*BASE_CHAR_HEIGHT,"INSUFFICIENT MATERIAL",22,BACKGROUND,BLACK,3,1);
            break;
        case 3:
            drawString(SCREEN_WIDTH/2-14*3*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2+BASE_CHAR_HEIGHT*4*2-2*BASE_CHAR_HEIGHT,"TOP AMOUNT OF MOVES REACHED",28,BACKGROUND,BLACK,3,1);
            break;
        case 4:
            drawString(SCREEN_WIDTH/2-15*3*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2+BASE_CHAR_HEIGHT*4*2-2*BASE_CHAR_HEIGHT,"50 MOVES REACHED WHITOUT EATS",30,BACKGROUND,BLACK,3,1);
        default:
            break;
        }
    } else {
        clearScreen(0);
        drawString(SCREEN_WIDTH/2-2*4*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2-2*BASE_CHAR_HEIGHT,"PAUSE",5,BACKGROUND,BLACK,4,1);
        drawString(SCREEN_WIDTH/2-15*2*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2+BASE_CHAR_HEIGHT*4*2-2*BASE_CHAR_HEIGHT,"TO RESUME THE GAME PRESS [ P ]",31,BACKGROUND,BLACK,2,1);
    }
    drawString(SCREEN_WIDTH/2-16*2*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2+BASE_CHAR_HEIGHT*4*3-2*BASE_CHAR_HEIGHT,"TO SEE THE GAME LOG PRESS [ Q ]",32,BACKGROUND,BLACK,2,1);
    drawString(SCREEN_WIDTH/2-20*2*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2+BASE_CHAR_HEIGHT*4*4-2*BASE_CHAR_HEIGHT,"TO SEE GO TO THE MAIN MENU PRESS [ M ]",39,BACKGROUND,BLACK,2,1);
    drawString(SCREEN_WIDTH/2-12*2*BASE_CHAR_WIDTH,SCREEN_HEIGHT/2+BASE_CHAR_HEIGHT*4*5-2*BASE_CHAR_HEIGHT,"PRESS [ X ] KEY TO EXIT",24,BACKGROUND,BLACK,2,1);
    while(1){
        uint8_t bufferLength = 1;
        char buf[bufferLength];
        for (int i = 0; i < bufferLength; i++){
            buf[i] = 0;
        }
        if (readKeyboard(buf, bufferLength)) {
            buf[0]=toUpper(buf[0]);
            if (buf[0]=='Q'){
                if (winner==-1){
                    drawFullConsole(2);
                } else {
                    drawFullConsole(1);
                    endGame(winner,by);
                }
                return;
            } else if( buf[0]=='M'){
                chess();
                return;
            } else if (buf[0]=='X'){
                myExit();
            }
            if (buf[0]=='P'){
                console();
            }
            
        }
    }
}

static int detectCheckMate(){
    int team = detectCheck();
    int saveBoard[SQUARES][SQUARES] ={{0}};
    for (int i = 0; i < SQUARES; i++){
        for (int j = 0; j < SQUARES;j++){
            saveBoard[i][j]=board[i][j];
        }
    }    
    if (team==1){
        for (int i = 0; i < SQUARES; i++){
            for (int j = 0; j < SQUARES;j++){
                if (board[i][j]<0 ){
                    for (int k = 0; k < SQUARES; k++){
                        for (int l = 0; l < SQUARES;l++){   
                            if (validMovement(j,i,l,k,1) ){
                                board[k][l]=board[i][j];
                                board[i][j]=0;
                                if (detectCheck()){
                                    board[k][l]= saveBoard[k][l];
                                    board[i][j]= saveBoard[i][j];
                                } else {
                                    board[k][l]= saveBoard[k][l];
                                    board[i][j]= saveBoard[i][j];
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
        } 
        return 1;
    }else if (team==2){
        for (int i = 0; i < SQUARES; i++){
            for (int j = 0; j < SQUARES;j++){
                if (board[i][j]>0){
                    for (int k = 0; k < SQUARES; k++){
                        for (int l = 0; l < SQUARES;l++){ 
                            if (validMovement(j,i,l,k,2) ){
                                board[k][l]=board[i][j];
                                board[i][j]=0;
                                if (detectCheck()){
                                    board[k][l]= saveBoard[k][l];
                                    board[i][j]= saveBoard[i][j];
                                } else {
                                    board[k][l]= saveBoard[k][l];
                                    board[i][j]= saveBoard[i][j];
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        return 2;
    }
    return 0;
    
}

static int detectCheck(){
    int x1,y1,x2,y2;
    for (int i = 0; i < SQUARES; i++){
        for (int j = 0; j < SQUARES;j++){
            if (board[i][j]==KING){
                y2=i;
                x2=j;
            } else if (board[i][j]==-KING){
                y1=i;
                x1=j;
            }
        }
    }
    for (int i = 0; i < SQUARES; i++){
        for (int j = 0; j < SQUARES;j++){
            if (validMovement(j,i,x1,y1,-1)) return 1;
            if (validMovement(j,i,x2,y2,-1)) return 2;
        }
    }
    return 0;
}

static int fewPieces(){
    int sum=0;
    for (int i =0 ;i<SQUARES;i++){
        for (int j =0 ;j <SQUARES;j++){
            if (board[i][j]){
                sum++;
            }
        }
    }
    if (sum==2)return 1;
    if (sum==3){
        for (int i =0 ;i<SQUARES;i++){
            for (int j =0 ;j <SQUARES;j++){
                if (iabs(board[i][j])==KNIGHT || iabs(board[i][j])==BISHOP){
                    return 1;
                }
            }
        }
    }
    return 0;
}

static int noMoreMoves(){
    if (turn >= TOTAL_LINES_CHESS){
        return 1;
    }
    return 0;
}

static int drownedKing(){
    int team = detectCheck();
    int saveBoard[SQUARES][SQUARES] ={{0}};
    for (int i = 0; i < SQUARES; i++){
        for (int j = 0; j < SQUARES;j++){
            saveBoard[i][j]=board[i][j];
        }
    }    
    if (team) return 0;
    if (((turn+1)%2+1)==1){
        for (int i = 0; i < SQUARES; i++){
            for (int j = 0; j < SQUARES;j++){
                if (board[i][j]<0 ){
                    for (int k = 0; k < SQUARES; k++){
                        for (int l = 0; l < SQUARES;l++){   
                            if (validMovement(j,i,l,k,0) ){
                                board[k][l]=board[i][j];
                                board[i][j]=0;
                                if (detectCheck()){
                                    board[k][l]= saveBoard[k][l];
                                    board[i][j]= saveBoard[i][j];
                                } else {
                                    board[k][l]= saveBoard[k][l];
                                    board[i][j]= saveBoard[i][j];
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
        } 
        return 2;
    }else if (((turn+1)%2+1)==2){
        for (int i = 0; i < SQUARES; i++){
            for (int j = 0; j < SQUARES;j++){
                if (board[i][j]>0){
                    for (int k = 0; k < SQUARES; k++){
                        for (int l = 0; l < SQUARES;l++){ 
                            if (validMovement(j,i,l,k,0) ){
                                board[k][l]=board[i][j];
                                board[i][j]=0;
                                if (detectCheck()){
                                    board[k][l]= saveBoard[k][l];
                                    board[i][j]= saveBoard[i][j];
                                } else {
                                    board[k][l]= saveBoard[k][l];
                                    board[i][j]= saveBoard[i][j];
                                    return 0;
                                }
                            }
                        }
                    }
                }
            }
        }
        return 1;
    }
    return 0;
}

static void reDrawChessConsole(char buffer[][CONSOLE_SIZE_X/BASE_CHAR_WIDTH-2]){
    int x= CONSOLE_LIMIT_X;
    int y = CONSOLE_LIMIT_Y+CONSOLE_SIZE_Y-BASE_CHAR_HEIGHT*2;
    char player[1] = {(turn+1)%2+'1'};
    drawRect(CONSOLE_LIMIT_X,CONSOLE_LIMIT_Y,CONSOLE_SIZE_X,CONSOLE_SIZE_Y,BLACK);
    drawString(CONSOLE_LIMIT_X,CONSOLE_LIMIT_Y+CONSOLE_SIZE_Y-BASE_CHAR_HEIGHT,"P  |> ",7,WHITE,BLACK,1,0);
    drawString(CONSOLE_LIMIT_X+BASE_CHAR_WIDTH,CONSOLE_LIMIT_Y+CONSOLE_SIZE_Y-BASE_CHAR_HEIGHT,player,1,WHITE,BLACK,1,0);
    for (int i = turn,j=0; j<TOTAL_LINES_CHESS/10; i--,j++){
        if (i==turn){
            drawString(x,y,buffer[i%TOTAL_LINES_CHESS],MAX_LENGTH,RED,BLACK,1,1);
        }else {
            drawString(x,y,buffer[i%TOTAL_LINES_CHESS],MAX_LENGTH,WHITE,BLACK,1,1);
        }
        y-=BASE_CHAR_HEIGHT;
    }
}

static void drawFullConsole(int end){
    clearScreen(0x80000);
    int x =PLAYER_12_PLACE_X,y=64;
    for (int i= turn,j=0; j < turn;i--,j++){
        drawString(x,y,buffer[i%TOTAL_LINES_CHESS],MAX_LENGTH,WHITE,BLACK,1,1);
        y+=BASE_CHAR_HEIGHT;
        if ( ((j+1)%30)==0) {
            x+=CONSOLE_SIZE_X;
            y=SQUAREDIM;
        }
    }
    if (end){
        drawString(SCREEN_WIDTH/2-(20)*BASE_CHAR_WIDTH*3,PROMOTE_LOGS+3*BASE_CHAR_HEIGHT,"PRESS ANY KEY TO GO BACK TO THE END MENU",41,WHITE,BLACK,3,1);
    }else {
       drawString(SCREEN_WIDTH/2-(37/2)*BASE_CHAR_WIDTH*3,PROMOTE_LOGS+3*BASE_CHAR_HEIGHT,"PRESS ANY KEY TO GO BACK TO THE GAME",37,WHITE,BLACK,3,1);
    }
    while(1){
        uint8_t bufferLength = 1;
        char buf[bufferLength];
        for (int i = 0; i < bufferLength; i++){
            buf[i] = 0;
        }
        if (readKeyboard(buf, bufferLength)) {
            break;
        }
    }
    if (end==0 ){
        drawGame();
    }
    else if (end==2){
        endGame(-1,0);
    }
}

static void drawHelp(){
    int x=PLAYER_12_PLACE_X,y=PROMOTE_LOGS;
    drawString(x,y,"HELP",4,WHITE,BLACK,2,1);
    y+=2*BASE_CHAR_HEIGHT;
    drawString(x,y,"THE MOVEMENTS FORMAT IS \"LN TO LN\"",35,WHITE,BLACK,1,1);
    y+=BASE_CHAR_HEIGHT;
    drawString(x,y,"YOU CAN CASTLE WRITTING \"CASTLE L\" OR \"CASTLE S\" (L = LARGE | S = SHORT)",73,WHITE,BLACK,1,1);
    y+=BASE_CHAR_HEIGHT;
    drawString(x,y,"PRESS [ Q ] TO SEE THE COMPLETE LOG",36,WHITE,BLACK,1,1);
    y+=BASE_CHAR_HEIGHT;
    drawString(x,y,"PRESS [ P ] TO PAUSE THE GAME",30,WHITE,BLACK,1,1);
    y+=BASE_CHAR_HEIGHT;
    drawString(x,y,"PRESS [ R ] TO ROTATE THE BOARD",32,WHITE,BLACK,1,1);
}

static void drawGame(){
    clearScreen(BACKGROUND);
    drawString(PLAYER_12_PLACE_X,PLAYER_1_PLACE_Y,"Player 1",strlen("player 1"),WHITE,BLACK,2,1);
    drawString(PLAYER_12_PLACE_X,PLAYER_2_PLACE_Y,"Player 2",strlen("player 2"),WHITE,BLACK,2,1);
    drawBoard();
    drawHelp();
    reDrawChessConsole(buffer);
    drawRect(CONSOLE_LIMIT_X,PROMOTE_LOGS,CONSOLE_SIZE_X,10*BASE_CHAR_HEIGHT,BLACK);
}
