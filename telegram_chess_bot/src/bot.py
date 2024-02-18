from .constants import *
from .imports import *
from .texts import *

logging.basicConfig(
    format="%(asctime)s - %(name)s - %(levelname)s - %(message)s",
    level=logging.INFO
)

path = os.environ["stockfish_path"]
games = {}
user_lobbies = {}
multiplayer_games = {}
img = {}
bs = {}
mv = {"a": 7, "b": 6, "c": 5, "d": 4, "e": 3, "f": 2, "g": 1, "h": 0}

engine_stockfish = chess.engine.SimpleEngine.popen_uci(path)
engine_stockfish.configure({"UCI_LimitStrength": True})
engine_stockfish.configure({"UCI_Elo": 1350})


def generate_move(update: Update):
    result = engine_stockfish.play(
        games[update.effective_chat.id], chess.engine.Limit(time=0.1)
    )
    return result.move;


def offline_image_handler(update: Update):
    img[update.effective_chat.id] = chess.svg.board(
        games[update.effective_chat.id],
        flipped=(bs[update.effective_chat.id] == "white")
    )
    svg2png(img[update.effective_chat.id], write_to="image.png")


def online_image_handler(update: Update):
    img[update.effective_chat.id] = chess.svg.board(
        multiplayer_games[user_lobbies[update.effective_chat.id]][0],
        flipped=(update.effective_chat.id != multiplayer_games[
                    user_lobbies[update.effective_chat.id]][2]))
    svg2png(img[update.effective_chat.id], write_to="image.png")


async def offline_move(update: Update, context: ContextTypes.DEFAULT_TYPE):
    if update.effective_chat.id not in games or \
            games[update.effective_chat.id] is None:
        await context.bot.send_message(chat_id=update.effective_chat.id,
                                       text=no_game_started_text)
    if update.message.text not in possible:
        await context.bot.send_message(chat_id=update.effective_chat.id,
                                       text=move_format_not_valid_text)
        return
    if chess.Move.from_uci(update.message.text) in \
            games[update.effective_chat.id].legal_moves:
        games[update.effective_chat.id].push_san(update.message.text)
        if games[update.effective_chat.id].is_checkmate():
            offline_image_handler(update)
            await context.bot.send_photo(
                chat_id=update.effective_chat.id, caption=board_text,
                photo="image.png")
            await context.bot.send_message(
                chat_id=update.effective_chat.id, text=checkmate_texts[0])
            games[update.effective_chat.id] = None
            return
    else:
        await context.bot.send_message(chat_id=update.effective_chat.id,
                                       text=invalid_move_text)
        return
    result = engine_stockfish.play(games[update.effective_chat.id],
                                   chess.engine.Limit(time=0.1))
    await context.bot.send_message(
        chat_id=update.effective_chat.id,
        text="My move is " +
             str(games[update.effective_chat.id].san(result.move)), )
    games[update.effective_chat.id].push(result.move)
    if games[update.effective_chat.id].is_checkmate():
        offline_image_handler(update)
        await context.bot.send_photo(chat_id=update.effective_chat.id,
                                     caption=board_text, photo="image.png")
        await context.bot.send_message(chat_id=update.effective_chat.id,
                                       text=checkmate_texts[1])
        games[update.effective_chat.id] = None
        return
    offline_image_handler(update)
    await context.bot.send_photo(
        chat_id=update.effective_chat.id, caption=board_text, photo="image.png")


async def online_move(txt, update: Update, context: ContextTypes.DEFAULT_TYPE):
    if (update.effective_chat.id in user_lobbies and
            user_lobbies[update.effective_chat.id] is not None and
            multiplayer_games[
                user_lobbies[update.effective_chat.id]] is not None):
        if txt[1] not in possible:
            await context.bot.send_message(chat_id=update.effective_chat.id,
                                           text=move_format_not_valid_text)
            return
        curr = multiplayer_games[user_lobbies[update.effective_chat.id]][3]
        if (curr == "w" and update.effective_chat.id == multiplayer_games[
            user_lobbies[update.effective_chat.id]][1]) or \
                (curr == "b" and update.effective_chat.id == multiplayer_games[
                    user_lobbies[update.effective_chat.id]][2]):
            await context.bot.send_message(chat_id=update.effective_chat.id,
                                           text=turn_text)
            return
        if chess.Move.from_uci(txt[1]) not in multiplayer_games[
                user_lobbies[update.effective_chat.id]][0].legal_moves:
            await context.bot.send_message(chat_id=update.effective_chat.id,
                                           text=invalid_move_text)
            return
        multiplayer_games[user_lobbies[update.effective_chat.id]][0].push_san(
            txt[1])
        multiplayer_games[user_lobbies[update.effective_chat.id]][3] = \
            sides[(multiplayer_games[
                       user_lobbies[update.effective_chat.id]][3] == "w") % 2]
        online_image_handler(update)
        await context.bot.send_photo(chat_id=update.effective_chat.id,
                                     caption=board_text, photo="image.png")
        online_image_handler(update)
        other_id = \
            multiplayer_games[user_lobbies[update.effective_chat.id]][
                (multiplayer_games[
                    user_lobbies[update.effective_chat.id]].index(
                    update.effective_chat.id) % 2) + 1]
        await context.bot.send_photo(chat_id=other_id,
                                     caption=move_change_text, photo="image.png")
    else:
        await context.bot.send_message(chat_id=update.effective_chat.id,
                                       text=no_game_online_text)


async def start(update: Update, context: ContextTypes.DEFAULT_TYPE):
    await context.bot.send_message(chat_id=update.effective_chat.id,
                                   text=start_text)


async def elo(update: Update, context: ContextTypes.DEFAULT_TYPE):
    user_num = "".join(context.args)
    if not user_num.isdigit():
        await context.bot.send_message(
            chat_id=update.effective_chat.id, text=elo_texts[1]
        )
        return
    if not int(user_num) in range(1350, 2850):
        await context.bot.send_message(
            chat_id=update.effective_chat.id, text=elo_texts[2]
        )
        return
    engine_stockfish.configure({"UCI_Elo": int(user_num)})
    await context.bot.send_message(chat_id=update.effective_chat.id,
                                   text=elo_texts[0])


async def play_offline(update: Update, context: ContextTypes.DEFAULT_TYPE):
    if update.effective_chat.id in games.keys():
        if games[update.effective_chat.id] is not None:
            await context.bot.send_message(
                chat_id=update.effective_chat.id, text=play_offline_texts[0])
            return
    user_text = "".join(context.args)
    if user_text.lower() == "white":
        await context.bot.send_message(
            chat_id=update.effective_chat.id, text=play_offline_texts[1]
        )
        bs[update.effective_chat.id] = "black"
    elif user_text.lower() == "black":
        await context.bot.send_message(
            chat_id=update.effective_chat.id, text=play_offline_texts[2]
        )
        bs[update.effective_chat.id] = "white"
    else:
        await context.bot.send_message(
            chat_id=update.effective_chat.id, text=play_offline_texts[3])
        return
    games[update.effective_chat.id] = chess.Board()
    offline_image_handler(update)
    await context.bot.send_message(
        chat_id=update.effective_chat.id, text=play_offline_texts[4])
    if user_text.lower() == "white":
        await context.bot.send_message(
            chat_id=update.effective_chat.id,
            text=bot_move_texts[1])
    else:
        engine_move = generate_move(update)
        await context.bot.send_message(
            chat_id=update.effective_chat.id,
            text=(bot_move_texts[0] +
                  str(games[update.effective_chat.id].san(engine_move)) +
                  "\n " + bot_move_texts[1]))
        games[update.effective_chat.id].push(engine_move)
    await context.bot.send_photo(
        chat_id=update.effective_chat.id, caption=board_text, photo="image.png")


async def host(update: Update, context: ContextTypes.DEFAULT_TYPE):
    user_lobbies[update.effective_chat.id] = "".join(context.args)
    multiplayer_games["".join(context.args)] = [
        chess.Board(),
        -1,
        update.effective_chat.id,
        "w",
    ]
    await context.bot.send_message(chat_id=update.effective_chat.id,
                                   text=host_text)


async def exit(update: Update, context: ContextTypes.DEFAULT_TYPE):
    if (update.effective_chat.id not in user_lobbies
            or user_lobbies[update.effective_chat.id] is None):
        await context.bot.send_message(
            chat_id=update.effective_chat.id,
            text=exit_texts[0]
        )
    if user_lobbies[update.effective_chat.id] is not None:
        if multiplayer_games[user_lobbies[update.effective_chat.id]][1] != -1:
            await context.bot.send_message(
                chat_id=multiplayer_games[
                    user_lobbies[update.effective_chat][1]],
                text=exit_texts[1] +
                     multiplayer_games[
                         user_lobbies[update.effective_chat.id]][0]
            )
        multiplayer_games[user_lobbies[update.effective_chat.id]] = None
        user_lobbies[update.effective_chat.id] = None
        await context.bot.send_message(
            chat_id=update.effective_chat.id,
            text=exit_texts[2]
        )


async def join(update: Update, context: ContextTypes.DEFAULT_TYPE):
    id = "".join(context.args)
    if id not in multiplayer_games.keys():
        await context.bot.send_message(
            chat_id=update.effective_chat.id,
            text=join_texts[0]
        )
        return
    user_lobbies[update.effective_chat.id] = id
    multiplayer_games[id][1] = update.effective_chat.id
    await context.bot.send_message(chat_id=multiplayer_games[id][2],
                                   text=join_texts[1])
    await context.bot.send_message(
        chat_id=update.effective_chat.id,
        text=join_texts[2] + id + join_texts[3]
    )


async def board(update: Update, context: ContextTypes.DEFAULT_TYPE):
    if update.effective_chat.id not in games or \
            games[update.effective_chat.id] is None:
        await context.bot.send_message(
            chat_id=update.effective_chat.id,
            text=no_game_offline_text
        )
        return
    img[update.effective_chat.id] = chess.svg.board(
        games[update.effective_chat.id],
        flipped=(bs[update.effective_chat.id] == "white"),
    )
    svg2png(img[update.effective_chat.id], write_to="image.png")
    await context.bot.send_photo(
        chat_id=update.effective_chat.id,
        caption=board_text,
        photo="image.png"
    )


async def move(update: Update, context: ContextTypes.DEFAULT_TYPE):
    txt = list(update.message.text.split(" "))
    if len(txt) == 2 and txt[0] == "online":
        await online_move(txt, update, context)
        return
    await offline_move(update, context)


async def end(update: Update, context: ContextTypes.DEFAULT_TYPE):
    if (
            update.effective_chat.id not in games.keys()
            or games[update.effective_chat.id] is None
    ):
        await context.bot.send_message(
            chat_id=update.effective_chat.id, text=no_game_started_text
        )
        return
    games[update.effective_chat.id] = None
    await context.bot.send_message(chat_id=update.effective_chat.id,
                                   text=end_text)
