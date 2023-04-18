while (!isExit)
{
    StandBy gameStandBy;

    if (gameState == standByState)
    {
        cout << "test" << endl;
        gameStandBy.loadBoard("board1.txt");
        // gameStandBy.loadBoard("board2.txt");
        // gameStandBy.loadBoard("board5.txt");

        if (gameStandBy.checkBoardLoaded() && !gameStandBy.checkError())
        {
            gameStandBy.setGameAnswer(answerBoard);
            gameStandBy.setPlayingBoard(playingBoard);
        }
        else
        {
            continue;
        }

        row = gameStandBy.getRow();
        col = gameStandBy.getColumn();
        gameStandBy.printGameAnswer();
        gameStandBy.printPlayingBoard(playingBoard);
        gameStart.printGameState(gameState);
        gameStandBy.startGame(gameState);
    }

    Playing gamePlaying(row, col, answerBoard);

    if (gameState == playingState)
    {
        gamePlaying.printBombCount();
        gamePlaying.printFlagCount(playingBoard);
        gamePlaying.printOpenBlankCount(playingBoard);
        gamePlaying.printRemainBlankCount(playingBoard);

        // board1.txt
        // gamePlaying.openCell(playingBoard, answerBoard, 1, 1);

        for (int i = 0; i < 7; i++)
        {
            gamePlaying.rightClick(playingBoard, 2, 4);
            cout << "Flag: ";
            gamePlaying.printFlagCount(playingBoard);
            cout << playingBoard[2][4] << endl;
        }

        gamePlaying.openCell(playingBoard, answerBoard, 2, 4);
        gameStandBy.printPlayingBoard(playingBoard);
        gameStandBy.printGameAnswer();
        gameStart.printGameState(gameState);

        gamePlaying.rightClick(playingBoard, 2, 4);
        gamePlaying.rightClick(playingBoard, 2, 4);
        gamePlaying.openCell(playingBoard, answerBoard, 2, 4);
        gamePlaying.handleEnd(gameState);


        /*
        // board5.txt
        gamePlaying.openCell(playingBoard, answerBoard, 0, 0);
        gamePlaying.rightClick(playingBoard, 1, 1);
        gamePlaying.openCell(playingBoard, answerBoard, 0, 2);
        gamePlaying.openCell(playingBoard, answerBoard, 0, 0);
        gamePlaying.openCell(playingBoard, answerBoard, 1, 2);
        gamePlaying.rightClick(playingBoard, 2, 1);
        gamePlaying.openCell(playingBoard, answerBoard, 1, 0);
        gamePlaying.openCell(playingBoard, answerBoard, 2, 0);
        gamePlaying.rightClick(playingBoard, 2, 2);
        gamePlaying.openCell(playingBoard, answerBoard, 0, 1);
        gameStandBy.printPlayingBoard(playingBoard);
        gamePlaying.handleEnd(gameState);
        */
    }

    GameOver gameGameOver;

    if (gameState == gameOverState)
    {
        gameGameOver.replay(gameState);
        gameStart.printGameState(gameState);
        gameGameOver.quit(gameState, isExit);
        gameStart.printGameState(gameState);
    }
}
