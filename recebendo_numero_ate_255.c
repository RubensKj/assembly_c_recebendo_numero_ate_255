/* Main.c file generated by New Project wizard
 *
 * Created:   sex ago 13 2021
 * Processor: 8086
 * Compiler:  Digital Mars C
 *
 * Before starting simulation set Internal Memory Size 
 * in the 8086 model properties to 0x10000
 */

#define IO0 0000H
#define IO1 0200H
#define IO2 0400H
#define IO3 0600H
#define IO4 0800H
#define IO5 0A00H
#define IO6 0C00H
#define IO7 0E00H
#define IO8 1000H
#define IO9 1200H

unsigned char DDH = 2;
unsigned char UUH = 3;
unsigned char DDM = 5;
unsigned char UUM = 9;
unsigned char DDS = 5;
unsigned char UUS = 9;

// 8251A USART 

#define ADR_USART_DATA   (IO9 + 00h)
//ONDE VOCE VAI MANDAR E RECEBER DADOS DO 8251

#define ADR_USART_CMD   (IO9 + 02h)
//É O LOCAL ONDE VOCE VAI ESCREVER PARA PROGRAMAR O 8251
//WRITE 1202H

#define ADR_USART_STAT  (IO9 + 02h)
//RETORNA O STATUS SE UM CARACTER FOI DIGITADO NO TERMINAL
//RETORNA O STATUS SE POSSO TRANSMITIR CARACTER PARA O TERMINAL
//READ 1202H 

#define true  1
#define false 0

#define NULL 0

#define TAM 999
#define TAM_NUMERO 3

char STRING[TAM+1];
unsigned char QNT_CARACTERES_DIGITADOS;

void RECEBER_CARACTER(void);
void MANDA_CARACTER(void);

#define NULL 0

char MENS1[] = "ENTRE COM O NUMERO DA TABUADA";

// ============ INICIO 8251 ============

//19200,8,N,1
void INICIALIZA_8251(void)
{
   _asm {
   MOV AL,0
   MOV DX, ADR_USART_CMD
   OUT DX,AL
   OUT DX,AL
   OUT DX,AL
   MOV AL,40H
   OUT DX,AL
   MOV AL,4DH
   OUT DX,AL
   MOV AL,37H
   OUT DX,AL
      }
}

// ============ FINAL 8251 ============
// ============ INICIO CODIGO ============


//ao finalizar este subprograma, devemos ter em AL o byte digitado
void RECEBER_NUMERO(void)
{
   _asm
   {
      pushf
      push bx
      push cx
      lea bx, STRING //parametro
      mov QNT_CARACTERES_DIGITADOS,0
      
      // Primeiro Numero
      mov cl,0 // NULL
      mov dh,0 // NULL
      
      // Segundo Numero
      mov ch,0 // NULL
      mov dl,0 // NULL
      
RECEBER_STRING_CARACTER_NUMERO:      
      call RECEBER_CARACTER // al = ascii da tecla pressionada
      cmp al, 13 //CR
      je  RECEBER_CARACTER_TRATA_CR_NUMERO
      cmp al, 8 //BS
      je  RECEBER_CARACTER_TRATA_BS_NUMERO	
      cmp QNT_CARACTERES_DIGITADOS,TAM_NUMERO //atingiu limite ????
      je  RECEBER_STRING_CARACTER_NUMERO //ignora armazenamento da tecla     
     
      
      cmp al,'0'
      jl  RECEBER_STRING_CARACTER_NUMERO //l less
      cmp al,'9'
      jg  RECEBER_STRING_CARACTER_NUMERO //g greater  
      
      // Validações ficam aqui:
	 
      cmp QNT_CARACTERES_DIGITADOS,0
      je DIGITOU_PRIMEIRO_NUMERO
      
      cmp QNT_CARACTERES_DIGITADOS,1
      je DIGITOU_SEGUNDO_NUMERO
      
      cmp QNT_CARACTERES_DIGITADOS,2
      je DIGITOU_TERCEIRO_NUMERO
      
DIGITOU_TERCEIRO_NUMERO:
      cmp cl,true
      je VALIDA_TERCEIRO_PELO_PRIMEIRO

      cmp ch,false
      je DIGITA_TERCEIRO_NUMERO
      
      jmp RECEBER_STRING_CARACTER_NUMERO
      
VALIDA_TERCEIRO_PELO_PRIMEIRO:
      cmp dl,true
      je DEIXA_DIGITAR_PELO_SEGUNDO_NUMERO
      
      cmp ch,true
      je RECEBER_STRING_CARACTER_NUMERO
      
      cmp dh,'3'
      jl DIGITA_TERCEIRO_NUMERO
      
      jmp RECEBER_STRING_CARACTER_NUMERO
      
DEIXA_DIGITAR_PELO_SEGUNDO_NUMERO:
      cmp dh,'2'
      jg RECEBER_STRING_CARACTER_NUMERO

      cmp al,'6'
      jl DIGITA_TERCEIRO_NUMERO
      jmp RECEBER_STRING_CARACTER_NUMERO
      
DIGITA_TERCEIRO_NUMERO:
      jmp ENVIA_CARACTER_TERMINAL
      
DIGITOU_SEGUNDO_NUMERO:
      cmp cl,true
      je VALIDA_SEGUNDO_NUMERO_MAIOR
      jmp ENVIA_CARACTER_TERMINAL
    
VALIDA_SEGUNDO_NUMERO_MAIOR:  
      cmp al,'4'
      jg TROCA_CH_PARA_TRUE
      
      cmp al,'6'
      jl TROCA_CH_PARA_FALSE
      
TROCA_CH_PARA_TRUE:
      mov ch,true
      cmp al,'5'
      je VALIDA_SEGUNDO_VALOR_IGUAL_CINCO
      mov dl,false
      jmp ENVIA_CARACTER_TERMINAL
      
TROCA_CH_PARA_FALSE:
      mov ch,false
      cmp al,'5'
      je VALIDA_SEGUNDO_VALOR_IGUAL_CINCO
      mov dl,false
      jmp ENVIA_CARACTER_TERMINAL

VALIDA_SEGUNDO_VALOR_IGUAL_CINCO:
      mov dl,true
      jmp ENVIA_CARACTER_TERMINAL
     
DIGITOU_PRIMEIRO_NUMERO:
      mov dh,al
      cmp al,'1'
      jg TROCA_CL_PARA_TRUE
      
      cmp al,'3'
      jl TROCA_CL_PARA_FALSE
     
TROCA_CL_PARA_TRUE:
      mov cl,true
      jmp ENVIA_CARACTER_TERMINAL
      
TROCA_CL_PARA_FALSE:
      mov cl,false
      jmp ENVIA_CARACTER_TERMINAL
      
RECEBER_CARACTER_TRATA_CR_NUMERO: 
      cmp QNT_CARACTERES_DIGITADOS,3 //voce digitou 3 caracteres ? 
      jl  RECEBER_STRING_CARACTER_NUMERO
      mov al,0 //NULL
      mov [bx], al
      jmp RECEBER_STRING_SAI_NUMERO
      
RECEBER_CARACTER_TRATA_BS_NUMERO:      
      cmp QNT_CARACTERES_DIGITADOS,0
      je LIMPA_REGISTRADORES

      cmp QNT_CARACTERES_DIGITADOS,1
      je LIMPA_REGISTRADORES_PRIMEIRO_NUMERO

      cmp QNT_CARACTERES_DIGITADOS,2
      je LIMPA_REGISTRADORES_SEGUNDO_NUMERO
      
      jmp FINALIZA_BACKSPACE
      
      
LIMPA_REGISTRADORES_SEGUNDO_NUMERO:
      mov ch,0
      mov dl,0
      jmp FINALIZA_BACKSPACE
   
LIMPA_REGISTRADORES_PRIMEIRO_NUMERO:
      call LIMPA_REGISTRADORES_SEGUNDO_NUMERO
      mov cl,0
      mov dh,0
      jmp FINALIZA_BACKSPACE
      
LIMPA_REGISTRADORES:
      call LIMPA_REGISTRADORES_SEGUNDO_NUMERO
      call LIMPA_REGISTRADORES_PRIMEIRO_NUMERO
      jmp FINALIZA_BACKSPACE

FINALIZA_BACKSPACE:
      cmp QNT_CARACTERES_DIGITADOS,0
      je  RECEBER_STRING_CARACTER_NUMERO
      dec bx //ou sub bx,1
      dec QNT_CARACTERES_DIGITADOS
      mov al, 8 //BS
      call MANDA_CARACTER //manda backspace para o TERMINAL
      jmp RECEBER_STRING_CARACTER_NUMERO

ENVIA_CARACTER_TERMINAL:
      mov  [bx], al //armazena em STRING na posicao bx
      inc  bx //avanca bx
      inc QNT_CARACTERES_DIGITADOS
      call MANDA_CARACTER // manda para terminal burro
      jmp  RECEBER_STRING_CARACTER_NUMERO

RECEBER_STRING_SAI_NUMERO:
      lea bx, STRING
      mov ch, [bx] // "1"-->1   cx <--- ch,cl
      inc bx
      mov cl, [bx] // "7"-->7
      sub ch, '0' // "1" - 48 = 1
      sub cl, '0' // "7" - 48 = 7
      // mul
      // ax = al * xx
      mov ah, 10  //multiplicador
      mov al, ch
      mul ah  // ax <==== al * ah // neste caso, ax terá 10 
      add al, cl //veja, temos em AL o byte!!!!!!!      
      pop cx
      pop bx
      popf
   }  
}

//certificar que AL tenha o numero a ser impresso
//AL = 17
void MANDA_NUMERO(void)
{
 _asm
 {
      pushf
      push ax //pq estas salvando al, se al é parametro ? na verdade estou salvando ah
      push bx
      //DIV 
      //DIV XX
      //AX <= AX / XX ---> RESTO --> AH       QUOCIENTE ----> AL
      mov ah, 0
      mov bh, 10
      div bh
      // AX / bh ===> teremos em AH = 7 e em AL = 1
      add al,'0' // 1 ---> "1"
      call MANDA_CARACTER
      add ah,'0' // 7 ---> "7"
      mov al,ah // move "7" para Al para ser impresso!!!!!!!!!!!!!
      call MANDA_CARACTER
      pop bx
      pop ax
      popf
 }  
}


void PULA_LINHA(void)
{
   _asm 
   {
	 pushf
	 push ax
	 mov al, 13 //cr
	 call MANDA_CARACTER
	 mov al, 10 //lf
	 call MANDA_CARACTER
	 pop ax
	 popf
   }
}

//AO TERMINO DESTA ROTINA, TEREMOS EM AL
//O CODIGO ASCII DA TECLA DIGITADA
void RECEBER_CARACTER(void)
{
   _asm {
      PUSHF
      PUSH DX
AGUARDA_CARACTER:
      MOV DX, ADR_USART_STAT
      IN  AL,DX
      TEST AL,2 ;000000S0
      JZ AGUARDA_CARACTER
      MOV DX, ADR_USART_DATA
      IN AL,DX
      SHR AL,1 
NAO_RECEBIDO:
      POP DX
      POPF
   }
}

//TENHA CERTEZA QUE O CARACTER ESTEJA EM AL
//AL É A PASSAGEM DE PARAMETRO - (POR REGISTRADOR)
void MANDA_CARACTER(void)
{
   _asm {
      PUSHF  ; SALVA FLAGS Z E C
      PUSH DX
      PUSH AX  ; SALVA AL   AX = AH/AL
BUSY:
      MOV DX, ADR_USART_STAT
      IN  AL,DX
      TEST AL,1 ; 0000000S
      JZ BUSY
      MOV DX, ADR_USART_DATA
      POP AX  ; RESTAURA AL
      OUT DX,AL
      POP DX
      POPF ; RESTAURA FLAGS Z E C
   }  
}

void RECEBE_NUMERO_GRANDE(void)
{
   _asm {
      call RECEBER_NUMERO
   }
}

// ============ FINAL CODIGO ============

// ============ INICIO MAIN ============
void main(void)
 {
   // Write your code here
   INICIALIZA_8251(); //UMA UNICA VEZ 
   
   while(true)
   { 
      _asm
      {
	 call RECEBE_NUMERO_GRANDE
      }
   }          
}

// ============ FINAL MAIN ============
