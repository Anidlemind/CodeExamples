FROM ubuntu:22.04

RUN apt-get update && apt-get install -y python3 python3-pip libcairo2 libcairo2-dev
RUN apt-get install -y stockfish

ENV stockfish_path=/usr/games/stockfish

RUN mkdir /app
WORKDIR /app
COPY . .

RUN pip3 install -r requirements.txt

CMD python3 -m src