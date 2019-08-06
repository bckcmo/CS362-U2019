#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include "math.h"

#define NOISY_TEST 1

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
  if(result) {
    printf("Test passed\n");
  } else {
    printf("Test failed\n");
  }
}

int checkPlayMinion(int currentPlayer, int choice1, int choice2, int handPos, struct gameState *post) {
  int r, discarded = 1, newCards = 0, newCoins = 0, otherPlayersChanged, handCount, shouldChange = 0;
  struct gameState pre;
  memcpy(&pre, post, sizeof(struct gameState));

  r = minionEffect(choice1, choice2, post, handPos);;
  otherPlayersChanged = checkOtherPlayers(currentPlayer, &pre, post);
  handCount = pre.handCount[currentPlayer] - discarded;

  if(choice1) {
    newCoins = 2;
  } else {
    newCards = 4;
    discarded += pre.handCount[currentPlayer];
    handCount = newCards;
  }

  for(int i = 0; i < pre.numPlayers; i++) {
    if(i != currentPlayer && pre.handCount[i] > 4) {
      shouldChange = 1;
    }
  }

  printf("Card is played = %i, expected = %i\n", r, 0);
  assertTrue(r == 0);

  printf("Hand count = %d, expected = %d\n", post->handCount[currentPlayer], handCount);
  assertTrue(post->handCount[currentPlayer] == handCount);

  printf("Cards in played pile = %d, expected = %d\n", post->playedCardCount, pre.playedCardCount + discarded);
  assertTrue(post->playedCardCount == pre.playedCardCount + discarded);

  printf("Coins = %d, expected = %d\n", post->coins, pre.coins + newCoins);
  assertTrue(post->coins == pre.coins + newCoins);

  printf("Actions = %d, expected = %d\n", post->numActions, pre.numActions + 1);
  assertTrue(post->numActions == pre.numActions + 1);

  printf("Other players affected = %i, expected = %i\n", otherPlayersChanged, (choice2 && !choice1 && shouldChange));
  assertTrue(otherPlayersChanged == (choice2 && !choice1 && shouldChange));

  return 0;
}

int main () {

  int i, n, currentPlayer, choice1, choice2, handPos, handCount, nextPlayer, seed;
  int k[10] = {adventurer, council_room, feast, gardens, mine,
         remodel, smithy, village, baron, minion};

  struct gameState G;

  printf ("Testing playMinion.\n");

  printf ("RANDOM TESTS.\n");

  SelectStream(2);
  PutSeed(3);

  for (n = 0; n < 1000; n++) {
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
    G.numPlayers = players;
    G.deckCount[currentPlayer] = floor(Random() * MAX_DECK);
    G.discardCount[currentPlayer] = floor(Random() * MAX_DECK);
    G.handCount[currentPlayer] = handCount;
    G.playedCardCount = floor(Random() * 11); //set played card count to range 0 - 10
    G.coins = floor(Random() * 11); //set coins to range 0 - 10
    G.hand[currentPlayer][handPos] = minion;
    G.whoseTurn = currentPlayer;
    choice1 = floor(Random() * 2); //gain +2 coins, 0 or 1
    choice2 = floor(Random() * 2); //redraw, 0 or 1
    checkPlayMinion(currentPlayer, choice1, choice2, handPos, &G);
  }

  printf ("ALL TESTS OK\n");

  return 0;
}
