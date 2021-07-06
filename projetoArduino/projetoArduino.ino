
#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <string.h>
#include "SymbolMono18pt7b.h"

// PINOS DISPLAY
#define TFT_CS                10
#define TFT_RST               8  // define reset pin, or set to -1 and connect to Arduino RESET pin
#define TFT_DC                9  // define data/command pin

// PINOS BOTÕES
#define BOTAOSELECT_PIN       2  //botão para selecionar
#define BOTAOCONFIRM_PIN      3 //botão para confirmar

// DEFINIÇÕES DE ALINHAMENTO
#define INICIO_LINHA_MENU     10
#define TAMANHO_LINHA_MENU    38

// CORES
#define COR_TEXTO_BASE        0xFFE0
#define COR_FUNDO             0x00
#define COR_CENARIO           0x5acb 

// MAXIMO RANKING E MAXIMO NOME DO JOGADOR
#define MAXIMO_RANKING        3
#define MAXIMO_NOME_JOGADOR   4

// TEMPO PARA MILLIS()
#define DELAY20           20
#define DELAY15           15
#define DELAYATIRAR       200
#define DELAYINIMIGO      50

//MÁXIMO TIROS
#define MAXIMO_TIROS       3

// CORAÇÃO
#define GLYPH_HEART        0

// INIMIGOS
#define MAXIMO_INIMIGOS    4

void imprimirMenu();
void ImprimirLogo();
void ocultarSeletorAnterior();
void mostrarSeletorMenu();
void atualizarSeletorMenu();
void selecionarOpcaoMenu();
void lerJogador();
void iniciarJogo();
void imprimirRanking();
void ordenarRanking();
void creditos();
void sair();
void voltarMenu();
void moverNave(int8_t x);
void renderizarNave();
void apagarNave();
void renderizarEstrela();
void apagarEstrela();
void descerInimigo(uint8_t y);
void moverEstrela(uint8_t x);                   // usar função random()? escolher um número dentro do previsto, por ser usado para fazer o inimigo nascer
void renderizarInimigo(int ID);
void apagarInimigo(int ID);
void atirar();
void moverTiro(uint8_t y);
void renderizarTiro();
void apagarTiro(uint8_t tiro);
void apagarTiroEstrela(int tiro);
void scoreDestruirInimigo();
void perderVida();
void atualizarPlacar();
void atualizarJogo(int botaoEsquerdo, int botaoDireito);
void Spawn();
void SpawnEstrela();
void atirarEstrela();
void limparTela();
void formatarTextoBase(uint8_t tamanho);
void imprimirTexto(uint8_t x, uint8_t y, const __FlashStringHelper* texto);
void renderizarTiroEstrela();
void moverTiroEstrela(uint8_t y);
void cenario();
void renderizarSimbolo(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t Size);

const unsigned char nave[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00,
  0x00, 0x0d, 0x80, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x07, 0x00, 0x00,
  0x00, 0x0f, 0x80, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x1f, 0xc0, 0x00, 0x00, 0x3f, 0xe0, 0x00,
  0x00, 0x7f, 0xf0, 0x00, 0x00, 0x7f, 0xf0, 0x00, 0x00, 0xff, 0xf8, 0x00, 0x01, 0xff, 0xfc, 0x00,
  0x03, 0xdf, 0xde, 0x00, 0x03, 0x9f, 0xce, 0x00, 0x00, 0x38, 0xe0, 0x00, 0x00, 0x50, 0x50, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};

const unsigned char inimigo[] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x0e, 0xe0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x0e, 0xe0, 0x00,
  0x00, 0x06, 0xc0, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0xf3, 0x9f, 0x00,
  0x00, 0xfb, 0xbe, 0x00, 0x00, 0x7b, 0xbc, 0x00, 0x00, 0x33, 0x98, 0x00, 0x00, 0x07, 0xc0, 0x00,
  0x00, 0x13, 0x90, 0x00, 0x00, 0x0b, 0xa0, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x63, 0x8c, 0x00,
  0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x02, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00,
  0x00, 0x01, 0x00, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x03, 0x80, 0x00, 0x00, 0x01, 0x00, 0x00,
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};



// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
uint8_t direcao = 0;
uint8_t menuSelecionado = 1;
uint8_t menuAnterior = 0;
uint8_t telaAtual = 0;
uint8_t posicaoNave = 100;
uint8_t posicaoInimigo = 0;
uint8_t incrementaJogador = 0;
uint8_t alterarPosicaoJogador = 0;
unsigned long tempoNave = 0;
unsigned long tempoInimigo = 0;
unsigned long tempoSpawn = 0;
unsigned long tempoScore = 0;
int vida = 3;
unsigned long score = 0;
unsigned long scoreBOSS = 0;
unsigned long tempoAtirar = 0;
unsigned long tempoAtirarEstrela = 0;
typedef struct {
  uint8_t posicaoInimigoY;
  uint8_t posicaoInimigoX;
  uint32_t tempoInimigo;
  uint8_t Hp;
  boolean Inimigo = false;
} Inimigos;
Inimigos spawn[MAXIMO_INIMIGOS];
typedef struct {
  uint8_t posicaoEstrelaY = 0;
  uint8_t posicaoEstrelaX = 0;
  unsigned long tempoEstrela = 0;
  uint8_t Hp = 10;
  boolean Estrela = false;
} EstrelaConfig;
EstrelaConfig BOSS;

typedef struct {
  uint8_t posicaoTiroY;
  uint8_t posicaoTiroX;
  unsigned long tempoTiro = 0;
  boolean ativo = false;
} tiro;

tiro tiros[MAXIMO_TIROS];
tiro tirosEstrela[MAXIMO_TIROS];



int pontos[MAXIMO_RANKING] = {0, 0, 0};
char jogador[MAXIMO_RANKING][MAXIMO_NOME_JOGADOR] = {"", "", ""};
char gameOverJogador[MAXIMO_NOME_JOGADOR];



void setup(void) {
  Serial.begin(9600);

  // if the display has CS pin try with SPI_MODE0
  tft.init(240, 240, SPI_MODE2);    // Init ST7789 display 240x240 pixel
  tft.setRotation(2);

  ImprimirLogo();
  delay(300);

  pinMode(BOTAOSELECT_PIN, INPUT);
  pinMode(BOTAOCONFIRM_PIN, INPUT);

  ordenarRanking();
  imprimirMenu();
}

void ImprimirLogo() {
  formatarTextoBase(4);
  imprimirTexto(40, 40, F("Attack"));
  imprimirTexto(40, 80, F("on"));
  imprimirTexto(40, 120, F("Death"));
  imprimirTexto(40, 160, F("Star"));
}

void formatarTextoBase(uint8_t tamanho)
{
  tft.setTextSize(tamanho);
  tft.setTextColor(COR_TEXTO_BASE);
}

void imprimirTexto(uint8_t x, uint8_t y, const __FlashStringHelper* texto)
{
  tft.setCursor(x, y);
  tft.print(texto);
}

void imprimirMenu() {
  limparTela();
  formatarTextoBase(4);

  imprimirTexto(36, INICIO_LINHA_MENU + TAMANHO_LINHA_MENU, F("Jogar"));
  imprimirTexto(36, INICIO_LINHA_MENU + (2 * TAMANHO_LINHA_MENU), F("Ranking"));
  imprimirTexto(36, INICIO_LINHA_MENU + (3 * TAMANHO_LINHA_MENU), F("Creditos"));
  imprimirTexto(36, INICIO_LINHA_MENU + (4 * TAMANHO_LINHA_MENU), F("Opcoes"));

  mostrarSeletorMenu();
}

void ocultarSeletorAnterior()
{
  tft.setTextColor(COR_FUNDO);
  imprimirTexto(10, INICIO_LINHA_MENU + (menuAnterior * TAMANHO_LINHA_MENU), F("\t"));
}

void mostrarSeletorMenu()
{
  tft.setTextColor(COR_TEXTO_BASE);
  imprimirTexto(10, INICIO_LINHA_MENU + (menuSelecionado * TAMANHO_LINHA_MENU), F("\t"));
  delay(20);
  ocultarSeletorAnterior();
}

void atualizarSeletorMenu()
{
  menuAnterior = menuSelecionado;
  if (menuSelecionado < 4) {
    menuSelecionado++;
  } else {
    menuSelecionado = 1;
  }
  mostrarSeletorMenu();
}

void selecionarOpcaoMenu()
{
  switch (menuSelecionado) {
    case 1: iniciarJogo();
      break;
    case 2: imprimirRanking();
      break;
    case 3: creditos();
      break;
  }
  telaAtual = menuSelecionado;
}

void iniciarJogo()
{
  scoreBOSS = 0;
  for(int i = 0; i < MAXIMO_TIROS; i++){
    tiros[i].ativo = false;
    tirosEstrela[i].ativo = false;
  }
  for(int i = 0; i < MAXIMO_INIMIGOS; i++){
    spawn[i].Inimigo = false;
  }
  BOSS.Estrela = false; 
  spawn[0].Inimigo = true;
  limparTela();
  renderizarNave();
  vida = 3;
  score = 0;
  strcpy(gameOverJogador, "   ");
  incrementaJogador = 0;
  alterarPosicaoJogador = 0;
  atualizarPlacar();
  renderizarSimbolo(80, 225, GLYPH_HEART, ST77XX_RED ,0,1);
}

void creditos()
{
  limparTela();
  formatarTextoBase(3);

  imprimirTexto(36, INICIO_LINHA_MENU + TAMANHO_LINHA_MENU, F("Creditos"));

  imprimirTexto(36, INICIO_LINHA_MENU + (2 * TAMANHO_LINHA_MENU), F("Gabriel"));
  imprimirTexto(36, INICIO_LINHA_MENU + (3 * TAMANHO_LINHA_MENU), F("Helcio"));
  imprimirTexto(36, INICIO_LINHA_MENU + (4 * TAMANHO_LINHA_MENU), F("Jean"));
  imprimirTexto(36, INICIO_LINHA_MENU + (5 * TAMANHO_LINHA_MENU), F("Kellwyn"));
  imprimirTexto(36, INICIO_LINHA_MENU + (6 * TAMANHO_LINHA_MENU), F("Rafael"));
}

void limparTela()
{
  tft.fillScreen(COR_FUNDO);
}

void rankingGameOver() {
  if (vida == 0) {
    tft.fillRect(43, 115, 125, 35, COR_FUNDO);
    tft.setCursor(43, 115);
    formatarTextoBase(3);
    atualizarNomeJogador();
    tft.print(gameOverJogador);
  }
}

void atualizarNomeJogador()
{
  gameOverJogador[alterarPosicaoJogador] = 'A' + incrementaJogador;
  if (gameOverJogador[alterarPosicaoJogador] <= 'Z') {
    incrementaJogador++;
    delay(200);
  } else {
    incrementaJogador = 0;
  }
}

void imprimirRanking()
{
  limparTela();
  formatarTextoBase(3);
  imprimirTexto(36, INICIO_LINHA_MENU + TAMANHO_LINHA_MENU, F("Ranking"));

  for (int i = 0; i < 3; i++) {
    tft.setCursor(36, INICIO_LINHA_MENU + ((i + 2) * TAMANHO_LINHA_MENU));
    tft.print(jogador[i]);
    tft.print (": ");
    tft.print(pontos[i]);
  }
}

void moverNave(int8_t x)
{
  if (millis() > tempoNave + DELAY20) {
    apagarNave();
    if (posicaoNave + x >= 0) {
      posicaoNave += x;
    }
    if (x > 0 && posicaoNave > 215) {
      posicaoNave = 215;
    }
    else if (x < 0 && posicaoNave < 0) {
      posicaoNave = 0;
    }
    renderizarNave();
    tempoNave = millis();
  }
}

void renderizarNave()
{
  tft.drawBitmap(posicaoNave, 200, nave, 32, 32, ST77XX_WHITE);   // x = 100, y = 200 , nave array onde ta o bitmap, width , height, color
}

void apagarNave()
{
  tft.drawBitmap(posicaoNave, 200, nave, 32, 32, COR_FUNDO);
}

void renderizarEstrela()
{
  tft.fillCircle(BOSS.posicaoEstrelaX, BOSS.posicaoEstrelaY, 16 , ST77XX_WHITE);
}
void apagarEstrela()
{
  tft.fillCircle(BOSS.posicaoEstrelaX, BOSS.posicaoEstrelaY, 16, COR_FUNDO);
}

void descerInimigo(uint8_t y)
{ for (int i = 0; i < 4; i++) {
    if (spawn[i].Inimigo == true) {
      if (millis() > spawn[i].tempoInimigo + DELAYINIMIGO) {
        apagarInimigo(i);
        spawn[i].posicaoInimigoY += y;
        if (spawn[i].posicaoInimigoY > 240) {
          spawn[i].posicaoInimigoY = 0;             // arrumar
          spawn[i].posicaoInimigoX = random(20, 220);
          perderVida();
        }
        renderizarInimigo(i);
        spawn[i].tempoInimigo = millis();
      }
    }
  }
}

void SpawnEstrela()
{ if (BOSS.Estrela == false) {
    BOSS.posicaoEstrelaY = 16;
    BOSS.posicaoEstrelaX = 120;
    BOSS.Hp = 10;
    BOSS.Estrela = true;
  }
}
void moverEstrela()
{
  if (BOSS.Estrela == true) {
    if (millis() > BOSS.tempoEstrela + DELAYINIMIGO) {
      BOSS.tempoEstrela = millis();
      apagarEstrela();

      if ( direcao == 0) {
        BOSS.posicaoEstrelaX -= 3;
      }
      if (BOSS.posicaoEstrelaX <= 40) {
        direcao = 1;

      }
      if (direcao == 1) {
        BOSS.posicaoEstrelaX += 3;
      }
      if (BOSS.posicaoEstrelaX >= 200) {
        direcao = 0;
      }
      renderizarEstrela();
    }
  }

}
void Spawn()
{ for (int i = 0; i < 4; i++) {
    if (spawn[i].Inimigo == false) {
      if (millis() > tempoSpawn + 3000) {
        spawn[i].posicaoInimigoX = random(0, 240);
        spawn[i].posicaoInimigoY = 0;
        spawn[i].Inimigo = true;
        tempoSpawn = millis();
      }
    }
  }
}
void renderizarInimigo(int ID)
{

  tft.drawBitmap(spawn[ID].posicaoInimigoX, spawn[ID].posicaoInimigoY, inimigo, 32, 32, ST77XX_WHITE); //tft.fillCircle(120, posicaoInimigo, 25, ST77XX_WHITE);
}

void apagarInimigo(int ID)
{
  tft.drawCircle(spawn[ID].posicaoInimigoX + 15, spawn[ID].posicaoInimigoY + 15, 16, COR_FUNDO);
  tft.drawBitmap(spawn[ID].posicaoInimigoX, spawn[ID].posicaoInimigoY, inimigo, 32, 32, COR_FUNDO); //tft.fillCircle(120, posicaoInimigo, 25, COR_FUNDO);
}
void atirarEstrela()
{
  if (millis() > tempoAtirarEstrela + DELAYATIRAR) {
    if(BOSS.Estrela == true){
      for(int i = 0; i < MAXIMO_TIROS; i++){
        if(tirosEstrela[i].ativo == false){
          tirosEstrela[i].posicaoTiroY = 40;
          tirosEstrela[i].posicaoTiroX = BOSS.posicaoEstrelaX + 16;
          tirosEstrela[i].ativo = true;
          tempoAtirarEstrela = millis();
          break;
        }
      }
    }
  }
}
void atirar()
{
  if (millis() > tempoAtirar + DELAYATIRAR) {
    for (int i = 0; i < MAXIMO_TIROS; i++) {
      if (tiros[i].ativo == false) {
        tiros[i].posicaoTiroY = 200;
        tiros[i].posicaoTiroX = posicaoNave + 16;
        tiros[i].ativo = true;
        tempoAtirar = millis();
        break;
      }
    }
  }
}

void moverTiroEstrela(uint8_t y)
{
  if (BOSS.Estrela == true) {
    for (int i = 0; i < MAXIMO_TIROS; i++) {
      if (tirosEstrela[i].ativo == true) {
        if (millis() > tirosEstrela[i].tempoTiro + DELAY20) {
          apagarTiroEstrela(i);
          if (tirosEstrela[i].posicaoTiroY >= 225) {
            tirosEstrela[i].ativo = false;
          }
          else {
            tirosEstrela[i].posicaoTiroY += y;
            renderizarTiroEstrela(i);
            if ((tirosEstrela[i].posicaoTiroY >= 200 && tirosEstrela[i].posicaoTiroY <= 220) &&
                tirosEstrela[i].posicaoTiroX >= posicaoNave && tirosEstrela[i].posicaoTiroX <= posicaoNave + 32) {
              Serial.print("teste");
              perderVida();
              break;
            }
          }
          tirosEstrela[i].tempoTiro = millis();
  
        }
      }
    }
  }
}

void moverTiro(uint8_t y) // recebido do mover tiro, 5;
{
  bool resultado;
  for (int i = 0; i < MAXIMO_TIROS; i++) {
    if (tiros[i].ativo == true) {
      if (millis() > tiros[i].tempoTiro + DELAY20) {
        apagarTiro(i);
        if (tiros[i].posicaoTiroY - y < 0) {
          tiros[i].ativo = false;
        }
        else {
          tiros[i].posicaoTiroY -= y;
        }
        renderizarTiro(i);
        tiros[i].tempoTiro = millis();
      }
    }
    if (tiros[i].posicaoTiroY <= 0) {
      apagarTiro(i);
    }
    for (int j = 0; j < 4; j++) {
      bool resultado = tiroColideInimigo(i, j);
      if (spawn[j].Inimigo == true) {
        if (resultado == true) {
          spawn[j].Inimigo = false;
          tiros[i].ativo = false;
          apagarTiro(i);
          apagarInimigo(j);
          scoreDestruirInimigo();
        }
      }
    }
    if (BOSS.Estrela == true) {
      resultado = tiroColideEstrela(i);
      if (resultado == true) {
        tiros[i].ativo = false;
        apagarTiro(i);
        BOSS.Hp -= 1;
        if ( BOSS.Hp == 0) {
          BOSS.Estrela = false;
          apagarEstrela();
        }
          

      }
    }
  }

}

void renderizarTiroEstrela(int Tiro)
{
  tft.drawRect(tirosEstrela[Tiro].posicaoTiroX, tirosEstrela[Tiro].posicaoTiroY, 2, 5, ST77XX_WHITE);
}
void renderizarTiro(int Tiro)
{
  tft.drawRect(tiros[Tiro].posicaoTiroX, tiros[Tiro].posicaoTiroY, 2, 5, ST77XX_WHITE);
}

void apagarTiroEstrela(int Tiro)
{
  tft.drawRect(tirosEstrela[Tiro].posicaoTiroX, tirosEstrela[Tiro].posicaoTiroY, 2, 5, COR_FUNDO);
}

void apagarTiro(int Tiro)
{
  tft.drawRect(tiros[Tiro].posicaoTiroX, tiros[Tiro].posicaoTiroY, 2, 5, COR_FUNDO);
}

bool tiroColideEstrela(int Tiro)
{ if ((pow(tiros[Tiro].posicaoTiroX - BOSS.posicaoEstrelaX - 15, 2) + pow(tiros[Tiro].posicaoTiroY  - BOSS.posicaoEstrelaY , 2)) <= pow(16, 2)) {
    return true;
  }
  else {
    return false;
  }
}

bool tiroColideInimigo(int Tiro, int Inimigo)
{
  if ((pow(tiros[Tiro].posicaoTiroX - spawn[Inimigo].posicaoInimigoX - 15, 2) + pow(tiros[Tiro].posicaoTiroY  - spawn[Inimigo].posicaoInimigoY , 2)) <= pow(16, 2)) {
    return true;
  } else {
    return false;
  }
}

bool tiroColideNave(int Tiro)
{ 
  if ((pow(tirosEstrela[0].posicaoTiroX - posicaoNave - 15, 2) + pow(tirosEstrela[0].posicaoTiroY, 2)) <= pow(16, 2)) {
    return true;
  }
  else {
    return false;
  }
  return true;
}

void scoreDestruirInimigo()
{
  scoreBOSS++;
  score++;
  atualizarPlacar();

  if (scoreBOSS == 3) { //  mudar para 10
    SpawnEstrela();
    scoreBOSS = 0;
  }
}

void perderVida()
{
  if (vida - 1 > 0) {
    vida--;
  } else {
    formatarTextoBase(3);
    tft.setCursor(43, 80);
    tft.print("GAME OVER");
    vida = 0;
    rankingGameOver();
  }
  atualizarPlacar();
}

void atualizarPlacar()
{
  tft.fillRect(10, 225, 250, 30, COR_FUNDO);
  formatarTextoBase(2);
  tft.setCursor(10, 225);
  tft.print(score);
  tft.setCursor(100, 225);
  tft.setTextColor(ST77XX_RED);
  tft.print(vida);

  tempoScore = millis();

}



void ordenarRanking()
{
  int i, j, troca;
  char troca2[MAXIMO_RANKING];
  for (i = 0; i < MAXIMO_RANKING - 1; i++) {
    for (j = i + 1; j < MAXIMO_RANKING; j++) {
      if (pontos[j] > pontos[i]) {
        troca = pontos[j];
        pontos[j] = pontos[i];
        pontos[i] = troca;
        strcpy(troca2, jogador[j]);
        strcpy(jogador[j], jogador[i]);
        strcpy(jogador[i], troca2);
      }
    }
  }
}

void atualizarJogo(int botaoEsquerdo, int botaoDireito)
{
  Spawn();
  cenario();
  atirarEstrela();
  moverEstrela();
  moverTiroEstrela(5);
  descerInimigo(3);
  if (botaoEsquerdo == HIGH) {
    moverNave(5);
  }
  if (botaoDireito == HIGH) {
    moverNave(-5);
  }
  atirar();
  moverTiro(5);
}

void voltarMenu()
{
  telaAtual = 0;
  imprimirMenu();
}

void cenario()
{
  tft.fillRect(120,120,6,2, COR_CENARIO);
  tft.fillRect(122,118,2,6, COR_CENARIO);  
  tft.fillRect(77,141,6,2, COR_CENARIO);
  tft.fillRect(79,139,2,6, COR_CENARIO);
  tft.fillRect(12,77,6,2, COR_CENARIO);
  tft.fillRect(14,75,2,6, COR_CENARIO);
  tft.fillRect(200,30,6,2, COR_CENARIO);
  tft.fillRect(202,28,2,6, COR_CENARIO);
}

void renderizarSimbolo(uint8_t x, uint8_t y, uint8_t c, uint16_t color, uint16_t bg, uint8_t Size){
    tft.setFont(&SymbolMono18pt7b);
    tft.drawChar(x,y,c,color,bg,Size);
    tft.setFont();
}

void loop() {
  int selecionaEstado = digitalRead(BOTAOSELECT_PIN);
  int confirmaEstado = digitalRead(BOTAOCONFIRM_PIN);

  if (telaAtual == 1) {
    if (vida > 0) {
      atualizarJogo(confirmaEstado, selecionaEstado);
    } else if (vida == 0) {
      if (score > pontos[MAXIMO_RANKING - 1]) {
        if (selecionaEstado == HIGH) {
          rankingGameOver();              //voltarMenu(); confirmar para voltar o menu depois que ranking funcionar
        } else if (confirmaEstado == HIGH) {
          if (alterarPosicaoJogador < 2) {
            incrementaJogador = 0;
            alterarPosicaoJogador++;
            rankingGameOver();
            delay(400);
          } else {
            pontos[MAXIMO_RANKING - 1] = score;
            strcpy(jogador[MAXIMO_RANKING-1], gameOverJogador);
            ordenarRanking();
            voltarMenu();
          }
        }
      } else {
        if (confirmaEstado == HIGH || selecionaEstado == HIGH) {
          voltarMenu();
        }
      }
    }
  } else {
    if (selecionaEstado == HIGH) {
      if (telaAtual == 0) {
        atualizarSeletorMenu();
        delay(500);
      } else {
        voltarMenu();
      }
    }
    if (confirmaEstado == HIGH) {
      if (telaAtual == 0) {
        selecionarOpcaoMenu();
        delay(500);
      } else {
        voltarMenu();
      }
    }
  }
}
