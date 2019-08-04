/*
 * cardtest2.c
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

#define TESTFUNCTION "shuffle"

void assertTrue(int result, char* test) {
  if(!result) {
    printf("%s failed\n", test);
  } else {
    printf("%s passed\n", test);
  }
}

void testOne(struct gameState *game) {
  struct gameState testGame;
  int changed = 0, currentPlayer = 0;

  // copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));
  // initialize a game state and player cards
  int ret = shuffle(0, &testGame);

  //succeeds
  printf("Test 0: shuffle succeeds\n");
  assertTrue(ret == 0 , "Test 0");

  //verify at least one card changed position in deck
  for(int i = 0; i < testGame.deckCount[currentPlayer]; i++) {
    if(testGame.deck[currentPlayer][i] != game->deck[currentPlayer][i]) {
      changed = 1;
    }
  }
  printf("Test 1: deck changed\n");
  assertTrue(changed, "Test 1");

  //deck count was not changed
  printf("Test 2: deck count = %d, expected = %d\n", testGame.deckCount[currentPlayer], game->deckCount[currentPlayer]);
  assertTrue(testGame.deckCount[currentPlayer] == game->deckCount[currentPlayer], "Test 2");

}

void testTwo(struct gameState *game) {
  struct gameState testGame;
  int changed = 0, currentPlayer = 0;

  game->deckCount[0] = 0;
  // copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));
  int ret = shuffle(0, &testGame);

  //fails
  printf("Test 0: shuffle fails\n");
  assertTrue(ret != 0 , "Test 0");

  //verify deck did not change
  for(int i = 0; i < testGame.deckCount[currentPlayer]; i++) {
    if(testGame.deck[currentPlayer][i] != game->deck[currentPlayer][i]) {
      changed = 1;
    }
  }
  printf("Test 1: deck changed\n");
  assertTrue(!changed, "Test 1");

  printf("Test 2: deck count = %d, expected = %d\n", testGame.deckCount[currentPlayer], game->deckCount[currentPlayer]);
  assertTrue(testGame.deckCount[currentPlayer] == game->deckCount[currentPlayer], "Test 2");

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

  printf("TEST SUITE 1: shuffle\n");
  testOne(&G);

  printf("\nTEST SUITE 2: shuffle with deck count at 0\n");
  testTwo(&G);

}
