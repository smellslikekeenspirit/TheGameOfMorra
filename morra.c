
/** @author Prionti Nasir
** pdn3628@rit.edu
** CS251 Project 2
**/

#include <mpi.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#define PMAIN 0
#define DELAY usleep(1)
#define FINGERS 5

/// main function
/// parses user parameters, initializes, implements and terminates MPI
int main( int argc, char* argv[] ){
	
  if( argc != 2 ){
    fprintf(stderr, "Not enough arguments: Retry with parameter(s)\n" );
    exit(EXIT_FAILURE);
  }

  double start;
  double end;
  int rounds;
  start = MPI_Wtime();
  if((rounds = atoi(argv[1]))< 0){
    fprintf(stderr, "Invalid number of rounds: %d\n", rounds);
    exit( EXIT_FAILURE );
  }
  if( MPI_Init( &argc, &argv ) != MPI_SUCCESS ){
    fprintf( stderr, "MPI Initialization failed. \n" );
    exit( EXIT_FAILURE );
  }
  int players, myrank;
  int err_s = MPI_Comm_size( MPI_COMM_WORLD, &players );
  int err_r = MPI_Comm_rank( MPI_COMM_WORLD, &myrank );

  if(err_s + err_r != MPI_SUCCESS){
    fprintf(stderr, "Error: internal problem with MPI\n" );
    MPI_Finalize();
    exit(EXIT_FAILURE);
  }
  if (players < 2 ){
    fprintf( stderr, "Error: Invite more players and retry\n" );
    MPI_Abort(MPI_COMM_WORLD, EXIT_SUCCESS );
  }
  srand(time(NULL)+myrank);
  // single process
  int fingers=0;
  int guess=0;
  int score=0; 
  int total_scores=0;
  // all processes
  int total_fingers=0;
  int winners_count=0;
  int total_for_processor[players];

  for( int k=0; k<rounds; k++ ){
	  
    fingers = rand() % (FINGERS + 1);
    guess = fingers + rand() % ((players-1) * (FINGERS + 1));
    // for print statements to not clog
    DELAY;
    printf("I am %d. For run %d, I extend %d and my guess is %d.\n", 
          myrank, k+1, fingers, guess);
    fflush(stdout);
    MPI_Allreduce(&fingers, &total_fingers, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    MPI_Barrier( MPI_COMM_WORLD );
	
    if(guess == total_fingers){
      score = 1;
    }
    MPI_Allreduce(&score, &winners_count, 1, MPI_INT, MPI_SUM, MPI_COMM_WORLD);
    if(!winners_count && myrank == PMAIN){
      DELAY;
      printf("No one won this round %d.\n", k+1 );
      fflush (stdout);
    
    }
	else if(winners_count > 1 && score == 1 ) {
      DELAY;
      printf("I am %d, and I almost won the run %d.\n", myrank, k+1);
      fflush(stdout);
    }
    else if (winners_count == 1 && score == 1){
      DELAY;
      total_scores++;
	  printf("I am %d, and I won the run %d.\n", myrank, k+1);
      fflush(stdout );
    }
    DELAY;  
    score = 0;
    MPI_Barrier( MPI_COMM_WORLD );  
  }
  MPI_Allgather(&total_scores, 1, MPI_INT, total_for_processor, 1, MPI_INT, MPI_COMM_WORLD );
  int placement = 0;
  for( int j=0; j<players; j++ )
    if( total_scores > total_for_processor[j] ){
      placement++;
	}
	
  MPI_Barrier( MPI_COMM_WORLD );
  printf("Hey! I am %d, and I won %d time(s). Overall, I played better than %d player(s).\n", myrank, total_scores, placement); 
  end = MPI_Wtime();
  MPI_Barrier( MPI_COMM_WORLD );
  if( myrank == PMAIN)
    printf("For %d players and %d rounds, the time for this game is %.4lf.\n", players, rounds, end-start );
  MPI_Finalize();
  return EXIT_SUCCESS;
}


