start_text = "Hi! I am a chess bot, developed as a project for MIPT python " \
             "course!\n If you want to play a game of chess with me - type : " \
             "/play_offline \"which side you want to play on (white or " \
             "black)\" \nIf " \
             "you want to configure the ELO at which bot plays: type in /elo " \
             "\"number between 1350 and 2850\" \n The default ELO set is " \
             "1350. \n If you'd like to play with your friends: type in /host" \
             " \"game_name\" to host a game and /join \"game_name\" to join."
board_text = "Here's the board."
no_game_offline_text = "We are not currently playing a game!"
no_game_online_text = "You are not playing an online game currently!"
move_format_not_valid_text = "Sorry, that's not a move, as a reminder of what" \
                             " the move should look like: \"The start " \
                             "position\"\"The end position\""
turn_text = "Not your turn yet!"
invalid_move_text = "Not a valid move!"
move_change_text = "It's your move now!."
no_game_started_text = "We haven't started a new game with you yet!"
end_text = "Ended the previous game. You can now start a new one!"
host_text = "Created a game. Tell your game id to another person to play a " \
            "multiplayer game with them. I will now wait for the " \
            "confirmation. If you want to stop the wait and leave the lobby: " \
            "type /exit"
elo_texts = ["Bot configured, you can now start the game!",
             "Sorry, that's not a valid ELO",
             "Sorry, that's not in given ELO range"]
play_offline_texts = ["Are you sure you want to end theprevious game? \nType "
                      "/end to end the previous game. \nType /board to see "
                      "the current board",
                      "Okay, I will play as black!",
                      "Okay, I will play as white!",
                      "Sorry, I am not really sure, what that side is. Try "
                      "again!",
                      "From now on - type in your moves in the format: "
                      "starting square, ending square. For example: e2e4 or "
                      "a1b2"]
bot_move_texts = ["First move goes to me! ", "What's your move?"]
exit_texts = ["No game is played currently",
              "Your opponent left your previous game with tag: ",
              "You left the game!"]
join_texts = ["No such game exists. Try again!",
              "Opponent joined your game! \n As you are the one, who created "
              "the lobby - you play as white. Play the moves by typing: "
              "\"online\" \"your move\"",
              "Joined game ",
              ". You are playing as black. Awaiting for your opponent's "
              "move.\nPlay the moves by typing: \"online\" \"your move\""]
checkmate_texts = ["Checkmate! What a great game! Maybe you would want to "
                   "play another one on a higher ELO?",
                   "Checkmate! What a great game! Maybe you would like to try "
                   "another one on a lower ELO?"]