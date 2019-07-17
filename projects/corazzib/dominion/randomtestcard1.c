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

int playerStateChanged(int player, struct gameState *game, struct gameState *testGame) {
  int ret = 0;

  if(game->handCount[player] != testGame->handCount[player]) {
    ret = 1;
  }

  if(game->deckCount[player] != testGame->deckCount[player]) {
    ret = 1;
  }

  if(game->discardCount[player] != testGame->discardCount[player]) {
    ret = 1;
  }

  for(int i = 0; i < game->handCount[player]; i++) {
    if(game->hand[player][i] != testGame->hand[player][i]) {
      ret = 1;
    }
  }

  for(int i = 0; i < game->deckCount[player]; i++) {
    if(game->deck[player][i] != testGame->deck[player][i]) {
      ret = 1;
    }
  }

  for(int i = 0; i < game->discardCount[player]; i++) {
    if(game->discard[player][i] != testGame->discard[player][i]) {
      ret = 1;
    }
  }

  return ret;
}

int checkOtherPlayers(int currentPlayer, struct gameState *pre, struct gameState *post) {
  int changed = 0;
  for(int i = 0; i < pre->numPlayers; i++) {
    if(i != currentPlayer) {
      changed = playerStateChanged(i, pre, post);
    }
  }
  return changed;
}

void assertTrue(int result) {
  if(!result) {
    printf("Test failed\n");
  } else {
    printf("Test passed\n");
  }
}

int checkPlayBaron(int currentPlayer, int choice, struct gameState *post) {
  int r, discarded = 1, newCards = 1, newCoins = 0;
  struct gameState pre;
  memcpy(&pre, post, sizeof(struct gameState));

  int estateHandBefore = countCard(estate, currentPlayer, &pre);

  //if player chooses to discard estate and they have an estate to discard
  if(choice && estateHandBefore > 0) {
    newCards = 0;
    discarded += 1;
    newCoins = 4;
  }

  r = playBaron(currentPlayer, choice, post);

  int estateHandAfter = countCard(estate, currentPlayer, post);
  int otherPlayersChanged = checkOtherPlayers(currentPlayer, &pre, post);

  printf("Card is played = %i, expected = %i\n", r, 0);
  assertTrue(r == 0);

  printf("Hand count = %d, expected = %d\n", post->handCount[currentPlayer], pre.handCount[currentPlayer] + newCards - discarded);
  assertTrue(post->handCount[currentPlayer] == pre.handCount[currentPlayer] + newCards - discarded);

  printf("Estates in hand = %d, expected = %d\n", estateHandAfter, estateHandBefore + newCards);
  assertTrue(estateHandAfter == estateHandBefore + newCards);

  printf("Cards in played pile = %d, expected = %d\n", post->playedCardCount, pre.playedCardCount + discarded);
  assertTrue(post->playedCardCount == pre.playedCardCount + discarded);

  printf("Coins = %d, expected = %d\n", post->coins, pre.coins + newCoins);
  assertTrue(post->coins == pre.coins + newCoins);

  printf("Buys = %d, expected = %d\n", post->numBuys, pre.numBuys + 1);
  assertTrue(post->numBuys == pre.numBuys + 1);

  printf("Other players affected = %i, expected = %i\n", otherPlayersChanged, 0);
  assertTrue(otherPlayersChanged == 0);

  return 0;
}

int main () {

  int i, n, x, currentPlayer, estateHandCount, choice, seed;
  int k[10] = {adventurer, council_room, feast, gardens, mine,
         remodel, smithy, village, baron, great_hall};

  struct gameState G;

  printf ("Testing playBaron.\n");

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
    G.numPlayers = players;
    G.deckCount[currentPlayer] = floor(Random() * MAX_DECK);
    G.discardCount[currentPlayer] = floor(Random() * MAX_DECK);
    G.handCount[currentPlayer] = floor(Random() * MAX_HAND);
    G.playedCardCount = floor(Random() * 11); //set played card count to range 0 - 10
    G.coins = floor(Random() * 11); //set coins to range 0 - 10
    choice = floor(Random() * 2); // whether player should discard estate or not, 0 or 1
    estateHandCount = floor(Random() * 4); // number of estates to give player, between 0-3
    for(x = 0; x < estateHandCount; x++) {
      G.hand[currentPlayer][x] = estate;
    }
    G.supplyCount[estate] = floor(Random() * 9); // number of estates for supply, between 0-8
    checkPlayBaron(currentPlayer, choice, &G);
  }

  printf ("ALL TESTS OK\n");

  return 0;
}
