import controlP5.*;
import processing.serial.*;

Serial myPort;

ControlP5 cp5;
int col = color(255);
int N = 8;

int ind=0;

int[] bit = new int[10];
Toggle[] basis = new Toggle[10];
int[] basisAlice = new int[10];
Button send, reset;

void setup() {
  int i;

  size(1200, 900);

  String portName = Serial.list()[1];
  myPort = new Serial(this, portName, 9600);

  smooth();
  cp5 = new ControlP5(this);

  for (i=0; i<N; i++) {
    basis[i] = cp5.addToggle("basis"+str(i))
      .setCaptionLabel("")
      .setPosition((i+2)*width/12., 200)
      .setSize(70, 35)
      .setMode(ControlP5.SWITCH)
      .setColorBackground(color(50))
      .setColorActive(color(200));
  }

  send = cp5.addButton("receive")
    .setCaptionLabel("Medir")
    .setFont(createFont("SansSerif", 25))
    .setPosition((width-300)/2.+100, 780)
    .setSize(200, 80)
    .updateSize()
    .setColorBackground(color(50))
    .setColorForeground(color(100))
    .setColorLabel(color(200))
    .setColorActive(color(120));

  reset = cp5.addButton("reset")
    .setCaptionLabel("Limpar")
    .setFont(createFont("SansSerif", 25))
    .setPosition((width-300)/2.-100, 780)
    .setSize(150, 80)
    .updateSize()
    .setColorBackground(color(50))
    .setColorForeground(color(100))
    .setColorLabel(color(200))
    .setColorActive(color(120));
}

void reset() {
  int i;
  for (i=0; i<N; i++) {
    basis[i].setValue(false);
    bit[i] = 0;
    basisAlice[i] = 0;
  }
  myPort.write('R');
}

void receive() {
  int i;
  myPort.write('S');
  for (i=0; i<N; i++) {
    myPort.write(str(int(basis[i].getValue())));
  }
}

void draw() {
  int i, read;

  read=myPort.read();
  if (read=='S') {
    ind=0;
    while (ind<8) {
      read=myPort.read();
      if (read!=-1) {
        bit[ind]=read;
        ind++;
      }
    }
  } else if (read=='B') {
    ind=0;
    while (ind<8) {
      read=myPort.read();
      if (read!=-1) {
        basisAlice[ind]=read;
        ind++;
      }
    }
  }

  background(70);

  fill(200);
  textSize(60);
  textAlign(CENTER);
  text("Bob", 600, 80); 

  textSize(35);
  textAlign(LEFT, TOP);
  text("Base:", 40, 200);

  text("Base Alice:", 40, 400); 

  textSize(35);
  textAlign(LEFT, TOP);
  text("Valor:", 40, 600);

  textSize(30);
  textAlign(LEFT, TOP);
  for (i=0; i<N; i++) {
    fill(200);
    if (basis[i].getValue() == 0) {
      text("+", (i+2.3)*width/12., 250);
    } else {
      text("x", (i+2.3)*width/12., 250);
    }
    
    if (basisAlice[i] == 0) {
      text("+", (i+2.3)*width/12., 450);
    } else {
      text("x", (i+2.3)*width/12., 450);
    }

    text(bit[i], (i+2.3)*width/12., 650);

    pushMatrix();
    fill(150*basisAlice[i]+50);
    noStroke();
    ellipse((i+2.38)*width/12., 410, 40, 40);
    popMatrix();
    
    pushMatrix();
    fill(150*bit[i]+50);
    noStroke();
    ellipse((i+2.38)*width/12., 610, 40, 40);
    popMatrix();
  }
}