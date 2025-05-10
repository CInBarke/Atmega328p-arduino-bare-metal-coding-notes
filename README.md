# Atmega328p-arduino-bare-metal-coding-notes
anotando diversas coisas interessantes sobre "bare metal" coding no arduino uno / atmega328p, e sobre bare metal coding em geral, logo que muita coisa vista aqui será vista novamente depois. PS. isso vai ser literalmente só uma readme file com vários códigos que eu vou enviar ao longo do tempo
ps. eu literalmente leio sobre algo e coloco aqui, então isso é muito mais próximo de anotações sobre um determinado conteúdo doque uma "aula", então pfv, se eu estiver errado em alguma parte desse documento, me mande mensagem com a correção, que eu ficarei bem grato!

## 1. BLINKY:
o blinky é muito bom de falar logo, logo que ele basicamente resolve todos nossos problemas no que concerne escrever valores para os pinos digitais. Enfim, esse blinky não vai ser com o led_builtin do arduino, mesmo sendo bastante fácil de fazê-lo nele. Segue uma imagem tirada direto do datasheet do arduino uno que irá nos ajudar na nossa tarefa de fazer um blinky.

![image](https://github.com/user-attachments/assets/d75f9c8d-6d71-4d4b-a50f-8b78269eeef9)

na imagem ao lado, temos descritos os PORTS. Cada port tem 8 pinos associados a ele, e dá para ver qual port determinado pino pertence vendo oque tem na "bolinha" laranja claro. Por exemplo, o pino digital 3 é agora o PD3, ou seja, podemos claramente ver que ele pertence ao PORTD, enquanto, por exemplo, os pinos analógicos pertencem ao PORTC, e o led_builtin pertence ao PORTB. fácil né? Agora vamos ver como podemos alterar os valores dos pinos pertencentes a esses ports.

### ANÁLISE DE CÓDIGO:

#### OPERAÇÕES BITWISE
primeiramente, creio que seria útil falar sobre algumas operações bitwise que ocorrem aqui dentro do código, e que vão voltar a aparecer com determinada frequência.
X = X | (1 << n)
na linha acima, estou pegando o registrador X, e setando seu n'esimo bit como 1. vamos ver como isso acontece.
primeiramente, o shiftleft acima, para o exemplo de n = 2, fará o seguinte: 00001 -> 00100. Como é possível ver, o bit 1 foi para a segunda casa. (considerando indexação iniciada em 0). Agora, aplicando a operação mostrada para X = 10011, por exemplo, teremos:
10011 | 00100 = 10111. Como é possível ver, o único bit alterado foi o que se desejava alterar, e ele agora foi setado como '1'.
Agora, devemos transformar o bit em questão de volta a 0. Mas como? veremos a seguir:
X = X & ~(1 << n)
a operação acima é a que faz isso, agora vamos explicar sua lógica.
primeiramente, vou pular a etapa do shift left, logo que ela foi descrita acima. Vou considerar o mesmo n = 2. De modo que a expressão vira:
X = X & ~(00100)
aplicando ~(00100) = 11011.
depois disso, o and segue agora a mesma lógica do 'or' anterior. Onde os "dont cares" serem 1 indicam que o valor inicial desses bits em X será preservado, logo que, das propriedades da porta AND, tiramos que X & 1 = X.
agora, sendo X = 10111, 10111 & 11011 = 10011, assim, conseguimos tirar o 1 que haviamos colocado antes.

#### 1. DDRD = DDRD | (1 << 3);
  A linha acima atua mudando o valor contido no "data direction register) para o PORTD (DDRD significa "data direction register D"). Oque esses data direction registers fazem é indicar se um pino é de output ou de input. Aqui, estamos setando o 3 bit do DDRD como '1', ou seja, indicando que o pino 3  do PORTD é um output. Caso quiséssemos fazê-lo um pino de input, fariamos: DDRD = DDRD &~ (1 << 3); ou seja, setamos o 3 bit como 0.

#### 2. O resto:
o while(1) está sendo usado de modo a criar um loop infinito. Depois, oque fazemos é ligar e desligar o pino 3 do PORTD, isso é feito nas linhas:
  PORTD = PORTD | (1 << 3); //pino 3 do PORTD recebe nível lógico 1.
  _delay_ms(1000);          //delay de 1000ms, ou 1s.
  PORTD = PORTD&~ (1 << 3); //pino 3 do PORTD recebe nível lógico 0.
  _delay_ms(1000);          //delay de 1000ms, ou 1s.
creio que essa parte seja bem autoexplicativa. Mas em resumo, os estados dos pinos é modificado através do registrador PORTN, nesse caso, PORTD. Logo, que queremos colocar o 3 pino desse port (que é o pino digital 3) com determinado nível lógico, basta alterar o valor que está armazenado no PORTD que o representa, ou seja, o bit 3 desse registrador. Caso quiséssemos alterar o pino digital 2, alteraríamos o 2 bit do PORTD, e assim por diante.

## 2. LENDO INPUTS DIGITAIS:
agora que sabemos como alterar o nível lógico dos pinos digitais, vamos aprender como ler o valor escrito neles por algum sensor, ou qualquer outra coisa que você quiser ler o valor de saída:











