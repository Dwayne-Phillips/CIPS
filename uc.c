/*
   uc.c
   Dwayne Phillips

   This program illustrates the uniform cost
   search algorithm.

   April 1999

*/


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#undef DEBUG    

#define NOTHING -1
#define H       10
#define V       10

/************************************************/


int cost_matrix[H][V] =
   { {-1,  5,  5,  6, -1,  7, -1, -1, -1, -1},
     { 5, -1, -1, -1, -1, -1, -1,  4, -1, -1},
     { 5, -1, -1, -1,  6, -1, -1, -1, -1, -1},
     { 6, -1, -1, -1,  5, -1, -1, -1,  2, -1},
     {-1, -1,  6,  5, -1, -1,  4, -1, -1,  5},
     { 7, -1, -1, -1, -1, -1,  3, -1, -1, -1},
     {-1, -1, -1, -1,  4,  3, -1, -1, -1, -1},
     {-1,  4, -1, -1, -1, -1, -1, -1, -1,  2},
     {-1, -1, -1,  2, -1, -1, -1, -1, -1,  1},
     {-1, -1, -1, -1,  5, -1, -1,  2,  1, -1}};

struct node {
   int    number;
   int    cost;
   struct node *parent;
   struct node *next;
};

/************************************************/

struct node * remove_from_list(struct node **, int);
struct node * create_new_node(int, int);
void add_to_list(struct node **, struct node **);
void print_node(struct node *);
void print_usage();
void free_nodes_in_list(struct node *);
void traverse_list(struct node *);
int is_empty(struct node *);
int is_a_goal_node(struct node *, int);
int no_solution_exists(int *);
int solution_found(struct node *, int *);


/************************************************/

main(int argc, char *argv[])
{

char response[80];
int stupid = 99;

   int cost, 
       goal,
       i, 
       searching   = 1;

   struct node *CLOSED,
               *n,
               *new,
               *OPEN;

      /******************************************
      *
      *   Read the command line.
      *
      ******************************************/

   if(argc < 2){
      print_usage();
      exit(0);
   }

   goal = atoi(argv[1]);

      /******************************************
      *
      *   Initialize this problem.
      *
      ******************************************/

   OPEN   = NULL;
   CLOSED = NULL;

   n = create_new_node(0, 0);
   add_to_list(&OPEN, &n);

      /******************************************
      *
      *   Search through the state space for 
      *   a solution.
      *
      ******************************************/

   while(searching){

      if(is_empty(OPEN))
         no_solution_exists(&searching);

      else{  /* OPEN list is not empty */

            /************************************
            *
            *   Take the node on OPEN list
            *   that has the lowest cost
            *   and put it on the CLOSED list.
            *
            ************************************/

         n = remove_from_list(&OPEN, goal);
         add_to_list(&CLOSED, &n);
#ifdef DEBUG
printf("\nPulled this node from OPEN added to CLOSE");
print_node(n);
gets(response);
#endif

         if(is_a_goal_node(n, goal))
            solution_found(n, &searching);
         else{

            /************************************
            *
            *   Expand node n using the 
            *   cost matrix.
            *
            ************************************/

            for(i=0; i<H; i++){
               cost = cost_matrix[n->number][i];
               if(cost != NOTHING){

                  /*******************************
                  *
                  *   First expand n if it is 
                  *   the start node.
                  *
                  *******************************/

                  if(n->parent == NULL){
                     new = create_new_node(i, 
                              cost+n->cost);
                     new->parent = n;
                     add_to_list(&OPEN, &new);
#ifdef DEBUG
printf("\nCreated this new node");
print_node(new);
printf("\nhit enter "); gets(response);
#endif
                  }  /* ends if start node */

                  /*******************************
                  *
                  *   Expand nodes that are not
                  *   the start node.
                  *
                  *   Do not expand the parent 
                  *   node of node n.
                  *
                  *******************************/

                  else{  /* not start node */
                     if((n->parent)-> number != i){
                        new = create_new_node(i, 
                                 cost+n->cost);
                        new->parent = n;
                        add_to_list(&OPEN, &new);
#ifdef DEBUG
printf("\nCreated this new node");
print_node(new);
printf("\nhit enter "); gets(response);
#endif
                     }  /* ends if */
                  }  /* ends else not start node */
               }  /* ends if != NOTHING */
            }  /* ends expanding loop over i */

         }  /* ends else not done yet */

      }  /* ends else OPEN is not empty */

   }  /* ends while searching */


   free_nodes_in_list(OPEN);
   free_nodes_in_list(CLOSED);

}  /* ends main */

/************************************************/
/*
   Add a node to the end of the linked list 
   pointed to be the head pointer.
*/

void add_to_list(struct node **head, 
                 struct node **new)
{
   struct node *temp;

   if((*head) == NULL)
      (*head) = (*new);
   else{
      temp = (*head);
      while(temp->next != NULL)
         temp = temp->next;
      temp->next = (*new);
   }  /* ends else */

   (*new)->next = NULL;

}  /* ends add_to_list */

/************************************************/

void free_nodes_in_list(struct node *head)
{
   struct node *temp;

   printf("\n\nFreeing nodes in a list");
   while(head != NULL){
      temp = head;
      print_node(temp);
      head = temp->next;
      free(temp);
   }  /* ends while */
}  /* ends free_nodes_in_list */

/************************************************/

int is_empty(struct node *head)
{
   int result = 0;
   if(head == NULL)
      result = 1;
   return(result);
}  /* ends is_empty */

/************************************************/

int no_solution_exists(int *searching)
{
   *searching = 0;
   printf("\nThe OPEN list is empty, "
          "no solution exists");
   return(1);
}  /* ends no_solution_exists */

/************************************************/

void print_node(struct node *a)
{
   printf("\nNode number=%d cost=%d",
   a->number, a->cost);
}  /* ends print_node */

/************************************************/
/*
   This routine will remove and return one node 
   from the linked list pointed to by head.

   1. If there are any goal nodes in the list,
   remove the one with the lowest cost.

   2. No goal nodes in the list, remove the node
   with the lowest cost.
*/

struct node * remove_from_list(struct node **head,
                               int goal)
{
   int    done       = 0,
          goal_found = 0,
          min        = 16000,
          min_number = 0;
   struct node *temp, *result;

   if(*head == NULL){
      printf("\nEMPTY LIST CANNOT REMOVE ");
      exit(1);
   }  /* ends if NULL */

   temp = *head;

      /******************************************
      *
      *   Traverse the list looking for goal
      *   nodes.  If goal nodes exist, set
      *   result to the one with the lowest
      *   cost.
      *
      ******************************************/

   while(temp != NULL){
      if(is_a_goal_node(temp, goal)){
         goal_found = 1;
         if(temp->cost < min){
            min        = temp->cost;
            min_number = temp->number;
            result     = temp;
         }  /* ends if */
      }  /* ends if is_a_goal_node */
      temp = temp->next;
   }  /* ends while temp != NULL */

      /******************************************
      *
      *   If no goal node were inthe list,
      *   traverse the list again to find the
      *   node with the lowest cost.
      *
      ******************************************/

   if(goal_found == 0){
      temp = *head;
      while(temp != NULL){
         if(temp->cost < min){
            min        = temp->cost;
            min_number = temp->number;
            result     = temp;
         }  /* ends if */
         temp = temp->next;
      }  /* ends while temp != NULL */
   }  /* ends if goal_found == 0 */

      /******************************************
      *
      *   Now pull the result node out of the
      *   list.
      *
      *   First is the special case where result
      *   is at the head of the list.
      *
      ******************************************/

   temp = *head;

   if(temp == result)
      *head = result->next;
   else{
      while(temp != NULL){
         if(temp->next == result){
            temp->next   = result->next;
            result->next = NULL;
            temp         = NULL;
            done         = 1;
         }  /* ends if temp->next == result */
         if(done != 1) 
            temp = temp->next;
      }  /* ends while temp != NULL */
   }  /* ends else result is not at the HEAD */

   return(result);

}  /* ends remove_from_list */

/************************************************/


/************************************************/


/************************************************/

int is_a_goal_node(struct node *n, int goal)
{
   int result = 0;
   if(n->number == goal)
      result = 1;
   return(result);
}  /* ends is_a_goal_node */


/************************************************/

int solution_found(struct node *end, 
                   int *searching)
{
   struct node *temp;

   temp       = end;
   *searching = 0;

   printf("\n\n\nHere is the solution");
   while(temp != NULL){
      print_node(temp);
      temp = temp->parent;
   }  /* ends while */

   return(1);
}  /* ends solution_found */

/************************************************/


/************************************************/


/************************************************/


/************************************************/


/************************************************/

struct node * create_new_node(int number, 
                              int cost)
{
   struct node *new;
   new = (struct node *)
         calloc(1, sizeof(struct node));
   new->number = number;
   new->cost   = cost;
   new->next   = NULL;
   new->parent = NULL;
   return(new);
}  /* ends create_new_node */

/************************************************/


/************************************************/

void print_usage()
{
   printf("\nusage: uc goal-node");
}  /* ends print_usage */

/************************************************/

void traverse_list(struct node *head)
{
   struct node *temp;

   printf("\n\nTraversing list");
   while(head != NULL){
      temp = head;
      print_node(temp);
      head = temp->next;
   }  /* ends while */
   printf("\n\n");
}  /* ends free_nodes_in_list */
