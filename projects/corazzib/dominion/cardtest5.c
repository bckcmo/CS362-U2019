/*
 * cardtest5.c
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

#define TESTFUNCTION "drawCard"

void assertTrue(int result, char* test) {
  if(!result) {
    printf("%s failed\n", test);
  } else {
    printf("%s passed\n", test);
  }
}

void test(struct gameState *game, struct gameState *testGame, int player) {
  int ret = drawCard(player, testGame);

  //succeeds
  printf("Test 0: drawCard succeeds\n");
  assertTrue(ret == 0 , "Test 0");

  //hand count increased by 1
  printf("Test 1: hand count = %d, expected = %d\n", testGame->handCount[player], game->handCount[player] + 1);
  assertTrue(testGame->handCount[player] == game->handCount[player] + 1, "Test 1");

  //hand is different (i.e. there is indeed a new card there)
  int changed = 0;
  for(int i = 0; i < testGame->handCount[player]; i++) {
    if(testGame->hand[player][i] != game->hand[player][i]) {
      changed = 1;
    }
  }

  printf("Test 2: hand changed = %d, expected = %d\n", changed, 1);
  assertTrue(changed == 1, "Test 2");
}

int main() {
  int seed = 100;
  int numPlayers = 2;
  int player = 0;
  struct gameState G;
  struct gameState testGame;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
      sea_hag, tribute, smithy, council_room};

  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  // copy the game state to a test case
  memcpy(&testGame, &G, sizeof(struct gameState));

  printf("----------------- Testing Function: %s ----------------\n", TESTFUNCTION);

  printf("TEST SUIT 1: player 0 draws card\n");
  test(&G, &testGame, player);

  //set state for player 1 so they have 0 cards in deck, but cards in discard pile
  srand(time(0));
  int card = rand() % 16 + 1;
  player = 1;
  testGame.deckCount[player] = 0;
  for(int i = 0; i < 10; i++) {
    card = rand() % 5 + 1;
    testGame.discardCount[player] = 10;
    testGame.discard[player][i] = card;
  }

  printf("\nTEST SUIT 2: player 1 draws card\n");
  test(&G, &testGame, player);
}
