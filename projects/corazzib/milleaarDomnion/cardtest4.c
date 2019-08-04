/*
 * cardtest4.c
 */


#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <time.h>

#define TESTFUNCTION "getWinners"

void assertTrue(int result, char* test) {
  if(!result) {
    printf("%s failed\n", test);
  } else {
    printf("%s passed\n", test);
  }
}

void testOne(struct gameState *game, int numPlayers, int winners[]) {
  struct gameState testGame;
  int players[numPlayers];

  //loop over hand, set state for all players to be equal, and give winner[s] all provinces
  for(int j = 0; j < numPlayers; j++) {
    game->handCount[j] = 5;
    game->discardCount[j] = 0;
    game->deckCount[j] = 5;
    game->whoseTurn = 1;
    if(winners[j] == 1) {
      for(int i = 0; i < game->handCount[j]; i++) {
        game->hand[j][i] = province;
      }
    }
  }

  // copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));
  // initialize a game state and player cards
  int ret = getWinners(players, &testGame);

  //succeeds
  printf("Test 0: getWinners succeeds\n");
  assertTrue(ret == 0 , "Test 0");

  //winner is marked as winner in players array
  for(int i = 0; i < numPlayers; i++) {
    if(winners[i] == 1) {
      printf("Test 1: player %i = %d, expected = %d\n", i, players[i], 1);
      assertTrue(players[i] == 1, "Test 1");
    } else {
      printf("Test 1: player %i = %d, expected != %d\n", i, players[i], 1);
      assertTrue(players[i] != 1, "Test 1");
    }
  }
}

int main() {
  int seed = 100;
  int numPlayers = 2;
  struct gameState G;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
      sea_hag, tribute, smithy, council_room};

  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----------------- Testing Function: %s ----------------\n", TESTFUNCTION);

  int winners[2] = {1,0};

  printf("TEST SUIT 1: player 0 is winner\n");
  testOne(&G, numPlayers, winners);

  printf("\nTEST SUIT 2: extra player\n");
  testOne(&G, 3, winners);

  winners[1] = 1;
  printf("\nTEST SUIT 3: player 0 and 1 are tied\n");
  testOne(&G, numPlayers, winners);
}
