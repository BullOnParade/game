#include <iostream>
#include "MapNode.hpp"

using namespace std;

// POSTCONDITION: INIZIALIZZA IL NODO CON COORDINATE 0.0
MapNode::MapNode()
{
    X = 0;
    Y = 0;
    building = -1;
    N = NULL;
    S = NULL;
    E = NULL;
    W = NULL;
    next = NULL;
    LineRight = NULL;
}

// PRECONDITION: x e y sono le coordinate del nodo cercato
// POSTCONDITION: ritorna un puntatore al nodo se il nodo esiste, NULL altrimenti
MapNode* MapNode::FindNode(int x, int y)
{
    MapNode* t = this;
    bool found = false;
    while((t!=NULL)&&(!found))
    {
        if ((x == t->X)&&(y == t->Y)) found = true;
        else t = t->next;
    }
    return(t);
}

// POSTCONDITION: ritorna l'ordinata minima presente nelle cordinate della mappa;
Estremi MapNode::EstremiMappa()
{
    Estremi e;
    e.maxX = 0;
    e.maxY = 0;
    e.minX = 0;
    e.minY = 0;
    MapNode* t = this->next;
    while(t!=NULL)
    {
        if (t->X > e.maxX) e.maxX = t->X;
        else if (t->X < e.minX) e.maxX = t->X;
        if (t->Y > e.maxY) e.maxY = t->Y;
        else if (t->Y < e.minY) e.maxY = t->Y;
        t = t->next;
    }
    return(e);
}

// PRECONDITION: pos è la posizione che deve essere collegata ai nodi dello stesso livello
void MapNode::LinkLine(MapNode* pos)
{
    MapNode *t = this, *succ = NULL, *prec = NULL;
    while(t != NULL)
    {
        if(t->Y == pos->Y)          /* Consideriamo solo i nodi sullo stesso livello */
        {
            if (t->X < pos->X)
            {
                if (prec == NULL) prec = t;
                else if (t->X > prec->X) prec = t;
            }
            else if (t->X > pos->X)
            {
                if (succ == NULL) succ = t;
                else if (t->X < succ->X) succ = t;
            }
        }
        t = t->next;
    }
    if (prec != NULL) prec->LineRight = pos;
    pos->LineRight = succ;
}

// PRECONDITION: y è l'ordinata dell'elemento cercato
// POSTCONDITION: ritorna un puntatore all'elemento più a sinistra di quella linea
MapNode* MapNode::mostLeftLine(int y)
{
    MapNode *t = this, *mostleft = NULL;
    while(t != NULL)
    {
        if (y == t->Y)
        {
            if (mostleft == NULL) mostleft = t;
            else if (t->X < mostleft->X) mostleft = t;
        }
        t = t->next;
    }
    return(mostleft);
}

// PRECONDITION: pos != NULL
// POSTCONDITION: ritorna un pos->N
MapNode* MapNode::GateNord(MapNode* pos)
{
    return(pos->N);
}

MapNode* MapNode::GateSud(MapNode* pos)
{
    return(pos->S);
}

MapNode* MapNode::GateEast(MapNode* pos)
{
    return(pos->E);
}

MapNode* MapNode::GateWest(MapNode* pos)
{
    return(pos->W);
}

// PRECONDITION: pos->N == NULL
// POSTCONDITION: crea un nuovo nodo a NORD di pos e aggiorna i puntatori della mappa e il puntatore next.
//                Ritorna un puntatore al nodo
MapNode* MapNode::CreateNord(MapNode* pos)
{
    pos->N = new MapNode();
    MapNode* NewNode = pos->N;
    NewNode->X = pos->X;
    NewNode->Y = pos->Y + 1;
    NewNode->S = pos;
    NewNode->N = FindNode( NewNode->X, NewNode->Y + 1);
    if (NewNode->N != NULL) NewNode->N->S = NewNode;
    NewNode->E = FindNode( NewNode->X + 1, NewNode->Y);
    if (NewNode->E != NULL) NewNode->E->W = NewNode;
    NewNode->W = FindNode( NewNode->X - 1, NewNode->Y);
    if (NewNode->W != NULL) NewNode->W->E = NewNode;
    LinkLine(NewNode);
    /* Aggiorniamo la pseudolista: l'inserimento in coda ha costo lineare, mentre l'inserimento al secondo posto ha costo
    costante. Inseriamo al secondo posto e non al primo perchè la funzione FindNode viene chiamata sul nodo (0,0).*/
    NewNode->next = next; /* Il secondo next si riferisce a quello del nodo (0,0).*/
    next = NewNode;
    return(NewNode);
}

MapNode* MapNode::CreateSud(MapNode* pos)
{
    pos->S = new MapNode();
    MapNode* NewNode = pos->S;
    NewNode->X = pos->X;
    NewNode->Y = pos->Y - 1;
    NewNode->N = pos;
    NewNode->S = FindNode( NewNode->X, NewNode->Y - 1);
    if (NewNode->S != NULL) NewNode->S->N = NewNode;
    NewNode->E = FindNode( NewNode->X + 1, NewNode->Y);
    if (NewNode->E != NULL) NewNode->E->W = NewNode;
    NewNode->W = FindNode( NewNode->X - 1, NewNode->Y);
    if (NewNode->W != NULL) NewNode->W->E = NewNode;
    LinkLine(NewNode);
    NewNode->next = next; /* Il secondo next si riferisce a quello del nodo (0,0).*/
    next = NewNode;
    return(NewNode);
}

MapNode* MapNode::CreateEast(MapNode* pos)
{
    pos->E = new MapNode();
    MapNode* NewNode = pos->E;
    NewNode->X = pos->X + 1;
    NewNode->Y = pos->Y;
    NewNode->W = pos;
    NewNode->S = FindNode( NewNode->X, NewNode->Y - 1);
    if (NewNode->S != NULL) NewNode->S->N = NewNode;
    NewNode->E = FindNode( NewNode->X + 1, NewNode->Y);
    if (NewNode->E != NULL) NewNode->E->W = NewNode;
    NewNode->N = FindNode( NewNode->X, NewNode->Y + 1);
    if (NewNode->N != NULL) NewNode->N->S = NewNode;
    LinkLine(NewNode);
    NewNode->next = next; /* Il secondo next si riferisce a quello del nodo (0,0).*/
    next = NewNode;
    return(NewNode);
}

MapNode* MapNode::CreateWest(MapNode* pos)
{
    pos->W = new MapNode();
    MapNode* NewNode = pos->W;
    NewNode->X = pos->X - 1;
    NewNode->Y = pos->Y;
    NewNode->E = pos;
    NewNode->S = FindNode( NewNode->X, NewNode->Y - 1);
    if (NewNode->S != NULL) NewNode->S->N = NewNode;
    NewNode->W = FindNode( NewNode->X - 1, NewNode->Y);
    if (NewNode->W != NULL) NewNode->W->E = NewNode;
    NewNode->N = FindNode( NewNode->X, NewNode->Y + 1);
    if (NewNode->N != NULL) NewNode->N->S = NewNode;
    LinkLine(NewNode);
    NewNode->next = next; /* Il secondo next si riferisce a quello del nodo (0,0).*/
    next = NewNode;
    return(NewNode);
}

void MapNode::Stampa()
{
    Estremi l = EstremiMappa();
    cout << "\n\n\n";
    for(int current_y = l.maxY; current_y >= l.minY; current_y = current_y - 1)
    {
        MapNode *mostLeft = mostLeftLine(current_y);
        MapNode *curr = mostLeft->LineRight, *prec = mostLeft;
        // PRIMA PARTE
        for(int nodiFantasma = mostLeft->X - l.minX; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "           ";
        cout << "|^ ^ ^|";
        while (curr != NULL)
        {
            if (curr->W != NULL) cout << "  ";
            else for(int nodiFantasma = curr->X - prec->X -1 ; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "           ";
            cout << "|^ ^ ^|";
            curr = curr->LineRight;
            prec = prec->LineRight;
        }
        cout << "\n";
        prec = mostLeft;
        curr = mostLeft->LineRight;
        // SECONDA PARTE
        for(int nodiFantasma = mostLeft->X - l.minX; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "           ";
        cout << "| ^ ^ |";
        while (curr != NULL)
        {
            if (curr->W != NULL) cout << "--";
            else for(int nodiFantasma = curr->X - prec->X -1 ; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "           ";
            cout << "| ^ ^ |";
            curr = curr->LineRight;
            prec = prec->LineRight;
        }
        cout << "\n";
        prec = mostLeft;
        curr = mostLeft->LineRight;
        // TERZA PARTE
        for(int nodiFantasma = mostLeft->X - l.minX; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "           ";
        cout << "|^ ^ ^|";
        while (curr != NULL)
        {
            if (curr->W != NULL) cout << "  ";
            else for(int nodiFantasma = curr->X - prec->X -1 ; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "           ";
            cout << "|^ ^ ^|";
            curr = curr->LineRight;
            prec = prec->LineRight;
        }
        cout << "\n";
        prec = mostLeft;
        curr = mostLeft->LineRight;
        // QUARTA PARTE: EVENTUALI COLLEGAMENTI
        for(int i = 2; i > 0; i = i - 1)
        {
            for(int nodiFantasma = mostLeft->X - l.minX; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "         ";
            if (mostLeft->S != NULL) cout << "   |   ";
            while (curr != NULL)
            {
                if (curr->W != NULL) cout << "  ";
                else for(int nodiFantasma = curr->X - prec->X - 1 ; nodiFantasma > 0; nodiFantasma = nodiFantasma - 1) cout << "           ";
                if (curr->S != NULL) cout << "   |   ";
                curr = curr->LineRight;
                prec = prec->LineRight;
            }
            cout << "\n";
            prec = mostLeft;
            curr = mostLeft->LineRight;
        }
    }
}
