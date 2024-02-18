import os

import logging

import chess
import chess.engine
import chess.svg
from cairosvg import svg2png
from telegram import Update
from telegram.ext import filters, ApplicationBuilder, ContextTypes, \
    CommandHandler, MessageHandler