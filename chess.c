#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<limits.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<time.h>

int blackcheakcastle = 0, whitecheakcastle = 0, blackrookkcastle = 0, blackrookqcastle = 0, whiterookkcastle = 0, whiterookqcastle = 0, undowrookkcastle = 0, undowrookqcastle = 0, undobrookkcastle = 0, undobrookqcastle = 0, undowcheakcastle = 0, undobcheakcastle = 0;
//to cheak castle condition in the game

int cheakundo = 0;
// cheakundo prevents undo after an undo

int cheakcheck = 0;
// cheakcheck restrict to move such as to protect the king

int empass = 0;
// to undo an empass

int psmoveassurance = 0;

char pieceprev[9][9][4];

typedef struct piecesave {
	int row, column;
	char str[5];
}piecesave;
typedef struct killingone {
	int row, column;
}killingone;
killingone kone;
// piecesave is written in a file and the readen when continue we have to continue the game

typedef struct piecestore {
	int rownow, columnnow, rowprev, columnprev, playerprev;
	char piecestr[5], piecekilled[5], empasskilled[5];
}piecestore;
piecestore ps, ups;
// piecestore stores the last move every time to make an undo move to the player

void printboard(char piece[9][9][4], int player);
int readline(char line[50], int len, int player, char piece[9][9][4]);
int cheak(char piece[9][9][4], int r, int c, int player);
void save(char piece[9][9][4], int player);
void undo(char piece[9][9][4]);
#define	color	"\x1b["
void clrscr() {
	printf(color"2J");
}
// clears the screen
int error() {
	printf("ERROR OCCURED\n");
	return INT_MIN;
}

int move(char line[5], int player, char piece[9][9][4]);

void stop(char piece[9][9][4], int player) {
	printf("\nTHE GAME HAS STOPPED\n#########################################\n");
	char a = '1';
	printf("   Press 1 if you want to save the\n    ");
	scanf("%c", &a);
	if(a != '1')
		return;

	if(player == 1)
		save(piece, 2);
	else
		save(piece, 1);
	printf("THE GAME SAVED SUCCESSFULLY\n");
}
// stop is called when we have to stop the game

// enteronceagain is called when there is an illegal move
// it will call itself until the leagal move is entered


typedef struct psmov {
	int psrow, pscolumn;
}psmov;

typedef struct psmoves {
	psmov psmove[30];
	int noofpsmoves;
}psmoves;

//TO FIND POSSIBLE MOVES OF A PIECE

psmoves possiblemoves(char piece[9][9][4], int player, int row, int column) {
	psmoveassurance = 1;
	psmoves p;
	int i = 0;
	int k;
	char pspiece[9][9][4];
	char psline[6];
	int r, j;
	int rkw, jkw;
	for(r = 1; r <= 8; r++) {
		for(j = 1; j <= 8; j++) {
			strcpy(pspiece[r][j], piece[r][j]);
			if(pspiece[r][j][0] == 'K' && player == 1) {
				rkw = r;
				jkw = j;
			}
			else if(pspiece[r][j][0] == 'k' && player == 2) {
				rkw = r;
				jkw = j;
			}
		}
	}
	psline[0] = piece[row][column][0];
	psline[1] = row + '0';
	psline[2] = column + '0';
	
	if(player == 1) {
		if(piece[row][column][0] == 'P') {
			if(strlen(piece[row + 1][column]) == 0 && row + 1 < 9) {
				psline[3] = row + 1 + '0';
				psline[4] = column + '0';
				psline[5] = '\0';
				move(psline, 1, pspiece);
				if(cheak(pspiece, rkw, jkw, 2) == 0) {
					p.psmove[i].psrow = row + 1;
					p.psmove[i++].pscolumn = column;
				}
				undo(pspiece);
			}
			if(strlen(piece[row + 2][column]) == 0 && row == 2 && strlen(piece[row + 1][column]) == 0)  {
				psline[3] = row + 2 + '0';
                                psline[4] = column + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {
                                        p.psmove[i].psrow = row + 2;
                                        p.psmove[i++].pscolumn = column;
                                }
                                undo(pspiece);
			}
			if(strlen(piece[row + 1][column + 1]) == 2 && row + 1 < 9 && column + 1 < 9) {
				psline[3] = row + 1 + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {
					p.psmove[i].psrow = row + 1;
					p.psmove[i++].pscolumn = column + 1;
				}
				undo(pspiece);
			}
			if(strlen(piece[row + 1][column - 1]) == 2 && row + 1 < 9 && column - 1 > 0) {
				psline[3] = row + 1 + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {

					p.psmove[i].psrow = row + 1;
					p.psmove[i++].pscolumn = column - 1;
				}
				undo(pspiece);
			}
			if(strlen(piece[row + 1][column - 1]) == 0 && column - 1 > 0 && row == 5 && ups.piecestr[0] == 'p' && !strcmp(ups.piecestr, piece[row][column - 1]) && ups.rownow == row && ups.rowprev == ups.rownow + 2) {
				psline[3] = row + 1 + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {
					p.psmove[i].psrow = row + 1;
					p.psmove[i++].pscolumn = column - 1;
				}
				undo(pspiece);
			}
			if(strlen(piece[row + 1][column + 1]) == 0 && column + 1 < 9 && row == 5 && ups.piecestr[0] == 'p' && !strcmp(ups.piecestr, piece[row][column + 1]) && ups.rownow == row && ups.rowprev == ups.rownow + 2) {
				psline[3] = row + 1 + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {

					p.psmove[i].psrow = row + 1;
					p.psmove[i++].pscolumn = column + 1;
				}
				undo(pspiece);
			}
		}
		else if(piece[row][column][0] == 'N') {
			if(strlen(piece[row + 1][column + 2]) != 3 && row + 1 < 9 && column + 2 < 9) {
				psline[3] = row + 1 + '0';
                                psline[4] = column + 2 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {

					p.psmove[i].psrow = row + 1;
					p.psmove[i++].pscolumn = column + 2;
				}
				undo(pspiece);
			}
			if(strlen(piece[row + 1][column - 2]) != 3 && row + 1 < 9 && column - 2 > 0) {
				psline[3] = row + 1 + '0';
                                psline[4] = column - 2+ '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {

					p.psmove[i].psrow = row + 1;
					p.psmove[i++].pscolumn = column - 2;
				}
				undo(pspiece);
                        }
			if(strlen(piece[row - 1][column + 2]) != 3 && row - 1 > 0 && column + 2 < 9) {
				psline[3] = row - 1 + '0';
                                psline[4] = column + 2 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {

                               		p.psmove[i].psrow = row - 1;
                                	p.psmove[i++].pscolumn = column + 2;
				}
				undo(pspiece);
                        }
			if(strlen(piece[row - 1][column - 2]) != 3 && row - 1 > 0 && column - 2 > 0) {
				psline[3] = row - 1 + '0';
                                psline[4] = column - 2 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                p.psmove[i].psrow = row - 1;
        	                        p.psmove[i++].pscolumn = column - 2;
				}
				undo(pspiece);
                        }
			if(strlen(piece[row + 2][column + 1]) != 3 && row + 2 < 9 && column + 1 < 9) {
				psline[3] = row + 2 + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                p.psmove[i].psrow = row + 2;
        	                        p.psmove[i++].pscolumn = column + 1;
				}
				undo(pspiece);
                        }
			if(strlen(piece[row + 2][column - 1]) != 3 && row + 2 < 9 && column - 1 > 0) {
				psline[3] = row + 2 + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                p.psmove[i].psrow = row + 2;
        	                        p.psmove[i++].pscolumn = column - 1;
				}
				undo(pspiece);
                        }
			if(strlen(piece[row - 2][column + 1]) != 3 && row - 2 > 0 && column + 1 < 9) {
				psline[3] = row - 2 + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {

                          		p.psmove[i].psrow = row - 2;
                                	p.psmove[i++].pscolumn = column + 1;
				}
				undo(pspiece);
                        }
			if(strlen(piece[row - 2][column - 1]) != 3 && row - 2 > 0 && column - 1 > 0) {
				psline[3] = row - 2 + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                	p.psmove[i].psrow = row - 2;
                                	p.psmove[i++].pscolumn = column - 1;
				}
				undo(pspiece);
                        }
		}
		else if(piece[row][column][0] == 'B') {
			k = 1;
			while(1) {
				if(strlen(piece[row + k][column + k]) == 0 && row + k < 9 && column + k < 9) {
					psline[3] = row + k + '0';
                             		psline[4] = column + k + '0';
                                	psline[5] = '\0';
                                	move(psline, 1, pspiece);
                                	if(cheak(pspiece, rkw, jkw, 2) == 0) {

						p.psmove[i].psrow = row + k;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					k++;

				}
				else if(strlen(piece[row + k][column + k]) == 2 && row + k < 9 && column + k < 9) {
					psline[3] = row + k + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
	                                if(cheak(pspiece, rkw, jkw, 2) == 0) {

						p.psmove[i].psrow = row + k;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					break;
				}
				else 
					break;
			}
			k = 1;
			while(1) {
                                if(strlen(piece[row + k][column - k]) == 0 && row + k < 9 && column - k > 0) {
					psline[3] = row + k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                        	p.psmove[i].psrow = row + k;
                                        	p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        k++;
                                }
                                else if(strlen(piece[row + k][column - k]) == 2 && row + k < 9 && column - k > 0) {
					psline[3] = row + k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                        	p.psmove[i].psrow = row + k;
                                        	p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
			k = 1;
			while(1) {
                                if(strlen(piece[row - k][column + k]) == 0 && row - k > 0 && column + k < 9) {
					psline[3] = row - k + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                        p.psmove[i].psrow = row - k;
        	                                p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
                                        k++;
                                }
                                else if(strlen(piece[row - k][column + k]) == 2 && row - k > 0 && column + k < 9) {
					psline[3] = row - k + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                        	p.psmove[i].psrow = row - k;
                                        	p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
			k = 1;
			while(1) {
                                if(strlen(piece[row - k][column - k]) == 0 && row - k > 0 && column - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                        p.psmove[i].psrow = row - k;
        	                                p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        k++;
                                }
                                else if(strlen(piece[row - k][column - k]) == 2 && row - k > 0 && column - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                        p.psmove[i].psrow = row - k;
	                                        p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
		}
		else if(piece[row][column][0] == 'R') {
			k = 1;
			while(1) {
				if(strlen(piece[row][column + k]) == 0 && column + k < 9) {
					psline[3] = row + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

						p.psmove[i].psrow = row;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					k++;
				}
				else if(strlen(piece[row][column + k]) == 2 && column + k < 9) {
					psline[3] = row + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

						p.psmove[i].psrow = row;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					break;
				}
				else
					break;
			}
			k = 1;
                        while(1) {
                                if(strlen(piece[row][column - k]) == 0 && column - k > 0) {
					psline[3] = row + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                        	p.psmove[i].psrow = row;
                                        	p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
					k++;
                                }
                                else if(strlen(piece[row][column - k]) == 2 && column - k > 0) {
					psline[3] = row + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

        	                                p.psmove[i].psrow = row;
	                                        p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }

			k = 1;
                        while(1) {
                                if(strlen(piece[row + k][column]) == 0 && row + k < 9) {
					psline[3] = row + k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                        	p.psmove[i].psrow = row + k;
                                        	p.psmove[i++].pscolumn = column;
					}

					undo(pspiece);
					k++;
                                }
                                else if(strlen(piece[row + k][column]) == 2 && row + k < 9) {\
					psline[3] = row + k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                        p.psmove[i].psrow = row + k;
        	                                p.psmove[i++].pscolumn = column;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
			k = 1;
                        while(1) {
                                if(strlen(piece[row - k][column]) == 0 && row - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                        	p.psmove[i].psrow = row - k;
                                        	p.psmove[i++].pscolumn = column;
					}
					undo(pspiece);
					k++;
                                }
                                else if(strlen(piece[row - k][column]) == 2 && row - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                        p.psmove[i].psrow = row - k;
        	                                p.psmove[i++].pscolumn = column;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
		}
		else if(piece[row][column][0] == 'Q') {
			k = 1;
			while(1) {
				if(strlen(piece[row + k][column + k]) == 0 && row + k < 9 && column + k < 9) {
					psline[3] = row + k + '0';
                             		psline[4] = column + k + '0';
                                	psline[5] = '\0';
                                	move(psline, 1, pspiece);
                                	if(cheak(pspiece, rkw, jkw, 2) == 0) {

						p.psmove[i].psrow = row + k;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					k++;

				}
				else if(strlen(piece[row + k][column + k]) == 2 && row + k < 9 && column + k < 9) {
					psline[3] = row + k + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
	                                if(cheak(pspiece, rkw, jkw, 2) == 0) {

						p.psmove[i].psrow = row + k;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					break;
				}
				else 
					break;
			}
			k = 1;
			while(1) {
                                if(strlen(piece[row + k][column - k]) == 0 && row + k < 9 && column - k > 0) {
					psline[3] = row + k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                        	p.psmove[i].psrow = row + k;
                                        	p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        k++;
                                }
                                else if(strlen(piece[row + k][column - k]) == 2 && row + k < 9 && column - k > 0) {
					psline[3] = row + k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                        	p.psmove[i].psrow = row + k;
                                        	p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
			k = 1;
			while(1) {
                                if(strlen(piece[row - k][column + k]) == 0 && row - k > 0 && column + k < 9) {
					psline[3] = row - k + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                        p.psmove[i].psrow = row - k;
        	                                p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
                                        k++;
                                }
                                else if(strlen(piece[row - k][column + k]) == 2 && row - k > 0 && column + k < 9) {
					psline[3] = row - k + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                        	p.psmove[i].psrow = row - k;
                                        	p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
			k = 1;
			while(1) {
                                if(strlen(piece[row - k][column - k]) == 0 && row - k > 0 && column - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                        p.psmove[i].psrow = row - k;
        	                                p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        k++;
                                }
                                else if(strlen(piece[row - k][column - k]) == 2 && row - k > 0 && column - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                        p.psmove[i].psrow = row - k;
	                                        p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
                        k = 1;
			while(1) {
				if(strlen(piece[row][column + k]) == 0 && column + k < 9) {
					psline[3] = row + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

						p.psmove[i].psrow = row;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					k++;
				}
				else if(strlen(piece[row][column + k]) == 2 && column + k < 9) {
					psline[3] = row + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

						p.psmove[i].psrow = row;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					break;
				}
				else
					break;
			}
			k = 1;
                        while(1) {
                                if(strlen(piece[row][column - k]) == 0 && column - k > 0) {
					psline[3] = row + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                        	p.psmove[i].psrow = row;
                                        	p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
					k++;
                                }
                                else if(strlen(piece[row][column - k]) == 2 && column - k > 0) {
					psline[3] = row + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

        	                                p.psmove[i].psrow = row;
	                                        p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }

			k = 1;
                        while(1) {
                                if(strlen(piece[row + k][column]) == 0 && row + k < 9) {
					psline[3] = row + k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                        	p.psmove[i].psrow = row + k;
                                        	p.psmove[i++].pscolumn = column;
					}

					undo(pspiece);
					k++;
                                }
                                else if(strlen(piece[row + k][column]) == 2 && row + k < 9) {\
					psline[3] = row + k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                        p.psmove[i].psrow = row + k;
        	                                p.psmove[i++].pscolumn = column;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
			k = 1;
                        while(1) {
                                if(strlen(piece[row - k][column]) == 0 && row - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

                                        	p.psmove[i].psrow = row - k;
                                        	p.psmove[i++].pscolumn = column;
					}
					undo(pspiece);
					k++;
                                }
                                else if(strlen(piece[row - k][column]) == 2 && row - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 1, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 2) == 0) {

	                                        p.psmove[i].psrow = row - k;
        	                                p.psmove[i++].pscolumn = column;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
		}
		else if(piece[row][column][0] == 'K') {
			if(strlen(piece[row + 1][column]) != 3 && row + 1 < 9) {
				psline[3] = row + 1 + '0';
				psline[4] = column + '0';
				psline[5] = '\0';
				move(psline, 1, pspiece);
				if(cheak(pspiece, row + 1, column, 2) == 0) {
					p.psmove[i].psrow = row + 1;
					p.psmove[i++].pscolumn = column;
				}
				undo(pspiece);
			}
			if(strlen(piece[row + 1][column + 1]) != 3 && row + 1 < 9 && column + 1 < 9) {
                                psline[3] = row + 1 + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, row + 1, column + 1, 2) == 0) {
                                        p.psmove[i].psrow = row + 1;
                                        p.psmove[i++].pscolumn = column + 1;
                                }
                                undo(pspiece);
                        }
			if(strlen(piece[row][column + 1]) != 3 && column + 1 < 9) {
                                psline[3] = row + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, row, column + 1, 2) == 0) {
                                        p.psmove[i].psrow = row;
                                        p.psmove[i++].pscolumn = column + 1;
                                }
                                undo(pspiece);
                        }
			if(strlen(piece[row + 1][column - 1]) != 3 && row + 1 < 9 && column - 1 > 0) {
                                psline[3] = row + 1 + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, row + 1, column - 1, 2) == 0) {
                                        p.psmove[i].psrow = row + 1;
                                        p.psmove[i++].pscolumn = column - 1;
                                }
                                undo(pspiece);
                        }
			if(strlen(piece[row][column - 1]) != 3 && column - 1 > 0) {
                                psline[3] = row + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, row, column - 1, 2) == 0) {
                                        p.psmove[i].psrow = row;
                                        p.psmove[i++].pscolumn = column - 1;
                                }
                                undo(pspiece);
                        }
			if(strlen(piece[row - 1][column + 1]) != 3 && row - 1 > 0 && column + 1 < 9) {
                                psline[3] = row - 1 + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, row - 1, column + 1, 2) == 0) {
                                        p.psmove[i].psrow = row - 1;
                                        p.psmove[i++].pscolumn = column + 1;
                                }
                                undo(pspiece);
                        }
			if(strlen(piece[row - 1][column - 1]) != 3 && row - 1 > 0 && column - 1 > 0) {
                                psline[3] = row - 1 + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, row - 1, column - 1, 2) == 0) {
                                        p.psmove[i].psrow = row - 1;
                                        p.psmove[i++].pscolumn = column - 1;
                                }
                                undo(pspiece);
                        }
			if(strlen(piece[row - 1][column]) != 3 && row - 1 > 0) {
                                psline[3] = row - 1 + '0';
                                psline[4] = column + '0';
                                psline[5] = '\0';
                                move(psline, 1, pspiece);
                                if(cheak(pspiece, row - 1, column, 2) == 0) {
                                        p.psmove[i].psrow = row - 1;
                                        p.psmove[i++].pscolumn = column;
                                }
                                undo(pspiece);
                        }
		}	
	}
	else if(player == 2) {
		if(piece[row][column][0] == 'p') {
			if(strlen(piece[row - 1][column]) == 0 && row - 1 > 0) {
				psline[3] = row - 1 + '0';
				psline[4] = column + '0';
				psline[5] = '\0';
				move(psline, 2, pspiece);
				if(cheak(pspiece, rkw, jkw, 1) == 0) {
					p.psmove[i].psrow = row - 1;
					p.psmove[i++].pscolumn = column;
				}
				undo(pspiece);
			}
			if(strlen(piece[row - 2][column]) == 0 && row == 7 && strlen(piece[row - 1][column]) == 0) {
				psline[3] = row - 2 + '0';
                                psline[4] = column + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {
                                        p.psmove[i].psrow = row - 2;
                                        p.psmove[i++].pscolumn = column;
                                }
                                undo(pspiece);
			}

			if(strlen(piece[row - 1][column + 1]) == 3 && row - 1 > 0 && column + 1 < 9) {
				psline[3] = row - 1 + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {
					p.psmove[i].psrow = row - 1;
					p.psmove[i++].pscolumn = column + 1;
				}
				undo(pspiece);
			}
			if(strlen(piece[row - 1][column - 1]) == 3 && row - 1 > 0 && column - 1 > 0) {
				psline[3] = row - 1 + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {

					p.psmove[i].psrow = row - 1;
					p.psmove[i++].pscolumn = column - 1;
				}
				undo(pspiece);
			}
			if(strlen(piece[row - 1][column - 1]) == 0 && column - 1 > 0 && row == 4 && ups.piecestr[0] == 'P' && !strcmp(ups.piecestr, piece[row][column - 1]) && ups.rownow == row && ups.rowprev == ups.rownow - 2) {
				psline[3] = row - 1 + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {
					p.psmove[i].psrow = row - 1;
					p.psmove[i++].pscolumn = column - 1;
				}
				undo(pspiece);
			}
			if(strlen(piece[row - 1][column + 1]) == 0 && column + 1 < 9 && row == 4 && ups.piecestr[0] == 'P' && !strcmp(ups.piecestr, piece[row][column + 1]) && ups.rownow == row && ups.rowprev == ups.rownow - 2) {
				psline[3] = row - 1 + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {

					p.psmove[i].psrow = row - 1;
					p.psmove[i++].pscolumn = column + 1;
				}
				undo(pspiece);
			}
		}
		else if(piece[row][column][0] == 'n') {
			if(strlen(piece[row + 1][column + 2]) != 2 && row + 1 < 9 && column + 2 < 9) {
				psline[3] = row + 1 + '0';
                                psline[4] = column + 2 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {

					p.psmove[i].psrow = row + 1;
					p.psmove[i++].pscolumn = column + 2;
				}
				undo(pspiece);
			}
			if(strlen(piece[row + 1][column - 2]) != 2 && row + 1 < 9 && column - 2 > 0) {
				psline[3] = row + 1 + '0';
                                psline[4] = column - 2+ '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {

					p.psmove[i].psrow = row + 1;
					p.psmove[i++].pscolumn = column - 2;
				}
				undo(pspiece);
                        }
			if(strlen(piece[row - 1][column + 2]) != 2 && row - 1 > 0 && column + 2 < 9) {
				psline[3] = row - 1 + '0';
                                psline[4] = column + 2 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {

                               		p.psmove[i].psrow = row - 1;
                                	p.psmove[i++].pscolumn = column + 2;
				}
				undo(pspiece);
                        }
			if(strlen(piece[row - 1][column - 2]) != 2 && row - 1 > 0 && column - 2 > 0) {
				psline[3] = row - 1 + '0';
                                psline[4] = column - 2 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                p.psmove[i].psrow = row - 1;
        	                        p.psmove[i++].pscolumn = column - 2;
				}
				undo(pspiece);
                        }
			if(strlen(piece[row + 2][column + 1]) != 2 && row + 2 < 9 && column + 1 < 9) {
				psline[3] = row + 2 + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                p.psmove[i].psrow = row + 2;
        	                        p.psmove[i++].pscolumn = column + 1;
				}
				undo(pspiece);
                        }
			if(strlen(piece[row + 2][column - 1]) != 2 && row + 2 < 9 && column - 1 > 0) {
				psline[3] = row + 2 + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                p.psmove[i].psrow = row + 2;
        	                        p.psmove[i++].pscolumn = column - 1;
				}
				undo(pspiece);
                        }
			if(strlen(piece[row - 2][column + 1]) != 2 && row - 2 > 0 && column + 1 < 9) {
				psline[3] = row - 2 + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {

                          		p.psmove[i].psrow = row - 2;
                                	p.psmove[i++].pscolumn = column + 1;
				}
				undo(pspiece);
                        }
			if(strlen(piece[row - 2][column - 1]) != 2 && row - 2 > 0 && column - 1 > 0) {
				psline[3] = row - 2 + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                	p.psmove[i].psrow = row - 2;
                                	p.psmove[i++].pscolumn = column - 1;
				}
				undo(pspiece);
                        }
		}
		else if(piece[row][column][0] == 'b') {
			k = 1;
			while(1) {
				if(strlen(piece[row + k][column + k]) == 0 && row + k < 9 && column + k < 9) {
					psline[3] = row + k + '0';
                             		psline[4] = column + k + '0';
                                	psline[5] = '\0';
                                	move(psline, 2, pspiece);
                                	if(cheak(pspiece, rkw, jkw, 1) == 0) {

						p.psmove[i].psrow = row + k;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					k++;

				}
				else if(strlen(piece[row + k][column + k]) == 3 && row + k < 9 && column + k < 9) {
					psline[3] = row + k + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
	                                if(cheak(pspiece, rkw, jkw, 1) == 0) {

						p.psmove[i].psrow = row + k;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					break;
				}
				else
					break;
			}
			k = 1;
			while(1) {
                                if(strlen(piece[row + k][column - k]) == 0 && row + k < 9 && column - k > 0) {
					psline[3] = row + k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                        	p.psmove[i].psrow = row + k;
                                        	p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        k++;
                                }
                                else if(strlen(piece[row + k][column - k]) == 3 && row + k < 9 && column - k > 0) {
					psline[3] = row + k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                        	p.psmove[i].psrow = row + k;
                                        	p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
			k = 1;
			while(1) {
                                if(strlen(piece[row - k][column + k]) == 0 && row - k > 0 && column + k < 9) {
					psline[3] = row - k + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                        p.psmove[i].psrow = row - k;
        	                                p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
                                        k++;
                                }
                                else if(strlen(piece[row - k][column + k]) == 3 && row - k > 0 && column + k < 9) {
					psline[3] = row - k + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                        	p.psmove[i].psrow = row - k;
                                        	p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
			k = 1;
			while(1) {
                                if(strlen(piece[row - k][column - k]) == 0 && row - k > 0 && column - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                        p.psmove[i].psrow = row - k;
        	                                p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        k++;
                                }
                                else if(strlen(piece[row - k][column - k]) == 3 && row - k > 0 && column - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                        p.psmove[i].psrow = row - k;
	                                        p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
		}
		else if(piece[row][column][0] == 'r') {
			k = 1;
			while(1) {
				if(strlen(piece[row][column + k]) == 0 && column + k < 9) {
					psline[3] = row + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

						p.psmove[i].psrow = row;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					k++;
				}
				else if(strlen(piece[row][column + k]) == 3 && column + k < 9) {
					psline[3] = row + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

						p.psmove[i].psrow = row;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					break;
				}
				else
					break;
			}
			k = 1;
                        while(1) {
                                if(strlen(piece[row][column - k]) == 0 && column - k > 0) {
					psline[3] = row + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                        	p.psmove[i].psrow = row;
                                        	p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
					k++;
                                }
                                else if(strlen(piece[row][column - k]) == 3 && column - k > 0) {
					psline[3] = row + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

        	                                p.psmove[i].psrow = row;
	                                        p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }

			k = 1;
                        while(1) {
                                if(strlen(piece[row + k][column]) == 0 && row + k < 9) {
					psline[3] = row + k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                        	p.psmove[i].psrow = row + k;
                                        	p.psmove[i++].pscolumn = column;
					}

					undo(pspiece);
					k++;
                                }
                                else if(strlen(piece[row + k][column]) == 3 && row + k < 9) {\
					psline[3] = row + k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                        p.psmove[i].psrow = row + k;
        	                                p.psmove[i++].pscolumn = column;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
			k = 1;
                        while(1) {
                                if(strlen(piece[row - k][column]) == 0 && row - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                        	p.psmove[i].psrow = row - k;
                                        	p.psmove[i++].pscolumn = column;
					}
					undo(pspiece);
					k++;
                                }
                                else if(strlen(piece[row - k][column]) == 3 && row - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                        p.psmove[i].psrow = row - k;
        	                                p.psmove[i++].pscolumn = column;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
		}
		else if(piece[row][column][0] == 'q') {
			k = 1;
			while(1) {
				if(strlen(piece[row + k][column + k]) == 0 && row + k < 9 && column + k < 9) {
					psline[3] = row + k + '0';
                             		psline[4] = column + k + '0';
                                	psline[5] = '\0';
                                	move(psline, 2, pspiece);
                                	if(cheak(pspiece, rkw, jkw, 1) == 0) {

						p.psmove[i].psrow = row + k;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					k++;

				}
				else if(strlen(piece[row + k][column + k]) == 3 && row + k < 9 && column + k < 9) {
					psline[3] = row + k + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
	                                if(cheak(pspiece, rkw, jkw, 1) == 0) {

						p.psmove[i].psrow = row + k;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					break;
				}
				else
					break;
			}
			k = 1;
			while(1) {
                                if(strlen(piece[row + k][column - k]) == 0 && row + k < 9 && column - k > 0) {
					psline[3] = row + k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                        	p.psmove[i].psrow = row + k;
                                        	p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        k++;
                                }
                                else if(strlen(piece[row + k][column - k]) == 3 && row + k < 9 && column - k > 0) {
					psline[3] = row + k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                        	p.psmove[i].psrow = row + k;
                                        	p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
			k = 1;
			while(1) {
                                if(strlen(piece[row - k][column + k]) == 0 && row - k > 0 && column + k < 9) {
					psline[3] = row - k + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                        p.psmove[i].psrow = row - k;
        	                                p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
                                        k++;
                                }
                                else if(strlen(piece[row - k][column + k]) == 3 && row - k > 0 && column + k < 9) {
					psline[3] = row - k + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                        	p.psmove[i].psrow = row - k;
                                        	p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
			k = 1;
			while(1) {
                                if(strlen(piece[row - k][column - k]) == 0 && row - k > 0 && column - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                        p.psmove[i].psrow = row - k;
        	                                p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        k++;
                                }
                                else if(strlen(piece[row - k][column - k]) == 3 && row - k > 0 && column - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                        p.psmove[i].psrow = row - k;
	                                        p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
                        k = 1;
			while(1) {
				if(strlen(piece[row][column + k]) == 0 && column + k < 9) {
					psline[3] = row + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

						p.psmove[i].psrow = row;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					k++;
				}
				else if(strlen(piece[row][column + k]) == 3 && column + k < 9) {
					psline[3] = row + '0';
                                        psline[4] = column + k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

						p.psmove[i].psrow = row;
						p.psmove[i++].pscolumn = column + k;
					}
					undo(pspiece);
					break;
				}
				else
					break;
			}
			k = 1;
                        while(1) {
                                if(strlen(piece[row][column - k]) == 0 && column - k > 0) {
					psline[3] = row + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                        	p.psmove[i].psrow = row;
                                        	p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
					k++;
                                }
                                else if(strlen(piece[row][column - k]) == 3 && column - k > 0) {
					psline[3] = row + '0';
                                        psline[4] = column - k + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

        	                                p.psmove[i].psrow = row;
	                                        p.psmove[i++].pscolumn = column - k;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }

			k = 1;
                        while(1) {
                                if(strlen(piece[row + k][column]) == 0 && row + k < 9) {
					psline[3] = row + k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                        	p.psmove[i].psrow = row + k;
                                        	p.psmove[i++].pscolumn = column;
					}

					undo(pspiece);
					k++;
                                }
                                else if(strlen(piece[row + k][column]) == 3 && row + k < 9) {\
					psline[3] = row + k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                        p.psmove[i].psrow = row + k;
        	                                p.psmove[i++].pscolumn = column;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
			k = 1;
                        while(1) {
                                if(strlen(piece[row - k][column]) == 0 && row - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

                                        	p.psmove[i].psrow = row - k;
                                        	p.psmove[i++].pscolumn = column;
					}
					undo(pspiece);
					k++;
                                }
                                else if(strlen(piece[row - k][column]) == 3 && row - k > 0) {
					psline[3] = row - k + '0';
                                        psline[4] = column + '0';
                                        psline[5] = '\0';
                                        move(psline, 2, pspiece);
                                        if(cheak(pspiece, rkw, jkw, 1) == 0) {

	                                        p.psmove[i].psrow = row - k;
        	                                p.psmove[i++].pscolumn = column;
					}
					undo(pspiece);
                                        break;
                                }
                                else
                                        break;
                        }
		}
		else if(piece[row][column][0] == 'k') {
			if(strlen(piece[row + 1][column]) != 2 && row + 1 < 9) {
				psline[3] = row + 1 + '0';
				psline[4] = column + '0';
				psline[5] = '\0';
				move(psline, 2, pspiece);
				if(cheak(pspiece, row + 1, column, 1) == 0) {
					p.psmove[i].psrow = row + 1;
					p.psmove[i++].pscolumn = column;
				}
				undo(pspiece);
			}
			if(strlen(piece[row + 1][column + 1]) != 2 && row + 1 < 9 && column + 1 < 9) {
                                psline[3] = row + 1 + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, row + 1, column + 1, 1) == 0) {
                                        p.psmove[i].psrow = row + 1;
                                        p.psmove[i++].pscolumn = column + 1;
                                }
                                undo(pspiece);
                        }
			if(strlen(piece[row][column + 1]) != 2 && column + 1 < 9) {
                                psline[3] = row + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, row, column + 1, 1) == 0) {
                                        p.psmove[i].psrow = row;
                                        p.psmove[i++].pscolumn = column + 1;
                                }
                                undo(pspiece);
                        }
			if(strlen(piece[row + 1][column - 1]) != 2 && row + 1 < 9 && column - 1 > 0) {
                                psline[3] = row + 1 + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, row + 1, column - 1, 1) == 0) {
                                        p.psmove[i].psrow = row + 1;
                                        p.psmove[i++].pscolumn = column - 1;
                                }
                                undo(pspiece);
                        }
			if(strlen(piece[row][column - 1]) != 2 && column - 1 > 0) {
                                psline[3] = row + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, row, column - 1, 1) == 0) {
                                        p.psmove[i].psrow = row;
                                        p.psmove[i++].pscolumn = column - 1;
                                }
                                undo(pspiece);
                        }
			if(strlen(piece[row - 1][column + 1]) != 2 && row - 1 > 0 && column + 1 < 9) {
                                psline[3] = row - 1 + '0';
                                psline[4] = column + 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, row - 1, column + 1, 1) == 0) {
                                        p.psmove[i].psrow = row - 1;
                                        p.psmove[i++].pscolumn = column + 1;
                                }
                                undo(pspiece);
                        }
			if(strlen(piece[row - 1][column - 1]) != 2 && row - 1 > 0 && column - 1 > 0) {
                                psline[3] = row - 1 + '0';
                                psline[4] = column - 1 + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, row - 1, column - 1, 1) == 0) {
                                        p.psmove[i].psrow = row - 1;
                                        p.psmove[i++].pscolumn = column - 1;
                                }
                                undo(pspiece);
                        }
			if(strlen(piece[row - 1][column]) != 2 && row - 1 > 0) {
                                psline[3] = row - 1 + '0';
                                psline[4] = column + '0';
                                psline[5] = '\0';
                                move(psline, 2, pspiece);
                                if(cheak(pspiece, row - 1, column, 1) == 0) {
                                        p.psmove[i].psrow = row - 1;
                                        p.psmove[i++].pscolumn = column;
                                }
                                undo(pspiece);
                        }
		}
	}
	psmoveassurance = 0;
	p.noofpsmoves = i;
	return p;

}

int enteronceagain(char line[5], int player, char piece[9][9][4]) {
	int i = 0;
	printf("\n");
	printf(color"91m");
	printf("MOVE INVALID ENTER YOUR MOVE ONCE AGAIN");
	printf(color"0m");
	printf("\n");
	if(player == 1)
		printboard(piece, 2);
	else 
		printboard(piece, 1);
	if(player == 1) {
		printf(color"97m");
		printf(color"100m");
		printf("     WHITE PLAYER MOVE                                   ");
		printf(color"0m");
		printf("\n");
		printf("     ");
	}
	else {
		printf(color"107m");
		printf(color"30m");
		printf("                                 BLACK PLAYER MOVE       ");
		printf(color"0m");
		printf("\n");
		printf("                                 ");
	}
	strcpy(line, "");
	if(player == 1)
		i = readline(line, 5, 2, piece);
	else
		i = readline(line, 5, 1, piece);
	if(i == 1) {
		stop(piece, player);
		return i;
	}
	else if(i == 30)
		return i;
	i = move(line, player, piece);
	return i;
}
/// move actually changes the 2d string piece according to an legal move

int move(char line[5], int player, char piece[9][9][4]) {
	int i = 0;
	int rofp, cofp, rtop, ctop;
	printf(color"91m");
	if(line[1] > 48 && line[1] < 57 && line[2] > 48 && line[2] < 57 && line[3] > 48 && line[3] < 57 && line[4] > 48 && line[4] < 57) {
		rofp = line[1] - '0';
		cofp = line[2] - '0';
		rtop = line[3] - '0';
		ctop = line[4] - '0';
		line[0] = piece[rofp][cofp][0];
	}
	else {
		printf("THE PLACE IS NOT VALID FOR THE MOVE");
		i = enteronceagain(line, player, piece);
		return i;
	}
	int rprev, cprev;
	for(rprev = 1; rprev <= 8; rprev++) {
		for(cprev = 1; cprev <= 8; cprev++) {
			strcpy(pieceprev[rprev][cprev], "");
			strcpy(pieceprev[rprev][cprev], piece[rprev][cprev]);
		}
	}
	if(player == 1) {
		if(strlen(piece[rtop][ctop]) == 3) {
			printf("YOUR PLAYER HAS OCCUPIED THE PLACE");
			i = enteronceagain(line, player, piece);
			return i;
		}
		else if((piece[rofp][cofp][0] == line[0] - 32 || piece[rofp][cofp][0] == line[0]) && strlen(piece[rofp][cofp]) == 3) {
			if(piece[rofp][cofp][0] == 'P') {
				if(rtop == rofp + 1) {
					if(ctop == cofp) {
						if(strlen(piece[rtop][ctop]) == 0) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
						}
						else {
							printf("PAWN CANNOT MOVE TO THE ENTERED POSITION AS IT IS OCCUPIED");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
					else if(ctop == cofp + 1 || ctop == cofp - 1) {
						if(strlen(piece[rtop][ctop]) == 2) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
						}
						else if(strlen(piece[rtop][ctop]) == 0 && rofp == 5) {
							if(ups.piecestr[0] == 'p' && ups.rownow == rofp && ups.rowprev == ups.rownow + 2) {
								if(piece[rofp][cofp - 1][0] == 'p' && ctop == cofp - 1) {
									strcpy(piece[rtop][ctop], piece[rofp][cofp]);
									strcpy(piece[rofp][cofp],"");
									strcpy(piece[rofp][cofp - 1], "");
								}
								else if(piece[rofp][cofp + 1][0] == 'p' && ctop == cofp + 1) {
									strcpy(piece[rtop][ctop], piece[rofp][cofp]);
                                                                        strcpy(piece[rofp][cofp],"");
                                                                        strcpy(piece[rofp][cofp + 1], "");
								}
								else {
									printf("INVALID MOVE FOR THE PAWN");
									i = enteronceagain(line, player, piece);
									return i;
								}
							}
							else {
								printf("INVALID MOVE FOR THE PAWN");
								i = enteronceagain(line, player, piece);
								return i;
							}
						}


						else {
							printf("PAWN CAN MOVE IN THE GIVEN POSITION ONLY IF THERE IS AN OPPONENT PIECE");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
					else {
						printf("THE PAWN CANNOT MOVE IN THAT ROW");
						i = enteronceagain(line, player, piece);
						return i;

					}
				}
				else if(rofp == 2 && rtop == 4 && cofp == ctop && strlen(piece[rofp + 1][cofp]) == 0) {
					if(strlen(piece[rtop][ctop]) == 0) {
						strcpy(piece[rtop][ctop], piece[rofp][cofp]);
						strcpy(piece[rofp][cofp], "");
					}
					else {
						printf("THE PAWN CANNOT MOVE TO THE ENTERED POSITION AS THE ENTERED POSITION IS NOT EMPTY");
						i = enteronceagain(line, player, piece);
						return i;

					}
				}
				else {
					printf("THE PAWN CANNOT MOVE TO THE POSITION ENTERED");
					i = enteronceagain(line, player, piece);
					return i;
				}
			}
			else if(piece[rofp][cofp][0] == 'R') {
				int k;
				if(rofp == rtop) {
					if(cofp < ctop)
						k = cofp + 1;
					else
						k = cofp - 1;
					while(1) {
						if(k == ctop) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
							break;
						}
						else if(strlen(piece[rofp][k]) == 0) {
							if(cofp < ctop)
								k++;
							else
								k--;
						}
						else {
							printf("ROOK CANNOT JUMP OVER THE PIECES");
							i = enteronceagain(line, player, piece);
							return i;
						}
							
					}
				}
				else if(cofp == ctop) {
					if(rofp < rtop)
						k = rofp + 1;
					else
						k = rofp - 1;
					while(1) {
						if(k == rtop) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
							break;
						}
						else if(strlen(piece[k][cofp]) == 0) {
							if(rofp < rtop)
								k++;
							else
								k--;
						}
						else {
							printf("ROOK CANNOT JUMP OVER THE PIECES");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
				}
				else {
					printf("THE ROOK CANNOT MOVE TO THE POSITION ENTERED");
					i = enteronceagain(line, player, piece);
					return i;
				}

			}
			else if(piece[rofp][cofp][0] == 'Q') {
				int k, l;
				if(rtop == rofp) {
					if(ctop < cofp)
						k = cofp - 1;
					else
						k = cofp + 1;
					while(1) {
						if(k == ctop) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
							break;
						}
						else if(strlen(piece[rofp][k]) == 0) {
							if(ctop < cofp)
								k--;
							else
								k++;
						}
						else {
							printf("QUEEN CANNOT JUMP OVER THE PIECES");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
				}
				else if(ctop == cofp) {
					if(rtop < rofp)
						k = rofp - 1;
					else
						k = rofp + 1;
					while(1) {
						if(k == rtop) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
							break;
						}
						else if(strlen(piece[k][cofp]) == 0) {
							if(rtop < rofp)
								k--;
							else
								k++;
						}
						else {
							printf("QUEEN CANNOT JUMP OVER THE PIECES");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
				}
				else {
					if(rtop < rofp)
						k = rofp - 1;
					else
						k = rofp + 1;
					if(ctop < cofp)
						l = cofp - 1;
					else
						l = cofp + 1;
					while(1) {
						if(k == rtop && l == ctop) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
							break;
						}
						else if(k == rtop || l == ctop) {
							printf("QUEEN CANNOT MOVE TO THE ENTERED PLACE");
							i = enteronceagain(line, player, piece);
							return i;
						}
						else if(strlen(piece[k][l]) == 0) {
							if(rofp < rtop)
								k++;
							else
								k--;
							if(cofp < ctop)
								l++;
							else
								l--;
						}
						else {
							printf("QUEEN CANNOT JUMP OVER THE PIECES");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
				}
			}



			else if(piece[rofp][cofp][0] == 'B') {
				int k, l;
				if(rtop < rofp) {
					k = rofp - 1;
					if(ctop < cofp)
						l = cofp - 1;
					else
						l = cofp + 1;
				}
				else {
					k = rofp + 1;
					if(ctop < cofp)
						l = cofp - 1;
					else
						l = cofp + 1;
				}
				while(1) {
					if(k == rtop && l == ctop) {
						strcpy(piece[rtop][ctop], piece[rofp][cofp]);
						strcpy(piece[rofp][cofp], "");
						break;
					}
					else if(k == rtop || l == ctop) {
						printf("THE BISHOP CANNOT MOVE TO THE ENTERED POSITION");
						i = enteronceagain(line, player, piece);
						return i;
					}
					else if(strlen(piece[k][l]) == 0) {
						if(rtop < rofp)
							k--;
						else
							k++;
						if(ctop < cofp)
							l--;
						else
							l++;
					}
					else {
						printf("BISHOP CANNNOT JUMP OVER THE PIECES");
						i = enteronceagain(line, player, piece);
						return i;
					}
				}
			}
			else if(piece[rofp][cofp][0] == 'N') {
				if(((rtop == rofp + 1) && (ctop == cofp + 2)) || ((rtop == rofp + 1) && (ctop == cofp - 2)) || ((rtop == rofp - 1) && (ctop == cofp + 2)) || ((rtop == rofp - 1) && (ctop == cofp - 2)) || ((rtop == rofp + 2) && (ctop == cofp + 1)) || ((rtop == rofp + 2) && (ctop == cofp - 1)) || ((rtop == rofp - 2) && (ctop == cofp + 1)) || ((rtop == rofp - 2) && (ctop == cofp - 1))) {
					strcpy(piece[rtop][ctop], piece[rofp][cofp]);
					strcpy(piece[rofp][cofp], "");
				}
				else {
					printf("THE KNIGHT CANNOT MOVE TO THE ENTERED PLACE");
					i = enteronceagain(line, player, piece);
					return i;
				}
			}
			else if(piece[rofp][cofp][0] == 'K') {
				if(rtop == rofp && ctop == cofp - 2) {
					if(!whitecheakcastle && !whiterookkcastle && strlen(piece[rofp][cofp - 1]) == 0 && strlen(piece[rtop][ctop]) == 0) {
						if(cheak(piece, rtop, ctop, 2) == 1 && !psmoveassurance) {
                                                        printf("THE POSITION IS NOT SAFE FOR KING TO MOVE");
                                                        i = enteronceagain(line, player, piece);
                                                        return i;
                                                }
                                                else {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);	
							strcpy(piece[rofp][cofp], "");
							strcpy(piece[rofp][cofp - 1], piece[rofp][cofp - 3]);
							strcpy(piece[rofp][cofp - 3], "");
						}
					}
					else {
						printf("YOU CANNNOT CASTLE");
						i = enteronceagain(line, player, piece);
						return i;
					}
				}
				else if(rtop == rofp && ctop == cofp + 2) {
					if(!whitecheakcastle && !whiterookqcastle && strlen(piece[rofp][cofp + 1]) == 0 && strlen(piece[rtop][ctop]) == 0 && strlen(piece[rofp][cofp + 3]) == 0) {
						if(cheak(piece, rtop, ctop, 2) == 1 && !psmoveassurance) {
							printf("THE POSITION IS NOT SAFE FOR KING TO MOVE");
							i = enteronceagain(line, player, piece);
							return i;
						}
						else {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
							strcpy(piece[rofp][cofp + 1], piece[rofp][cofp + 4]);
							strcpy(piece[rofp][cofp + 4], "");
						}
					
					}
					else {
						printf("YOU CANNOT CASTLE");
						i = enteronceagain(line, player, piece);
						return i;
					}
				}


				else if(rtop == rofp + 1 || rtop == rofp - 1)  {
					if(ctop == cofp + 1 || ctop == cofp - 1 || ctop == cofp) {
						if(cheak(piece, rtop, ctop, 2) == 1 && !psmoveassurance) {
							printf("THE PLACE IS NOT SAFE FOR THE KING TO MOVE");
							i = enteronceagain(line, player, piece);
							return i;
						}
						else {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
						}
					}
					else {
						printf("THE KING CANNOT MOVE TO THE ENTERED PLACE");
						i = enteronceagain(line, player, piece);
						return i;
					}
				}
				else if(ctop == cofp + 1 || ctop == cofp - 1) {
					if(rtop == rofp) {
						if(cheak(piece, rtop, ctop, 2) == 1 && !psmoveassurance) {
							printf("%d, %d\n", kone.row, kone.column);
							printf("THE PLACE IS NOT SAFE FOR THE KING TO MOVE");
							i = enteronceagain(line, player, piece);
							return i;
						}
						else {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
						}
					}
					else {
						printf("THE KING CANNOT MOVE TO THE ENTERED PLACE");
						i = enteronceagain(line, player, piece);
						return i;
					}
				}
				else {
					printf("THE KING CANNOT MOVE TO THE ENTERED PLACE");
					i = enteronceagain(line, player, piece);
					return i;
				}
			}
		}
		else {
			i = enteronceagain(line, player, piece);
			return i;
		}
	}
	else if(player == 2) {
		if(strlen(piece[rtop][ctop]) == 2) {
			printf("YOUR PLAYER HAS OCCUPIED THE PLACE");
			i = enteronceagain(line, player, piece);
			return i;
		}
		else if((piece[rofp][cofp][0] == line[0] || piece[rofp][cofp][0] == line[0] + 32) && strlen(piece[rofp][cofp]) == 2) {
			if(piece[rofp][cofp][0] == 'p') {
				if(rtop == rofp - 1) {
					if(ctop == cofp) {
						if(strlen(piece[rtop][ctop]) == 0) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
						}
						else {
							printf("THE PAWN CANNOT MOVE IN THE ENTERED POSITION AS THE ENTERED POSITION IS OCCUPIED");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
					else if(ctop == cofp + 1 || ctop == cofp - 1) {
						if(strlen(piece[rtop][ctop]) == 3) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
						}
						else if(strlen(piece[rtop][ctop]) == 0 && rofp == 4) {
                                                        if(ups.piecestr[0] == 'P' && ups.rownow == rofp && ups.rowprev == ups.rownow - 2) {
                                                                if(piece[rofp][cofp - 1][0] == 'P' && ctop == cofp - 1) {
                                                                        strcpy(piece[rtop][ctop], piece[rofp][cofp]);
                                                                        strcpy(piece[rofp][cofp],"");
                                                                        strcpy(piece[rofp][cofp - 1], "");
                                                                }
                                                                else if(piece[rofp][cofp + 1][0] == 'P' && ctop == cofp + 1) {
                                                                        strcpy(piece[rtop][ctop], piece[rofp][cofp]);
                                                                        strcpy(piece[rofp][cofp],"");
                                                                        strcpy(piece[rofp][cofp + 1], "");
                                                                }
                                                                else {
                                                                        printf("INVALID MOVE FOR THE PAWN");
                                                                        i = enteronceagain(line, player, piece);
                                                                        return i;
                                                                }
                                                        }
                                                        else {
                                                                printf("INVALID MOVE FOR THE PAWN");
                                                                i = enteronceagain(line, player, piece);
                                                                return i;
                                                        }
                                                }

						else {
							printf("THE PAWN CAN MOVE TO THE ENTERED POSITION ONLY IF THERE IS A OPPONENT PIECE");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
					else {
						printf("THE PAWN CANNOT MOVE TO THAT ROW");
						i = enteronceagain(line, player, piece);
						return i;
					}
				}
				else if(rofp == 7 && rtop == 5 && cofp == ctop && strlen(piece[rofp - 1][cofp]) == 0) {
					if(strlen(piece[rtop][ctop]) == 0) {
						strcpy(piece[rtop][ctop], piece[rofp][cofp]);
						strcpy(piece[rofp][cofp], "");
					}
					else {
						printf("THE PAWN CANNOT MOVE TO THE ENTERED POSITION AS IT IS NOT EMPTY");
						i = enteronceagain(line, player, piece);
						return i;
					}
				}
				else {
					printf("THE PAWN CANNOT MOVE TO THE ENTERED POSITION");
					i = enteronceagain(line, player, piece);
					return i;
				}
			}
			else if(piece[rofp][cofp][0] == 'r') {
				int k;
				if(rofp == rtop) {
					if(cofp < ctop)
						k = cofp + 1;
					else
						k = cofp - 1;
					while(1) {
						if(k == ctop) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
							break;
						}
						else if(strlen(piece[rofp][k]) == 0) {
							if(cofp < ctop)
								k++;
							else
								k--;
						}
						else {
							printf("ROOK CANNOT JUMP OVER THE PIECES");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
				}
				else if(cofp == ctop) {
					if(rofp < rtop)
						k = rofp + 1;
					else
						k = rofp - 1;
					while(1) {
						if(k == rtop) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
							break;
						}
						else if(strlen(piece[k][cofp]) == 0) {
							if(rofp < rtop)
								k++;
							else
								k--;
						}
						else {
							printf("ROOK CANNOT JUMP OVER THE PIECES");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
				}
				else {
					printf("THE ROOK CANNOT MOVE TO THE ENTERED POSITION");
					i = enteronceagain(line, player, piece);
					return i;
				}
			}
			else if(piece[rofp][cofp][0] == 'q') {
				int k, l;
				if(rtop == rofp) {
					if(cofp < ctop)
						k = cofp + 1;
					else
						k = cofp - 1;
					while(1) {
						if(k == ctop) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
							break;
						}
						else if(strlen(piece[rofp][k]) == 0) {
							if(ctop < cofp)
								k--;
							else
								k++;
						}
						else {
							printf("QUEEN CANNOT JUMP OVER THE PIECES");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
				}
				else if(ctop == cofp) {
					if(rtop < rofp)
						k = rofp - 1;
					else
						k = rofp + 1;
					while(1) {
						if(k == rtop) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
							break;
						}
						else if(strlen(piece[k][cofp]) == 0) {
							if(rtop < rofp)
								k--;
							else
								k++;
						}
						else {
							printf("QUEEN CANNOT JUMP OVER THE PIECES");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
				}
				else {
					if(rtop < rofp)
						k = rofp - 1;
					else
						k = rofp + 1;
					if(ctop < cofp)
						l = cofp - 1;
					else
						l = cofp + 1;
					while(1) {
						if(k == rtop && l == ctop) {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
							break;
						}
						else if(k == rtop || l == ctop) {
							printf("QUEEN CANNOT MOVE TO THE ENTERED PLACE");
							i = enteronceagain(line, player, piece);
							return i;
						}
						else if(strlen(piece[k][l]) == 0) {
							if(rofp < rtop)
								k++;
							else
								k--;
							if(cofp < ctop)
								l++;
							else
								l--;
						}
						else {
							printf("QUEEN CANNOT JUMP OVER THE PIECES");
							i = enteronceagain(line, player, piece);
							return i;
						}
					}
				}
			}
					
			else if(piece[rofp][cofp][0] == 'b') {
				int k, l;
				if(rofp < rtop)
					k = rofp + 1;
				else
					k = rofp - 1;
				if(cofp < ctop)
					l = cofp + 1;
				else
					l = cofp - 1;
				while(1) {
					if(k == rtop && l == ctop) {
						strcpy(piece[rtop][ctop], piece[rofp][cofp]);
						strcpy(piece[rofp][cofp], "");
						break;
					}
					else if(k == rtop || l == ctop) {
						printf("THE BISHOP CANNOT MOVE TO THE ENTERED PLACE");
						i = enteronceagain(line, player, piece);
						return i;
					}
					else if(strlen(piece[k][l]) == 0) {
						if(rofp < rtop)
							k++;
						else
							k--;
						if(cofp < ctop)
							l++;
						else
							l--;
					}
					else {
						printf("BISHOP CANNOT JUMP OVER THE PIECES");
						i = enteronceagain(line, player, piece);
						return i;
					}
				}
			}
			else if(piece[rofp][cofp][0] == 'n') {
				if(((rtop == rofp + 1) && (ctop == cofp + 2)) || ((rtop == rofp + 1) && (ctop == cofp - 2)) || ((rtop == rofp - 1) && (ctop == cofp + 2)) || ((rtop == rofp - 1) && (ctop == cofp - 2)) || ((rtop == rofp + 2) && (ctop == cofp + 1)) || ((rtop == rofp + 2) && (ctop == cofp - 1)) || ((rtop == rofp - 2) && (ctop == cofp + 1)) || ((rtop == rofp - 2) && (ctop == cofp - 1))) {
					strcpy(piece[rtop][ctop], piece[rofp][cofp]);
					strcpy(piece[rofp][cofp], "");
				}
				else {
					printf("THE KNIGHT CANNOT MOVE TO THE ENTERED POSITION");
					i = enteronceagain(line, player, piece);
					return i;
				}
			}
			else if(piece[rofp][cofp][0] == 'k') {
				if(rtop == rofp && ctop == cofp - 2) {
                                        if(!blackcheakcastle && !blackrookkcastle && strlen(piece[rofp][cofp - 1]) == 0 && strlen(piece[rtop][ctop]) == 0) {
						if(cheak(piece, rtop, ctop, 1) == 1 && !psmoveassurance) {
                                                        printf("THE POSITION IS NOT SAFE FOR KING TO MOVE");
                                                        i = enteronceagain(line, player, piece);
                                                        return i;
                                                }
                                                else {

                                             		strcpy(piece[rtop][ctop], piece[rofp][cofp]);
                                                	strcpy(piece[rofp][cofp], "");
                                                	strcpy(piece[rofp][cofp - 1], piece[rofp][cofp - 3]);
                                                	strcpy(piece[rofp][cofp - 3], "");
						}
                                        }
                                        else {
                                                printf("YOU CANNNOT CASTLE");
                                                i = enteronceagain(line, player, piece);
                                                return i;
                                        }
                                }
                                else if(rtop == rofp && ctop == cofp + 2) {
                                        if(!blackcheakcastle && !blackrookqcastle && strlen(piece[rofp][cofp + 1]) == 0 && strlen(piece[rtop][ctop]) == 0 && strlen(piece[rofp][cofp + 3]) == 0) {
						if(cheak(piece, rtop, ctop, 1) == 1 && !psmoveassurance) {
                                                        printf("THE POSITION IS NOT SAFE FOR KING TO MOVE");
                                                        i = enteronceagain(line, player, piece);
                                                        return i;
                                                }
                                                else {

                                                	strcpy(piece[rtop][ctop], piece[rofp][cofp]);
                                                	strcpy(piece[rofp][cofp], "");
                                                	strcpy(piece[rofp][cofp + 1], piece[rofp][cofp + 4]);
                                                	strcpy(piece[rofp][cofp + 4], "");
						}
                                        }
                                        else {
                                                printf("YOU CANNOT CASTLE");
                                                i = enteronceagain(line, player, piece);
                                                return i;
                                        }
                                }

				else if(rtop == rofp + 1 || rtop == rofp - 1) {
					if(ctop == cofp - 1 || ctop == cofp + 1 || ctop == cofp) {
						if(cheak(piece, rtop, ctop, 1) == 1 && !psmoveassurance) {
							printf("THE POSITION IS NOT SAFE FOR THE KING TO MOVE");
							i = enteronceagain(line, player, piece);
							return i;
						}
						else {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
						}
					}
					else {
						printf("THE KING CANNOT MOVE TO THE ENTERED PLACE");
						i = enteronceagain(line, player, piece);
						return i;
					}
				}
				else if(ctop == cofp + 1 || ctop == cofp - 1) {
					if(rtop == rofp) {
						if(cheak(piece, rtop, ctop, 1) == 1 && !psmoveassurance) {
							printf("%d, %d\n", kone.row, kone.column);
							printf("THE POSITION IS NOT SAFE FOR THE KING TO MOVE");
							i = enteronceagain(line, player, piece);
							return i;
						}
						else {
							strcpy(piece[rtop][ctop], piece[rofp][cofp]);
							strcpy(piece[rofp][cofp], "");
						}
					}
					else {
						printf("THE KING CANNOT MOVE TO THE ENTERED PLACE");
						i = enteronceagain(line, player, piece);
						return i;
					}
				}
				else {
					printf("THE KING CANNOT MOVE TO THE ENTERED PLACE");
					i = enteronceagain(line, player, piece);
					return i;
				}
			}


		}
		else {
			i = enteronceagain(line, player, piece);
			return i;
		}
	}
	printf(color"0m");
	strcpy(ps.empasskilled, "");
	ps.rowprev = rofp;
	ps.rownow = rtop;
	ps.columnprev = cofp;
	ps.columnnow = ctop;
	strcpy(ps.piecestr, "");
	strcpy(ps.piecestr, piece[rtop][ctop]);
	ps.playerprev = player;
	strcpy(ps.piecekilled, "");
	strcpy(ps.piecekilled, pieceprev[rtop][ctop]);
	if(ps.piecestr[0] == 'k') {
		undobcheakcastle = undobcheakcastle + 1;
		blackcheakcastle = 1;
	}
	else if(ps.piecestr[0] == 'K') {
		undowcheakcastle = undowcheakcastle + 1;
		whitecheakcastle = 1;
	}
	else if(ps.piecestr[0] == 'r') {
		if(ps.piecestr[1] == '1') {
			undobrookkcastle = undobrookkcastle + 1;
			blackrookkcastle = 1;
		}
		else {
			undobrookqcastle = undobrookqcastle + 1;
			blackrookqcastle = 1;
		}
	}
	else if(ps.piecestr[0] == 'R') {
		if(ps.piecestr[1] == '1') {
			undowrookkcastle = undowrookkcastle + 1;
			whiterookkcastle = 1;
		}
		else {
			undowrookqcastle = undowrookqcastle + 1;
			whiterookqcastle = 1;
		}
	}
	else if(ps.piecestr[0] == 'P' && rofp == 5) {
		if(pieceprev[rtop - 1][ctop][0] == 'p' && strlen(piece[rtop - 1][ctop]) == 0) {
			strcpy(ps.empasskilled, pieceprev[rtop - 1][ctop]);
		}
	}
	else if(ps.piecestr[0] == 'p' && rofp == 4) {
                if(pieceprev[rtop + 1][ctop][0] == 'P' && strlen(piece[rtop + 1][ctop]) == 0) {
                        strcpy(ps.empasskilled, pieceprev[rtop + 1][ctop]);
		}
        }
	if(psmoveassurance)
		return 0;
        
	int row, column;
	for(row = 1; row <= 8; row++) {
		for(column = 1; column <= 8; column++) {
			if(player == 1) {
				if(piece[row][column][0] == 'K') {
					if(cheak(piece, row, column, 2) == 1) {
						undo(piece);
						printf("YOU CANNOT HAVE THIS MOVE YOUR KING WILL NOT BE SAFE");
						i = enteronceagain(line, player, piece);
						return i;
					}
				}
			}
			else if(player == 2) {
				if(piece[row][column][0] == 'k') {
					if(cheak(piece, row, column, 1) == 1) {
						undo(piece);
						printf("YOU CANNOT HAVE THIS MOVE YOUR KING WILL NOT BE SAFE");
						i = enteronceagain(line, player, piece);
						return i;
					}
				}
			}
		}
	}
	strcpy(ups.empasskilled, "");
        ups.rowprev = rofp;
        ups.rownow = rtop;
        ups.columnprev = cofp;
        ups.columnnow = ctop;
        strcpy(ups.piecestr, "");
        strcpy(ups.piecestr, piece[rtop][ctop]);
        ups.playerprev = player;
        strcpy(ups.piecekilled, "");
        strcpy(ups.piecekilled, pieceprev[rtop][ctop]);
        cheakundo = 1;
        if(ups.piecestr[0] == 'k') {
                undobcheakcastle = undobcheakcastle + 1;
                blackcheakcastle = 1;
        }
        else if(ups.piecestr[0] == 'K') {
                undowcheakcastle = undowcheakcastle + 1;
                whitecheakcastle = 1;
        }
        else if(ups.piecestr[0] == 'r') {
                if(ups.piecestr[1] == '1') {
                        undobrookkcastle = undobrookkcastle + 1;
                        blackrookkcastle = 1;
                }
                else {
                        undobrookqcastle = undobrookqcastle + 1;
                        blackrookqcastle = 1;
                }
        }
        else if(ups.piecestr[0] == 'R') {
                if(ups.piecestr[1] == '1') {
                        undowrookkcastle = undowrookkcastle + 1;
                        whiterookkcastle = 1;
                }
                else {
                        undowrookqcastle = undowrookqcastle + 1;
                        whiterookqcastle = 1;
                }
        }
        else if(ups.piecestr[0] == 'P' && rofp == 5) {
                if(pieceprev[rtop - 1][ctop][0] == 'p' && strlen(piece[rtop - 1][ctop]) == 0) {
                        strcpy(ups.empasskilled, pieceprev[rtop - 1][ctop]);
                }
        }
        else if(ups.piecestr[0] == 'p' && rofp == 4) {
                if(pieceprev[rtop + 1][ctop][0] == 'P' && strlen(piece[rtop + 1][ctop]) == 0) {
                        strcpy(ups.empasskilled, pieceprev[rtop + 1][ctop]);
                }
        }

						



	return 0;
}
// makepieces assigns strings to the piece at the start of the new game

void makepieces(char piece[9][9][4]) {
	int row, column;
	char tmp;
	for(row = 1; row <= 8; row++) {
		for(column = 1; column <= 8; column++) {
			if(row == 1)
				switch(column) {
					case 1: strcpy(piece[row][column], "R1."); break;
					case 2: strcpy(piece[row][column], "N1."); break;
					case 3: strcpy(piece[row][column], "B1."); break;
					case 4: strcpy(piece[row][column], "K.."); break;
					case 5: strcpy(piece[row][column], "Q.."); break;
					case 6: strcpy(piece[row][column], "B2."); break;
					case 7: strcpy(piece[row][column], "N2."); break;
					case 8: strcpy(piece[row][column], "R2."); break;
				}
			else if(row == 2) {
				tmp = column + '0';
				piece[row][column][0] = 'P';
				piece[row][column][1] = tmp;
				piece[row][column][2] = '.';
				piece[row][column][3] = '\0';
			}
			else if(row == 7) {
				tmp = column + '0';
				piece[row][column][0] = 'p';
				piece[row][column][1] = tmp;
				piece[row][column][2] = '\0';
			}
			else if(row == 8)
				switch(column) {
					case 1: strcpy(piece[row][column], "r1"); break;
					case 2: strcpy(piece[row][column], "n1"); break;
					case 3: strcpy(piece[row][column], "b1"); break;
					case 4: strcpy(piece[row][column], "k."); break;
					case 5: strcpy(piece[row][column], "q."); break;
					case 6: strcpy(piece[row][column], "b2"); break;
					case 7: strcpy(piece[row][column], "n2"); break;
					case 8: strcpy(piece[row][column], "r2"); break;
				}
			else
				strcpy(piece[row][column], "");
		}
	}	
}
//print is called by fuction printboard to print the pieces with proper colour

void print(char piece[4]) {
	if(piece[0] >= 65 && piece[0] <= 90) {
		printf(color"97m");
		if(piece[0] == 'P')
			printf("[p]");
		else
			printf("[%c]", piece[0]);
	}
	else if(piece[0] >= 97 && piece[0] <= 122) {
		printf(color"30m");
		if(piece[0] == 'p')
			printf("[p]");
		else
			printf("[%c]", piece[0] - 32);
	}
	else
		printf("   ");
}
// cheak cheaks weather a piece is at row = r and column = c is being attacked by any other opponent piece

int cheak(char piece[9][9][4], int r, int c, int player) {
	int l = 0, k = 0, r1 = 1, r2 = 1, r3 = 1, r4 = 1, b1 = 1, b2 = 1, b3 = 1, b4 = 1, i;
	if(player == 2) {
		i = 0;
	}
	else {
		i = -32;
	}
		if((piece[r + 1][c + 1][0] == 'p' + i && !i && r + 1 < 9 && c + 1 < 9)) {
			kone.row = r + 1;
			kone.column = c + 1;
			return 1;
		}
		else if(piece[r - 1][c + 1][0] == 'p' + i && i && r - 1 > 0 && c + 1 < 9) {
				kone.row = r - 1;
		       		kone.column = c + 1;
				return 1;
		}
		else if(piece[r - 1][c - 1][0] == 'p' + i && i && r - 1 > 0 && c - 1 > 0)  {
			kone.row = r - 1;
			kone.column = c - 1;
			return 1;
		}
		else if(piece[r + 1][c - 1][0] == 'p' + i && !i && r + 1 < 9 && c - 1 > 0) {
			kone.row = r + 1;
			kone.column = c - 1;
			return 1;
		}
		else if(piece[r + 1][c + 2][0] == 'n' + i && r + 1 < 0 && c + 2 < 9) {
		       kone.row = r + 1;
			kone.column = c + 2;
	 		return 1;
		}
		else if(piece[r + 2][c + 1][0] == 'n' + i && r + 2 < 9 && c + 1 < 9) {
			kone.row = r + 2;
			kone.column = c + 1;
			return 1;
		}
		else if(piece[r + 1][c - 2][0] == 'n' + i && r + 1 < 9 && c - 2 > 0) {
			kone.row = r + 1;
			kone.row = c - 2;
			return 1;
		}
		else if(piece[r + 2][c - 1][0] == 'n' + i && r + 2 < 9 && c - 1 > 0) {
		       kone.row = r + 2;
			kone.column = c - 1;
			return 1;
		}
		else if(piece[r - 1][c + 2][0] == 'n' + i && r - 1 > 0 && c + 2 < 9) {
	       		kone.row = r - 1;
			kone.column = c + 2;
			return 1;
		}
		else if(piece[r - 2][c + 1][0] == 'n' + i && r - 2 > 0 && c + 1 < 9) {
			kone.row = r - 2;
			kone.column = c + 1;
			return 1;
		}

		else if(piece[r - 1][c - 2][0] == 'n' + i && r - 1 > 0 && c - 2 > 0) {
	       		kone.row = r - 1;
			kone.column = c - 2;
			return 1;
		}
		else if(piece[r - 2][c - 1][0] == 'n' + i && r - 2 > 0 && c - 1 > 0) {

			kone.row = r - 2;
			kone.column = c - 1;
			return 1;
		}
		else if(piece[r][c - 1][0] == 'k' + i && c - 1 > 0) {
			kone.row = r;
			kone.column = c - 1;
			return 1;
		}
		else if(piece[r][c + 1][0] == 'k' + i && c + 1 < 9) {
		       	kone.row = r;
			kone.column = c + 1;
			return 1;
		}
		else if(piece[r - 1][c][0] == 'k' + i && r - 1 > 0) {
			kone.row = r - 1;
			kone.column = c;
			return 1;
		}
		else if(piece[r + 1][c][0] == 'k' + i && r + 1 < 9) {
		       kone.row = r + 1;
	       		kone.column = c;
	 		return 1;
		}
		else if(piece[r - 1][c - 1][0] == 'k' + i && r - 1 > 0 && c - 1 > 0) {
		       kone.row = r - 1;
	       		kone.column = c - 1;
	 		return 1;
		}
		else if(piece[r - 1][c + 1][0] == 'k' + i && r - 1 > 0 && c + 1 < 9) {
		       	kone.row = r - 1;
			kone.column = c + 1;
			return 1;
		}
		else if(piece[r + 1][c - 1][0] == 'k' + i && r + 1 < 9 && c - 1 > 0) {
		       kone.row = r + 1;
	       		kone.column = c - 1;
	 		return 1;
		}
		else if(piece[r + 1][c + 1][0] == 'k' + i && r + 1 < 9 && c + 1 < 9) {
			kone.row = r + 1;
			kone.column = c + 1;
			return 1;
		}
		while(1) {
			l++;
			k++;
			if(c + l <= 8 && r1) {
				if((piece[r][c + l][0] == 'r' + i || piece[r][c + l][0] == 'q' + i)) {
					kone.row = r;
					kone.column = c + l;
					return 1;
				}
				else if(strlen(piece[r][c + l]) != 0)
					r1 = 0;
			}
			else
				r1 = 0;
			if(c - l >= 1 && r2) {	
				if((piece[r][c - l][0] == 'r' + i || piece[r][c - l][0] == 'q' + i)) {
					kone.row = r;
					kone.column = c - l;
					return 1;
				}
				else if(strlen(piece[r][c - l]) != 0)
					r2 = 0;
			}
			else
				r2 = 0;
			if(r + k <= 8 && r3) {
				if((piece[r + k][c][0] == 'r' + i || piece[r + k][c][0] == 'q' + i)) {
					kone.row = r + k;
					kone.column = c;
					return 1;
				}
				else if(strlen(piece[r + k][c]) != 0)
					r3 = 0;
			}
			else
				r3 = 0;
			if(r - k >= 1 && r4) {
				if((piece[r - k][c][0] == 'r' + i || piece[r - k][c][0] == 'q' + i)) {
					kone.row = r - k;
					kone.column = c;
					return 1;
				}
				else if(strlen(piece[r - k][c]) != 0)
					r4 = 0;
			}
			else
				r4 = 0;
			if(r + k <= 8 && c + l <= 8 && b1) {
				if((piece[r + k][c + l][0] == 'b' + i || piece[r + k][c + l][0] == 'q' + i)) {
					kone.row = r + k;
					kone.column = c + l;
					return 1;
				}
				else if(strlen(piece[r + k][c + l]) != 0)
					b1 = 0;
			}
			else
				b1 = 0;
			if(r + k <= 8 && c - l >= 1 && b2) {
				 if((piece[r + k][c - l][0] == 'b' + i || piece[r + k][c - l][0] == 'q' + i)) {
					kone.row = r + k;
					kone.column = c - l;
					 return 1;
				 }
				 else if(strlen(piece[r + k][c - l]) != 0)
					 b2 = 0;
			}
			else
				b2 = 0;
			if(r - k >= 1 && c + l <= 8 && b3) {
				 if((piece[r - k][c + l][0] == 'b' + i || piece[r - k][c + l][0] == 'q' + i)) {
					kone.row = r - k;
					kone.column = c + l;
					 return 1;
				 }
				 else if(strlen(piece[r - k][c + l]) != 0)
					 b3 = 0;
			}
			else
				b3 = 0;
			if(r - k >= 1 && c - l >= 1 && b4) {
				if(piece[r - k][c - l][0] == 'b' + i || piece[r - k][c - l][0] == 'q' + i) {
					kone.row = r - k;
					kone.column = c - l;
					return 1;
				}
				else if(strlen(piece[r - k][c - l]) != 0)
					b4 = 0;
			}
			else
				b4 = 0;
			if(b1 || b2 || b3 || b4 || r1 || r2 || r3 || r4);
			else
				return 0;
		}

}




// chmate is shows cheak and cheamate when they occurs

int chmate(char piece[9][9][4], int player) {
	int row, column;
	int r, c;
	for(row = 1; row <= 8; row++) {
		for(column = 1; column <= 8; column++) {
			if(player == 2) {
				if(piece[row][column][0] == 'K') {
					r = row;
					c = column;
				}
			}
			else if(player == 1) {
				if(piece[row][column][0] == 'k') {
					r = row;
					c = column;
				}
			}
		}
	}
	int k;
	int a, b, d = 0, e = 0, f = 0;
	for(a = -1; a <= 1; a++) {
		for(b = -1; b <= 1; b++) {
			if((strlen(piece[r + a][c + b]) != strlen(piece[r][c])) && r + a <= 8 && r + a >= 1 && c + b <= 8 && c + b >= 1) {
				k = cheak(piece, r + a, c + b, player);
				e = e + k;
				f++;
			}
			else if(a == 0 && b == 0) {
				k = cheak(piece, r, c, player);
				d = k;
			}
		}
	}
	psmoves p;
	int psno = 0;
	for(row = 1; row <= 8; row++) {
		for(column = 1; column <= 8; column++) {
			p = possiblemoves(piece, 3 - player, row, column);
			psno = psno + p.noofpsmoves;
		}
	}

	if(psno == 0) {
		printf(color"31m");
		printf(color"5m");
		if(d == 1)
			printf("|| CHECKMATE ||");
		else
			printf("|| STALEMATE ||");
		printf(color"0m");
		printf("\n");
		return 1;
	}
	else if(d == 1) {
		printf(color"31m");
		printf(color"5m");
		if(player == 2)
			printf("|| CHECK ||");
		else
			printf("                              || CHECK ||");
		printf(color"0m");
		printf("\n");
	}
	return 0;
}

// printboard is called when we have to print the board

void printboard(char piece[9][9][4], int player) {
	int row, column;
	int k;
	printf("\n");
		for(row = 0; row <= 9; row++) {
			k = row;
			row = 9 - row;
			for(column = 9; column >= 1; column--) {
				if((row == 0 && column == 9) || (row == 9 && column ==9)) {
					if(player == 1)
						printf(color"47m");
					else
						printf(color"41m");

					printf("   ");
					printf(color"0m");
				}
				else if(row == 0 || row == 9) {
					printf(color"7m");
					if((column + row) % 2)
						printf(color"93m");
					else
						printf(color"96m");
					printf(" %c " , 'h' - column + 1);
					printf(color"0m");
				}
				else if(column == 9) {
					printf(color"7m");
					if(row % 2)
						printf(color"93m");
					else
						printf(color"96m");
					printf(" %d ", row);
					printf(color"0m");
				}
				else if((row + column) % 2) {
					printf(color"100m");
					print(piece[row][column]);
				}
				else {
					printf(color"43m");
					print(piece[row][column]);
				}
				printf(color"0m");
			}
			row = k;
			for(column = 0; column <=9; column++) {
				if((row == 9 || row == 0) && column == 0) {
					
					printf(color"41m");
					printf("   ");
					printf(color"0m");
				}
				else if(column == 9 && (row == 9 || row == 0)) {
					if(player == 2)
						printf(color"47m");
					else
						printf(color"41m");
					printf("   ");
					printf(color"0m");
				}
				else if(row == 9 || row == 0) {
					printf(color"7m");
					if((row + column) % 2)
						printf(color"93m");
					else
						printf(color"96m");
					printf(" %c ", 'a' + column - 1);
					printf(color"0m");
				}
				else if(column == 0 || column == 9) {
					printf(color"7m");
					if(row % 2)
						printf(color"96m");
					else
						printf(color"93m");
					printf(" %d ", 9 - row);
					printf(color"0m");

				}
				else if((row + column) % 2) {
					printf(color"100m");
					print(piece[row][column]);
				}
				else {
					printf(color"43m");
					print(piece[row][column]);
				}
				printf(color"0m");
			}
			printf("\n");
		}
		printf(color"0m");
}
void gameundo(char piece[9][9][4]) {
        if(piece[ups.rownow][ups.columnnow][0] == 'k' && undobcheakcastle == 1) {
                undobcheakcastle = 0;
                blackcheakcastle = 0;
        }
        else if(piece[ups.rownow][ups.columnnow][0] == 'K' && undowcheakcastle == 1) {
                undowcheakcastle = 0;
                whitecheakcastle = 0;
        }
        else if(piece[ups.rownow][ups.columnnow][0] == 'r') {
                if(piece[ups.rownow][ups.columnnow][1] == '1' && undobrookkcastle == 1) {
                        undobrookkcastle = 0;
                        blackrookkcastle = 0;
                }
                else if(piece[ups.rownow][ups.columnnow][1] == '2' && undobrookqcastle == 1) {
                        undobrookqcastle = 0;
                        blackrookqcastle = 0;
                }
        }
        else if(piece[ups.rownow][ups.columnnow][0] == 'R') {
                if(piece[ups.rownow][ups.columnnow][1] == '1' && undowrookkcastle == 1) {
                        undowrookkcastle = 0;
                        whiterookkcastle = 0;
                }
                else if(piece[ups.rownow][ups.columnnow][1] == '2' && undowrookqcastle == 1) {
                        undowrookqcastle = 0;
                        whiterookqcastle = 0;
                }
        }
        if(ups.piecestr[0] == 'k' && ups.rownow == ups.rowprev && ups.columnnow == ups.columnprev - 2) {
                strcpy(piece[ups.rowprev][ups.columnprev - 3], piece[ups.rowprev][ups.columnprev - 1]);
                strcpy(piece[ups.rowprev][ups.columnprev - 1], "");
        }
        else if(ups.piecestr[0] == 'k' && ups.rownow == ups.rowprev && ups.columnnow == ups.columnprev + 2) {
                strcpy(piece[ups.rowprev][ups.columnprev + 4], piece[ups.rowprev][ups.columnprev + 1]);
                strcpy(piece[ups.rowprev][ups.columnprev + 1], "");
        }
        else if(ups.piecestr[0] == 'K' && ups.rownow == ups.rowprev && ups.columnnow == ups.columnprev - 2) {
                strcpy(piece[ups.rowprev][ups.columnprev - 3], piece[ups.rowprev][ups.columnprev - 1]);
                strcpy(piece[ups.rowprev][ups.columnprev - 1], "");
        }
        else if(ups.piecestr[0] == 'K' && ups.rownow == ups.rowprev && ups.columnnow == ups.columnprev + 2) {
                strcpy(piece[ups.rowprev][ups.columnprev + 4], piece[ups.rowprev][ups.columnprev + 1]);
                strcpy(piece[ups.rowprev][ups.columnprev + 1], "");
        }
        strcpy(piece[ups.rowprev][ups.columnprev], piece[ups.rownow][ups.columnnow]);
        strcpy(piece[ups.rownow][ups.columnnow], ups.piecekilled);
        if(strlen(ups.empasskilled) == 2)
                strcpy(piece[ups.rownow - 1][ups.columnnow], ups.empasskilled);
        else if(strlen(ps.empasskilled) == 3)
                strcpy(piece[ups.rownow + 1][ups.columnnow], ups.empasskilled);


}


void undo(char piece[9][9][4]) {
	if(piece[ps.rownow][ps.columnnow][0] == 'k' && undobcheakcastle == 1) {
		undobcheakcastle = 0;
		blackcheakcastle = 0;
	}
	else if(piece[ps.rownow][ps.columnnow][0] == 'K' && undowcheakcastle == 1) {
		undowcheakcastle = 0;
		whitecheakcastle = 0;
	}
	else if(piece[ps.rownow][ps.columnnow][0] == 'r') {
		if(piece[ps.rownow][ps.columnnow][1] == '1' && undobrookkcastle == 1) {
			undobrookkcastle = 0;
			blackrookkcastle = 0;
		}
		else if(piece[ps.rownow][ps.columnnow][1] == '2' && undobrookqcastle == 1) {
			undobrookqcastle = 0;
			blackrookqcastle = 0;
		}
	}
	else if(piece[ps.rownow][ps.columnnow][0] == 'R') {
		if(piece[ps.rownow][ps.columnnow][1] == '1' && undowrookkcastle == 1) {
			undowrookkcastle = 0;
			whiterookkcastle = 0;
		}
		else if(piece[ps.rownow][ps.columnnow][1] == '2' && undowrookqcastle == 1) {
			undowrookqcastle = 0;
			whiterookqcastle = 0;
		}
	}
	if(ps.piecestr[0] == 'k' && ps.rownow == ps.rowprev && ps.columnnow == ps.columnprev - 2) {
		strcpy(piece[ps.rowprev][ps.columnprev - 3], piece[ps.rowprev][ps.columnprev - 1]);
		strcpy(piece[ps.rowprev][ps.columnprev - 1], "");
	}
	else if(ps.piecestr[0] == 'k' && ps.rownow == ps.rowprev && ps.columnnow == ps.columnprev + 2) {
                strcpy(piece[ps.rowprev][ps.columnprev + 4], piece[ps.rowprev][ps.columnprev + 1]);
                strcpy(piece[ps.rowprev][ps.columnprev + 1], "");
        }
	else if(ps.piecestr[0] == 'K' && ps.rownow == ps.rowprev && ps.columnnow == ps.columnprev - 2) {
                strcpy(piece[ps.rowprev][ps.columnprev - 3], piece[ps.rowprev][ps.columnprev - 1]);
                strcpy(piece[ps.rowprev][ps.columnprev - 1], "");
        }
	else if(ps.piecestr[0] == 'K' && ps.rownow == ps.rowprev && ps.columnnow == ps.columnprev + 2) {
                strcpy(piece[ps.rowprev][ps.columnprev + 4], piece[ps.rowprev][ps.columnprev + 1]);
                strcpy(piece[ps.rowprev][ps.columnprev + 1], "");
        }
	strcpy(piece[ps.rowprev][ps.columnprev], piece[ps.rownow][ps.columnnow]);
	strcpy(piece[ps.rownow][ps.columnnow], ps.piecekilled);
	if(strlen(ps.empasskilled) == 2)
		strcpy(piece[ps.rownow - 1][ps.columnnow], ps.empasskilled);
	else if(strlen(ps.empasskilled) == 3)
		strcpy(piece[ps.rownow + 1][ps.columnnow], ps.empasskilled);


}
// readline reads the input from the user
int readline(char line[50], int len, int player, char piece[9][9][4]) {
	char ch;
	int length;
	int i = 0, k = 0;
	line[i++] = 'a';
	while(1) {
		ch = getchar();
		if(ch == ' ')
			continue;
		else if(ch == '\n') {
			line[i] = '\0';
			break;
		}
		else {
			if(i == 1 || i == 3) {
				if(player == 2) {
					if(ch >= 97 && ch <= 104)
						ch = 'a' - ch + 56;
					else if(ch >= 65 && ch <= 72)
						ch = 'A' - ch + 56;
				}
				else {
					if(ch >= 97 && ch <= 104)
						ch = ch - 'a' + 49;
					else if(ch >= 65 && ch <= 72)
						ch = ch - 'A' + 49;
				}
			}
			else if((i == 2 || i == 4) && ch > 48 && ch < 57 && player == 1)
				ch = 57 - ch + '0';

				
			line[i++] = ch;
		}
	}
	if(strlen(line) !=  len) {
		if(!strcmp(line, "as") || !strcmp(line, "aS")) {
			k = 1;
			return k;
		}
		else if((!strcmp(line, "au") || !strcmp(line, "aU")) && cheakundo) {
			gameundo(piece);
			cheakundo = 0;
			return 30;
		}
		printf(color"91m");
		printf("      SOMETHING WRONG IN YOUR INPUT MOVE\n        ENTER YOUR CHOICE ONCE AGAIN\n");
		printf(color"0m");
		strcpy(line, "");
		if(player == 2)
			printboard(piece, 2);
		else
			printboard(piece, 1);
		if(player == 2) {
			printf(color"97m");
			printf(color"100m");
			printf("     WHITE PLAYER MOVE                                   ");
			printf(color"0m");
			printf("\n");
			printf("     ");
		}
		else {
			printf(color"107m");
			printf(color"30m");
			printf("                                BLACK PLAYER MOVE        ");
			printf(color"0m");
			printf("\n");
			printf("                                ");
		}
		length = readline(line, 5, player, piece);
		
		i = length;
		return i;
	}
	char tm;
	tm = line[1];
	line[1] = line[2];
	line[2] = tm;
	tm = line[3];
	line[3] = line[4];
	line[4] = tm;
	return i;
}
//save assings the piecesave structure and store it in a file called chess.txt
void save(char piece[9][9][4], int player) {
	int a, b, i = 0;
	piecesave s[32], k;
	for(a = 1; a <= 8; a++) {
		for(b = 1; b <= 8; b++) {
			if(strlen(piece[a][b]) != 0) {
				k.row = a;
				k.column = b;
				strcpy(k.str, piece[a][b]);
				s[i++] = k;
			}
		}
	}
	int fp;
	fp = open("chess.txt", O_WRONLY | O_CREAT, S_IWUSR | S_IRUSR);
	write(fp, &player, 4);
	write(fp, &i, 4);
	write(fp, s, i * sizeof(s[0]));
	close(fp);
}
// savepiece assing the the 2d string piece at the begining when a game is contined
void savepiece(char piece[9][9][4], piecesave s[32], int j) {
	int i;
	int row, column;
	for(row = 1; row <= 8; row++)
		for(column = 1; column <= 8; column++)
			strcpy(piece[row][column], "");
	for(i = 0; i < j; i++) {
		strcpy(piece[s[i].row][s[i].column], s[i].str);
	}
}
int preference(char c) {
		switch(c) {
			case 'Q': return 3; break;
			case 'R': return 2; break;
			case 'N': return 2; break;
			case 'B': return 2; break;
			case 'P': return 1; break;
                        case 'q': return 3; break;
                        case 'r': return 2; break;
                        case 'n': return 2; break;
                        case 'b': return 2; break;
                        case 'p': return 1; break;
			default : return 0; break;
	        }
}
struct aop {
	int pre, num, r, c;
}aop[20];
struct kop {
	int pre, num, no, r, c;
}kop[20];

struct cmove {
	int rofp, cofp, rtop, ctop;
}cmove;
void computermove(char piece[9][9][4], int player) {
	psmoves p[20];
	int row, column, i = 0;
	int a, b = 0, bt, c;
	for(row = 1; row <= 8; row++) {
		for(column = 1; column <= 8; column++) {
			if(player == 2) {
				if(strlen(piece[row][column]) == 2) {
					p[i] = possiblemoves(piece, 2, row, column);
					if(p[i].noofpsmoves) {
					aop[i].pre = preference(piece[row][column][0]);
					aop[i].num = i;
					aop[i].r = row;
					aop[i].c = column;
					for(a = 0; a < p[i].noofpsmoves; a++) {
						if(strlen(piece[p[i].psmove[a].psrow][p[i].psmove[a].pscolumn]) == 3) {
							kop[b].pre = preference(piece[p[i].psmove[a].psrow][p[i].psmove[a].pscolumn][0]);
							kop[b].r = row;
							kop[b].c = column;
							kop[b].num = i;
							kop[b].no = a;
							b++;
						}
					}
					i++;
					}
					//insert in list
				}
			}
			else if(player == 1) {
				if(strlen(piece[row][column]) == 3) {
					p[i] = possiblemoves(piece, 1, row, column);
					if(p[i].noofpsmoves) {
					aop[i].pre = preference(piece[row][column][0]);
					aop[i].num = i;
					aop[i].r = row;
					aop[i].c = column;
					for(a = 0; a < p[i].noofpsmoves; a++) {
                                                if(strlen(piece[p[i].psmove[a].psrow][p[i].psmove[a].pscolumn]) == 2) {
                                                        kop[b].pre = preference(piece[p[i].psmove[a].psrow][p[i].psmove[a].pscolumn][0]);
                                                        kop[b].r = row;
                                                        kop[b].c = column;
                                                        kop[b].num = i;
							kop[b].no = a;
							b++;
                                                }
                                        }

					i++;
					}
					// insert in the list
				}
			}
		}
	}
	bt = b;
	int tmpre, tmnum, tmc, tmr, tmno;
	for(a = 0; a < i; a++) {
		for(b = a + 1; b < i; b++) {
			if(aop[a].pre < aop[b].pre) {
				tmpre = aop[a].pre;
				tmnum = aop[a].num;
				tmc = aop[a].c;
				tmr = aop[a].r;
				aop[a].pre = aop[b].pre;
				aop[a].num = aop[b].num;
				aop[a].r = aop[b].r;
				aop[a].c = aop[b].c;
				aop[b].pre = tmpre;
				aop[b].num = tmnum;
				aop[b].r = tmr;
				aop[b].c = tmc;
			}
		}
	}
	for(a = 0; a < bt; a++) {
		for(b = a + 1; b < bt; b++) {
			if(kop[a].pre < kop[b].pre) {
                                tmpre = kop[a].pre;
                                tmnum = kop[a].num;
                                tmc = kop[a].c;
                                tmr = kop[a].r;
				tmno = kop[a].no;
                                kop[a].pre = kop[b].pre;
                                kop[a].num = kop[b].num;
                                kop[a].r = kop[b].r;
                                kop[a].c = kop[b].c;
				kop[a].no = kop[b].no;
                                kop[b].pre = tmpre;
                                kop[b].num = tmnum;
                                kop[b].r = tmr;
                                kop[b].c = tmc;
				kop[b].no = tmno;
                        }

		}
	}
	b = 0;
	c = 0;
	if(kop[b].pre > aop[c].pre && bt && i) {
                        cmove.rofp = kop[b].r;
                        cmove.cofp = kop[b].c;
                        cmove.rtop = p[kop[b].num].psmove[kop[b].no].psrow;
                        cmove.ctop = p[kop[b].num].psmove[kop[b].no].pscolumn;
                        return;
        }
	if(cheak(piece, aop[b].r, aop[b].c, 3 - player) == 1) {
		for(a = 0; a < p[aop[b].num].noofpsmoves; a++) {
			if(cheak(piece, p[aop[b].num].psmove[a].psrow, p[aop[c].num].psmove[a].pscolumn, 3 - player) == 0) {
				cmove.rofp = aop[b].r;
				cmove.cofp = aop[b].c;
				cmove.rtop = p[aop[b].num].psmove[a].psrow;
				cmove.ctop = p[aop[b].num].psmove[a].pscolumn;
				return;
			}
		}
	}

	if(kop[b].pre <= aop[b].pre && bt && i) {
		if(cheak(piece, p[kop[c].num].psmove[kop[c].no].psrow, p[kop[c].num].psmove[kop[c].no].pscolumn, 3 - player) == 0) {
                                        cmove.rofp = kop[c].r;
                                        cmove.cofp = kop[c].c;
                                        cmove.rtop = p[kop[c].num].psmove[kop[c].no].psrow;
                                        cmove.ctop = p[kop[c].num].psmove[kop[c].no].pscolumn;
                                        return;
		}
	}


	for(b = 0; b < bt; b++) {
	for(c = 0; c < i; c++) {
	if(cheak(piece, aop[c].r, aop[c].c, 3 - player) == 1) {
		if(kop[b].pre > aop[c].pre) {
			cmove.rofp = kop[b].r;
			cmove.cofp = kop[b].c;
			cmove.rtop = p[kop[b].num].psmove[kop[b].no].psrow;
			cmove.ctop = p[kop[b].num].psmove[kop[b].no].pscolumn;
			return;
		}
		else {
			for(a = 0; a < p[aop[c].num].noofpsmoves; a++) {
				if(cheak(piece, p[aop[c].num].psmove[a].psrow, p[aop[c].num].psmove[a].pscolumn, 3 - player) == 0) {
					cmove.rofp = aop[c].r;
					cmove.cofp = aop[c].c;
					cmove.rtop = p[aop[c].num].psmove[a].psrow;
					cmove.ctop = p[aop[c].num].psmove[a].pscolumn;
					return;
				}
			}
		}
	}
	}
	}
	b = 0;
	while(b < bt) {
		if(cheak(piece, p[kop[b].num].psmove[kop[b].no].psrow, p[kop[b].num].psmove[kop[b].no].pscolumn, 3 - player) == 0) {
			cmove.rtop = p[kop[b].num].psmove[kop[b].no].psrow;
			cmove.ctop = p[kop[b].num].psmove[kop[b].no].pscolumn;
			cmove.rofp = kop[b].r;
			cmove.cofp = kop[b].c;
			return;
		}
		else if(preference(piece[p[kop[b].num].psmove[kop[b].no].psrow][p[kop[b].num].psmove[kop[b].no].pscolumn][0]) > preference(piece[kop[b].r][kop[b].c][0])) {
			cmove.rtop = p[kop[b].num].psmove[kop[b].no].psrow;
                        cmove.ctop = p[kop[b].num].psmove[kop[b].no].pscolumn;
                        cmove.rofp = kop[b].r;
                        cmove.cofp = kop[b].c;
                        return;
		}
		b++;
	}
	int v, w;
	time_t tt;
	srandom(time(&tt));
	if(i)
		v = random() % i;
	else
		v = 0;
	if(p[aop[v].num].noofpsmoves)
		w = random() % p[aop[v].num].noofpsmoves;
	else
		w = 0;
	int n;
	//we have to acccordin v and w for the case and it is not going to easy\
	//
	cmove.rofp = aop[v].r;
	cmove.cofp = aop[v].c;
	cmove.rtop = p[aop[v].num].psmove[w].psrow;
        cmove.ctop = p[aop[v].num].psmove[w].pscolumn;
	printf("IT IS A RANDOM MOVE\n");
	return;

		
	// aop[0]  will have the highest preference so first of all we tend to save it
				
	//now sort the array according to preference
	//sort the list
	//now we will have to select a i;
	//we will first select our players for whom cheak is one store them in a list and then sort according to preference
	//we will then have player of the user which are bieng killed in another list and then sort them
	//we will select most preference plyaer to the compile it
	//if it is killing and cheak at killing position is 0 we will have the move 
	//if it is one then we willl chaek preference of our preference of its killing one and my killed one then

}
char promotioninput() {
	char c, d;
	printf("   PROMOTION : WHAT DO YOU WANT TO CHANGE THE PAWN TO\n   PRESS Q -> FOR QUEEN  R -> ROOK  B -> BISHOP  N -> KNIGHT\n    ");
	scanf("%c", &c);
	while(1) {
		d = getchar();
		if(d == '\n')
			break;
	}
	if(c == 'q' || c == 'Q' || c == 'R' || c == 'r' || c == 'b' || c == 'B' || c == 'N' || c == 'n')
		return c;
	c = promotioninput();
	return c;
}
void promotion(char piece[9][9][4], int player, int k) {
	int column;
	char c = 'q';
	int i = 0;
	if(player == 1) {
		for(column = 1; column <= 8; column++) {
			if(piece[8][column][0] == 'P') {
				if(k)
					c = promotioninput();
				if(c > 97)
					i = -32;
				piece[8][column][0] = c + i;
				return;
			}
		}
	}
	else if(player == 2) {
                for(column = 1; column <= 8; column++) {
                        if(piece[1][column][0] == 'p') {
				if(k)
                     	        	c = promotioninput();
                                if(c < 97)
                                        i = 32;
                                piece[1][column][0] = c + i;
                                return;
                        }
                }
        }
}
		
			

int main() {
	char line[50];
	int len, k = 0, i = 0, player = 2;
	clrscr();
	int a, b , d;
	char c;
	while(b != 1 && b != 2 && b != 3) {
		printf("   1 -> TWO PLAYERS  2 -> SINGLE PLAYER   3 -> no player\n   ");
		scanf("%d", &b);
		while(1) {
			c = getchar();
			if(c == '\n')
				break;
		}
	}

		
	if(b == 2) {
		while(d != 1 && d != 2) {
			printf("   SELECT YOUR COLOUR   1 -> WHITE   2 -> BLACK\n   ");
			scanf("%d", &d);
			while(1) {
				c = getchar();
				if(c == '\n')
					break;
			}
		}
	}
	while(a != 1 && a != 2) {	
		printf("   1 -> NEW GAME           2 -> CONTINUE THE SAVED GAME\n   ");
		scanf("%d", &a);

		while(1) {
			c = getchar();
			if(c == '\n')
				break;
		}
	}
	char piece[9][9][4];
	int chm;
	if(a == 2) {
		piecesave s[32];
		int fp;
		int i;
		fp  = open("chess.txt", O_RDONLY);
		if(fp == -1) {
			printf("NO SAVED GAME\n");
			exit(0);
		}
		read(fp, &player, 4);
		read(fp, &i, 4);
		read(fp, s, i * sizeof(s[0]));
		close(fp);
		savepiece(piece, s, i);
	}
	else
		makepieces(piece);
	if(b == 3) {
		printboard(piece, player);
		while(1) {
			if(player == 2) {
				char ch;
        	        	strcpy(line, "");
               			k = 0;
                		printf("   press ENTER for computer move\n   ");
	               		while(1) {
        	        		ch = getchar();
      	        			if(ch == '\n') {
                				line[k] = '\0';
                				break;
	                		}
        	        	line[k++] = ch;
	
	                	}
        	                if(!strcmp(line, "s") || !strcmp(line, "S")) {
                	        	stop(piece, player);
                        		break;
	                        }
	
        	                else {
	
         		               	strcpy(line, "");
                                        player = 1;
                                        computermove(piece, player);
                                        line[0] = 'a';
                                        line[1] = cmove.rofp + '0';
                                        line[2] = cmove.cofp + '0';
                                        line[3] = cmove.rtop + '0';
                                        line[4] = cmove.ctop + '0';
                                        line[5] = '\0';
					printf("%s\n", line);
                                        move(line, player, piece);
                                        promotion(piece, player, 0);

                                        printboard(piece, player);
                                        chm = chmate(piece, player);
                                        if(chm == 1) {
                                                printf("###   WHITE WINS   ###\n");
                                                exit(0);
                                        }
                                }
			}

		 	else if(player == 1) {
                                strcpy(line, "");
                                char ch;
                                printf("   press ENTER for computer move\n   ");
                                while(1) {
                                        ch = getchar();
                                        if(ch == '\n') {
                                                line[k] = '\0';
                                                break;
                                        }
                                        line[k++] = ch;
                                }
                                if(!strcmp(line, "s") || !strcmp(line, "S")) {
                                        stop(piece, player);
                                        break;
                                }
				else {
					player = 2;
        	                        //comp move
                	                strcpy(line, "");
                        	        computermove(piece, player);
	                                line[0] = 'a';
        	                        line[1] = cmove.rofp + '0';
                	                line[2] = cmove.cofp + '0';
                        	        line[3] = cmove.rtop + '0';
	                                line[4] = cmove.ctop + '0';
        	                        line[5] = '\0';
					printf("%s\n", line);
                	                move(line, player, piece);
                        	        promotion(piece, player, 0);
                                	printboard(piece, player);
	                                strcpy(line, "");
        	                        chm = chmate(piece, player);
                	                if(chm == 1) {
                        	                printf("###  BLACK WINS  ###\n");
                                	        exit(0);
	                                }
				}
			}
		}
	}
	else if(b == 2) {
		if(d == 1) {
			player = 2;
			printboard(piece, player);
			while(1) {
				strcpy(line, "");
				printf(color"97m");
                                printf(color"100m");
                                printf("     YOUR MOVE                                           ");
                                printf(color"0m");
                                printf("\n");
                                printf("     ");
                                len = readline(line, 5, player, piece);
                                player = 1;
                                if(len == 1) {
                                        stop(piece, player);
                                        break;
                                }
                                else if(len == 30);
                                else {
                                        i = move(line, player, piece);
                                        if(i == 1)
                                                break;
                                }
				promotion(piece, player, 1);
                                strcpy(line, "");
                                int chm;
				printboard(piece, player);
                                chm = chmate(piece, player);
                                if(chm == 1) {
                                        printf("####  YOU WINS  ###\n");
                                        exit(0);
				}
				player = 2;
				strcpy(line, "");
				char ch;
				printf("   press ENTER for computer move or U for undo\n   ");
				while(1) {
					ch = getchar();
					if(ch == '\n') {
						line[k] = '\0';
						break;
					}
					line[k++] = ch;
				}
				if(!strcmp(line, "u") || !strcmp(line, "U")) {
					gameundo(piece);
					printboard(piece, player);
					continue;
				}
				else if(!strcmp(line, "s") || !strcmp(line, "S")) {
                                        stop(piece, player);
					break;
                                }


				//comp move
				strcpy(line, "");
				computermove(piece, player);
				line[0] = 'a';
				line[1] = cmove.rofp + '0';
				line[2] = cmove.cofp + '0';
				line[3] = cmove.rtop + '0';
				line[4] = cmove.ctop + '0';
				line[5] = '\0';
				printf("%s\n", line);
				move(line, player, piece);
				promotion(piece, player, 0);
				printboard(piece, player);
				strcpy(line, "");
				chm = chmate(piece, player);
				if(chm == 1) {
					printf("###  YOU LOSSE  ###\n");
					exit(0);
				}
			}
		}
		else {

			player = 1;
			printboard(piece, 3 - player);
			while(1) {
				char ch;
				strcpy(line, "");
				k = 0;
				printf("   press ENTER for computer move or U for undo\n   ");
				while(1) {
					ch = getchar();
					if(ch == '\n') {
						line[k] = '\0';
						break;
					}
					line[k++] = ch;
					
				}
				if(!strcmp(line, "u") || !strcmp(line, "U")) {
					gameundo(piece);
					player = 1;
					printboard(piece, player);
				}
				else if(!strcmp(line, "s") || !strcmp(line, "S")) {
                                        stop(piece, player);
                                        break;
                                }

				else {

					strcpy(line, "");
					player = 1;
					computermove(piece, player);
					line[0] = 'a';
                        	        line[1] = cmove.rofp + '0';
                	                line[2] = cmove.cofp + '0';
        	                        line[3] = cmove.rtop + '0';
	                                line[4] = cmove.ctop + '0';
                        	        line[5] = '\0';
					printf("%s\n", line);
        	                        move(line, player, piece);
					promotion(piece, player, 0);
	
					printboard(piece, player);
					chm = chmate(piece, player);
					if(chm == 1) {
						printf("###   YOU LOSSE   ###\n");
						exit(0);
					}
				}
				printf(color"107m");

                                printf(color"30m");
                                printf("                                      YOUR  MOVE         ");
                                printf(color"0m");
                                printf("\n");
                                printf("                                 ");

                                len = readline(line, 5, player, piece);
                                player = 2;
                                if(len == 1) {
                                        stop(piece, player);
                                        break;
                                }
                                else if(len == 30);
                                else {
                                        i = move(line, player, piece);
                                        if(i == 1)
                                                break;
                                }
				promotion(piece, player, 1);
                                strcpy(line, "");

				printboard(piece, player);
                                chm = chmate(piece, player);
                                if(chm == 1) {
                                        printf("###  YOU WINS  ###\n");
                                        exit(0);
                                }
			}

		}

	}	
	else {
		printboard(piece, player);
		while(1) {
			if(player == 2) {
				printf(color"97m");
				printf(color"100m");
				printf("     WHITE PLAYER MOVE                                   ");
				printf(color"0m");
				printf("\n");
				printf("     ");
				len = readline(line, 5, player, piece);
				player = 1;
				if(len == 1) {
					stop(piece, player);
					break;
				}
				else if(len == 30);
				else {
					i = move(line, player, piece);
					if(i == 1)
						break;
				}

				promotion(piece, player, 1);
				printboard(piece, player);
				strcpy(line, "");
				int chm;
				chm = chmate(piece, player);
				if(chm == 1) {
					printf("####  WHITE PLAYER WINS  ###\n");
					exit(0);
				}
	//		int r, k;
	//		for(r = 1; r <= 8; r++) {
	//			for(k = 1; k <= 8; k++) {
	//				if(piece[r][k][0] < 90)
	//					p = possiblemoves(piece, 1, r, k);
	//				else if(piece[r][k][0] > 90)
	//					p = possiblemoves(piece, 2 , r, k);
	//				printf("%s\t%d\n", piece[r][k], p.noofpsmoves);
	//			}
	//		}
			}
			else if(player == 1) {
				printf(color"107m");
		
				printf(color"30m");
				printf("                                 BLACK PLAYER MOVE       ");
				printf(color"0m");
				printf("\n");
				printf("                                 ");
			
				len = readline(line, 5, player, piece);
				player = 2;
				if(len == 1) {
					stop(piece, player);
					break;
				}
				else if(len == 30);
				else {
					i = move(line, player, piece);
					if(i == 1)
						break;
				}
				promotion(piece, player, 1);
				printboard(piece, player);
				strcpy(line, "");
				int chm;
				chm = chmate(piece, player);
				if(chm == 1) {
					printf("###  BLACK PLAYER WINS  ###");
					exit(0);
				}
			}
		}
	}

	return 0;
}

