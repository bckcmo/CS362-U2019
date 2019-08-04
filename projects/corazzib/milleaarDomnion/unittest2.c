/*
 * unittest2.c
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

#define TESTCARD "minion"

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

void testOne(struct gameState *game, int numPlayers) {
  struct gameState testGame;
  int choice1 = 1;
  int choice2 = 0;
  srand(time(0));
  int handPos = rand() % 5 + 1;
  int discarded = 1; // minion
  int currentPlayer = 0;
  int newCards = 0;

  // copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));

  //replace card with a minion
  testGame.hand[currentPlayer][handPos] = minion;

  int cardPlayed = minionEffect(choice1, choice2, &testGame, handPos);

  //card should be played
  printf("Test 0: card is played\n");
  assertTrue(cardPlayed == 0, "Test 0");

  //player should have -1 cards (-1 minion)
  printf("Test 1: hand count = %d, expected = %d\n", testGame.handCount[currentPlayer], game->handCount[currentPlayer] + newCards - discarded);
  assertTrue(testGame.handCount[currentPlayer] == game->handCount[currentPlayer] + newCards - discarded, "Test 1");

  //played pile should have +1 cards
  printf("Test 2: cards in played pile = %d, expected = %d\n", testGame.playedCardCount, game->playedCardCount + discarded);
  assertTrue(testGame.playedCardCount == game->playedCardCount + discarded, "Test 2");

  //player should have +2 coins
  printf("Test 3: coins = %d, expected = %d\n", testGame.coins, game->coins + 2);
  assertTrue(testGame.coins == game->coins + 2, "Test 3");

  //player should have +1  action
  printf("Test 4: actions = %d, expected = %d\n", testGame.numActions, game->numActions + 1);
  assertTrue(testGame.numActions == game->numActions + 1, "Test 4");

  //other player should not be effected
  printf("Test 5: no state change on other players\n");
  int changed = 1;
  for(int i = 0; i < numPlayers; i++) {
    if(i != currentPlayer) {
      if(playerStateUnchanged(i, game, &testGame) == 0) {
        changed = 0;
      }
    }
  }
  assertTrue(changed, "Test 5");
}

void testTwo(struct gameState *game, int numPlayers) {
  struct gameState testGame;
  int currentPlayer = 0;
  int choice1 = 0;
  int choice2 = 1;
  srand(time(0));
  int handPos = rand() % 5 + 1;
  int newCards = 4;
  int discarded = 1; //minion

  // copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));

  //replace card with a minion
  testGame.hand[currentPlayer][handPos] = minion;

  int cardPlayed = minionEffect(choice1, choice2, &testGame, handPos);

  discarded += game->handCount[currentPlayer];

  //card should be played
  printf("Test 0: card is played\n");
  assertTrue(cardPlayed == 0 , "Test 0");

  //player should have 4 cards in hand
  printf("Test 1: hand count = %d, expected = %d\n", testGame.handCount[currentPlayer], newCards);
  assertTrue(testGame.handCount[currentPlayer] == newCards, "Test 1");

  //player should have same number of coins
  printf("Test 2: coins = %d, expected = %d\n", testGame.coins, game->coins);
  assertTrue(testGame.coins == game->coins, "Test 2");

  //player should have +1  action
  printf("Test 3: actions = %d, expected = %d\n", testGame.numActions, game->numActions + 1);
  assertTrue(testGame.numActions == game->numActions + 1, "Test 3");

  //other players should have 4 cards in hand and each player should add their handcount to playedCardCount
  printf("Test 4: state change on other players\n");
  int changed = 1;
  for(int i = 0; i < numPlayers; i++) {
    if(i != currentPlayer && game->handCount[i] > 4) {
      discarded += game->handCount[i];
      if(testGame.handCount[i] != newCards ) {
        printf("  Player %i hand count = %d, expected = %d\n", i, testGame.handCount[i], newCards);
        changed = 0;
      }
    } else if(i != currentPlayer) {
      if(playerStateUnchanged(i, game, &testGame) == 0) {
        changed = 0;
      }
    }
  }
  assertTrue(changed, "Test 4");

  //played pile should have +1 cards + all cards from hand
  printf("Test 5: cards in played pile = %d, expected = %d\n", testGame.playedCardCount, discarded);
  assertTrue(testGame.playedCardCount == discarded, "Test 5");

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

  printf("TEST SUITE 1: player chooses +2 coins\n");
  testOne(&G, numPlayers);

  printf("\nTEST SUITE 2: player chooses to discard hand\n");
  testTwo(&G, numPlayers);

}
