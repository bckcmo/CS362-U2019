/*
 * unittest4.c
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

#define TESTCARD "tribute"

void assertTrue(int result, char* test) {
  if(!result) {
    printf("%s failed\n", test);
  } else {
    printf("%s passed\n", test);
  }
}

void testOne(struct gameState *game, int numPlayers, int reveal1, int reveal2) {
  struct gameState testGame;
  int handPos = 0;
  int discarded = 1; // tribute
  int currentPlayer = 0;
  int nextPlayer = currentPlayer + 1;
  if (nextPlayer > (game->numPlayers - 1)){
    nextPlayer = 0;
  }

  // copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));

  //set top two cards of next players deck
  testGame.hand[nextPlayer][0] = reveal1;
  testGame.hand[nextPlayer][1] = reveal2;

  //replace card with an tribute
  testGame.hand[currentPlayer][handPos] = tribute;

  int cardPlayed = tributeEffect(currentPlayer, nextPlayer, &testGame);

  //card should be played
  printf("Test 0: card is played\n");
  assertTrue(cardPlayed == 0, "Test 0");

  //player should have +2 cards - discarded if revealedCard is victory card
  if(isType(reveal1, victory_card) || isType(reveal2, victory_card)) {
    printf("Test 1: hand count = %d, expected = %d\n", testGame.handCount[currentPlayer], game->handCount[currentPlayer] - discarded + 2);
    assertTrue(testGame.handCount[currentPlayer] == game->handCount[currentPlayer] - discarded + 2, "Test 1");
  } else {
    printf("Test 1: hand count = %d, expected = %d\n", testGame.handCount[currentPlayer], game->handCount[currentPlayer] - discarded);
    assertTrue(testGame.handCount[currentPlayer] == game->handCount[currentPlayer] - discarded, "Test 1");
  }

  //played pile should have +3 cards, one from currentPlayer 2 from nextPlayer
  printf("Test 2: cards in played pile = %d, expected = %d\n", testGame.playedCardCount, game->playedCardCount + discarded + 2);
  assertTrue(testGame.playedCardCount == game->playedCardCount + discarded + 2, "Test 2");

  //player should have +2 coins if revealedCard is treasure card
  if(isType(reveal1, treasure_card) || isType(reveal2, treasure_card)) {
    printf("Test 3: coins = %d, expected = %d\n", testGame.coins, game->coins + 2);
    assertTrue(testGame.coins == game->coins + 2, "Test 3");
  } else {
    printf("Test 3: coins = %d, expected = %d\n", testGame.coins, game->coins);
    assertTrue(testGame.coins == game->coins, "Test 3");
  }

  //player should have +2 actions if revealedCard is action card
  if(isType(reveal1, action_card) || isType(reveal2, action_card)) {
    printf("Test 4: actions = %d, expected = %d\n", testGame.numActions, game->numActions + 2);
    assertTrue(testGame.numActions == game->numActions + 2, "Test 4");
  } else {
    printf("Test 4: actions = %d, expected = %d\n", testGame.numActions, game->numActions);
    assertTrue(testGame.numActions == game->numActions, "Test 4");
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

  printf("----------------- Testing Card: %s ----------------\n", TESTCARD);

  printf("TEST SUITE 1: player plays card\n");
  testOne(&G, numPlayers, curse, ambassador);

  printf("TEST SUITE 2: player plays card\n");
  testOne(&G, numPlayers, copper, estate);

  printf("TEST SUITE 3: player plays card\n");
  testOne(&G, numPlayers, gardens, minion);

}
