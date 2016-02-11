/*
   d.c
   Dwayne Phillips

   This program illustrates the depth first
   search algorithm.

   April 1999

*/


#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

#undef DEBUG    

#define COUNT      2
#define TABLE     -1
#define MAX_DEPTH 99

/************************************************/


struct node {
   int    number;
   int    blocks[COUNT+1];
   int    depth;
   struct node *parent;
   struct node *next;
};

/************************************************/

struct node * remove_from_list(struct node **);
struct node * create_new_node(int, int [], int);
struct node * move_block(struct node *,
                         int , int , int );
void add_to_list(struct node **, struct node **);
void print_node(struct node *);
void free_nodes_in_list(struct node *);
void traverse_list(struct node *);
int is_empty(struct node *);
int is_a_goal_node(struct node *, int []);
int no_solution_exists(int *);
int solution_found(struct node *, int *);
int is_uncovered(struct node *, int );
int is_the_same(struct node *, int []);
int state_doesnt_exist(struct node *,
                       struct node *,
                       struct node *,
                       int, int);


/************************************************/

main(int argc, char *argv[])
{

char response[80];
int stupid = 99;

   int b[COUNT+1],
       goal[COUNT+1],
       i, 
       j,
       node_number = 0,
       searching   = 1;

   struct node *CLOSED,
               *n,
               *new,
               *OPEN;

      /******************************************
      *
      *   Initialize this problem.
      *
      *   0
      *   1 2
      *
      ******************************************/

   OPEN   = NULL;
   CLOSED = NULL;

   b[0] = 1;
   b[1] = TABLE;
   b[2] = TABLE;
   n = create_new_node(node_number, b, 0);
   node_number++;
   add_to_list(&OPEN, &n);

      /******************************************
      *
      *   Make up a goal
      *
      *   0
      *   1 
      *   2
      *
      ******************************************/

      goal[0] = 1;
      goal[1] = 2;
      goal[2] = TABLE;

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
            *   Take the first node on OPEN list
            *   and put it on the CLOSED list.
            *
            *   Try to expand the node.  Three
            *   rules for expanding (moving a 
            *   block to another place).
            *
            *   1. The block must be uncovered,
            *      i.e. no blocks sitting on it.
            *   2. Cannot put block a on block b
            *      is b has something on it, i.e.
            *      block b must be uncovered.
            *   3. Cannot repeat a state, i.e.
            *      the new state cannot be in 
            *      the OPEN or CLOSED list.
            *
            ************************************/

         n = remove_from_list(&OPEN);
         add_to_list(&CLOSED, &n);

         if(n->depth < MAX_DEPTH){

               /*********************************
               *
               *   Expand node n.
               *   Look at each block in node n.
               *   
               *********************************/

            for(i=0; i<=COUNT; i++){

/* RULE 1 */   if(is_uncovered(n, i)){

                     /* Special case
                        Try to move block i 
                        to the TABLE */
                  if(n->blocks[i] != TABLE){
/* RULE 3 */         if(state_doesnt_exist(OPEN,
                                           CLOSED,
                                           n, i,
                                           TABLE)){
                        new = move_block(n, 
                                    node_number, 
                                    i, TABLE);
                        if(is_a_goal_node(new, 
                                          goal)){
                           solution_found(new, 
                                      &searching);
                        }
                        node_number++;
                        add_to_list(&OPEN, &new);
                     }  /* ends state_doesnt_exist */
                  }  /* ends if TABLE */

                     /* Try to move block i 
                        onto block j */
                  for(j=0; j<=COUNT; j++){
                     if(i != j){
/* RULE 2 */            if(is_uncovered(n, j)){
/* RULE 3 */               if(state_doesnt_exist(
                                        OPEN,
                                        CLOSED,
                                        n, i, j)){
                              new = move_block(n,
                                    node_number,
                                    i, j);
                              if(is_a_goal_node(new,
                                         goal)){
                                 solution_found(new, 
                                      &searching);
                              }
                              node_number++;
                              add_to_list(&OPEN, 
                                          &new);
                           }  /* statedoesntexist */
                        }  /* ends if j is uncovered */
                     }  /* ends if i != j */
                  }  /* ends loop over j */
               }  /* ends if is_uncovered */
            }  /* ends loop over i */
         }  /* ends if depth < MAX_DEPTH */
      }  /* ends else OPEN is not empty */
   }  /* ends while searching */

   free_nodes_in_list(OPEN);
   free_nodes_in_list(CLOSED);

}  /* ends main */

/************************************************/
/*
   Add a node to the start of the linked list 
   pointed to be the head pointer.
*/

void add_to_list(struct node **head, 
                 struct node **new)
{
   struct node *temp;

   if((*head) == NULL){
      (*head)      = (*new);
      (*new)->next = NULL;
   }  /* ends if head is NULL */

   else{
      temp         = (*head);
      (*head)      = (*new);
      (*new)->next = temp;
   }  /* ends else */

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
   int i;

   printf("\nNode number=%d depth=%d",
   a->number, a->depth);
   printf("\nWhat is under the %d blocks\n", COUNT+1);

   for(i=0; i<=COUNT; i++)
      printf("%4d", a->blocks[i]);

}  /* ends print_node */

/************************************************/
/*
   Remove the node from the start of the list
   pointed to by head.
*/

struct node * remove_from_list(struct node **head)
{
   struct node *result;

   if(*head == NULL){
      printf("\nEMPTY LIST CANNOT REMOVE ");
      exit(1);
   }  /* ends if NULL */

   result  = (*head);
   (*head) = (*head)->next ;

   return(result);

}  /* ends remove_from_list */

/************************************************/

int is_uncovered(struct node *n, int block)
{
   int i, result = 1;

   for(i=0; i<=COUNT; i++){
      if(n->blocks[i] == block)
         result = 0;
   }
   return(result);
}  /* ends is_uncovered */

/************************************************/
/*
   If we put block on place, does that state 
   already exist in the list pointed to by open
   or the list pointed to by closed.  Return 1
   if neither list contains that state.
*/

int state_doesnt_exist(struct node *open,
                       struct node *closed,
                       struct node *n,
                       int block,
                       int place)
{
   int i, 
       tempb[COUNT+1],
       result=1;
   struct node *tempp;

   for(i=0; i<=COUNT; i++)
      tempb[i] = n->blocks[i];
   tempb[block] = place;

   tempp = open;
   while(tempp != NULL){
      if(is_the_same(tempp, tempb))
         result = 0;
      tempp = tempp->next;
   }

   tempp = closed;
   while(tempp != NULL){
      if(is_the_same(tempp, tempb))
         result = 0;
      tempp = tempp->next;
   }

   return(result);
}  /* ends state_doesnt_exist */

/************************************************/

int is_the_same(struct node *n, int b[])
{
   int i, result=1;

   for(i=0; i<=COUNT; i++){
      if(n->blocks[i] != b[i])
         result = 0;
   }
   return(result);
}  /* ends is_the_same */

/************************************************/

/************************************************/

/************************************************/

/************************************************/

/************************************************/

int is_a_goal_node(struct node *n, int g[])
{
   int i, 
       result = 1;

   for(i=0; i<=COUNT; i++){
      if(n->blocks[i] != g[i])
         result = 0;
   }

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
/*
   This function creates a new node.  The new
   node has the same block positions as node n,
   with the change that block is on place.
   The parent of the new node is n and the depth
   of the new node is n's depth plus 1.
*/

struct node * move_block(struct node *n,
                         int number, 
                         int block, 
                         int place)
{
   int i, b[COUNT+1];
   struct node *result;

   for(i=0; i<=COUNT; i++)
      b[i] = n->blocks[i];
   b[block] = place;

   result = create_new_node(number, b, n->depth+1);
   result->parent = n;

   return(result);

}  /* ends move_block */


/************************************************/

struct node * create_new_node(int number, 
                              int b[],
                              int depth)
{ 
   int i;
   struct node *new;

   new = (struct node *)
         calloc(1, sizeof(struct node));
   new->number    = number;
   new->depth     = depth;
   new->next      = NULL;
   new->parent    = NULL;

   for(i=0; i<=COUNT; i++)
      new->blocks[i] = b[i];

   return(new);
}  /* ends create_new_node */

/************************************************/


/************************************************/

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
