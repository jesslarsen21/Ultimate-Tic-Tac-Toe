"use strict";

var StartGame = function(WS, dataCallback) {
	//Globals
	var CELL_SIZE = 50;
	var GRID_SIZE = 150;
	var WALL_OFFSET = 20;
	var USER_PIECE;
	var CUR_TURN;
	var GRID_IN_PLAY;
	var CTX;
	var BOARD_STATE = new Array(8);
	var FULL_BOARDS = [];

	var setRed = function() {
		CTX.fillStyle ="rgb(255, 50, 50)";
	}

	var setBlue = function() {
		CTX.fillStyle ="rgb(50, 50, 255)";
	}

	var setRedWin = function() {
		CTX.globalAlpha=0.5;
		CTX.fillStyle ="rgb(255, 150, 150)";
	}

	var setBlueWin = function() {
		CTX.globalAlpha=0.5;
		CTX.fillStyle ="rgb(150, 150, 255)";
	}

	var setBlack = function() {
		CTX.globalAlpha=1;
		CTX.fillStyle="rgb(0, 0, 0)";
	}
	//returns blank board
	var cleanBoard = function() {
		var grid = new Array(8);
		var i;
		for(i=0;i<9;i++) {
			grid[i] = new Array(8);
		}
		return grid;
	}

	// still global
	var GAME_BOARD = cleanBoard();

	// only essential WebSocket functions
	var sendData = function(data) {
		WS.send(data);
	}

//Game Logic functions

	var setBoardState = function(piece, board) {
		BOARD_STATE[board] = piece;
	}

	//draws both the small and large game grids
	var drawGrid = function(x, y, lineWeight, scale) {
		// default value for scale is cellsize, need for drawing larger grid
		var scale = scale || CELL_SIZE;
		CTX.fillRect(x+scale-(lineWeight/2), y, lineWeight, scale*3);
		CTX.fillRect(x+scale*2-(lineWeight/2), y, lineWeight, scale*3);
		CTX.fillRect(x, y+scale-(lineWeight/2), scale*3, lineWeight);
		CTX.fillRect(x, y+scale*2-(lineWeight/2), scale*3, lineWeight);
	}

	//takes an array of length 9, that contains the moves on the board represented as a 'X' or 'O'
	//can populate any game grid
	var renderCells = function(cells, gridNum) {
		CTX.font = "28px Arial";
		//magic numbers, just get over it
		var baseX = 30 + (153 * (gridNum % 3));
		var baseY = 55 + (153 * Math.floor(gridNum/3));
		var i;
		for(i=0; i<9; i++) {
			if(cells[i] == 'X' && gridNum == GRID_IN_PLAY) {
				setRed();
			} else if (cells[i] == 'O' && gridNum == GRID_IN_PLAY) {
				setBlue();
			} else {
				setBlack();
			}
			if(cells[i] && cells[i] != 'N') {
				CTX.fillText(cells[i], baseX + (CELL_SIZE * (i%3)), baseY + (CELL_SIZE * Math.floor(i/3)));
			}
		}
	}

	// draws all the lines on the canvas
	var renderGameGrid = function() {
		var i;
		var j;
		for(i=0; i<3; i++) {
			for(j=0; j<3; j++) {
				if(GRID_IN_PLAY == -1) {
					if(!_.includes(FULL_BOARDS, (j*3+i))) {
						if(CUR_TURN == 'X') {
							setRed();
						} else if (CUR_TURN == 'O') {
							setBlue();
						}
					} else {
						setBlack();
					}
				} else {
					if((j * 3 + i) == GRID_IN_PLAY && CUR_TURN == 'X') {
						setRed();
					}
					if((j * 3 + i) == GRID_IN_PLAY && CUR_TURN == 'O') {
						setBlue();
					}
				}
				drawGrid(i*GRID_SIZE + WALL_OFFSET, j*GRID_SIZE + WALL_OFFSET, 2);
				setBlack();
			}
		}
		drawGrid(5, 5, 6, 160);
	}

	// draws all players moves onto board
	var renderBoard = function() {
		var i;
		for(i=0; i<9; i++) {
			if(i == GRID_IN_PLAY && CUR_TURN == 'X') {
				setRed();
			}
			if(i == GRID_IN_PLAY && CUR_TURN == 'O') {
				setBlue();
			}
			renderCells(GAME_BOARD[i], i);
			setBlack();
		}
	}

	var colorWin = function(piece, boardNum) {
		var x = boardNum % 3;
		var y = Math.floor(boardNum / 3);
		var cornerX = x * GRID_SIZE + WALL_OFFSET;
		var cornerY = y * GRID_SIZE + WALL_OFFSET;
		if(x == 0) {
			cornerX -= 6;
		} else if(x == 2) {
			cornerX += 6;
		}
		if(y == 0) {
			cornerY -= 6;
		} else if(y == 2) {
			cornerY += 6;
		}
		if(piece == 'X') {
			setRedWin();
		}
		if(piece == 'O') {
			setBlueWin();
		}
		CTX.fillRect(cornerX, cornerY, GRID_SIZE, GRID_SIZE);
		setBlack();
	}

	var checkWins = function() {
		var i;
		for(i=0; i<BOARD_STATE.length; i++) {
			if(BOARD_STATE[i] != 'N' && BOARD_STATE[i]) {
				colorWin(BOARD_STATE[i], i);
			}
		}
	}

	var redisplay = function() {
		CTX.clearRect(0,0,500,500);
		checkWins();
		renderBoard();
		renderGameGrid();
		return ;
	}

	var displayPiece = function(piece) {
		var opponentPiece;
		if(piece == "X") {
			opponentPiece = "O";
		} else {
			opponentPiece = "X";
		}
		dataCallback({userPiece: piece, opponentPiece: opponentPiece});
	}

	var displayTurn = function(turn) {
		dataCallback({curTurn: turn});
	}

	// updates arrays with new move made by players
	var playUpdate = function(curPiece, gridNum, cellNum) {
		GAME_BOARD[gridNum][cellNum] = curPiece;
		if(!_.includes(GAME_BOARD[gridNum], 'N')) {
			if(!_.includes(FULL_BOARDS, gridNum)) {
				FULL_BOARDS.push(gridNum);
			}
		}
		redisplay();
	}

	//handles the event of a user click and calculates the grid and cell that was clicked
	var getGridAndCell = function(evt) {
		var clickX = evt.offsetX;
		var clickY = evt.offsetY;
		var gridX = Math.floor((clickX - WALL_OFFSET) / GRID_SIZE);
		var gridY = Math.floor((clickY - WALL_OFFSET) / GRID_SIZE);
		var gridNum = gridX + (gridY * 3);
		var cornerX = WALL_OFFSET + (gridX * GRID_SIZE);
		var cornerY = WALL_OFFSET + (gridY * GRID_SIZE);
		var cellX = Math.floor((clickX - cornerX) / CELL_SIZE);
		var cellY = Math.floor((clickY - cornerY) / CELL_SIZE);
		var cellNum = cellX + (cellY * 3);
		return {grid: gridNum, cell: cellNum};
	}

	//call back for the user clicking the canvas
	var canvasEventHandler = function(evt) {
		var clickData = getGridAndCell(evt);
		handleClick(clickData);
	}

	//takes action if a valid area was clicked
	var handleClick = function(data) {
		if(CUR_TURN == USER_PIECE) {
			if(GRID_IN_PLAY == -1 && !(_.includes(FULL_BOARDS, data.grid))) {
				var req = "turn " + USER_PIECE + " " + data.grid + " " + data.cell;
				sendData(req);
				return;
			} else if(data.grid == GRID_IN_PLAY) {
				var req = "turn " + USER_PIECE + " " + data.grid + " " + data.cell;
				sendData(req);
				return;
			}
		}
		return;
	}

	var wsPlay = function(piece, board, pos) {
		playUpdate(piece, board, pos);
	}

	var setTurn = function(piece, board) {
		GRID_IN_PLAY = board;
		CUR_TURN = piece;
		redisplay();
	}

	// Handle WS messages
	var parseMessage = function(evt) {
		var data = evt.data.split(" ");
		switch(data[0]) {
			case "marker":
				wsPlay(data[1], parseInt(data[2]), parseInt(data[3]));
				break;
			case "nextturn":
				setTurn(data[1], parseInt(data[2]));
				displayTurn(data[1]);
				break;
			case "winstate":
				break;
			case "boardstate":
				setBoardState(data[1], parseInt(data[2]));
				break;
			case "youare":
				USER_PIECE = data[1];
				displayPiece(data[1]);
				dataCallback({ yourName: data[2] });
				break;
			case "opponentname":
				dataCallback({ opponentName: data[1] });
			case "turnfailed":
				break;
			default:
				break;
		}
	}

	WS.onmessage = parseMessage;

	var startUp = function(){
		var canvas = document.getElementById('UTTT');
		canvas.addEventListener('click', canvasEventHandler, false);
		canvas.width = 500;
		canvas.height = 500;
		CTX = canvas.getContext('2d');
		redisplay();
	};

	startUp();
}
