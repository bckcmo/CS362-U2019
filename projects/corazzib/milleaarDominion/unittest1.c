/*
 * unittest1.c
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

#define TESTCARD "baron"

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
  srand(time(0));
  int randCard = rand() % 5 + 1;
  int estatesHandBefore = 0, estatesHandAfter = 0;
  int discarded = 2; // estate + baron which was played
  int currentPlayer = 0;
  int newCards = 0;

  // copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));

  //replace card with an estate so we know for sure that the player has at least one
  testGame.hand[currentPlayer][randCard] = estate;

  for(int i = 0; i < testGame.handCount[currentPlayer]; i++) {
    if(testGame.hand[currentPlayer][i] == estate) {
      estatesHandBefore++;
    }
  }

  int cardPlayed = baronEffect(choice1, &testGame);

  for(int i = 0; i < testGame.handCount[currentPlayer]; i++) {
    if(testGame.hand[currentPlayer][i] == estate) {
      estatesHandAfter++;
    }
  }

  //card should be played
  printf("Test 0: card is played\n");
  assertTrue(cardPlayed == 0, "Test 0");

  //player should have -2 cards (-1 estate, -1 baron)
  printf("Test 1: hand count = %d, expected = %d\n", testGame.handCount[currentPlayer], game->handCount[currentPlayer] + newCards - discarded);
  assertTrue(testGame.handCount[currentPlayer] == game->handCount[currentPlayer] + newCards - discarded, "Test 1");

  //player should have -1 estates in hand
  printf("Test 2: estates in hand = %d, expected = %d\n", estatesHandBefore - estatesHandAfter, estatesHandBefore - 1);
  assertTrue(estatesHandBefore - estatesHandAfter == estatesHandBefore - 1, "Test 2");

  //played pile should have +2 cards
  printf("Test 3: cards in played pile = %d, expected = %d\n", testGame.playedCardCount, game->playedCardCount + discarded);
  assertTrue(testGame.playedCardCount == game->playedCardCount + discarded, "Test 3");

  //player should have +4 coins
  printf("Test 4: coins = %d, expected = %d\n", testGame.coins, game->coins + 4);
  assertTrue(testGame.coins == game->coins + 4, "Test 4");

  //player should have +1  buys
  printf("Test 5: buys = %d, expected = %d\n", testGame.numBuys, game->numBuys + 1);
  assertTrue(testGame.numBuys == game->numBuys + 1, "Test 5");

  //other player should not be effected
  printf("Test 6: no state change on other players\n");
  int changed = 1;
  for(int i = 0; i < numPlayers; i++) {
    if(i != currentPlayer) {
      if(playerStateUnchanged(i, game, &testGame) == 0) {
        changed = 0;
      }
    }
  }
  assertTrue(changed, "Test 6");

  //supply should have same number of estates
  printf("Test 7: estate supply = %d, expected = %d\n", testGame.supplyCount[estate], game->supplyCount[estate]);
  assertTrue(testGame.supplyCount[estate] == game->supplyCount[estate], "Test 7");
}

void testTwo(struct gameState *game, int numPlayers) {
  struct gameState testGame;
  int currentPlayer = 0;
  int choice1 = 1;
  int newCards = 1; //the new estate
  int discarded = 1; //the baron that was played
  int estatesHandBefore = 0, estatesHandAfter = 0;


  // copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));

  //replace estates with something else (copper)
  for(int i = 0; i < testGame.handCount[currentPlayer]; i++) {
    if(testGame.hand[currentPlayer][i] == estate) {
      testGame.hand[currentPlayer][i] = copper;
    }
  }

  int cardPlayed = baronEffect(choice1, &testGame);

  for(int i = 0; i < testGame.handCount[currentPlayer]; i++) {
    if(testGame.hand[currentPlayer][i] == estate) {
      estatesHandAfter++;
    }
  }

  //card should be played
  printf("Test 0: card is played\n");
  assertTrue(cardPlayed == 0 , "Test 0");

  //player should have same number of cards (-1 baron, +1 estate)
  printf("Test 1: hand count = %d, expected = %d\n", testGame.handCount[currentPlayer], game->handCount[currentPlayer] + newCards - discarded);
  assertTrue(testGame.handCount[currentPlayer] == game->handCount[currentPlayer] + newCards - discarded, "Test 1");

  //player should have +1 estate
  printf("Test 2: estates in hand = %d, expected = %d\n", estatesHandAfter, estatesHandBefore + 1);
  assertTrue(estatesHandAfter == estatesHandBefore + 1, "Test 2");

  //played pile should have +1 cards
  printf("Test 3: cards in played pile = %d, expected = %d\n", testGame.playedCardCount, game->playedCardCount + discarded);
  assertTrue(testGame.playedCardCount == game->playedCardCount + discarded, "Test 3");

  //player should have same number of coins
  printf("Test 4: coins = %d, expected = %d\n", testGame.coins, game->coins);
  assertTrue(testGame.coins == game->coins, "Test 4");

  //player should have +1 buys
  printf("Test 5: buys = %d, expected = %d\n", testGame.numBuys, game->numBuys + 1);
  assertTrue(testGame.numBuys == game->numBuys + 1, "Test 5");

  //no other player should be effected
  printf("Test 6: no state change on other players\n");
  int changed = 1;
  for(int i = 0; i < numPlayers; i++) {
    if(i != currentPlayer) {
      if(playerStateUnchanged(i, game, &testGame) == 0) {
        changed = 0;
      }
    }
  }
  assertTrue(changed, "Test 6");

  //supply should have -1 estate
  printf("Test 7: estate supply = %d, expected = %d\n", testGame.supplyCount[estate], game->supplyCount[estate] - 1);
  assertTrue(testGame.supplyCount[estate] == game->supplyCount[estate] - 1, "Test 7");
}

void testThree(struct gameState *game, int numPlayers) {
  struct gameState testGame;
  int currentPlayer = 0;
  int choice1 = 0;
  int newCards = 1; //the new estate
  int discarded = 1; //the baron that was played
  int estatesHandBefore = 0, estatesHandAfter = 0;


  // copy the game state to a test case
  memcpy(&testGame, game, sizeof(struct gameState));

  for(int i = 0; i < testGame.handCount[currentPlayer]; i++) {
    if(testGame.hand[currentPlayer][i] == estate) {
      estatesHandBefore++;
    }
  }

  int cardPlayed = baronEffect(choice1, &testGame);

  for(int i = 0; i < testGame.handCount[currentPlayer]; i++) {
    if(testGame.hand[currentPlayer][i] == estate) {
      estatesHandAfter++;
    }
  }

  //card should be played
  printf("Test 0: card is played\n");
  assertTrue(cardPlayed == 0 , "Test 0");

  //player should have same number of cards (-1 baron, +1 estate)
  printf("Test 1: hand count = %d, expected = %d\n", testGame.handCount[currentPlayer], game->handCount[currentPlayer] + newCards - discarded);
  assertTrue(testGame.handCount[currentPlayer] == game->handCount[currentPlayer] + newCards - discarded, "Test 1");

  //player should have +1 estate
  printf("Test 2: estates in hand = %d, expected = %d\n", estatesHandAfter, estatesHandBefore + 1);
  assertTrue(estatesHandAfter == estatesHandBefore + 1, "Test 2");

  //played pile should have +1 cards
  printf("Test 3: cards in played pile = %d, expected = %d\n", testGame.playedCardCount, game->playedCardCount + discarded);
  assertTrue(testGame.playedCardCount == game->playedCardCount + discarded, "Test 3");

  //player should have same number of coins
  printf("Test 4: coins = %d, expected = %d\n", testGame.coins, game->coins);
  assertTrue(testGame.coins == game->coins, "Test 4");

  //player should have +1 buys
  printf("Test 5: buys = %d, expected = %d\n", testGame.numBuys, game->numBuys + 1);
  assertTrue(testGame.numBuys == game->numBuys + 1, "Test 5");

  //no other player should be effected
  printf("Test 6: no state change on other players\n");
  int changed = 1;
  for(int i = 0; i < numPlayers; i++) {
    if(i != currentPlayer) {
      if(playerStateUnchanged(i, game, &testGame) == 0) {
        changed = 0;
      }
    }
  }
  assertTrue(changed, "Test 6");

  //supply should have -1 estate
  printf("Test 7: estate supply = %d, expected = %d\n", testGame.supplyCount[estate], game->supplyCount[estate] - 1);
  assertTrue(testGame.supplyCount[estate] == game->supplyCount[estate] - 1, "Test 7");
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

  printf("TEST SUITE 1: player has estate, discards estate, +4 coins, +1 buy\n");
  testOne(&G, numPlayers);

  printf("\nTEST SUITE 2: player does not have estate, discards estate\n");
  testTwo(&G, numPlayers);

  printf("\nTEST SUITE 3: player chooses to not discard estate\n");
  testThree(&G, numPlayers);

}
