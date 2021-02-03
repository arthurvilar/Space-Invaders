struct t_nodo {
    int tipo;
    int vida;
    char *sprite1;
    int posx;
    int posy;
    struct t_nodo *prox;
    struct t_nodo *prev;
};
typedef struct t_nodo t_nodo;

struct t_lista {
    t_nodo *atual;
    t_nodo *ini;
    t_nodo *fim;
    int tamanho;
};
typedef struct t_lista t_lista;


int inicializa_lista(t_lista *l);

int insere_lista(t_nodo *nodo, t_lista *l);

int inicializa_atual_inicio(t_lista *l);

int inicializa_atual_fim(t_lista *l);

void incrementa_atual(t_lista *l);

void decrementa_atual(t_lista *l);

int tamanho_lista(int *tam, t_lista *l);

int remove_item_atual(t_lista *l);