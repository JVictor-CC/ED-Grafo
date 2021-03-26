#include <stdio.h>
#include <stdlib.h>

typedef struct lista{
  int destino;
  int custo;
  struct lista *prox;
}lista;

void inicializar(lista **g,int tam){        //inicializa o grafo
  int i;
  for(i = 1; i <= tam ; i++){
    g[i] = NULL;
  }
}
//_____________________________________________________________________________________________
void ImprimeGrafoAux(lista *l){                                                     //printa o grafo
  if (l!=NULL)
  {
    printf("Destino -> %d\nCusto: %d\n",l->destino,l->custo);
    printf("\n");
    ImprimeGrafoAux(l->prox);
  }
}

void ImprimeGrafo(lista**g , int tam){
  int i;
  printf("Grafo:\n");
  for(i=1 ; i<=tam ; i++){
    printf("%d:\n________________________________________________________________\n",i);
    ImprimeGrafoAux(g[i]);
  }
}
//______________________________________________________________________________________________
int GREVertice(lista *g){
  if(g == NULL){
    return 0;
  }
  else{
  return 1 + GREVertice(g->prox) ;
  }
}

int Completo(lista **g,int tam)
{
  int i,soma=0;
  for (i=0;i<=tam;i++)
  {
    soma+=GREVertice(g[i]);
  }
  int comp = tam*(tam - 1);
  return soma == comp ? 1 : 0;
}
//_______________________________________________________________________________________
lista *Inserir(lista *l,int destino,int custo){                   //Inserir e Remover
  lista *no = (lista*)malloc(sizeof(lista));
  no->destino = destino;
  no->custo = custo;
  no->prox = l;
  return no;
}

void InserirAresta(lista **g, int origem ,int destino,int custo){
  g[origem] = Inserir(g[origem],destino,custo);
}

lista *Remover(lista *l, int destino){
    lista *aux = l;
    if(aux->destino == destino){
      l = aux->prox;
      free(aux);
    }
    else{
      while((aux->prox!=NULL) && (aux->prox->destino != destino)){
        aux = aux->prox;
      }
      if(aux->prox == NULL)
        printf("A aresta que tentou remover nao existe na lista\n");
      else{
        lista *aux2=aux->prox;
        aux->prox = aux2->prox;
        free(aux2);
      }
    }
    return l;
}

void RemoverAresta(lista **g, int origem ,int destino){
  g[origem] = Remover(g[origem] , destino );
}
//________________________________________________________________________________________
int percorreGRS(lista *g,int destino){
  if(g==NULL){
    return 0;
  }
  else{
    return g->destino == destino ? 1 + percorreGRS(g->prox,destino) : percorreGRS(g->prox,destino);
  }
}

int GRSVertice(lista **g,int tam,int vert){
  int i,soma=0;
  for(i=1; i<=tam ;i++){
    if(i != vert)
      soma+=percorreGRS(g[i],vert);
  }
  return soma;
}

void ImprimeGrau(lista **g, int tam, int vert){
    printf("Grau de entrada do vertice %d : %d\n",vert,GREVertice(g[vert]));
    printf("Grau de saida do vertice %d : %d\n",vert,GRSVertice(g,tam,vert));
}

int Existe(int *vet,int valor,int n){
  int i;
  for(i=0;i<n;i++){
    if(vet[i]==valor){
      return 1;
    }
  }
  return 0;
}

void Caminhos(lista **g, int b, int *vet, int pos){
  if(vet[pos-1]==b){
    int i;
    printf("\n");
    for(i=0;i<pos;i++){
      printf("%d",vet[i]);
    }
  }
  else{
    lista *p=g[vet[pos-1]];
    while(p!=NULL){
      if(!Existe(vet,p->destino,pos)){
        vet[pos]=p->destino;
        Caminhos(g,b,vet,pos+1);
      }
      p=p->prox;
    }
  }
}

void MenorCaminho(lista **g, int b, int *vet, int *vetAux, int pos,int *Menor){
  if(vet[pos-1]==b){
      if(pos-1 < *Menor){
        *Menor = pos;
        int i=0;
        for(i=0;i<pos;i++){
          vetAux[i]=vet[i];
        }
      }
  }
  else{
    lista *p=g[vet[pos-1]];
    while(p!=NULL){
      if(!Existe(vet,p->destino,pos)){
        vet[pos]=p->destino;
        MenorCaminho(g,b,vet,vetAux,pos+1,Menor);
      }
      p=p->prox;
    }
  }
}

void PrintaMenorCaminho(lista **g,int *vet,int tam){
  int origem,destino;
  printf("Digite a origem:");
  scanf("%d",&origem);
  printf("Digite o destino:");
  scanf("%d",&destino);
  vet[0]=origem;
  int *vetAux=(int*)malloc(sizeof(int)*tam),TamCam=4;
  MenorCaminho(g, destino,vet,vetAux,1,&TamCam);
  int i;
  for(i=0;i<TamCam;i++){
    printf("%d",vetAux[i]);
  }
}

void MenorCusto(lista **g, int dest, int *vet, int *vetAux, int pos,int *n,int *MenCust, int CustoAt){
  if(vet[pos-1]==dest){
      if(((*MenCust) == 0) || (CustoAt < (*MenCust))){
        *n = pos;
        *MenCust = CustoAt;
        int i;
        for(i=0;i<pos;i++){
          vetAux[i] = vet[i];
        }
      }
  }
  else{
    lista *p=g[vet[pos-1]];
    while(p!=NULL){
      if(!Existe(vet,p->destino,pos)){
        vet[pos]=p->destino;
        MenorCusto(g,dest,vet,vetAux,pos+1,n,MenCust,CustoAt+p->custo);
      }
      p=p->prox;
    }
  }
}

void PrintaMenorCusto(lista **g, int *vet,int tam){
  int origem,destino;
  printf("Digite a origem:");
  scanf("%d",&origem);
  printf("Digite o destino:");
  scanf("%d",&destino);
  vet[0]=origem;
  int *vetAux=(int*)malloc(sizeof(int)*tam),pos=0,MenCust=0;
  MenorCusto(g, destino,vet,vetAux,1,&pos,&MenCust,0);
  int i;
  for(i=0;i<pos;i++){
    printf("%d",vetAux[i]);
  }
}

int main()
{
  int tam=5,vert,*vet;
  int origem,destino,custo;
  int op;
  vet=(int*)malloc(sizeof(int)*tam);
  lista **g;
  g =(lista**)malloc((tam+1)*sizeof(lista*));
  inicializar(g,tam);
  printf("1-Inserir uma aresta no grafo\n");
  printf("2-Remover uma aresta do grafo\n");
  printf("3-Imprimir o grafo\n");
  printf("4-Imprimir os graus de entrada e saida do grafo\n");
  printf("5-Verifica se um grafo é completo\n");
  printf("6-Imprimir todos os caminhos entre uma origem e um destino\n");
  printf("7-Imprimir o caminho mais curto\n");
  printf("8-Imprimir o caminho de menor custo\n");
  printf("9-Sair\n");
  do
  {
    printf("\nDigite uma opcao:");
    scanf("%d",&op);
    switch(op)
    {
      case 1:
      printf("Digite a origem:");
      scanf("%d",&origem);
      printf("Digite o destino:");
      scanf("%d",&destino);
      printf("Digite o custo:");
      scanf("%d",&custo);
      InserirAresta(g,origem,destino,custo);
      break;

      case 2:
      printf("Digite a origem:");
      scanf("%d",&origem);
      printf("Digite o destino:");
      scanf("%d",&destino);
      RemoverAresta(g,origem,destino);
      break;

      case 3:
      ImprimeGrafo(g,tam);
      break;

      case 4:
      printf("Qual vertice deseja verificar? ");
      scanf("%d",&vert);
      if((vert > 0) && (vert <= tam))
        ImprimeGrau(g,tam,vert);
      else
        printf("O vertice não existe!\n");
      break;

      case 5:
      printf(Completo(g,tam) ? "O grafo está completo\n" : "O grafo está incompleto\n" );
      break;
      case 6:
      printf("Digite a origem:");
      scanf("%d",&origem);
      printf("Digite o destino:");
      scanf("%d",&destino);
      vet[0]=origem;
      Caminhos(g, destino, vet, 1);
      break;
      case 7:
      PrintaMenorCaminho(g,vet,tam);
      break;
      case 8:
      PrintaMenorCusto(g,vet,tam);
      break;
    }
  }while(op!=9);
  return 0;
}
