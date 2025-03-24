# Snake-Game
Multi-client server for snake game, can be played online, however someone must be hosting the server.

In contraire of the classical snake game, this one the snake is eating bugs of different values (from 1 to 5), the snake wins the game when it reaches 50 points. There may also be wasps which if the player bumps on them makes them loose 5 points. When to players bump heads their points become the average of the sum of their points.

A countdown is also implemented for when a player becomes inactive, kicking the player of the game.

Both server and clients have to be compiled with the Makefile before hand. Compilation is possible by just running "make".
