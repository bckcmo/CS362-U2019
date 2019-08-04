/*
 * unittest5.c
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

#define TESTCARD "mine"

int playerStateUnchanged(int player, struct gameState *game, struct gameState *testGame) {
  int ret = 1;

  if(game->handCount[player] != testGame->handCount[player]) {
    printf("  Player %i's hand count changed.\n", player);
    ret = 0;
  }

  if(game->deckCount[player] != testGame->deckCount[player]) {
    printf("  Player %i's deck count changed.\n", player);
    ret = 0;
  }

  if(game->discardCount[player] != testGame->discardCount[player]) {
    printf("  Player %i's discard count changed.\n", player);
    ret = 0;
  }

  for(int i = 0; i < game->handCount[player]; i++) {
    if(game->hand[player][i] != testGame->hand[player][i]) {
      printf("  Player %i's hand changed at position %i.\n", player, i);
      ret = 0;
    }
  }

  for(int i = 0; i < game->deckCount[player]; i++) {
    if(game->deck[player][i] != testGame->deck[player][i]) {
      printf("  Player %i's deck changed at position %i.\n", player, i);
      ret = 0;
    }
  }

  for(int i = 0; i < game->discardCount[player]; i++) {
    if(game->discard[player][i] != testGame->discard[player][i]) {
      printf("  Player %i's discard pile changed at position %i.\n", player, i);
      ret = 0;
    }
  }

  return ret;
}

void assertTrue(int result, char* test) {
  if(!result) {
    printf("%s failed\n", test);
  } else {
    printf("%s passed\n", test);
  }
}

void testOne(struct gameState *game, int numPlayers, int choice1, int choice2) {
  struct gameState testGame;
  int handPos = 0;
  int discarded = 2; // mine plus choice2
  int currentPlayer = 0;
  int silverBefore = 0;
  int silverAfter = 0;

  //make sure player has choice1
  game->hand[currentPlayer][0] = choice1;

  //copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));

  int cardPlayed = mineEffect(choice1, choice2, currentPlayer, &testGame, handPos);

  for(int i = 0; i < game->handCount[currentPlayer]; i++) {
    if(game->hand[currentPlayer][i] == silver) {
      silverBefore++;
    }
  }

  for(int i = 0; i < testGame.handCount[currentPlayer]; i++) {
    if(testGame.hand[currentPlayer][i] == silver) {
      silverAfter++;
    }
  }

  //card should be played
  printf("Test 0: card is played\n");
  assertTrue(cardPlayed == 0, "Test 0");

  //player should have -1 hand count
  printf("Test 1: hand count = %d, expected = %d\n", testGame.handCount[currentPlayer], game->handCount[currentPlayer] - discarded + 1);
  assertTrue(testGame.handCount[currentPlayer] == game->handCount[currentPlayer] - discarded + 1, "Test 1");

  //played pile should have +2 cards
  printf("Test 2: cards in played pile = %d, expected = %d\n", testGame.playedCardCount, game->playedCardCount + discarded);
  assertTrue(testGame.playedCardCount == game->playedCardCount + discarded, "Test 2");

  //player should have same number of coins
  printf("Test 3: coins = %d, expected = %d\n", testGame.coins, game->coins);
  assertTrue(testGame.coins == game->coins, "Test 3");

  //player should have same number of actions
  printf("Test 4: actions = %d, expected = %d\n", testGame.numActions, game->numActions);
  assertTrue(testGame.numActions == game->numActions, "Test 4");

  //player should have +1 silver
  printf("Test 5: silver = %d, expected = %d\n", silverAfter, silverBefore + 1);
  assertTrue(silverBefore + 1 == silverAfter, "Test 5");

  //supply should have -1 choice2
  printf("Test 6: supply = %d, expected = %d\n", testGame.supplyCount[choice2], game->supplyCount[choice2] - 1);
  assertTrue(testGame.supplyCount[choice2] == game->supplyCount[choice2] - 1, "Test 6");
}

void testTwo(struct gameState *game, int numPlayers, int choice1, int choice2) {
  struct gameState testGame;
  int handPos = 0;
  int currentPlayer = 0;
  int goldBefore = 0;
  int goldAfter = 0;

  //make sure player has choice1
  game->hand[currentPlayer][0] = choice1;

  //copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));

int cardPlayed = mineEffect(choice1, choice2, currentPlayer, &testGame, handPos);

  for(int i = 0; i < game->handCount[currentPlayer]; i++) {
    if(game->hand[currentPlayer][i] == gold) {
      goldBefore++;
    }
  }

  for(int i = 0; i < testGame.handCount[currentPlayer]; i++) {
    if(testGame.hand[currentPlayer][i] == gold) {
      goldAfter++;
    }
  }

  //card should be played
  printf("Test 0: card is not played\n");
  assertTrue(cardPlayed != 0, "Test 0");

  //player should same hand count
  printf("Test 1: hand count = %d, expected = %d\n", testGame.handCount[currentPlayer], game->handCount[currentPlayer]);
  assertTrue(testGame.handCount[currentPlayer] == game->handCount[currentPlayer], "Test 1");

  //played pile should same cards
  printf("Test 2: cards in played pile = %d, expected = %d\n", testGame.playedCardCount, game->playedCardCount);
  assertTrue(testGame.playedCardCount == game->playedCardCount, "Test 2");

  //player should have same number of coins
  printf("Test 3: coins = %d, expected = %d\n", testGame.coins, game->coins);
  assertTrue(testGame.coins == game->coins, "Test 3");

  //player should have same number of actions
  printf("Test 4: actions = %d, expected = %d\n", testGame.numActions, game->numActions);
  assertTrue(testGame.numActions == game->numActions, "Test 4");

  //player should same number of gold
  printf("Test 5: silver = %d, expected = %d\n", goldBefore, goldAfter);
  assertTrue(goldBefore == goldAfter, "Test 5");

  //supply should be same for choice2
  printf("Test 6: supply = %d, expected = %d\n", testGame.supplyCount[choice2], game->supplyCount[choice2]);
  assertTrue(testGame.supplyCount[choice2] == game->supplyCount[choice2], "Test 6");

}

int main() {
  int seed = 100;
  int numPlayers = 2;
  struct gameState G;
  int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
      sea_hag, tribute, smithy, council_room};


  // initialize a game state and player cards
  initializeGame(numPlayers, k, seed, &G);

  printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

  printf("TEST SUITE 1: player trashes copper, gains silver\n");
  testOne(&G, numPlayers, copper, silver);

  printf("\nTEST SUITE 2: player trashes copper, tries to gain gold\n");
  testTwo(&G, numPlayers, copper, gold);

  printf("\nTEST SUITE 3: player trashes copper, tries to gain non treasure card\n");
  testTwo(&G, numPlayers, copper, province);

  printf("\nTEST SUITE 4: player trashes non treasure card\n");
  testTwo(&G, numPlayers, curse, gold);

}
