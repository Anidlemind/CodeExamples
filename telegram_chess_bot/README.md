# ChessBot

> This is a telegram bot that allows you to play with latest Stockfish version on your desired ELO.

> This version is a docker-based version of the original

[Here's the bot link](https://t.me/BP_ChessBot)

## How to use:

> Type in /start
> 
> The bot will guide you through

## Running it on your pc
### Install
```
git clone git@github.com:Anidlemind/python_project_2.git
cd python_project_2
```

### Build
```
sudo docker build -t chessbot .
```

### Run
```
sudo docker run --rm -it chessbot
```

### Kill
> Ctrl + C in the command line.

## Available commands:

* /play_offline 'your side (white or black)' &mdash; Starts a new offline game

* /host 'game name' &mdash; Hosts a new online game:

* /join 'game name' &mdash; Joins an existing game:

* /elo 'desired ELO' &mdash; Sets stockfish desired ELO (in range of 1350 and 2850)

* /board &mdash; Shows the board of the current offline game

* /end &mdash; Ends current offline game

## Moves:

> Currently, the bot takes in UCI notation of the move which is: "starting point" "ending point", for example: e2e4.
> 
> After every move the bot will display the current board.
> 

## Here is an example of Bot usage:

![](https://sun9-51.userapi.com/impg/0WYHDNFMmrLtp-DfGlzNSTESltbcuMXEusRUsA/iqM-V-1D7IM.jpg?size=645x904&quality=96&sign=fc4f1bd48fcd392ec5c5907ae138d964&type=album)
![](https://sun9-40.userapi.com/impg/w8GXxed0Qz8DQQFjGb1AkNFwVEvCq9psbtQL1w/lo-2WYaEs88.jpg?size=643x932&quality=96&sign=b1e6d834073019d871759de49f62ea43&type=album)
