#include "assets.h"

int est_piece(Piece *board[][8], Coord pos){
  if(board[pos.y][pos.x] != NULL){
    return 1;
  }else{
    return 0;
  }
}

int en_dehors(Coord pos){
  if (pos.x >= 0 && pos.x <= 7 && pos.y >= 0 && pos.y <= 7) {
    return 0;
  }
  return 1;
}

int sur_allie(Piece *board[][8], Coord pos, Coord target){
  if(board[target.y][target.x] == NULL
    || board[target.y][target.x]->couleur != board[pos.y][pos.x]->couleur){
    return 0;
  }
  return 1;
}

int depasse_piece(Piece *board[][8], Coord pos, Coord target, Coord deplace){
  if(!(pos.x == (target.x - deplace.x) && pos.y == (target.y - deplace.y)) &&
   board[target.y - deplace.y][target.x - deplace.x] != NULL){
    return 1;
  }
  return 0;
}

void formeL_possible(Piece *board[][8], Coord pos, int *moves){
  int i, swp;
  Coord dec;
  Coord tmp;

  /*initialisation du vecteur dec, valeur de decalages*/
  dec.x = 1;
  dec.y = -2;
  for (i = 0; i < 8; i+=2) {
    tmp.x = pos.x + dec.x;
    tmp.y = pos.y + dec.y;
    moves[i] = (!en_dehors(tmp) && !sur_allie(board, pos, tmp)) * -1;

    /*Rotation de 90° du vecteur dec*/
    swp = dec.x;
    dec.x = -dec.y;
    dec.y = swp;
  }

  /*initialisation du vecteur dec, valeur de decalages*/
  dec.x = 2;
  dec.y = -1;
  for (i = 1; i < 8; i+=2) {
    tmp.x = pos.x + dec.x;
    tmp.y = pos.y + dec.y;
    moves[i] = (!en_dehors(tmp) && !sur_allie(board, pos, tmp)) * -1;

    /*Rotation de 90° du vecteur dec*/
    swp = dec.x;
    dec.x = -dec.y;
    dec.y = swp;
  }
}

void droit_possible(Piece *board[][8], Coord pos, int *moves){
  int i, j, valide, swp;
  Coord dec;
  Coord tmp;

  /*initialisation du vecteur dec, valeur de decalages*/
  dec.x = 0;
  dec.y = -1;

  for (i = 0; i < 8; i+=2) {
    valide = 1;
    tmp = pos;
    j = 0;
    while (valide && j < board[pos.y][pos.x]->move.limitation) {
      tmp.x = tmp.x + dec.x;
      tmp.y = tmp.y + dec.y;
      valide = !en_dehors(tmp) && !sur_allie(board, pos, tmp) && !depasse_piece(board, pos, tmp, dec);
      if(valide){
        moves[i]++;
      }
      j++;
    }

    /*Rotation de 90° du vecteur dec*/
    swp = dec.x;
    dec.x = -dec.y;
    dec.y = swp;
  }
}

void diagonal_possible(Piece *board[][8], Coord pos, int *moves){
  int i, j, valide, swp;
  Coord dec;
  Coord tmp;

  /*initialisation du vecteur dec, valeur de decalages*/
  dec.x = 1;
  dec.y = -1;

  for (i = 1; i < 8; i+=2) {
    valide = 1;
    tmp = pos;
    j = 0;
    while (valide && j < board[pos.y][pos.x]->move.limitation) {
      tmp.x = tmp.x + dec.x;
      tmp.y = tmp.y + dec.y;
      valide = !en_dehors(tmp) && !sur_allie(board, pos, tmp) && !depasse_piece(board, pos, tmp, dec);
      if(valide){
        moves[i]++;
      }
      j++;
    }

    /*Rotation de 90° du vecteur dec*/
    swp = dec.x;
    dec.x = -dec.y;
    dec.y = swp;
  }
}

void ajustement_p_possible(Piece *board[][8], Coord pos, int *moves){
  int i;
  Coord tmp;

  if (board[pos.y][pos.x]->couleur == Blanc) {
    i = 1;
    tmp.x = pos.x;
    tmp.y = pos.y - i;
    while (!en_dehors(tmp) && board[tmp.y][tmp.x] == NULL && i <= board[pos.y][pos.x]->move.ajustement[0]) {
      moves[0]++;
      i++;
      tmp.y = pos.y - i;
    }

    tmp.x = pos.x - 1;
    tmp.y = pos.y - 1;
    if (!en_dehors(tmp) && board[tmp.y][tmp.x] != NULL && board[pos.y - 1][pos.x - 1]->couleur == Noir){
      moves[7] = 1;
    }

    tmp.x = pos.x + 1;
    if (!en_dehors(tmp) && board[tmp.y][tmp.x] != NULL && board[pos.y - 1][pos.x + 1]->couleur == Noir){
      moves[1] = 1;
    }
  }

  if (board[pos.y][pos.x]->couleur == Noir) {
    i = 1;
    tmp.x = pos.x;
    tmp.y = pos.y + i;
    while (!en_dehors(tmp) && board[tmp.y][tmp.x] == NULL && i <= board[pos.y][pos.x]->move.ajustement[0]) {
      moves[4]++;
      i++;
      tmp.y = pos.y + i;
    }

    tmp.x = pos.x - 1;
    tmp.y = pos.y + 1;
    if (!en_dehors(tmp) && board[tmp.y][tmp.x] != NULL && board[pos.y + 1][pos.x - 1]->couleur == Blanc){
      moves[5] = 1;
    }

    tmp.x = pos.x + 1;
    if (!en_dehors(tmp) && board[tmp.y][tmp.x] != NULL && board[pos.y + 1][pos.x + 1]->couleur == Blanc){
      moves[3] = 1;
    }
  }
}

void moves_possibles(Piece *board[][8], Coord pos, int *moves){
  if(board[pos.y][pos.x]->move.formeL.val){
    formeL_possible(board, pos, moves);
  }

  if(board[pos.y][pos.x]->move.droit.val){
    droit_possible(board, pos, moves);
  }

  if(board[pos.y][pos.x]->move.diagonal.val){
    diagonal_possible(board, pos, moves);
  }

  if(board[pos.y][pos.x]->move.ajustement[1] == 'p'){
    ajustement_p_possible(board, pos, moves);
  }
}

Coord get_pos_roi(Piece *board[][8], unsigned int color){
  int i, j;
  Coord pos;
  pos.x = -1;
  pos.y = -1;
  for(i = 0; i < 8; i++){
    for(j = 0; j < 8; j++){
      if(board[i][j] != NULL && board[i][j]->rang == Roi && board[i][j]->couleur == color){
        pos.x = j;
        pos.y = i;
        return pos;
      }
    }
  }
  return pos;
}

int est_mortel(Piece *board[][8], Coord pos, unsigned int color){
  int i, j, x, moves[8];
  Coord enemy;

  for(i = 0; i < 8; i++){
    for(j = 0; j < 8; j++){
      if(board[i][j] != NULL && color !=board[i][j]->couleur){

        enemy.x = j;
        enemy.y = i;

        for(x = 0; x < 8; x++){
          moves[x] = 0;
        }
        moves_possibles(board, enemy, moves);
        for(x = 0; x < 8; x++){
          if(pos.x < enemy.x){
            if(pos.y < enemy.y){
              if(board[enemy.y][enemy.x]->rang == Cavalier && moves[7] == -1){
                if(enemy.y == pos.y+2 && enemy.x == pos.x+1){
                  return 1;
                }
              }
              if(board[enemy.y][enemy.x]->rang == Cavalier && moves[6] == -1){
                if(enemy.y == pos.y+1 && enemy.x == pos.x+2){
                  return 1;
                }
              }
              if(moves[7] >= (enemy.y-pos.y) && (enemy.y-pos.y) == (enemy.x-pos.x)){
                return 1;
              }
            }
            if(pos.y == enemy.y){
              if(moves[6] >= (enemy.x-pos.x)){
                return 1;
              }
            }
            if(pos.y > enemy.y){
              if(board[enemy.y][enemy.x]->rang == Cavalier && moves[5] == -1){
                if(enemy.y == pos.y-1 && enemy.x == pos.x+2){
                  return 1;
                }
              }
              if(board[enemy.y][enemy.x]->rang == Cavalier && moves[4] == -1){
                if(enemy.y == pos.y-2 && enemy.x == pos.x+1){
                  return 1;
                }
              }
              if(moves[5] >= (pos.y-enemy.y) && (pos.y-enemy.y) == (enemy.x-pos.x)){
                return 1;
              }
            }
          }
          if(pos.x > enemy.x){
            if(pos.y > enemy.y){
              if(board[enemy.y][enemy.x]->rang == Cavalier && moves[3] == -1){
                if(enemy.y == pos.y-2 && enemy.x == pos.x-1){
                  return 1;
                }
              }
              if(board[enemy.y][enemy.x]->rang == Cavalier && moves[2] == -1){
                if(enemy.y == pos.y-1 && enemy.x == pos.x-2){
                  return 1;
                }
              }
              if(moves[3] >= (pos.y-enemy.y) && (pos.y-enemy.y) == (pos.x-enemy.x)){
                return 1;
              }
            }
            if(pos.y == enemy.y){
              if(moves[2] >= (pos.x-enemy.x)){
                return 1;
              }
            }
            if(pos.y < enemy.y){
              if(board[enemy.y][enemy.x]->rang == Cavalier && moves[1] == -1){
                if(enemy.y == pos.y+1 && enemy.x == pos.x-2){
                  return 1;
                }
              }
              if(board[enemy.y][enemy.x]->rang == Cavalier && moves[0] == -1){
                if(enemy.y == pos.y+2 && enemy.x == pos.x-1){
                  return 1;
                }
              }
              if(moves[1] >= (pos.x-enemy.x) && (pos.x-enemy.x) == (enemy.y-pos.y)){
                return 1;
              }
            }
          }
          if(pos.x == enemy.x){
            if(pos.y > enemy.y){
              if(moves[4] >= (pos.y-enemy.y)){
                return 1;
              }
            }
            if(pos.y < enemy.y){
              if(moves[0] >= (enemy.y-pos.y)){
                return 1;
              }
            }
          }
        }
      }
    }
  }
  return 0;
}

int est_echec(Piece *board[][8], unsigned int color){
  Coord pos = get_pos_roi(board, color);
  if(est_mortel(board, pos, color)){
    return 1;
  }
  return 0;
}

int sgn(int i){
  if (i > 0) {
    return 1;
  } else if (i < 0) {
    return -1;
  } else {
    return 0;
  }
}

void moves_legaux(Piece *board[][8], Coord pos, int *moves){
  int i,j, swp;
  int a_change = 0;
  int del_start = -1;
  float angleVecCav[] = {0.8, 0.6};
  Piece *swpPiece;
  Piece *tempPiece = board[pos.y][pos.x];
  Coord dec;
  Coord posRoi = get_pos_roi(board, tempPiece->couleur);

  moves_possibles(board, pos, moves);

  moves[8] = est_echec(board, tempPiece->couleur);

  if(!(est_echec(board, tempPiece->couleur))){
    board[pos.y][pos.x] = NULL;

    if(est_echec(board, tempPiece->couleur)){
      if(tempPiece->rang != Cavalier){
        if (pos.x == posRoi.x) {
          del_start = 1;
        }

        if (pos.y == posRoi.y) {
          del_start = 3;
        }

        if (pos.x - posRoi.x == pos.y - posRoi.y) {
          del_start = 0;
        }

        if (pos.x - posRoi.x == posRoi.y - pos.y) {
          del_start = 2;
        }

        for (i = del_start; i < del_start + 3 ; i++) {
          moves[i] = 0;
          moves[(i+4) % 8] = 0;
        }
      } else {
        for (j = 0; j < 8; j++) {
          moves[j] = 0;
        }
      }
    }

    board[pos.y][pos.x] = tempPiece;

  }

  if(est_echec(board, tempPiece->couleur) || tempPiece->rang == Roi){
    if(tempPiece->rang != Cavalier){
      dec.x = 0;
      dec.y = -1;
    } else {
      dec.x = 1;
      dec.y = -2;
    }
    board[pos.y][pos.x] = NULL;

    for (i = 0; i < 8; i++) {
      if(tempPiece->rang != Cavalier){
        for (j = 1; j <= moves[i]; j++) {
          swpPiece = board[pos.y + dec.y * j][pos.x + dec.x * j];
          board[pos.y + dec.y * j][pos.x + dec.x * j] = tempPiece;

          if(!(est_echec(board, tempPiece->couleur))){
            moves[i] = j;
            a_change = 1;
          }

          board[pos.y + dec.y * j][pos.x + dec.x * j] = swpPiece;
        }
      } else {
        swpPiece = board[pos.y + dec.y][pos.x + dec.x];
        board[pos.y + dec.y][pos.x + dec.x] = tempPiece;

        if(!(est_echec(board, tempPiece->couleur))){
          a_change = 1;
        }

        board[pos.y + dec.y][pos.x + dec.x] = swpPiece;
      }

      if(a_change){
        a_change = 0;
      } else {
        moves[i] = 0;
      }

      swp = dec.x;
      if(tempPiece->rang != Cavalier){
        dec.x = sgn(swp - dec.y);
        dec.y = sgn(swp + dec.y);
      } else {
        dec.x = (int)(swp*angleVecCav[i % 2] - dec.y*angleVecCav[(i + 1) % 2]);
        dec.y = (int)(swp*angleVecCav[(i + 1) % 2] + dec.y*angleVecCav[i % 2]);
      }
    }

    board[pos.y][pos.x] = tempPiece;
  }
}

int est_legal(Piece *board[][8], Coord pos, Coord target, int *moves){
  int i, j, start, swp;
  float angleVecCav[] = {0.8, 0.6};
  Piece *tempPiece = board[pos.y][pos.x];
  Coord dec;

  if(tempPiece->rang != Cavalier){
    dec.x = 0;
    dec.y = -1;
  } else {
    dec.x = 1;
    dec.y = -2;
  }

  for (i = 0; i < 8; i++) {
    if(tempPiece->rang != Cavalier){
      if(moves[8]){
        start = moves[i];
      } else{
        start = 1;
      }
      for (j = start; j <= moves[i]; j++) {
        if (pos.x + dec.x*j == target.x && pos.y + dec.y*j == target.y){
          return 1;
        }
      }
    } else {
      if (pos.x + dec.x == target.x && pos.y + dec.y == target.y){
        return 1;
      }
    }

    swp = dec.x;
    if(tempPiece->rang != Cavalier){
      dec.x = sgn(swp - dec.y);
      dec.y = sgn(swp + dec.y);
    } else {
      dec.x = (int)(swp*angleVecCav[i % 2] - dec.y*angleVecCav[(i + 1) % 2]);
      dec.y = (int)(swp*angleVecCav[(i + 1) % 2] + dec.y*angleVecCav[i % 2]);
    }
  }

  return 0;
}
