/***************************************************************************
 *   Copyright (C) 2008 by Madhu Srinivasan,,,   *
 *   srinivma@gmail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/


#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <utility>
#include <iostream>
#include <cstdlib>
#include <stdio.h>
#include <sys/time.h>
#include <string.h>
#include <vector>
#include <deque>

using namespace std;

//it is important not to change the 
//order of these enums
enum{ROBOT,GOAL,BOX,WALL};
enum{UP,DOWN,LEFT,RIGHT};

const int CLEAR=8;

vector<pair<int,int> > goal_locs;
vector<pair<int,int> > box_locs;
deque<pair<int,int> > *goal_paths;

//grid
int **sokoban_world;

//indication for locations where
//walls can be grown
// 0 indicates walls cannot be started from here
// 1 indicates walls can be started from here. 
int **wall_mask;



//command line params
int num_rows, num_cols, num_walls, num_boxes;
long grandom_seed;

void usage();
bool process_command_line( int argc, char *argv[] );
void show_grid();
int generate_random(int a, int b);
void show_wall_mask();
int direction(pair<int,int> loc1,pair<int,int> loc2);
void init_robot_location();
void init_goal_structure();
bool guarantee_solution();
void set_wall_mask();
bool plan_path(int goal_id);
bool put_box(int goal_id);
bool can_put_box_here(int curr_i,int curr_j);
void enforce_direction_constraints();
void clear_box_neighborhood();
void setup_walls();
vector<pair<int, int> > get_free_locs();
void grow_wall(int curr_i,int curr_j,int size);

void output_typed_strips();
void output_untyped_strips();
void print_connectivity_predicates();
void print_location_predicates();
void print_box_predicates();
void print_all_other_predicates();
void print_goal_predicates();


void clear_memory(){
 cout<<";clearing memory: "<<endl;
 for(int i=0;i<goal_locs.size();++i){
  goal_paths[i].clear();
 }
 delete[] goal_paths;
 
 goal_locs.clear();
 
 box_locs.clear();
 
 for(int i=0;i<num_rows;++i){
  delete[] sokoban_world[i];
  delete[] wall_mask[i];
 }
 delete[] sokoban_world;
 delete[] wall_mask;
}

int main(int argc, char *argv[])
{
  /* seed the random() function */
  struct timeval tv;
  struct timezone tz;
  gettimeofday(&tv, &tz);
  grandom_seed = tv.tv_usec;

  /* command line treatment, first preset values
   */
  num_rows = 9;
  num_cols = 9;
  
  num_boxes = 4;
  num_walls = 10;
  

  if ( argc == 1 || ( argc == 2 && *++argv[0] == '?' ) ) {
    usage();
    exit( 1 );
  }
  if ( !process_command_line( argc, argv ) ) {
    usage();
    exit( 1 );
  }
  
  bool success=false;
  //keep generating problem until you can successfully 
  //generate one and halt.
  while(!success){
  
   //allocate space
   goal_paths = new deque<pair<int,int> >[num_boxes];
  
   sokoban_world = new int*[num_rows];
   wall_mask = new int*[num_rows];
   for(int i=0;i<num_rows;++i){
    sokoban_world[i]=new int[num_cols];
    wall_mask[i] = new int[num_cols];
    //initialize to -1
    for(int j=0;j<num_cols;++j){
     sokoban_world[i][j]=CLEAR;
     wall_mask[i][j]=!(j<1||j>num_cols-3);
    }
   }


   //initialize robot position
   init_robot_location();
   //initialize the standard goal structure
   init_goal_structure();
   
   //find goal paths
   success=guarantee_solution();
   //if unsuccessful (timed out!)
   //then go back and try again
   if(!success){
    cout<<"\n;Failed to generate a solvable instance... trying again: "<<endl<<endl;
    clear_memory();
    continue;
   }
    
   //output the problem 
   //in typed strips
   //output_typed_strips();
   
   //only for debugging
   show_grid();
   show_wall_mask();
   
  
   //output the problem
   //in untyped strips
   output_untyped_strips();
  }
  
   clear_memory();
   return EXIT_SUCCESS;
}


bool guarantee_solution(){
 printf("\n;In guarantee_solution():\n");
 //for each goal
 for(int i=0;i<goal_locs.size();++i){
  cout<<";Planning path: "<<i<<endl;
  //first plan a path
  if(!plan_path(i)) return false;
  //put a ball at a random location on this path
  //this is the tricky part. 
  while(!put_box(i)){
   cout<<";Replanning: "<<endl;
   //have to recalculate the goal path
   goal_paths[i].clear();
   if(!plan_path(i)) return false;
  }
  cout<<";length of path: "<<goal_paths[i].size()<<endl;
 }

 //set the wall_mask
 set_wall_mask();
 
 //go back and clear the locations adjacent to 
 //boxes in sokoban_world
 clear_box_neighborhood();

 //now handle direction constraints on generated
 //paths & update wall mask
 enforce_direction_constraints();
 
 //setup walls
 setup_walls();
 
 return true;
}

void setup_walls(){
 cout<<";In setup_walls(): "<<endl;
 
 if(num_walls==0){
  //don't do anything .. 
  return;
 }
 //first collect a list of free positions
 //by consulting wall_mask
 vector<pair<int,int> > free_locs=get_free_locs();

 
 int wall_count=num_walls;
 //if number of walls requested is greater than
 //the number of available locations for walls, 
 //then just make all the available locations as
 //walls.
 if(wall_count>free_locs.size()){
  for(int i=0;i<free_locs.size();++i){
   sokoban_world[free_locs[i].first][free_locs[i].second]=WALL;
  }
  return;
 }
 
 //randomly put walls around
 int num_steps,curr_i,curr_j ;
 int idx;
 while(--wall_count>=0&&free_locs.size()>0){
  //random width/height of wall
  num_steps= generate_random(0,num_rows/2);
  //pick a random start location
  idx=generate_random(0,free_locs.size()-1);
  curr_i=free_locs[idx].first;
  curr_j=free_locs[idx].second;
  //grow wall
  grow_wall(curr_i,curr_j,num_steps);
  //get an updated list of free locations 
  free_locs=get_free_locs();
 }
}

void grow_wall(int curr_i,int curr_j,int size){
  //cout<<";In grow_wall: "<<endl;
  int num_steps=size;
 
  //horizontal or vertical
  if(random()%2){
   //horizontal wall 
   if(random()%2){
    //grow right
    while(num_steps>=0&&wall_mask[curr_i][curr_j]){
     sokoban_world[curr_i][curr_j]=WALL;
     ++curr_j;
     --num_steps;
    }
   }else{
    //grow left
    while(num_steps>=0&&wall_mask[curr_i][curr_j]){
     sokoban_world[curr_i][curr_j]=WALL;
     --curr_j;
     --num_steps;
    }
   }
  }
  else{
   //vertical wall 
   if(random()%2){
    //grow Up
    while(curr_i>=0&&num_steps>=0&&wall_mask[curr_i][curr_j]){
     sokoban_world[curr_i][curr_j]=WALL;
     --curr_i;
     --num_steps;
    }
   }else{
    //grow down
    while(curr_i<num_rows&&num_steps>=0&&wall_mask[curr_i][curr_j]){
     sokoban_world[curr_i][curr_j]=WALL;
     ++curr_i;
     --num_steps;
    }
   }
  } 
}

//not the most efficient way, but will 
//work for us :)
vector<pair<int, int> > get_free_locs(){
 //cout<<";In free_locs: "<<endl;
 vector<pair<int, int> > free_locs;
 for(int i=0;i<num_rows;++i){
  for(int j=0;j<num_cols;++j){
   if(wall_mask[i][j])
    free_locs.push_back(make_pair(i,j));
  }
 }
 return free_locs;
}
void enforce_direction_constraints(){
 cout<<";In enforce_direction_constraints(): "<<endl;
 
 //for each goal path
 for(int i=0;i<goal_locs.size();++i){
  //for each location along this goal path
  for(int j=1;j<goal_paths[i].size()-1;++j){

   int dir1=direction(goal_paths[i][j-1],goal_paths[i][j]);
   int dir2=direction(goal_paths[i][j],goal_paths[i][j+1]);
   //cout<<dir1<<" "<<dir2<<endl;
   
   int curr_i=goal_paths[i][j].first;
   int curr_j=goal_paths[i][j].second;
   
   //(dir1, dir2) is the pattern while traveling along this goal-path
   if(dir1==UP && dir2==RIGHT){
    if((curr_j-1<num_cols-2)&&(curr_j-1>0)){
     //cout<<";UP & RIGHT: "<<endl;
     wall_mask[curr_i][curr_j-1]=0;
     wall_mask[curr_i+1][curr_j-1]=0;
    }
   }
   if(dir1==DOWN && dir2==RIGHT){
    if((curr_j-1<num_cols-2)&&(curr_j-1>0)){
     //cout<<";UP & RIGHT: "<<endl;
     wall_mask[curr_i][curr_j-1]=0;
     wall_mask[curr_i-1][curr_j-1]=0;
    }
   }
   if(dir1==RIGHT && dir2==UP){
    if((curr_j<num_cols-2)&&(curr_j-1>0)){
     //cout<<";UP & RIGHT: "<<endl;
     wall_mask[curr_i+1][curr_j]=0;
     wall_mask[curr_i+1][curr_j-1]=0;
    }
   }
   if(dir1==RIGHT && dir2==DOWN){
    if((curr_j<num_cols-2)&&(curr_j-1>0)){
     //cout<<";UP & RIGHT: "<<endl;
     wall_mask[curr_i-1][curr_j]=0;
     wall_mask[curr_i-1][curr_j-1]=0;
    }
   }
   
  }//end inner for
 }//end outer for

}

//clears the neighborhood of boxes that we 
//had marked as "occupied" while putting the 
//boxes there in put_box() function

void clear_box_neighborhood(){
 cout<<";In clear_box_neighborhood(): "<<endl;
 
 //for each box location
 for(int i=0;i<box_locs.size();++i){
  int current_i=box_locs[i].first;
  int current_j=box_locs[i].second;
  
  if((current_j+1)<(num_rows-2)){
   sokoban_world[current_i][current_j+1]=CLEAR;
  }
  if((current_j-1)>0){
   sokoban_world[current_i][current_j-1]=CLEAR;
  } 
  sokoban_world[current_i-1][current_j]=CLEAR;
  sokoban_world[current_i+1][current_j]=CLEAR;  
 }

}

void set_wall_mask(){
 cout<<";In set_wall_mask(): "<<endl;
 
 //first set the masks for the box locations
 //and their adjacent locations from sokoban_world
 
 for(int i=0;i<num_rows;++i){
  for(int j=0;j<num_cols;++j){
   if(sokoban_world[i][j]==BOX){
    wall_mask[i][j]=0;
   }
  }
 }
 
 //ok, now set the masks from all the goal paths
 //for each goal path
 for(int i=0;i<goal_locs.size();++i){
  //for each location along this goal path
  for(int j=0;j<goal_paths[i].size();++j){
   //set the mask to zero
   wall_mask[goal_paths[i][j].first][goal_paths[i][j].second]=0;
  }
 }
}

//put a box at a random location along the path
//also set the locations as occupied
bool put_box(int goal_id){
 cout<<";In put_box: "<<endl;
 //cout<<"Goal id: "<<goal_id<<endl;
 //
 
 int current_i,current_j;
 //first check to see if the box can indeed be put in any 
 //point along the path 
 //if there are any such positions, collect and store them
 vector<int> candidate_position_ids;
 for(int idx=0;idx<goal_paths[goal_id].size();++idx){
  int current_i=goal_paths[goal_id][idx].first;
  int current_j=goal_paths[goal_id][idx].second;
  if(can_put_box_here(current_i,current_j)){
   candidate_position_ids.push_back(idx);
  }
 }
 
 //if there are no useful positions to put the box, return failure
 if(candidate_position_ids.size()==0) return false;
 
 //otherwise, pick a random location from these positions 
 //and put the box there.
 
 //random index 
 int id = generate_random(0,candidate_position_ids.size());
 //get the id for goalpath.
 int idx = candidate_position_ids[id];
 current_i=goal_paths[goal_id][idx].first;
 current_j=goal_paths[goal_id][idx].second;
 
 //now the box must be kept at (current_i,current_j)
 //so update the world
 sokoban_world[current_i][current_j]=BOX;
 //also mark locations of surrounding
 //grids
 if((current_j+1)<(num_rows-2)){
  sokoban_world[current_i][current_j+1]=BOX;
 }
 if((current_j-1)>0){
  sokoban_world[current_i][current_j-1]=BOX;
 } 
 sokoban_world[current_i-1][current_j]=BOX;
 sokoban_world[current_i+1][current_j]=BOX;
  
  
 //add the box location to a list
 box_locs.push_back(make_pair(current_i,current_j));
 //report success
 return true;
}

bool can_put_box_here(int curr_i,int curr_j){
 //the portion where boxes can be put
 if(curr_j<num_rows-2 && curr_j>0){
  return (sokoban_world[curr_i][curr_j]==CLEAR);
  /*return (sokoban_world[curr_i][curr_j]==CLEAR &&
  sokoban_world[curr_i-1][curr_j]==CLEAR &&
  sokoban_world[curr_i+1][curr_j]==CLEAR &&
  sokoban_world[curr_i][curr_j+1]==CLEAR &&
  sokoban_world[curr_i][curr_j-1]==CLEAR );*/
 }
 else{
  return false;
 }

}

//plan a path from (i,j) back to the 
//agent's column. There are two conditions
//enforced:
// - path cannot touch the edge of the grid.
// - path cannot touch any ball.
bool plan_path(int goal_id){
 cout<<";In plan_path():"<<endl;

 enum{UP,LEFT,DOWN};
 
 int num_iterations=-1;

 int curr_i=goal_locs[goal_id].first;
 int curr_j=goal_locs[goal_id].second;
 
 int move=random()%3;
 int num_steps;
 while(curr_j>1){
 //if it takes more than n^2 steps, 
 //then definitely this loop is stuck here
 //exit with failure
 if(++num_iterations>num_rows*num_cols){
  return false;
 }
 
 //cout<<curr_i<<" "<<curr_j<<endl;
 switch(move){

  case UP:
   //cout<<"In case UP:"<<endl;
   num_steps=(random()%(curr_i+1))+2;
   while((--num_steps>0)&&curr_i>1){
    --curr_i;
    //if the next location is clear or its just
    //an existing goal location, you can move.
    if(sokoban_world[curr_i][curr_j]== CLEAR||
       sokoban_world[curr_i][curr_j]== GOAL){
     //mark this location so that a wall cannot be put here
     //wall_mask[curr_i][curr_j]=0;
     //store this location
     goal_paths[goal_id].push_front(make_pair(curr_i,curr_j));
    }
    else{
     //can't go up anymore, undo your step
     ++curr_i;
     break;
    }
   }
   
   //dont move DOWN now
   move=random()%2?UP:LEFT;
   break;
  
  case DOWN:
   //cout<<"In case DOWN:"<<endl;
   num_steps=(random()%(num_rows-curr_i))+2;
   
   //do not go further if you touch the bottom
   //of the grid
   while((--num_steps>0)&&curr_i<num_rows-2){
    ++curr_i;
    //move, only if the next location is clear
    if(sokoban_world[curr_i][curr_j]== CLEAR){
      //mark this location so that a wall cannot be put here
     //wall_mask[curr_i][curr_j]=0;
     //store this location
     goal_paths[goal_id].push_front(make_pair(curr_i,curr_j));
    }
    else{
     //can't go down anymore, undo your step
     --curr_i;
     break;
    }
   }
   
   //dont move UP now
   move=random()%2?DOWN:LEFT;
   break;
  
  case LEFT:
   //cout<<"In case LEFT:"<<endl;
   num_steps=3;//(random()%(curr_j))+3;
   while((--num_steps>0)&&curr_j>1){
    --curr_j;
    //move, only if the next location is clear
    if(sokoban_world[curr_i][curr_j]== CLEAR){
      //mark this location so that a wall cannot be put here
     //wall_mask[curr_i][curr_j]=0;
     //store this location
     goal_paths[goal_id].push_front(make_pair(curr_i,curr_j));
    }
    else{
     //can't go left anymore, undo your step
     ++curr_j;
     break;
    }    
   }   
   
   //move anywhere
   move=random()%3;
   break;
  default:
   cout<<";Error: in guarantee_solution() ";
   exit(9);
 };
 }
 return true;
}


//return the direction, while walking from 
//loc1 to loc2
int direction(pair<int,int> loc1,pair<int,int> loc2){
 int i1=loc1.first;
 int j1=loc1.second;
 int i2=loc2.first;
 int j2=loc2.second;
 
 if(j2==j1+1){
  return RIGHT;
 }
 if(i2==i1+1){
  return DOWN;
 }
 if(i2==i1-1){
  return UP;
 }
 //this should never happen
 cout<<";Fatal error in direction(..): Aborting!"<<endl;
 exit(9);
 return -1;
}

//robot is at a random row of the 
//first column
void init_robot_location(){
 //a random number between [0,num_rows-1]
 sokoban_world[random()%num_rows][0]=ROBOT;
}


//goal
void init_goal_structure(){
 int num_steps = num_boxes;//generate_random(num_boxes,num_rows-2);
 int current_i=num_rows;
 int current_j=num_cols-2;
 //the wall in front of goal locations
 while(--num_steps>=0){
  sokoban_world[--current_i][current_j]=WALL;
 }
 //the goal locations
 num_steps=num_boxes;
 current_i=num_rows-num_boxes-1;
 current_j=num_cols-1;
 while(--num_steps>=0){
  sokoban_world[++current_i][current_j]=GOAL;
  goal_locs.push_back(make_pair(current_i,current_j));
 }
}


//random number between a & b
int generate_random(int a, int b){
 int num=(random()%(b-a+1))+a;
 return num;
}

void usage()
{

  printf("\nusage:\n");

  printf("\nOPTIONS   DESCRIPTIONS\n\n");
  printf("-n <num>    grid size (minimal 5)\n");
  printf("-b <num>    number of boxes (minimal 1)\n");
  printf("-w <num>    number of walls (minimal 0)\n");
  printf("-s <num>    random seed\n\n");

}


//MADHU: Right now, there are no checks to verify
//the input parameters.
bool process_command_line( int argc, char *argv[] )
{

  char option;
  while ( --argc && ++argv ) {
    if ( *argv[0] != '-' || strlen(*argv) != 2 ) {
      return false;
    }
    option = *++argv[0];
    switch ( option ) {
    default:
      if ( --argc && ++argv ) {
	switch ( option ) {
	case 'n':
	  sscanf( *argv, "%d", &num_rows);
	  num_cols = num_rows;
	  break;
	case 'b':
	  sscanf( *argv, "%d", &num_boxes);
	  break;
	case 'w':
	  sscanf( *argv, "%d", &num_walls);
	  break;
	case 's':
	  sscanf( *argv, "%ld", &grandom_seed);
	  break;
	default:
	  printf( "\n\nunknown option: %c entered\n\n", option );
	  return false;
	}
      } else {
	return false;
      }
    }
  }

  if ( num_rows < 1 || num_cols < 1 )
  {
    return false;
  }
  
  int bound;
  if(num_rows<=5){
   bound = 2;
  }else{
   bound = (num_rows*num_rows-5*num_rows+6)/9.0;
  }
  if(num_boxes > bound){
   cout<<"\nFor the given grid size, upper bound on number of boxes = "<<bound<<endl;
   cout<<"Please make sure your input is within the above bound: "<<endl;
   
   return false;
  }

  return true;

}


void show_grid(){
 cout<<";Grid: "<<endl;
 cout<<endl;
 for(int i=0;i<num_rows;++i){
   cout<<"; ";
   for(int j=0;j<num_cols;++j){
    cout<<sokoban_world[i][j]<<" ";
   }
   cout<<endl;
  }
  cout<<endl;
}


void show_wall_mask(){
 cout<<";Wall mask: "<<endl;
 cout<<endl;
 for(int i=0;i<num_rows;++i){
   cout<<"; ";
   for(int j=0;j<num_cols;++j){
    cout<<wall_mask[i][j]<<" ";
   }
   cout<<endl;
  }
  cout<<endl;
}


void output_typed_strips(){
  /* now output problem in PDDL syntax
   */
  printf("\n\n\n");

  /* header
   */
  printf("(define (problem typed-sokoban-grid%d-boxes%d-walls%d)",
	 num_rows, num_boxes, num_walls);
  printf("\n(:domain typed-sokoban)");

  printf("\n(:objects ");
  //direction objects
  printf("\n        up down left right - DIR");
  printf("\n        ");
  for(int i=0;i<box_locs.size();++i)
    printf("box%d ",i);
  printf("- BOX");
  for (int i=0; i < num_rows; ++i) {
    printf("\n        ");
    for (int j=0; j < num_cols; ++j) {
      printf("f%d-%df ", i, j);
    }
  }
  printf(" - LOC\n)");
  
  //definition of intitial state
  printf("\n(:init");

  print_connectivity_predicates();
  print_box_predicates();
  print_all_other_predicates();
  printf("\n)");

  //what is the goal
  printf("\n(:goal");
  printf("\n(and");
  print_goal_predicates();
  printf("\n)");
  printf("\n)");
  printf("\n)");
  printf("\n\n\n");
  
}


void output_untyped_strips(){
  /* now output problem in PDDL syntax
   */
  printf("\n\n\n");

  /* header
   */
  printf("(define (problem untyped-sokoban-grid%d-boxes%d-walls%d)",
	 num_rows, num_boxes, num_walls);
  printf("\n(:domain untyped-sokoban)");

  printf("\n(:objects ");
  //direction objects
  printf("\n        up down left right ");
  printf("\n        ");
  for(int i=0;i<box_locs.size();++i)
    printf("box%d ",i);
  for (int i=0; i < num_rows; ++i) {
    printf("\n        ");
    for (int j=0; j < num_cols; ++j) {
      printf("f%d-%df ", i, j);
    }
  }
  printf("\n)");
  
  //definition of intitial state
  printf("\n(:init");
  //directions
  printf("\n(direction up)");
  printf("\n(direction down)");
  printf("\n(direction left)");
  printf("\n(direction right)");
  //boxes
  for(int i=0;i<box_locs.size();++i)
    printf("\n(box box%d) ",i);
    
  print_location_predicates();  
  print_connectivity_predicates();
  print_box_predicates();
  print_all_other_predicates();
  printf("\n)");

  //what is the goal
  printf("\n(:goal");
  printf("\n(and");
  print_goal_predicates();
  printf("\n)");
  printf("\n)");
  printf("\n)");
  printf("\n\n\n");
  
}

void print_goal_predicates(){
 for(int i=0;i<goal_locs.size();++i){
  printf("\n(at box%d f%d-%df) ", i,goal_locs[i].first,goal_locs[i].second);
 }  
}


void print_all_other_predicates(){
  for (int i=0; i < num_rows; ++i) {
    for (int j=0; j < num_cols; ++j) {
     //robot predicate
     if(sokoban_world[i][j]==ROBOT){
      printf("\n(at-robot f%d-%df) ", i, j);
      printf("\n(clear f%d-%df) ", i, j);
     }
     //clear predicate
     if(sokoban_world[i][j]==CLEAR||sokoban_world[i][j]==GOAL){
      printf("\n(clear f%d-%df) ", i, j);
     }
    }
  }
}

void print_box_predicates(){
 for(int i=0;i<box_locs.size();++i){
  printf("\n(at box%d f%d-%df) ", i,box_locs[i].first,box_locs[i].second);
 }
}

void print_location_predicates(){
  for (int i=0; i < num_rows; ++i) {
    for (int j=0; j < num_cols; ++j) {
     printf("\n(location f%d-%df) ", i, j);
    }
  }
}


void print_connectivity_predicates(){
  for (int i=0; i < num_rows; ++i) {
    for (int j=0; j < num_cols; ++j) {
     //left connectivity
     if((j-1)>=0){
      printf("\n(adjacent f%d-%df f%d-%df left)",i,j,i,j-1);
     }
     //right connectivity
     if(j+1<=num_cols-1){
      printf("\n(adjacent f%d-%df f%d-%df right)",i,j,i,j+1);
     }
     //up connectivity
     if(i-1>=0){
      printf("\n(adjacent f%d-%df f%d-%df up)",i,j,i-1,j);
     }
     //down connectivity
     if(i+1<=num_rows-1){
      printf("\n(adjacent f%d-%df f%d-%df down)",i,j,i+1,j);
     }
    }
  }
}
