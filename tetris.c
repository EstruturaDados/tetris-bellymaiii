#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h> 

#define MAX_FILA 5
#define MAX_PILHA 3

typedef struct {
    char nome[3]; 
    int id;       
} Peca;

typedef struct {
    Peca pecas[MAX_FILA];
    int frente;
    int tras;
    int count; 
} FilaCircular;


typedef struct {
    Peca pecas[MAX_PILHA];
    int topo; 
} Pilha;

void limpar_buffer();
void limparTela();
void exibir_status(const FilaCircular *fila, const Pilha *pilha, bool mostrarPilha);
Peca gerarPeca(int *globalID);

void inicializar_fila(FilaCircular *fila, int *globalID);
bool fila_cheia(const FilaCircular *fila);
bool fila_vazia(const FilaCircular *fila);
bool enqueue(FilaCircular *fila, Peca peca);
Peca dequeue(FilaCircular *fila);

void inicializar_pilha(Pilha *pilha);
bool pilha_cheia(const Pilha *pilha);
bool pilha_vazia(const Pilha *pilha);
bool push(Pilha *pilha, Peca peca);
Peca pop(Pilha *pilha);

void trocar_fila_pilha(FilaCircular *fila, Pilha *pilha);
void inverter_fila_pilha(FilaCircular *fila, Pilha *pilha);

void modo_novato();
void modo_aventureiro();
void modo_mestre();

int main(int argc, char *argv[]) {
    int modo;
    srand(time(NULL)); 

    do {
        limparTela();
        printf("========================================================\n");
        printf("      DESAFIO TETRIS STACK - Seleção de Nível\n");
        printf("========================================================\n");
        printf("1.   Fila de Peças (Future Queue)\n");
        printf("2.   Fila + Pilha de Reserva (Hold Stack)\n");
        printf("3.   Fila + Pilha + Estratégia Avançada\n");
        printf("0. Sair\n");
        printf("Escolha o modo: ");

        if (scanf("%d", &modo) != 1) { modo = -1; }
        limpar_buffer();

        switch (modo) {
            case 1: modo_novato(); break;
            case 2: modo_aventureiro(); break;
            case 3: modo_mestre(); break;
            case 0: printf("\nEncerrando o Desafio Tetris Stack. Boa jogatina!\n"); break;
            default: printf("\nOpção inválida. Tente novamente.\n");
        }
        
        if (modo != 0) { printf("\nPressione ENTER para voltar ao menu..."); getchar(); }

    } while (modo != 0);

    return 0;
}

void modo_novato() {
    FilaCircular fila;
    int globalID = 0;
    int opcao;

    inicializar_fila(&fila, &globalID);
    
    do {
        limparTela();
        printf("--- MODO NOVATO: Fila de Peças Futuras ---\n");
        exibir_status(&fila, NULL, false); 
        
        printf("\nOpções:\n");
        printf("1. Jogar Peça (Dequeue + Enqueue Automático)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) { opcao = -1; }
        limpar_buffer();

        switch (opcao) {
            case 1: {
                if (fila_vazia(&fila)) {
                    printf("Fila está vazia, não há o que jogar.\n");
                } else {
                    Peca jogada = dequeue(&fila);
                    Peca nova = gerarPeca(&globalID);
                    enqueue(&fila, nova); 
                    printf("\nPEÇA JOGADA: [%s | ID %d]\n", jogada.nome, jogada.id);
                    printf("PEÇA ADICIONADA: [%s | ID %d] (Fila repreenchida)\n", nova.nome, nova.id);
                }
                break;
            }
            case 0: break;
            default: printf("Opção inválida.\n");
        }
        
        if (opcao != 0) { printf("\nPressione ENTER para continuar..."); getchar(); }
    } while (opcao != 0);
}

void modo_aventureiro() {
    FilaCircular fila;
    Pilha pilha;
    int globalID = 0;
    int opcao;

    inicializar_fila(&fila, &globalID);
    inicializar_pilha(&pilha);
    
    do {
        limparTela();
        printf("--- MODO AVENTUREIRO: Fila e Pilha de Reserva ---\n");
        exibir_status(&fila, &pilha, true); 
        
        printf("\nOpções:\n");
        printf("1. Jogar Peça (Dequeue + Enqueue Automático)\n");
        printf("2. Reservar Peça da Frente (Push + Enqueue Automático)\n");
        printf("3. Usar Peça Reservada (Pop)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) { opcao = -1; }
        limpar_buffer();

        switch (opcao) {
            case 1: { 
                if (fila_vazia(&fila)) { printf("Fila vazia!\n"); } 
                else {
                    Peca jogada = dequeue(&fila);
                    Peca nova = gerarPeca(&globalID);
                    enqueue(&fila, nova); 
                    printf("\nPEÇA JOGADA: [%s | ID %d]\n", jogada.nome, jogada.id);
                    printf("PEÇA ADICIONADA: [%s | ID %d] (Fila repreenchida)\n", nova.nome, nova.id);
                }
                break;
            }
            case 2: { 
                if (fila_vazia(&fila)) { printf("\nNão há peça na frente da fila para reservar.\n"); }
                else if (pilha_cheia(&pilha)) { printf("\nReserva (Pilha) cheia! Capacidade máx. de %d peças.\n", MAX_PILHA); }
                else {
                    Peca reservada = dequeue(&fila);
                    push(&pilha, reservada);
                    Peca nova = gerarPeca(&globalID);
                    enqueue(&fila, nova); 
                    printf("\nPEÇA RESERVADA: [%s | ID %d] (Movida da fila para a pilha)\n", reservada.nome, reservada.id);
                    printf("PEÇA ADICIONADA: [%s | ID %d] (Fila repreenchida)\n", nova.nome, nova.id);
                }
                break;
            }
            case 3: { 
                if (pilha_vazia(&pilha)) { printf("\nNenhuma peça na reserva (Pilha) para usar.\n"); }
                else {
                    Peca usada = pop(&pilha);
                    printf("\nPEÇA USADA: [%s | ID %d] (Removida da Pilha)\n", usada.nome, usada.id);
                }
                break;
            }
            case 0: break;
            default: printf("Opção inválida.\n");
        }
        
        if (opcao != 0) { printf("\nPressione ENTER para continuar..."); getchar(); }
    } while (opcao != 0);
}

void modo_mestre() {
    FilaCircular fila;
    Pilha pilha;
    int globalID = 0;
    int opcao;
    
    Peca ultimaPecaJogada = {"", -1}; 
    bool ultimaOpFoiJogar = false;

    inicializar_fila(&fila, &globalID);
    inicializar_pilha(&pilha);
    
    do {
        limparTela();
        printf("--- MODO MESTRE: Estratégia Avançada ---\n");
        exibir_status(&fila, &pilha, true); 
        
        printf("\nOpções:\n");
        printf("1. Jogar Peça\n");
        printf("2. Reservar Peça\n");
        printf("3. Usar Peça Reservada\n");
        printf("4. Trocar Peça (Topo da Pilha <=> Frente da Fila)\n");
        printf("5. Desfazer Última Jogada/Reserva\n");
        printf("6. Inverter (Fila <=> Pilha)\n");
        printf("0. Voltar ao Menu Principal\n");
        printf("Escolha: ");

        if (scanf("%d", &opcao) != 1) { opcao = -1; }
        limpar_buffer();

        switch (opcao) {
            case 1: { 
                if (fila_vazia(&fila)) { printf("Fila vazia!\n"); } 
                else {
                    ultimaPecaJogada = dequeue(&fila);
                    ultimaOpFoiJogar = true;
                    Peca nova = gerarPeca(&globalID);
                    enqueue(&fila, nova); 
                    printf("\nPEÇA JOGADA: [%s | ID %d] (Registro para Desfazer)\n", ultimaPecaJogada.nome, ultimaPecaJogada.id);
                    printf("PEÇA ADICIONADA: [%s | ID %d]\n", nova.nome, nova.id);
                }
                break;
            }
            case 2: { 
                if (fila_vazia(&fila)) { printf("\nFila vazia.\n"); }
                else if (pilha_cheia(&pilha)) { printf("\nReserva (Pilha) cheia!\n"); }
                else {
                    ultimaPecaJogada = dequeue(&fila);
                    ultimaOpFoiJogar = false; 
                    push(&pilha, ultimaPecaJogada);
                    Peca nova = gerarPeca(&globalID);
                    enqueue(&fila, nova); 
                    printf("\nPEÇA RESERVADA: [%s | ID %d] (Registro para Desfazer)\n", ultimaPecaJogada.nome, ultimaPecaJogada.id);
                    printf("PEÇA ADICIONADA: [%s | ID %d]\n", nova.nome, nova.id);
                }
                break;
            }
            case 3: { 
                if (pilha_vazia(&pilha)) { printf("\nNenhuma peça na reserva (Pilha) para usar.\n"); }
                else {
                    Peca usada = pop(&pilha);
                    printf("\nPEÇA USADA: [%s | ID %d] (Removida da Pilha)\n", usada.nome, usada.id);
                }
                break;
            }
            case 4: { 
                if (fila_vazia(&fila) || pilha_vazia(&pilha)) {
                    printf("\nAmbas estruturas precisam ter peças para realizar a troca.\n");
                } else {
                    trocar_fila_pilha(&fila, &pilha);
                    printf("\nTROCA REALIZADA: A peça da frente da Fila foi trocada pela peça do Topo da Pilha.\n");
                }
                break;
            }
            case 5: { 
                if (ultimaPecaJogada.id == -1) {
                    printf("\nNão há última jogada/reserva para desfazer ou já foi desfeita.\n");
                } else {
                    if (ultimaOpFoiJogar) {
                        
                        if (!fila_vazia(&fila)) { dequeue(&fila); } 
                        enqueue(&fila, ultimaPecaJogada); 
                        printf("\nDESFEITO: A peça [%s | ID %d] (Jogada) foi colocada de volta no final da Fila.\n", ultimaPecaJogada.nome, ultimaPecaJogada.id);
                    } else { 
                        if (!pilha_vazia(&pilha)) { pop(&pilha); } 
                        if (!fila_vazia(&fila)) { dequeue(&fila); } 
                        enqueue(&fila, ultimaPecaJogada); 
                        printf("\nDESFEITO: A peça [%s | ID %d] (Reservada) foi removida da Pilha e colocada de volta na Fila.\n", ultimaPecaJogada.nome, ultimaPecaJogada.id);
                    }
                    ultimaPecaJogada.id = -1; 
                }
                break;
            }
            case 6: { 
                inverter_fila_pilha(&fila, &pilha);
                printf("\nINVERSÃO CONCLUÍDA: Conteúdo da Fila e da Pilha foram trocados.\n");
                break;
            }
            case 0: break;
            default: printf("Opção inválida.\n");
        }
        
        if (opcao != 0) { printf("\nPressione ENTER para continuar..."); getchar(); }
    } while (opcao != 0);
}

void limparTela() { 
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void limpar_buffer() { 
    int c; 
    while ((c = getchar()) != '\n' && c != EOF); 
}

Peca gerarPeca(int *globalID) {
    Peca nova;
    int tipo = rand() % 7; 
    char *nomes[] = {"I", "O", "T", "J", "L", "S", "Z"};
    
    strcpy(nova.nome, nomes[tipo]);
    nova.id = ++(*globalID);
    return nova;
}

void exibir_status(const FilaCircular *fila, const Pilha *pilha, bool mostrarPilha) {
    if (mostrarPilha && pilha != NULL) {
        printf("\n--- RESERVA (PILHA - LIFO) --- [Capacidade: %d/%d]\n", pilha->topo + 1, MAX_PILHA);
        if (pilha_vazia(pilha)) {
            printf("| VAZIA |\n");
        } else {
            for (int i = pilha->topo; i >= 0; i--) {
                printf("| %s | ID %d %s\n", pilha->pecas[i].nome, pilha->pecas[i].id, (i == pilha->topo ? "<-- TOPO" : ""));
            }
        }
    }
    
    printf("\n--- PEÇAS FUTURAS (FILA - FIFO) --- [Itens: %d/%d]\n", fila->count, MAX_FILA);
    if (fila_vazia(fila)) {
        printf("[ FRENTE ] -> VAZIA -> [ TRÁS ]\n");
    } else {
        printf("[ FRENTE ] ");
        int i = fila->frente;
        int count = 0;
        while (count < fila->count) {
            printf("-> [%s | ID %d] ", fila->pecas[i].nome, fila->pecas[i].id);
            i = (i + 1) % MAX_FILA;
            count++;
        }
        printf("-> [ TRÁS ]\n");
    }
}

void inicializar_fila(FilaCircular *fila, int *globalID) {
    fila->frente = 0;
    fila->tras = 0;
    fila->count = 0;

    for (int i = 0; i < MAX_FILA; i++) {
        Peca nova = gerarPeca(globalID);
        enqueue(fila, nova);
    }
}

bool fila_cheia(const FilaCircular *fila) {
    return fila->count == MAX_FILA;
}

bool fila_vazia(const FilaCircular *fila) {
    return fila->count == 0;
}

bool enqueue(FilaCircular *fila, Peca peca) {
    if (fila_cheia(fila)) {
        return false;
    }
    fila->pecas[fila->tras] = peca;
    fila->tras = (fila->tras + 1) % MAX_FILA;
    fila->count++;
    return true;
}

Peca dequeue(FilaCircular *fila) {
    Peca pecaVazia = {"", -1};
    if (fila_vazia(fila)) {
        return pecaVazia;
    }
    Peca peca = fila->pecas[fila->frente];
    fila->frente = (fila->frente + 1) % MAX_FILA;
    fila->count--;
    return peca;
}

void inicializar_pilha(Pilha *pilha) {
    pilha->topo = -1;
}

bool pilha_cheia(const Pilha *pilha) {
    return pilha->topo == MAX_PILHA - 1;
}

bool pilha_vazia(const Pilha *pilha) {
    return pilha->topo == -1;
}

bool push(Pilha *pilha, Peca peca) {
    if (pilha_cheia(pilha)) {
        return false;
    }
    pilha->topo++;
    pilha->pecas[pilha->topo] = peca;
    return true;
}

Peca pop(Pilha *pilha) {
    Peca pecaVazia = {"", -1};
    if (pilha_vazia(pilha)) {
        return pecaVazia;
    }
    Peca peca = pilha->pecas[pilha->topo];
    pilha->topo--;
    return peca;
}

void trocar_fila_pilha(FilaCircular *fila, Pilha *pilha) {
    Peca pecaFila = dequeue(fila);  
    Peca pecaPilha = pop(pilha);    
    
    push(pilha, pecaFila);
    
    enqueue(fila, pecaPilha);
}

void inverter_fila_pilha(FilaCircular *fila, Pilha *pilha) {
    
    Peca tempFila[MAX_FILA];
    int filaCount = fila->count;
    for(int i = 0; i < filaCount; i++) {
        tempFila[i] = dequeue(fila);
    }
    
    Peca tempPilha[MAX_PILHA];
    int pilhaCount = pilha->topo + 1;
    for(int i = 0; i < pilhaCount; i++) {
        
        tempPilha[i] = pop(pilha); 
    }
    
    for (int i = 0; i < pilhaCount; i++) {
        enqueue(fila, tempPilha[i]);
    }
    
    for (int i = filaCount - 1; i >= 0; i--) {
        push(pilha, tempFila[i]);
    }
}