int bTetris = 0;

#define FIELD_W 8
#define FIELD_H 48
#define NUM_FIGURES 7

int dropTime = 0;
int moveTime = 0;

#define fmap(k,i,j) (p[k]&(1<<(i*4+j)))
u16 p[7] = {51,4369,116,71,54,99,39};

int screen[FIELD_W][FIELD_H] = {0};
int map[4][4];
int px, py, score, nextmap;
u8 buf[192];

unsigned int seed = 0x269ec3;
int rnd( int range ) {
    seed = seed*0x343fD + 0x269ec3;
    return ((seed>>16)&0x7fff) % range;
}

void putpixel(int x, int y, int color, u8 * buf) {
  if (x<0 || x>=96) return;
  if (y<0 || y>=16) return;  
  x = 95 - x; // flip vertical  
  u8 b = 1 << (y % 8);
  buf += (y/8)*96 + (x%96);
  if (color)
    *buf |= b;
  else
    *buf &= ~b;
}

void draw_field(int x, int y, u8 * buf, int buff[FIELD_W][FIELD_H]) {
  int i,j,w,h;
  int p = 2;
  for (i=0; i<FIELD_H; i++)
    for (j=0; j<FIELD_W; j++)
      for (w=0; w<p; w++)
        for (h=0; h<p; h++)
          putpixel(96-1-(x+i*p+w), (y+j*p+h),buff[j][i], buf);
}

void print(void) {
  int i, j;
  static int buff[FIELD_W][FIELD_H];

  for(i = 0; i < FIELD_H; i++)
    for(j = 0; j < FIELD_W; j++)
      buff[j][i] = screen[j][i];

  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      if(map[j][i])
        buff[j+px][i+py] = 1;

  draw_field(0,0,buf, buff);
  
  for (i=0; i<96; i++) {
    putpixel(i,0,1,buf);
    putpixel(i,15,1,buf);
  }
  
  for (i=0; i<16; i++) {
    putpixel(0,i,1,buf);
  }  
}

int valid(int x, int y) {
  int i, j;
  if(x < 0) return FALSE;
  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      if(map[j][i]) {
        if ((j+x>=FIELD_W) || (i+y>= FIELD_H)) return FALSE;
        if (screen[j+x][i+y]) return FALSE;
      }
  return TRUE;
}

#define inv(x) ((x*(-1))+3)

void rotatemap(void) {
  int _map[4][4];
  int i, j, sx = 4, sy = 4;

  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++) {
      _map[j][i] = map[j][i];
      if(map[j][i]) {
        if(i < sx) sx = i;
        if(inv(j) < sy) sy = inv(j);
      }
    map[j][i] = 0;
  }

  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      if(_map[inv(i)][j]) map[j - sx][i - sy] = 1;

  if(!valid(px,py)) for(i = 0; i < 4; i++) for(j = 0; j < 4; j++) map[j][i] = _map[j][i];
}

void deleteline(void) {
  int i, j, k, cl;

  for(i = FIELD_H - 1; i >= 0; i--) {
    cl = 1;

    for(j = 0, cl = 1; j < FIELD_W; j++)
      if(!screen[j][i]) cl = 0;

    if(cl) {
        score += (((i * (-1)) + FIELD_H) * 10);
        for(k = i; k > 0; k--) {
            for(j = 0; j < FIELD_W; j++) {
                screen[j][k] = screen[j][k - 1];
            }
        }
        i++;
        print();
    }
  }
}

void createmap(void) {
  int i, j; 
  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      map[j][i] = fmap(nextmap,i,j)?1:0;
  py = 0;
  px = FIELD_W / 2;
  nextmap = rnd(NUM_FIGURES);
}

void clearscreen(void) {
  int i, j;
  for(i = 0; i < FIELD_H; i++) 
    for(j = 0; j < FIELD_W; j++) 
      screen[j][i] = 0;
}

void startgame(void) {
  clearscreen();
  px = FIELD_W / 2;
  py = 0;
  score = 0;
  nextmap = rnd(NUM_FIGURES);
  createmap();
}

int gameover(void) {
  int i;
  for(i = 0; i < FIELD_W; i++)
    if (screen[i][0]) return TRUE;
  return FALSE;
}

void advancefigure(void) {
  int i, j;

  if (!valid(px, py + 1)) {
    for(i = 0; i < 4; i++)
        for(j = 0; j < 4; j++)
            if (map[j][i])
              screen[px+j][py+i] = 1;

    createmap();
    deleteline();
  }
  else
    py++;
}

void dropfigure(void) {
  int i, j;
  for(; valid(px, py + 1); py++);
  for(i = 0; i < 4; i++)
    for(j = 0; j < 4; j++)
      if(map[j][i])
        screen[px+j][py+i] = 1;
}

int bInit = 0;

void Show_Tetris(int key, int tilt) {

  int currentTime = Timer_StartValue();
    
  if (!bInit) {
     startgame();
     bInit = 1;
  }

  if (tilt!=0 && currentTime > moveTime+10) {    
    if (tilt>0 && valid(px+1, py)) px++;
    if (tilt<0 && valid(px-1, py)) px--;
    moveTime = currentTime;
  }  
  
  switch(key) {
    case KEY_V1:      
      if (tilt!=0) {
         dropfigure(); print(); deleteline(); createmap();
      } else {        
        rotatemap(); 
      }    
    break;
    default: break;
  }

  if (currentTime > dropTime+50) {
    dropTime = currentTime;
    advancefigure();
  }

  print();

  if (gameover())
    startgame();

  Oled_DrawArea(0,0,96,16,buf);
}
