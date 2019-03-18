import controlP5.*;
import processing.serial.*;

Serial myPort;

ControlP5 cp5;
int col = color(255);
int N = 8;

Toggle[] basis = new Toggle[10];
Toggle[] bit = new Toggle[10];
Button send, reset;

void setup() {
  int i;

  size(1200, 700);
  
  String portName = Serial.list()[0];
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

    bit[i] = cp5.addToggle("bit"+str(i))
      .setCaptionLabel("")
      .setFont(createFont("SansSerif", 25))
      .setPosition((i+2)*width/12., 400)
      .setSize(70, 35)
      .setMode(ControlP5.SWITCH)
      .setColorBackground(color(50))
      .setColorActive(color(200));
  }

  send = cp5.addButton("send")
    .setCaptionLabel("Enviar chave")
    .setFont(createFont("SansSerif", 25))
    .setPosition((width-300)/2.+100, 580)
    .setSize(300, 80)
    .updateSize()
    .setColorBackground(color(50))
    .setColorForeground(color(100))
    .setColorLabel(color(200))
    .setColorActive(color(120));
    
  reset = cp5.addButton("reset")
    .setCaptionLabel("Limpar")
    .setFont(createFont("SansSerif", 25))
    .setPosition((width-300)/2.-100, 580)
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
    bit[i].setValue(false);
  }
  myPort.write('R');
}

void send() {
  int i;
  for (i=0; i<N; i++) {
    myPort.write(str(int(basis[i].getValue())));
  }
  for (i=0; i<N; i++) {
    myPort.write(str(int(bit[i].getValue())));
  }
}

void draw() {
  int i;
  background(70);

  fill(200);
  textSize(60);
  textAlign(CENTER);
  text("Alice", 600, 80); 

  textSize(35);
  textAlign(LEFT, TOP);
  text("Base:", 40, 200);

  textSize(35);
  textAlign(LEFT, TOP);
  text("Valor:", 40, 400);

  textSize(30);
  textAlign(LEFT, TOP);
  for (i=0; i<N; i++) {
    if(basis[i].getValue() == 0){
      text("+", (i+2.3)*width/12., 250);
    } else{
      text("x", (i+2.3)*width/12., 250);
    }
    
    text(int(bit[i].getValue()), (i+2.3)*width/12., 450);
  }
}
