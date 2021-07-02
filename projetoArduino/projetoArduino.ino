#include <Adafruit_GFX.h>
#include <Adafruit_ST7789.h>
#include <SPI.h>
#include <string.h>

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

// MAXIMO RANKING E MAXIMO NOME DO JOGADOR
#define MAXIMO_RANKING        3
#define MAXIMO_NOME_JOGADOR   31

// TEMPO PARA MILLIS()
#define DELAY20            20
#define DELAY15            15
#define DELAYATIRAR        200

//MÁXIMO TIROS
#define MAXIMO_TIROS       3
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
boolean tiroColideInimigo();
void scoreDestruirInimigo();
void perderVida();
void atualizarPlacar();
void atualizarJogo();
void Spawn();
void SpawnEstrela();

void limparTela();
void formatarTextoBase(uint8_t tamanho);
void imprimirTexto(uint8_t x, uint8_t y, const __FlashStringHelper* texto);


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
unsigned long tempoNave = 0;
unsigned long tempoInimigo = 0;
unsigned long tempoSpawn = 0;
unsigned long tempoScore = 0;
int vida = 3;
unsigned long score = 0;
unsigned long tempoAtirar = 0;
typedef struct {
  uint8_t posicaoInimigoY;
  uint8_t posicaoInimigoX;
  uint32_t tempoInimigo;
  uint8_t Hp;
  boolean Inimigo = false;
} Inimigos;
Inimigos spawn[4];
typedef struct {
  uint8_t posicaoEstrelaY = 0;
  uint8_t posicaoEstrelaX = 0;
  unsigned long tempoEstrela = 0;
  uint8_t Hp;
  boolean Estrela = false;
} EstrelaConfig;
EstrelaConfig BOSS;

typedef struct {
  uint8_t posicaoTiroY = 200;
  uint8_t posicaoTiroX = posicaoNave + 16;
  unsigned long tempoTiro = 0;
  boolean ativo = false;
} tiro;

tiro tiros[MAXIMO_TIROS];



int pontos[MAXIMO_RANKING] = {5, 6, 7};
char jogador[MAXIMO_RANKING][MAXIMO_NOME_JOGADOR] = {"HLJ", "RAO", "JGM"};


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
  spawn[0].Inimigo = true;
  limparTela();
  renderizarNave();
  vida = 3;
  //renderizarEstrela();
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
   if(posicaoNave + x >= 0){ posicaoNave += x;}
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
      if (millis() > spawn[i].tempoInimigo + DELAY20) {
        apagarInimigo(i);
        spawn[i].posicaoInimigoY += y;
        if (spawn[i].posicaoInimigoY > 240) {
          spawn[i].posicaoInimigoY = 0;             // arrumar
          spawn[i].posicaoInimigoX = random(0, 240);
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
    BOSS.Estrela = true;
  }
  renderizarEstrela(); // teste(apagar)
}
void moverEstrela()
{
  if (BOSS.Estrela == true) {
    if (millis() > BOSS.tempoEstrela + DELAY20) {
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
    }
  }
  renderizarEstrela();
}
void Spawn()
{ for (int i = 0; i < 4; i++) {
    if (spawn[i].Inimigo == false) {
      if (millis() > tempoSpawn + 5000) {
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
  tft.drawBitmap(spawn[ID].posicaoInimigoX, spawn[ID].posicaoInimigoY, inimigo, 32, 32, COR_FUNDO); //tft.fillCircle(120, posicaoInimigo, 25, COR_FUNDO);
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

void moverTiro(uint8_t y) // recebido do mover tiro, 5;
{
  bool a;
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
      bool a = tiroColideInimigo(i, j);
      if (spawn[j].Inimigo == true) {
        if (a == true) {
          spawn[j].Inimigo = false;
          apagarTiro(i);
          apagarInimigo(j);
          scoreDestruirInimigo();
        }
      }
    }
  }
}

void renderizarTiro(int Tiro)
{
  tft.drawRect(tiros[Tiro].posicaoTiroX, tiros[Tiro].posicaoTiroY, 2, 5, ST77XX_WHITE);
}

void apagarTiro(int Tiro)
{
  tft.drawRect(tiros[Tiro].posicaoTiroX, tiros[Tiro].posicaoTiroY, 2, 5, COR_FUNDO);
}

boolean tiroColideInimigo(int Tiro, int Inimigo)
{
  if ((pow(tiros[Tiro].posicaoTiroX - spawn[Inimigo].posicaoInimigoX , 2) + pow(tiros[Tiro].posicaoTiroY - spawn[Inimigo].posicaoInimigoY , 2)) <= pow(16, 2)) {
    return true;
  } else {
    return false;
  }
}

void scoreDestruirInimigo()
{
  score++;
  if (score >= 10) {
    renderizarEstrela();
  }
}

void perderVida()
{
  if(vida - 1> 0){
    vida --;
  } else {
    formatarTextoBase(3);
    tft.setCursor(41,120);
    tft.print("GAME OVER");
    vida = 0;
  } 
}

void atualizarPlacar()
{
  if (millis() > tempoScore + DELAY20) {
    tft.fillRect(10, 225, 250, 30, COR_FUNDO);
    formatarTextoBase(2);
    tft.setCursor(10, 225);
    tft.print(score);
    tft.setCursor(100, 225);
    tft.setTextColor(ST77XX_RED);
    tft.print(vida);
    tempoScore = millis();
  }
}



void ordenarRanking()
{
  int i, j, troca;
  char troca2[MAXIMO_RANKING];
  for (i = 0; i < 2; i++) {
    for (j = i + 1; j < 3; j++) {
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

void atualizarJogo()
{
  int selecionaEstado = digitalRead(BOTAOSELECT_PIN);
  int confirmaEstado = digitalRead(BOTAOCONFIRM_PIN);
  Spawn();
  SpawnEstrela();
  moverEstrela();
  descerInimigo(3);
  //atualizarPlacar();
  if (confirmaEstado == HIGH) {
    moverNave(5);
  }
  if (selecionaEstado == HIGH) {
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

void loop() {
  int selecionaEstado = digitalRead(BOTAOSELECT_PIN);
  int confirmaEstado = digitalRead(BOTAOCONFIRM_PIN);

  if (telaAtual == 1) {
    if(vida > 0){
      atualizarJogo();
     } else if (vida == 0){
        if(confirmaEstado == HIGH){
          voltarMenu();
        }
     }
  }else {
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
