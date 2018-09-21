#include<stdio.h>
#include<math.h>

#define TRUE 1
#define FALSE 0
#define MAXVAL 1000
#define MINVAL -1000

// Function to print the tic toe board
void showBoard(char board[][3])
{
    printf("\n\n");
     
    printf("\t\t\t  %c | %c  | %c  \n", board[0][0],
                             board[0][1], board[0][2]);
    printf("\t\t\t--------------\n");
    printf("\t\t\t  %c | %c  | %c  \n", board[1][0],
                             board[1][1], board[1][2]);
    printf("\t\t\t--------------\n");
    printf("\t\t\t  %c | %c  | %c  \n\n", board[2][0],
                             board[2][1], board[2][2]);
  
    return;
}

struct move{
    int row, col;
};

// Function to check if the game has ended
int isgameover(char board[][3])
{
    for(int i = 0; i < 3; i++)
    {
        if(board[i][0] == 'x' && board[i][1] == 'x' && board[i][2] == 'x') return 1;
        if(board[i][0] == 'o' && board[i][1] == 'o' && board[i][2] == 'o') return 2;
        if(board[0][i] == 'x' && board[1][i] == 'x' && board[2][i] == 'x') return 1;
        if(board[0][i] == 'o' && board[1][i] == 'o' && board[2][i] == 'o') return 2;
    }

    if(board[0][0] == 'x' && board[1][1] == 'x' && board[2][2] == 'x') return 1;
    if(board[0][0] == 'o' && board[1][1] == 'o' && board[2][2] == 'o') return 2;
    if(board[2][0] == 'x' && board[1][1] == 'x' && board[0][2] == 'x') return 1;
    if(board[2][0] == 'o' && board[1][1] == 'o' && board[0][2] == 'o') return 2;

    int count = 0;
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++)
            if (board[i][j]=='x' || board[i][j]=='o')
                count++;
    if(count==9) return 3;
    return 0;
}


int notmax(int isMAX){
    return isMAX==FALSE?TRUE:FALSE;
}

// Function to find the complementary symbol
char complement_sym(char sym){
    return sym=='o'?'x':'o';
}

int max(int num1, int num2)
{
    return (num1 > num2 ) ? num1 : num2;
}

int min(int num1, int num2) 
{
    return (num1 > num2 ) ? num2 : num1;
}

// Function to find the value function after the given number of lookahead steps 
int calcscore(char board[3][3], char own)
{   
    char turn_symb = complement_sym(own);
    int owncount=0, oppocount=0;
    int i=0;
    for(i=0;i<3;i++)
    {
        if(board[i][0]!=turn_symb && board[i][1]!=turn_symb && board[i][2]!=turn_symb)
            owncount++;
        if(board[0][i]!=turn_symb && board[1][i]!=turn_symb && board[2][i]!=turn_symb)
            owncount++;
        if(board[i][0]!=own && board[i][1]!=own && board[i][2]!=own)
            oppocount++;
        if(board[0][i]!=own && board[1][i]!=own && board[2][i]!=own)
            oppocount++;
    }
    if(board[0][0]!=turn_symb && board[1][1]!=turn_symb && board [2][2]!=turn_symb)
        owncount++;
    if(board[0][0]!=own && board[1][1]!=own && board [2][2]!=own)
        oppocount++;
    if(board[0][2]!=turn_symb && board[1][1]!=turn_symb && board[2][0]!=turn_symb)
        owncount++;
    if(board[0][2]!=own && board[1][1]!=own && board[2][0]!=own)
        oppocount++;
    return owncount-oppocount;       
}


// Function to recursively find the minimax 
int minimax(char board[3][3], int depth, int isMax, int lookahead, char sym, char my_sym)
{
    
    if (depth == lookahead || isgameover(board)==3) 
        return calcscore(board, my_sym);
    else if (isMax == TRUE)              // This section runs whenever we are in maxnode
    {
        int best = MINVAL;
        int break_flag=0;
        for (int i = 0; i<3; i++){
            for (int j = 0; j<3; j++)
                if (board[i][j] == '_')
                {
                    board[i][j] = sym;
                    if(isgameover(board)==1 || isgameover(board)==2){
                        best = MAXVAL;
                        break_flag=1;
                    }
                    else
                        best = max( best, minimax(board, depth+1, notmax(isMax), lookahead, complement_sym(sym),my_sym));
                    board[i][j] = '_'; 
                }
                if(break_flag)break;
            }
        return best;
    }
    else                                  // This section runs whenever we are in minnode
    {
        int best = MAXVAL;
        int break_flag=0;
        for (int i = 0; i<3; i++){
            for (int j = 0; j<3; j++)
                if (board[i][j] == '_')
                {
                    board[i][j] = sym;
                    if(isgameover(board)==1 || isgameover(board)==2){
                        best = MINVAL;
                        break_flag=1;
                    }
                    else
                        best = min( best, minimax(board, depth+1,  notmax(isMax), lookahead, complement_sym(sym),my_sym));
                    board[i][j] = '_';
                }
                if(break_flag)break;
        }
        return best;
    }
}

// Function to find the best move given the board configuration
struct move find_bestmove(char board[3][3], char sym, int lookahead, char my_sym){
    int bestVal = MINVAL;
    struct move bestMove;
    bestMove.row = -1;
    bestMove.col = -1;
    int break_flag = 0;
    int moveVal;
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++){
            if (board[i][j]=='_')
            {
                board[i][j] = sym;
                if(isgameover(board)==1 || isgameover(board)==2){
                        moveVal = MAXVAL;
                        break_flag=1;
                    }else{ 
                        moveVal = minimax(board, 1, FALSE, lookahead, complement_sym(sym), my_sym);
                    }
                board[i][j] = '_';
                if (moveVal > bestVal)
                {
                    bestMove.row = i;
                    bestMove.col = j;
                    bestVal = moveVal;
                }
            }
            if(break_flag)break;
        }
    return bestMove;
}

// Main Function

int main(){
    char board[3][3];
    char next_sym,sym;
    int lookahead=6;
    int x_count=0,o_count=0,choice;
    int end=0;
    int x_in,y_in;
    printf("\nARNAB MONDAL(14EC35031)");
    printf("\n***********************");
    printf("\nDo you want to play a game with the computer or to find the next best move given a state:\n");
    printf("1. Play a game\n");
    printf("2. Give a state to find the next best move\n");
    printf("Enter your choice:");
    scanf("%d",&choice);
    printf("\nEnter the value of lookahead:");
    scanf("%d",&lookahead);
    // Code segment to play the game dynamically
    if(choice==1){
     char board[3][3] =
        {
            { '_', '_', '_' },
            { '_', '_', '_' },
            { '_', '_', '_' }
        };
     printf("\n Current state of board: \n");
     showBoard(board);
     char ch;
     printf("\nDo you want computer to start:(y/n):");
     scanf(" %c",&ch);
     if(ch=='y'){
        sym='o';
        struct move bestmove = find_bestmove(board,complement_sym(sym),lookahead,complement_sym(sym));
     board[bestmove.row][bestmove.col]=complement_sym(sym);
     printf("\n Current state of board: \n");
     showBoard(board);
     }else{
        sym='x';
     }

     while(!end){
      printf("Your Turn"); 
      printf("\nEnter x position(1-3):");
      scanf("%d",&x_in);
      printf("Enter y position(1-3):");
      scanf("%d",&y_in); 
      board[x_in-1][y_in-1]=sym;
      printf("\n Current state of board: \n");
      showBoard(board);
      struct move bestmove = find_bestmove(board,complement_sym(sym),lookahead,complement_sym(sym));
      board[bestmove.row][bestmove.col]=complement_sym(sym);
      printf("\n Current state of board: \n");
      showBoard(board);
      if(isgameover(board)!=0){
        end=1;
        //printf("%c%d\n",sym,isgameover(board) );
        if((isgameover(board)==1 && sym=='x')||(isgameover(board)==2 && sym=='o')){
            printf("Congrats...You won the game.\n");
        }else if((isgameover(board)==1 && sym=='o')||(isgameover(board)==2 && sym=='x')){
            printf("OOppps...You Lost.\n");
        }else if(isgameover(board)==3){
            printf("Draw :( .\n");
        }
     }
    }
    }
    // Code segment to predict the next best move
    if(choice==2){
     printf("\n\nEnter the table(Elements: o, x, _ )\n\n");
     for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++){
          printf("Enter element(%d,%d):",i+1,j+1);
          scanf(" %c", &board[i][j]);
        } 
    printf("\n The state of board you entered \n");
    showBoard(board);
    for (int i = 0; i<3; i++)
        for (int j = 0; j<3; j++){
            if (board[i][j]=='x')
                x_count++;
            else if(board[i][j]=='o')
                o_count++;
        }
    if(abs(x_count-o_count)>1){
        printf("\nEnter a valid board state.\n");
    }else{ 
    if(x_count > o_count)
        next_sym='o';
    else 
        next_sym='x';
     struct move bestmove = find_bestmove(board,next_sym,lookahead,next_sym);
     board[bestmove.row][bestmove.col]=next_sym;
     printf("\n The best possible move at this point of time. \n");
     showBoard(board);
     }
    }
    
}