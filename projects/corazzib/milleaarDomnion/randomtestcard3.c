#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "math.h"

#define NOISY_TEST 1

int countCard(int card, int currentPlayer, struct gameState *game) {
  int count = 0;
  for(int i = 0; i < game->handCount[currentPlayer]; i++) {
    if(game->hand[currentPlayer][i] == card) {
      count++;
    }
  }
  return count;
}

void assertTrue(int result) {
  if(result) {
    printf("Test passed\n");
  } else {
    printf("Test failed\n");
  }
}

int checkPlayTribute(int currentPlayer, int nextPlayer, struct gameState *post) {
  int r, discarded = 3, newCards = 0, newCoins = 0, newActions = 0, reveal1, reveal2;
  struct gameState pre;
  memcpy(&pre, post, sizeof(struct gameState));

  r = tributeEffect(currentPlayer, nextPlayer, post);

  reveal1 = pre.hand[nextPlayer][0];
  reveal2 = pre.hand[nextPlayer][1];

  if(isType(reveal1, victory_card) || isType(reveal2, victory_card)) {
    newCards = 2;
  }

  if(isType(reveal1, treasure_card) || isType(reveal2, treasure_card)) {
    newCoins = 2;
  }

  if(isType(reveal1, action_card) || isType(reveal2, action_card)) {
    newActions = 2;
  }

  printf("Card is played = %i, expected = %i\n", r, 0);
  assertTrue(r == 0);

  printf("Cards in played pile = %d, expected = %d\n", post->playedCardCount, pre.playedCardCount + discarded);
  assertTrue(post->playedCardCount == pre.playedCardCount + discarded);

  printf("Coins = %d, expected = %d\n", post->coins, pre.coins + newCoins);
  assertTrue(post->coins == pre.coins + newCoins);

  printf("Actions = %d, expected = %d\n", post->numActions, pre.numActions + newActions);
  assertTrue(post->numActions == pre.numActions + 1);

  printf("Cards in hand = %i, expected = %i\n", post->handCount[currentPlayer], pre.handCount[currentPlayer] + newCards);
  assertTrue(post->handCount[currentPlayer] == pre.handCount[currentPlayer] + newCards);

  return 0;
}

int main () {

  int i, n, currentPlayer, handPos, handCount, nextPlayer, seed;
  int k[10] = {adventurer, council_room, feast, gardens, mine,
         remodel, smithy, village, baron, tribute};

  struct gameState G;

  printf ("Testing playTribute.\n");

  printf ("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 100000; n++) {
    for (i = 0; i < sizeof(struct gameState); i++) {
      ((char*)&G)[i] = floor(Random() * 256);
    }
    int players = floor(Random() * MAX_PLAYERS + 1);
    seed = floor(Random() * 1000);
    initializeGame(players, k, seed, &G);
    currentPlayer = floor(Random() * players);
    handCount = floor(Random() * MAX_HAND);
    handPos = floor(Random() * handCount + 1);
    nextPlayer = currentPlayer + 1;
    G.handCount[nextPlayer] = floor(Random() * MAX_HAND);
    G.hand[nextPlayer][0] = floor(Random() * 28);
    G.hand[nextPlayer][1] = floor(Random() * 28);
    G.numPlayers = players;
    G.deckCount[currentPlayer] = floor(Random() * MAX_DECK);
    G.discardCount[currentPlayer] = floor(Random() * MAX_DECK);
    G.discardCount[nextPlayer] = floor(Random() * MAX_DECK);
    G.deckCount[nextPlayer] = floor(Random() * MAX_DECK);
    if(n % 50 == 0) { //set these to 1 or 0 every 50 tests to increase coverage
      G.discardCount[nextPlayer] = floor(Random() * 2);
      G.deckCount[nextPlayer] = floor(Random() * 2);
    }
    G.handCount[currentPlayer] = handCount;
    G.playedCardCount = floor(Random() * 11); //set played card count to range 0 - 10
    G.coins = floor(Random() * 11); //set coins to range 0 - 10
    G.hand[currentPlayer][handPos] = tribute;
    G.whoseTurn = currentPlayer;
    checkPlayTribute(currentPlayer, nextPlayer, &G);
  }

  printf ("ALL TESTS OK\n");

  return 0;
}
