#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <string.h>


#define MAXQ 1000

#define MAXgenerations 1000
#define MAXindividuals 100

#define FALSE 0
#define TRUE  1

#define ABS(a) ((a) < 0 ? (-(a)) : (a))

int nqueens;      /* number of queens global metavliti */
int nqueens_array[MAXQ]; /* queen at (r,c) = queens[r] == c */

typedef struct
{
	int id;
	int i;
	int j;
} queens;

int QeensInDanger(int n, int array[n][n] );
int DiagonalSearch(int n, int array[][n], int k, int l);
void foo(int n);
void hillClimbing(int h, int n, int board[][n], int cycles);
/*****************/
void initializeRandomGenerator() ;
void initiateQueens(int flag) ;
int inConflict(int row0, int column0, int row1, int column1);
int inConflictWithAnotherQueen(int row, int col) ;
void printState() ;
int countConflicts() ;
int countConflictsArg(int ind[MAXQ]) ;
int evaluateState() ;
int evaluateStateArg(int ind[MAXQ]) ;
int pickRandom();
void newIndividual(int gen[MAXindividuals][MAXQ], int son1[MAXQ], int son2[MAXQ]);
void insertInOrder(int evaluations[MAXindividuals], int gen[MAXindividuals][MAXQ], int ind[MAXQ], int size);
void geneticAlgorithm();

int main(){
	
	srand(time(NULL));

	int n,algo;
	char again;
	
	do{
		printf("Give the dimension n: ");
		scanf("%d", &nqueens);

		printf("\nChoose Alorithm to run:\n");
		printf("\n1)Hill Climbing  2)Genetic Algorithm\n");
		scanf("%d", &algo);
		switch (algo) {
 				 case 1: foo(nqueens);       break;
  					case 2: geneticAlgorithm();	break;
  					}
		

			printf("\n\nTry again with other n?(y/n): ");
		do{
			scanf("\n%s", &again);
			if( again != 'y' && again != 'n' )
				printf("Please answer with (y/n): ");
		}while(!(again == 'y' || again == 'n'));
	}while(again != 'n');
	return 0;
}


void initializeRandomGenerator() {
  /* this routine initializes  random generator. 
   */
  time_t t;
  srand((unsigned) time(&t));
}

/* Generate an initial position.
 *  flag == 0,tote gia ka8e grammi mia vassilisa topo8etite stin proti stili .
 *  flag == 1,tote gia ka8e grammi mia vassilisa topo8etite se tixea stili .
 */
void initiateQueens(int flag) {
  int q;
  for (q = 0; q < nqueens; q++) {
    nqueens_array[q] = (flag == 0? 0 : rand()%nqueens); 
  }
}

/* epistrefi True ean i 8esi   (row0,column0) ine se
 * conflict me (row1,column1), allios FALSE.
 */
int inConflict(int row0, int column0, int row1, int column1) {
  if (row0 == row1) return TRUE; /*idia grammi */
  if (column0 == column1) return TRUE; /* stili, */
  if (ABS(row0-row1) == ABS(column0-column1)) return TRUE;/* diagonia */
  return FALSE; /* no conflict */
}

/* epistrefi TRUE ean in 8esi (row,col) ine se
 * conflict me opiadipote alli vasilisa sti skakiera , alios FALSE.
 */
int inConflictWithAnotherQueen(int row, int col) {
  int queen;
  for (queen=0; queen < nqueens; queen++) {
    if (inConflict(row, col, queen, nqueens_array[queen])) {
      if ((row != queen) || (col != nqueens_array[queen])) return TRUE;
    }
  }
  return FALSE;
}

/* print board on screen Q kefaleo ean exi conflict q mikro ean oxi */
void printState() {
  int row, column; 
  printf("\n");
  for(row = 0; row < nqueens; row++) {
    for(column = 0; column < nqueens; column++) {
      if (nqueens_array[row] != column) {
        printf (".");
      } else {
        if (inConflictWithAnotherQueen(row, column)) {
          printf("Q");
        } else {
          printf("q");
        }
      }
    }
    printf("\n");
  }
}




/* epistrefi ton ari8mon zeugarion vasilison pou ine se conflict */
int countConflicts() {
  int cnt = 0;
  int queen, other;
  for (queen=0; queen < nqueens; queen++) {
    for (other=queen+1; other < nqueens; other++) {
      if (inConflict(queen, nqueens_array[queen], other, nqueens_array[other])) {
        cnt++;
      }
    }
  }
  return cnt;
}

int countConflictsArg(int ind[MAXQ]) {
  int cnt = 0;
  int queen, other;
  for (queen=0; queen < nqueens; queen++) {
    for (other=queen+1; other < nqueens; other++) {
      if (inConflict(queen, ind[queen], other, ind[other])) {
        cnt++;
      }
    }
  }
  return cnt;
}

/* evaluation function. O megistos ari8mos vasilison pou ine se conflict
 * mpori na ine  1 + 2 + 3 + 4 + .. + (nquees-1)=(nqueens-1)*nqueens/2.
 * ke ka8os emis 8eloume au3ousa local searches i evaluation
 * function epistrefi (nqueens-1)*nqueens/2 - countConflicts().
 */
int evaluateState() {
  return (nqueens-1)*nqueens/2 - countConflicts();
}

int evaluateStateArg(int ind[MAXQ]) {
  return (nqueens-1)*nqueens/2 - countConflictsArg(ind);
}



/*************************************************************/


int pickRandom(){
	for (int i = 0; i<MAXindividuals; i++){
		if ((MAXindividuals - i) / 3 > (rand()%MAXindividuals)/2){
			return i;
		}
	}
	return MAXindividuals;
}

void newIndividual(int gen[MAXindividuals][MAXQ], int son1[MAXQ], int son2[MAXQ]){
	////////crossover - kovoume tous progonous se ena tixeo simio ke ta enonoume mazi  ///////////////
	int father = pickRandom();
	int mother;
	do{
		mother = pickRandom();
	}while(mother ==father);
	//printf("father is %d mother is %d\n", father, mother);
	
	int pos;
	for (pos = 0; pos<father; pos++){
		son1[pos] = gen[father][pos];
		son2[pos] = gen[mother][pos];
	}
	for (pos; pos<MAXQ; pos++){
		son1[pos] = gen[mother][pos];
		son2[pos] = gen[father][pos];
	}
	///////mutation - antalazoume mia stili gia mia grammi ///////////////
	son1[rand()%nqueens] = rand()%nqueens;
	son2[rand()%nqueens] = rand()%nqueens;

}

void insertInOrder(int evaluations[MAXindividuals], int gen[MAXindividuals][MAXQ], int ind[MAXQ], int size){
	int eval = evaluateStateArg(ind);
	int i;
	for (i = 0; i<size; i++){
		if(eval > evaluations [i]){
			for (int h = size; h >= i; h--){		// vre8ike meros ke ta kanome olla shift kata 1
				evaluations[h] = evaluations[h-1];
				for (int j = 0; j < nqueens; j++){
					gen[h][j] = gen [h-1][j];
				}
			}
			break;
		}
	}
	
	for(int x = 0; x<nqueens; x++){
		gen[i][x] = ind[x];
	}
	evaluations[i] = eval;
	return;
}

void geneticAlgorithm(){
	int generations = 0;
	int optimum = (nqueens-1)*nqueens/2;
	
	//printf("optimum is %d\n", optimum);
	printf("Doing Science wait...\n");
	
	int newGen[MAXindividuals][MAXQ];
	int currentGen[MAXindividuals][MAXQ];
	int evaluations[MAXindividuals];
	int son1[MAXQ];
	int son2[MAXQ];
	
	while (evaluateState() != optimum) {
		//printf("generation %d: evaluation=%d\n", generations++, evaluateState());

		generations++;
		int c=evaluateState();
		
		if (generations >= MAXgenerations) break;  /* telos prospa8ias*/
		
		
		for (int g = 0; g < MAXindividuals; g+=2){
			newIndividual(currentGen, son1, son2);
			insertInOrder(evaluations, newGen, son1, g);
			insertInOrder(evaluations, newGen, son2, g+1);
		}
		for (int i = 0; i < nqueens; i++){
			nqueens_array[i] = newGen[0][i];		// antigrafoume tin kaliteri lisi ston kirio pinaka gia elexo ke tipoma
		}		
		for (int ind = 0; ind<MAXindividuals; ind++){	// ananeosi tis torinis genias me tin kenourgia
			for (int pos = 0; pos < nqueens; pos++){
				currentGen[ind][pos] = newGen[ind][pos];
			}
		}
		
	  }
	  if (generations < MAXgenerations) {
		printf ("Solved puzzle. ");
	  }
	  printf ("Final state is");
	  printState();
	  printf("\nConflicts Remaining=%d\n", countConflicts());

}

int QeensInDanger(int n, int array[n][n]){
	
	int counter = 0, counterH = 0, counterV = 0, i, j, k, l, threat = 0;
	
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			if(array[i][j] == 1){		// Opote vriskoume Queen vlepoume an apilitai katheta h orizontia h diagonia
				for(k = 0; k < n; k++){
					if(	array[i][k] == 1 && k != j){	// orizontia
						counter++;
					}
					if(	array[k][j] == 1 && k != i){	// katheta
						counter++;
					}	
				}

				counter += DiagonalSearch(n, array, i, j);	// diagonia
				//printf("Queen(%d,%d) - %d\n", i, j, counter);
				threat += counter;	// to synolo ton apeilon kathe Queen
				counter = 0;			
			}
		}
	}
	return threat/2;	// pernoume ta misa gia na mn metrame amfidromes apeiles
			
}

int DiagonalSearch(int n, int array[][n], int k, int l){
	
	int i, j, counter = 0;
	
	i = k;
	j = l;

	while(i > 0 && j > 0){		// Pano Aristera
		i -= 1;
		j -= 1;
		if(array[i][j] == 1){
			counter++;
		}
	}
	
	i = k;
	j = l;

	while(i > 0 && j < n-1){		// Pano Deksia
		i -= 1;
		j += 1;
		if(array[i][j] == 1){
			counter++;
		}
	}
	
	i = k;
	j = l;

	while(i < n-1 && j < n-1){		// Kato Deksia
		i += 1;
		j += 1;
		if(array[i][j] == 1){
			counter++;
		}
	}
	
	i = k;
	j = l;

	while(i < n-1 && j > 0){	// Kato Aristera
		i += 1;
		j -= 1;
		if(array[i][j] == 1){
			counter++;
		}
	}
	
	return counter;	// synolo diagonion apeilon
}

void foo(int n){
	int array[n][n], i,j,k,l, endangered = 0;
	queens myQueens[n];
	
	for(i = 0; i < n; i++)
		for(j = 0; j < n; j++)
				array[i][j] = 0;		// Arxikopoihsh pinaka se 0
	
	printf("\nQueens Positions:\n", i, k);
	for(i = 0; i < n; i++){
		k = rand() % n;		// tyxaia thesh stilhs
		printf("(%d,%d)\n", k, i);
		array[k][i] = 1;		// Topothethsh Queen..1 ana stilh
		myQueens[i].id = i;
		myQueens[i].i = k;
		myQueens[i].j = i;
	}
	
	printf("\nStarting Board:\n");		// Ektiposh arxikhs katastashs
	for(i = 0; i < n; i++){
		for(j = 0; j < n; j++){
			printf("%d ", array[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	
	endangered = QeensInDanger(n, array);	// Pernoume to synolo ton apeilwn
	printf("h =  %d\n\n", endangered);
	
	hillClimbing(endangered, n, array, 1);	// Start Hill Climbing
	
}

void hillClimbing(int h, int n, int board[][n], int cycles){
	int i, j, h_temp, h_new, oldQueenPos_i, oldQueenPos_j, newQueen_i, newQueen_j, deleteOn_i, deleteOn_j;
	h_temp = h;
	
	if(cycles == 501){		// Dinoume perithoria 500 kyklon gia eyresh kalyterhs timhs h
		printf("\n%d Cycles Complete!\n\nFinal Board:\n", cycles-1);
		
		for(i = 0; i < n; i++){		// Ektyposh telikhs katastashs
			for(j = 0; j < n; j++){
				printf("%d ", board[i][j]);
			}
			printf("\n");
		}
		return;
	}
	
	for(j = 0; j < n; j++){		// Anazitish stilhs
		for(i = 0; i < n; i++){
			if(board[i][j] == 1){		// Svisimo palias Queen
				oldQueenPos_i = i;		// palia thesh Queen
				oldQueenPos_j = j;
				board[i][j] = 0;
			}
		}
	
		for(i = 0; i < n; i++){ 	// Anazitish grammhs
			board[i][j] = 1;	// Prosorinh topothethsh neas Queen
			h_new = QeensInDanger(n, board);	// Neos elegxos gia h
			if(h_new <= h ){		// Periptosh ligoteron apilwn
				h = h_new;		// neo kalytero h
				newQueen_i = i;	// nea kalyterh thesh gia Queen
				newQueen_j = j;
				deleteOn_i = oldQueenPos_i;	// Thesh diagrafhs palias Queen
				deleteOn_j = oldQueenPos_j;
			}
			board[i][j] = 0;	// svisimo ths prosorinhs Queen gia topothetish neas
		}
		
		board[oldQueenPos_i][j] = 1;	// epanafora sthn arxikh katastash meta apo enan kyklo elegxwn
	}
	
	if(h == h_temp && deleteOn_i == newQueen_i && deleteOn_j == newQueen_j){	// An h kalyterh thesh einai idia me thn arxikh
		printf("\nNothing had to be changed!");
	}else if(h == h_temp && deleteOn_i != newQueen_i && deleteOn_j != newQueen_j){	// An h kalyterh thesh einai kainourgia alla idio h
		board[deleteOn_i][deleteOn_j] = 0;
		board[newQueen_i][newQueen_j] = 1;
	}else if(h < h_temp){								// // An h kalyterh thesh einai kainourgia me mikrotero h 
		board[deleteOn_i][deleteOn_j] = 0;
		board[newQueen_i][newQueen_j] = 1;
	}
	
	printf("\nAfter Hill Climbing (cycle %d):\nNew h =  %d\n\n", cycles, h);	
	
	if(h == 0 || h == h_temp){		// An meta apo ton sygkekrimeno kyklo kataliksoume na exoume idio h me kamia periptosh epomenhs veltioshs
		printf("Found the best solution with h = %d on cycle %d!\n\nFinal Board:\n", h, cycles);
		for(i = 0; i < n; i++){
			for(j = 0; j < n; j++){
				printf("%d ", board[i][j]);
			}
			printf("\n");
		}
	}else{
		hillClimbing(h, n, board, cycles+1);		// Oso yparxei pithanotita gia evresh neoy kalyterou h ginetai anadromika Hill Climbing
	}
}

