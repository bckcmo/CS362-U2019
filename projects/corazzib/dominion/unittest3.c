/*
 * unittest3.c
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

#define TESTCARD "ambassador"

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

void testOne(struct gameState *game, int numPlayers, int choice2) {
  struct gameState testGame;
  int handPos = 0;
  int choice1 = handPos + 1;
  int discarded = 1; // ambassador
  int currentPlayer = 0;
  int revealedCard = province;
  int cardsGained = game->numPlayers - 1; //represents the number of revealed card drawn by other players

  // copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));

  //replace card with an ambassador
  testGame.hand[currentPlayer][handPos] = ambassador;

  //give player copies of card to reveal
  for(int i = 0; i < choice2; i++) {
    testGame.hand[currentPlayer][choice1 + i] = revealedCard;
  }

  int cardPlayed = playAmbassador(currentPlayer, choice1, choice2, handPos, &testGame);

  //card should be played
  printf("Test 0: card is played\n");
  assertTrue(cardPlayed == 0, "Test 0");

  //player should have -2 cards (-1 ambassador, -1 estate)
  printf("Test 1: hand count = %d, expected = %d\n", testGame.handCount[currentPlayer], game->handCount[currentPlayer] - discarded - choice2);
  assertTrue(testGame.handCount[currentPlayer] == game->handCount[currentPlayer] - discarded - choice2, "Test 1");

  //played pile should have +2 cards
  printf("Test 2: cards in played pile = %d, expected = %d\n", testGame.playedCardCount, game->playedCardCount + discarded + choice2);
  assertTrue(testGame.playedCardCount == game->playedCardCount + discarded + choice2, "Test 2");

  //player should have same number of coins
  printf("Test 3: coins = %d, expected = %d\n", testGame.coins, game->coins);
  assertTrue(testGame.coins == game->coins, "Test 3");

  //player should have same number of actions
  printf("Test 4: actions = %d, expected = %d\n", testGame.numActions, game->numActions);
  assertTrue(testGame.numActions == game->numActions, "Test 4");

  //supply should have (+choice2 - (choice2 * numPlayers - currentPlayer)) more cards of choice1
  //this also tells us if the other players gained the card
  printf("Test 5: supply = %d, expected = %d\n", testGame.supplyCount[revealedCard], game->supplyCount[revealedCard] + choice2 - cardsGained);
  assertTrue(testGame.supplyCount[revealedCard] == game->supplyCount[revealedCard] + choice2 - cardsGained, "Test 5");
}

void testTwo(struct gameState *game, int numPlayers, int choice2) {
  struct gameState testGame;
  int handPos = 0;
  int choice1 = handPos;
  int currentPlayer = 0;
  int revealedCard = estate;

  //replace card with an ambassador
  game->hand[currentPlayer][handPos] = ambassador;

  //give player copies of card to reveal
  for(int i = 0; i < choice2; i++) {
    game->hand[currentPlayer][choice1 + i] = revealedCard;
  }

  // copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));

  int cardPlayed = playAmbassador(currentPlayer, choice1, choice2, handPos, &testGame);

  //card should be played
  printf("Test 0: card should not be played\n");
  assertTrue(cardPlayed != 0, "Test 0");

  //state should not change
  printf("Test 1: state should not change\n");
  int changed = 1;
  for(int i = 0; i < numPlayers; i++) {
    if(playerStateUnchanged(i, game, &testGame) == 0) {
      changed = 0;
    }
  }
  assertTrue(changed, "Test 1");

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

  printf("TEST SUITE 1: player return 1 copy\n");
  testOne(&G, numPlayers, 1);

  printf("\nTEST SUITE 2: player returns 2 copies\n");
  testOne(&G, numPlayers, 2);

  printf("\nTEST SUITE 3: player returns 3 copies\n");
  testOne(&G, numPlayers, 3);

  printf("\nTEST SUITE 4: player reveals ambassador card\n");
  testTwo(&G, numPlayers, 2);

}
