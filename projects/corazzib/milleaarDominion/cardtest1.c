/*
 * cardtest1.c
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

#define TESTFUNCTION "initializeGame"

void assertTrue(int result, char* test) {
  if(!result) {
    printf("%s failed\n", test);
  } else {
    printf("%s passed\n", test);
  }
}

void testOne(int numPlayers, int *k, int seed, struct gameState *game) {
  int vc = 0, cc = 0, copperC = 0, silverC = 40, goldC = 30, ac = 10, currentPlayer = 0;

  switch(numPlayers) {
    case 2:
      vc = 8;
      cc = 10;
      copperC = 60 - (7 * numPlayers);
      break;

    case 3:
      vc = 12;
      cc = 20;
      copperC = 60 - (7 * numPlayers);
      break;

    default:
      vc = 12;
      cc = 30;
      copperC = 60 - (7 * numPlayers);
      break;
  }
  // initialize a game state and player cards
  int ret = initializeGame(numPlayers, k, seed, game);

  //number of players
  printf("Test 0: players = %i, expected %i\n", game->numPlayers, numPlayers);
  assertTrue(game->numPlayers == numPlayers , "Test 0");

  //number of victory cards
  for(int i = estate; i <= province; i++) {
    printf("Test 1: victory cards for card #%i = %d, expected = %d\n", i, game->supplyCount[i], vc);
    assertTrue(game->supplyCount[i] == vc, "Test 1");
  }

  //number of curse cards
  printf("Test 2: curses = %d, expected = %d\n", game->supplyCount[curse], cc);
  assertTrue(game->supplyCount[curse] == cc, "Test 2");

  //number of action cards
  for(int i = 0; i < 10; i++) {
    int expected = k[i] == gardens || k[i] ==  great_hall ? vc : ac;
    printf("Test 3: action cards for card #%i = %d, expected = %d\n", k[i], game->supplyCount[k[i]], expected);
    assertTrue(game->supplyCount[k[i]] == expected, "Test 3");
  }

  //number of copper
  printf("Test 4: copper = %d, expected = %d\n", game->supplyCount[copper], copperC);
  assertTrue(game->supplyCount[copper] == copperC, "Test 4");

  //number of silver
  printf("Test 5: silver = %d, expected = %d\n", game->supplyCount[silver], silverC);
  assertTrue(game->supplyCount[silver] == silverC, "Test 5");

  //number of gold
  printf("Test 6: gold = %d, expected = %d\n", game->supplyCount[gold], goldC);
  assertTrue(game->supplyCount[gold] == goldC, "Test 6");

  //check player state
  printf("Test 7: player 1 actions = %d, expected = %d\n", game->numActions, 1);
  assertTrue(game->numActions == 1, "Test 7");

  printf("Test 8: player 1 buys = %d, expected = %d\n", game->numBuys, 1);
  assertTrue(game->numBuys == 1, "Test 8");

  printf("Test 9: player 1 coins = %d, expected > %d\n", game->coins, 0);
  assertTrue(game->coins > 0, "Test 9");

  printf("Test 10: player 1 hand count = %d, expected = %d\n", game->handCount[currentPlayer], 5);
  assertTrue(game->handCount[currentPlayer] == 5, "Test 10");

  printf("Test 11: whose turn = %d, expected = %d\n", game->whoseTurn, currentPlayer);
  assertTrue(game->whoseTurn == currentPlayer, "Test 11");

  //success
  printf("Test 12: initialize succeeded\n");
  assertTrue(ret == 0 , "Test 12");
}

void testTwo(int numPlayers, int *k, int seed, struct gameState *game) {
  // initialize a game state and player cards
  int ret = initializeGame(numPlayers, k, seed, game);

  //failed
  printf("Test 0: initialize failed\n");
  assertTrue(ret != 0 , "Test 0");
}

int main() {
  int seed = 100;
  struct gameState G;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
      sea_hag, tribute, smithy, council_room};

  printf("----------------- Testing Function: %s ----------------\n", TESTFUNCTION);

  printf("TEST SUITE 1: 2 players\n");
  testOne(2, k, seed, &G);

  printf("\nTEST SUITE 2: 3 players\n");
  testOne(3, k, seed, &G);

  printf("\nTEST SUITE 2: 4 players\n");
  testOne(4, k, seed, &G);

  printf("\nTEST SUITE 3: 30 players\n");
  testTwo(30, k, seed, &G);

  printf("\nTEST SUITE 4: 0 players\n");
  testTwo(0, k, seed, &G);

  k[1] = adventurer;
  k[2] = adventurer;

  printf("\nTEST SUITE 5: repeat kingdom cards\n");
  testTwo(30, k, seed, &G);

}
