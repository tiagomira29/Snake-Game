# Snake-Game
This is a multi-client version of the classic Snake game that can be played online. One player must host the server for others to join.

Unlike the traditional Snake game, players collect bugs worth different point values (ranging from 1 to 5). The goal is to reach 50 points to win. Watch out for wasps — colliding with one will cause the player to lose 5 points.

If two players bump into each other head-on, their scores are averaged and both players receive the new value.

To prevent inactive players from stalling the game, a countdown timer is implemented. If a player becomes inactive for too long, they are automatically removed from the game.

Note: Both the server and clients must be compiled using the provided Makefile. Simply run make to compile everything.

## Collaborators
- [Pedro Apolónia](https://github.com/apolonia-p) – Co-developer / Original Collaborator
