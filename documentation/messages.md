Client to Server
=========================

Turn
----
A `turn` tells create a turn at the specified position.

- turn Player:X|O Board:0-8 Position:0-8
  ```
  turn X 3 7
  ```

  ```
  turn O 0 2
  ```

Who Am I?
---------
A `whoami` requests the server to send a `youare` message

- whoami
  ```
  whoami
	```

Update
------
A `update` requests the server to send the whole game state.

- update
  ```
  update
  ```


Login
-----
A `login` requests the server to login a specific user, validating to the database.

- login Email Password:[!-z]{6-128}
  ```
  login `marioMustache@gmail.com` 1mTh3W1nn3r!
  ```

  ```
  login `melons@gmail.com` letmein
  ```


Sign Up
-------
A `signup` requests the server to register the user in the database.

- signup Username:[A-Za-z0-9]{2-24} Email Password:[!-z]{6-128} Confirm:[!-z]{6-128}
  ```
  signup princess `Peach_huCha@gmail.com` sw33t sw33t
  ```



Server to Client
================

Sign Up Reply
-------------
A `signupreply` message, notifies the client the result of a `signup` request.

- signupreply Username: valid|invalid Email: valid|invalid PasswordConfirm: valid|unmatched|invalid
  ```
  - e.g. signupreply valid valid match
  ```

  ```
  - e.g. signupreply invalid invalid unmatched
  ```


Login Reply
-----------
A `loginreply` message, notifies the client the result of a `login` request.

- loginreply Authorization: valid|invalid

  @TODO: Do we want to send down a token or identifier at this point?
  ```
  loginreply valid
  ```

  ```
  loginreply invalid
  ```


Marker
------
A `marker` message, notifies the client of a new marker on the board.

- marker Player:X|O|N Board:0-8 Position:0-8
  ```
  marker X 5 4
  ```


Next Turn
---------
A `nextturn` message, notifies the client of whose turn is next, and which board they can play on.

- nextturn Player:X|O|N NextBoard:0-8|-1
  ```
  nextturn O 4
  ```


Win State
---------
A `winstate` message, notifies the client of who has won, `N` if no player has won yet.

- winstate Player:X|O|N
  ```
  winstate O
  ```


Board State
-----------
A `boardstate` message, notifies the client of who won a specific board. `N` if no player has won yet.

- boardstate Player:X|O|N Board:0-8
  ```
  boardstate O 3
  ```

You Are
-------
The `youare` message tells the client which player they are, X or O.

- youare Player:X|O|N Username:[A-Za-z0-9]{2-24}
	```
	youare X Vader
	```

Turn Failed
-----------
The `turnfailed` message informs the client that the turn taken by the player is invalid and failed.

- turnfailed Player:X|O Message
  ```
  turnfailed X already_played
  ```
  turnfailed O inactive_board
  ```

Opponent Name
-------------
The `opponentname` message tells the client the username of the player who is playing against you.

- opponentname Username:[A-Za-z0-9]{2-24}
  ```
  opponentname R2D2
  ```
