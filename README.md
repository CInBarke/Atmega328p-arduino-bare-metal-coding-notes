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

  PORTD = PORTD | (1 << 3); //pino 3 do PORTD recebe nível lógico 1. <br />
  _delay_ms(1000);          //delay de 1000ms, ou 1s. <br />
  PORTD = PORTD&~ (1 << 3); //pino 3 do PORTD recebe nível lógico 0. <br />
  _delay_ms(1000);          //delay de 1000ms, ou 1s. <br />
  
creio que essa parte seja bem autoexplicativa. Mas em resumo, os estados dos pinos é modificado através do registrador PORTN, nesse caso, PORTD. Logo, que queremos colocar o 3 pino desse port (que é o pino digital 3) com determinado nível lógico, basta alterar o valor que está armazenado no PORTD que o representa, ou seja, o bit 3 desse registrador. Caso quiséssemos alterar o pino digital 2, alteraríamos o 2 bit do PORTD, e assim por diante.

## 2. LENDO INPUTS DIGITAIS:
agora que sabemos como alterar o nível lógico dos pinos digitais, vamos aprender como ler o valor escrito neles por algum sensor, ou qualquer outra coisa que você quiser ler o valor de saída:
um impulso inicial que eu tive foi fazer simplesmente:

uint8_t state = (PORTD >> n) & 1;

logo que assim, o n'esimo bit (o bit do pino que queremos ler) vai até o bit 0 (por causa do shift right), e os demais bits se tornam 0 por causa da operação AND, efetuada como X & 000001. Logo que X & 0 = 0, todos os outros bits viram 0, portanto, se o n'esimo bit original for 0, o resultado da operação é 0. Caso o n'esimo bit for 1, o resultado da operação é 1. Porém, isso não funciona, logo que PORTD contêm somente os valores escritos, e não contêm os valores reais que estão armazenados no pino em tempo real. Para fazer isso, fazemos:

uint8_t state = (PIND >> n) & 1;

ou seja, mudamos de PORTD para PIND quando quisermos ler o valor atual que está no pino.
Além disso, como fizemos antes, devemos setar o data direction register do pino como 0. Logo que estamos tratando de um input.
Tirando isso, os demais elementos do código já foram descritos no blinky.

## 4. DAS FUNÇÕES:
basicamente, a única coisa a se falar aqui é que funções podem ser perigosas de mexer, logo que um instinto inicial lhe fala para simplesmente declarar as funções como void, ou uint8_t, ou qualquer outro tipo que você queira que sua função seja, contudo, as coisas são um pouco mais complicadas que isso, logo que fazer isso geralmente quebra o código todo (confesso que não sei exatamente o porque, sei que tem a ver com o compilador e com o arduino confundir essas funções com a main(), mas tirando isso, não muito.
como consertar isso? basta declarar as funções como static void, ou static int, etc etc. Por que? não faço ideia. Mas essa é a vida né. Quando eu tiver alguma ideia doque pode estar acontecendo e porque colocar tudo como static conserta, eu venho aqui e atualizo, mas por enquanto vai ficar por essa mesmo. Logo, quando em algum código futuro for lido funções declaradas como statics, é por isso.

## 5. BRINCANDO COM OS TIMERS:
isso é simplesmente a preparação para mexer com os pinos analógicos. Por que? Porque agora que não podemos nos defender por meio de uma camada de abstração, precisamos implementar tudo à mão. Isso significa mexer com PWM, que por sua vez significa mexer com os timers, e eventualmente até com os componentes internos do arduino responsáveis por essa parte, como o DAC (digital to analog converter), os timers (responsáveis pelo PWM), o Mux dos canais analógicos, etc etc. Enfim, espero ter deixado claro o porque de toda essa preparação para algo que parece trivial, como os pinos analógicos...

Enfim, voltando para os timers, o exercício agora vai ser implementar o blinky sem a biblioteca que implementa o _delay_ms(1000), ou seja, tentaremos recriar essa função do 0, a partir dos registradores dos timers. Aqui a parada fica meio bizonha, então aperte os cintos e tome um café, e vamos direto ao bagui














