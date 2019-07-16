/*
 * cardtest3.c
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

#define TESTFUNCTION "endTurn"

void assertTrue(int result, char* test) {
  if(!result) {
    printf("%s failed\n", test);
  } else {
    printf("%s passed\n", test);
  }
}

void testOne(struct gameState *game) {
  struct gameState testGame;
  int currentPlayer = 0, nextPlayer = currentPlayer + 1;

  // copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));
  // initialize a game state and player cards
  int ret = endTurn(&testGame);

  //succeeds
  printf("Test 0: endturn succeeds\n");
  assertTrue(ret == 0 , "Test 0");

  //current player discards hand
  printf("Test 1: discard count current player = %d, expected = %d\n", testGame.discardCount[currentPlayer], game->discardCount[currentPlayer] + game->handCount[currentPlayer]);
  assertTrue(testGame.discardCount[currentPlayer] == game->discardCount[currentPlayer] + game->handCount[currentPlayer], "Test 1");

  //current player hand count = 0
  printf("Test 2: current player handcount = %d, expected = %d\n", testGame.handCount[currentPlayer], 0);
  assertTrue(0 == testGame.handCount[currentPlayer], "Test 2");

  //check next player state
  printf("Test 3: player 1 actions = %d, expected = %d\n", testGame.numActions, 1);
  assertTrue(testGame.numActions == 1, "Test 3");

  printf("Test 4: player 1 buys = %d, expected = %d\n", testGame.numBuys, 1);
  assertTrue(testGame.numBuys == 1, "Test 4");

  printf("Test 5: player 1 played card count = %d, expected = %d\n", testGame.playedCardCount, 0);
  assertTrue(testGame.playedCardCount == 0, "Test 5");


  printf("Test 6: player 1 hand count = %d, expected = %d\n", testGame.playedCardCount, 0);
  assertTrue(testGame.playedCardCount == 0, "Test 6");


  printf("Test 7: whose turn = %d, expected = %d\n", testGame.whoseTurn, nextPlayer);
  assertTrue(testGame.whoseTurn == nextPlayer, "Test 7");

  printf("Test 7: next player hand count = %d, expected = %d\n", testGame.handCount[nextPlayer], 5);
  assertTrue(testGame.handCount[nextPlayer] == 5, "Test 7");
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

  //setup game state for test
  // G.
  printf("TEST SUIT 1: end turn\n");
  testOne(&G);

}
